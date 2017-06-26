//=============================================================================
// engine.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#ifndef ENGINE_H
#define ENGINE_H

#include <stdexcept>
#include <tuple>
#include <vector>

#include "matrix.h"
#include "read_data.h"


//=============================================================================
class TooFewObservations: public std::runtime_error {
   public :
      TooFewObservations( const std::string& message ) : std::runtime_error(message) {
      }
};

class CholeskyDecompositionFailed: public std::runtime_error {
   public :
      CholeskyDecompositionFailed( const std::string& message ) : std::runtime_error(message) {
      }
};

//=============================================================================
class Results {
   public:
      std::vector<double> k;
      std::vector<double> h;

      Matrix R_ev;
      Matrix R_sd;

      Matrix M_ev;
      Matrix M_sd;

      Matrix D_ev;
      Matrix D_sd;

      Results();
      Results( int k_count, int h_count );
};


//=============================================================================
Results Engine(
   double xo, double yo,
   double k_alpha, double k_beta, int k_count,
   double h_alpha, double h_beta, int h_count,
   double radius,
   std::vector<ObsRecord> obs,
   std::vector<WellRecord> wells
);

std::tuple<Matrix, Matrix, Matrix>
SetupQuadraticModel(
   double xo, double yo,
   double conductivity,
   double thickness,
   std::vector<ObsRecord> obs,
   std::vector<WellRecord> wells
);

std::tuple<Matrix, Matrix>
FitQuadraticModel(
   const Matrix& X,
   const Matrix& Vinv,
   const Matrix& Y
);

std::tuple<double, double, double, double, double, double>
ComputeGeohydrologyStatistics(
   const Matrix& P_ev,
   const Matrix& P_cov
);

//=============================================================================
#endif  // ENGINE_H
