//=============================================================================
// engine.cpp
//
//    Compute the boomerang statistic for each measured location using the
//    conic/quadratic groundwater model.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#include <cassert>
#include <iomanip>
#include <math.h>
#include <numeric>
#include <sstream>

#include "engine.h"
#include "linear_systems.h"
#include "numerical_constants.h"
#include "special_functions.h"

//=============================================================================
Results::Results() :
   k(),
   h(),
   R_ev(),
   R_sd(),
   M_ev(),
   M_sd(),
   D_ev(),
   D_sd() {
}

Results::Results( int k_count, int h_count ) :
   k(k_count),
   h(h_count),
   R_ev(k_count, h_count),
   R_sd(k_count, h_count),
   M_ev(k_count, h_count),
   M_sd(k_count, h_count),
   D_ev(k_count, h_count),
   D_sd(k_count, h_count) {
}

//=============================================================================
// SetupQuadraticModel
//=============================================================================
std::tuple<Matrix, Matrix, Matrix> SetupQuadraticModel(
   double xo,
   double yo,
   double conductivity,
   double thickness,
   std::vector<ObsRecord> obs,
   std::vector<WellRecord> wells) {
   const int M = obs.size();     // number of observations
   const int N = wells.size();   // number of pumping wells

   // Setup the regression matrix (X) for the quadratic discharge
   // potential model.
   Matrix X(M,6);

   for (int m = 0; m < M; ++m) {
      double dx = obs[m].x - xo;
      double dy = obs[m].y - yo;

      X(m,0) = dx*dx;
      X(m,1) = dy*dy;
      X(m,2) = dx*dy;
      X(m,3) = dx;
      X(m,4) = dy;
      X(m,5) = 1;
   }

   // Use a first-order second moment approximation to compute the expected
   // values and the standard deviations of the discharge potentials at the
   // observation locations from the given expected values and standard
   // deviations of the piezometric heads.
   std::vector<double> Phi_ev(M);
   std::vector<double> Phi_sd(M);

   for (int m = 0; m < M; ++m) {
      if (obs[m].head_ev < thickness) {
         Phi_ev[m] = 0.5*conductivity * (obs[m].head_ev*obs[m].head_ev + obs[m].head_sd*obs[m].head_sd);
         Phi_sd[m] = conductivity * obs[m].head_ev * obs[m].head_sd;
      } else {
         Phi_ev[m] = conductivity * thickness * (obs[m].head_ev - 0.5*thickness);
         Phi_sd[m] = conductivity * thickness * obs[m].head_sd;
      }
   }

   // Setup the the inverse of the observation variance matrix (Vinv).
   // Recall that V is a diagonal matrix, so inv(V) is simply the inverse
   // of the diagonal elements.
   Matrix Vinv(M,M,0.0);

   for (int m = 0; m < M; ++m) {
      Vinv(m,m) = 1.0/(Phi_sd[m]*Phi_sd[m]);
   }

   // Compute the contributions to the discharge potentials at the observation
   // locations due to all of the pumping wells combined.
   std::vector<double> Phi_wells(M);

   for (int m = 0; m < M; ++m) {
      Phi_wells[m] = 0.0;
      for (int n = 0; n < N; ++n) {
         double separation_distance = hypot(obs[m].x-wells[n].x, obs[m].y-wells[n].y);
         if (separation_distance >= wells[n].r)
            Phi_wells[m] += wells[n].q/TWO_PI * std::log(separation_distance);
         else
            Phi_wells[m] += wells[n].q/TWO_PI * std::log(wells[n].r);
      }
   }

   // Setup the right-hand-side matrix (Y).
   Matrix Y(M,1);

   for (int m = 0; m < M; ++m) {
      for ( int n = 0; n < N; ++n)
         Y(m,0) = Phi_ev[m] - Phi_wells[m];
   }

   return std::make_tuple(X, Vinv, Y);
}


//=============================================================================
// FitQuadraticModel
//
// Computes the least-squares-optimal values of the six parameter in the
// quadratic discharge potential model, {A, B, C, D, E, F}, using only the
// active data.
//=============================================================================
std::tuple<Matrix, Matrix> FitQuadraticModel(
   const Matrix& X,
   const Matrix& Vinv,
   const Matrix& Y ) {

   Matrix VinvX;
   Multiply_MM(Vinv, X, VinvX);

   Matrix XtVinvX;
   Multiply_MtM(X, VinvX, XtVinvX);

   Matrix VinvY;
   Multiply_MM(Vinv, Y, VinvY);

   Matrix XtVinvY;
   Multiply_MtM(X, VinvY, XtVinvY);

   Matrix L;
   if (!CholeskyDecomposition(XtVinvX, L)) {
      std::stringstream message;
      message << "Cholesky Decomposition failed." << std::endl;
      throw CholeskyDecompositionFailed(message.str());
   }

   Matrix P_ev;
   CholeskySolve(L, XtVinvY, P_ev);

   Matrix P_cov;
   CholeskyInverse(L, P_cov);

   return std::make_tuple(P_ev, P_cov);
}


//=============================================================================
// ComputeGeohydrologyStatistics
//
// Computes the expected values and standard deviations for the three hydro-
// geologic parameter, {recharge, magnitude, direction}, using the expected
// value and covariance matrices for the six parameters in the quadratic
// discharge potential model, {A, B, C, D, E, F}.
//
// Notes:
// o  All three of the computed standard deviations are forcibly bounded
//    below by sqrt(EPS), which is about 1.5e-8.
//=============================================================================
std::tuple<double, double, double, double, double, double>
ComputeGeohydrologyStatistics(
   const Matrix& P_ev,
   const Matrix& P_cov) {

   // To simplify the notation, we extract and rename the necessary components.
   double EA  = P_ev(0,0);
   double EB  = P_ev(1,0);
   double VA  = P_cov(0,0);
   double VB  = P_cov(1,1);
   double CAB = P_cov(0,1);

   double EQx   = -P_ev(3,0);
   double EQy   = -P_ev(4,0);
   double VQx   = P_cov(3,3);
   double VQy   = P_cov(4,4);
   double CQxQy = P_cov(3,4);

   // To simplify the notation, we define intermediate variables {S,T,U}, and
   // we compute the various necessary partial derivatives.
   double S = EQx*EQx + EQy*EQy;
   double T = sqrt(S);
   double U = std::atan2(EQy, EQx);

   // Compute the necessary partial derivatives.
   double dTdQx     = EQx / T;
   double dTdQy     = EQy / T;
   double d2TdQx2   = (EQy*EQy)/(T*T*T);
   double d2TdQy2   = (EQx*EQx)/(T*T*T);
   double d2TdQxdQy = -(EQx*EQy)/(T*T*T);

   double dUdQx     = -EQy / S;
   double dUdQy     =  EQx / S;
   double d2UdQx2   =  2*EQx*EQy / (S*S);
   double d2UdQy2   = -2*EQx*EQy / (S*S);
   double d2UdQxdQy = ( EQy*EQy - EQx*EQx ) / (S*S);

   // Compute the statistics for the recharge.
   double r_ev = -2.0*(EA + EB);
   double r_va = 4.0*(VA + VB + 2*CAB);
   double r_sd = std::sqrt( std::max(r_va, EPS));

   // Compute the statistics for the magnitude T.
   double m_ev = T + 0.5*(d2TdQx2*VQx + d2TdQy2*VQy + 2*d2TdQxdQy*CQxQy);
   double m_va = dTdQx*dTdQx*VQx + dTdQy*dTdQy*VQy + 2*dTdQx*dTdQy*CQxQy;
   double m_sd = std::sqrt( std::max( m_va, EPS ) );

   // Compute the statistics for the direction U.
   double d_ev = U + 0.5*(d2UdQx2*VQx + d2UdQy2*VQy + 2*d2UdQxdQy*CQxQy);
   double d_va = dUdQx*dUdQx*VQx + dUdQy*dUdQy*VQy + 2*dUdQx*dUdQy*CQxQy;
   double d_sd = std::sqrt( std::max(d_va, EPS) );

   return std::make_tuple(r_ev, r_sd, m_ev, m_sd, d_ev, d_sd);
}


//=============================================================================
// Engine
//
//
// Arguments:
//
// Returns:
//
// Notes:
// o  The reported boomerang statistics are defined using the hydrogeologic
//    descriptors,
//
//    -- regional recharge {recharge},
//    -- regional flow magnitude {magnitude}, and
//    -- regional flow direction {direction}
//
//    computed at the specified focus location.
//
//=============================================================================
Results Engine(
   double xo, double yo,
   double k_alpha, double k_beta, int k_count,
   double h_alpha, double h_beta, int h_count,
   double radius,
   std::vector<ObsRecord> obs,
   std::vector<WellRecord> wells) {

   // Manifest constants.
   const int MINIMUM_COUNT = 10; // At least this many unique observation locations.

   const int M = obs.size();     // number of observations
   const int N = wells.size();   // number of pumping wells

   // Deactivate observations that are too close to a pumping well.
   std::vector<int> is_active(M);
   std::fill(is_active.begin(), is_active.end(), 1);

   for (int m = 0; m < M; ++m) {
      for (int n = 0; n < N; ++n) {
         double separation_distance = hypot(obs[m].x-wells[n].x, obs[m].y-wells[n].y);
         if (separation_distance < radius) {
            is_active[m] = 0;
            std::cout << " --Obs(" << m << ") deactivated due to proximity with Well(" << n << ")" << std::endl;
         }
      }
   }

   int Mactive = std::accumulate(is_active.begin(), is_active.end(), int(0));
   if (Mactive < MINIMUM_COUNT) {
      std::stringstream message;
      message << "Too few active observations: " << Mactive << " < " << MINIMUM_COUNT << std::endl;
      throw TooFewObservations(message.str());
   }

   std::vector<ObsRecord> active_obs(Mactive);
   int mm = 0;
   for (int m = 0; m < M; ++m) {
      if (is_active[m]) {
         active_obs[mm++] = obs[m];
      }
   }
   std::cout << active_obs.size() << " active observation data records." << std::endl;

   // Initialize the results.
   Results results(k_count, h_count);

   // Compute the set-points for both k and h. Each set-point is at the center
   // of an interval containing equal probability. For example, if n = 10 the
   // set points would be at the {5, 15, 25, ..., 75, 85, 95} percentiles. As
   // a result, the distribution of the actual values, k's and h's, are highly
   // nonuniform.
   std::vector<double> k(k_count);
   for (int i = 0; i < k_count; ++i ) {
      double p = 1.0/double(2.0*k_count) + double(i)/double(k_count);
      k[i] = exp(k_alpha + k_beta*GaussianCDFInv(p));
   }
   results.k = k;

   std::vector<double> h(h_count);
   for (int j = 0; j < h_count; ++j ) {
      double p = 1.0/double(2.0*h_count) + double(j)/double(h_count);
      h[j] = exp(h_alpha + h_beta*GaussianCDFInv(p));
   }
   results.h = h;

   // Fill the results.
   for (int i = 0; i < k_count; ++i) {
      for (int j = 0; j < h_count; ++j) {

         // Setup the regression for the quadratic discharge potential model
         // using the current k and h, and only the active obs.
         Matrix X, Vinv, Y;
         std::tie(X, Vinv, Y) = SetupQuadraticModel(xo, yo, k[i], h[i], active_obs, wells);

         // Fit the parameters using all of the active observations.
         Matrix P_ev, P_cov;
         std::tie(P_ev, P_cov) = FitQuadraticModel(X, Vinv, Y);


         double r_ev, r_sd, m_ev, m_sd, d_ev, d_sd;
         std::tie(r_ev, r_sd, m_ev, m_sd, d_ev, d_sd) = ComputeGeohydrologyStatistics(P_ev, P_cov);

         results.R_ev(i,j) = r_ev;
         results.R_sd(i,j) = r_sd;

         results.M_ev(i,j) = m_ev;
         results.M_sd(i,j) = m_sd;

         results.D_ev(i,j) = d_ev;
         results.D_sd(i,j) = d_sd;
      }
   }

   return results;
}
