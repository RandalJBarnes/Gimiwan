//=============================================================================
// test_engine.cpp
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    23 June 2017
//=============================================================================
#include <utility>

#include "test_engine.h"
#include "unit_test.h"
#include "..\src\engine.h"

//-----------------------------------------------------------------------------
// Hide all of the testing details inside an unnamed namespace. This allows me
// to create many small unit tests with polluting the global namespace.
//-----------------------------------------------------------------------------
namespace{
   const double TOLERANCE = 1e-9;

   //--------------------------------------------------------------------------
   // TestSetupQuadraticModel
   //
   //    This is simply an example problem.  The "correct" solution was
   //    computed using parallel Matlab code.
   //--------------------------------------------------------------------------
   bool TestSetupQuadraticModel()
   {
      double xfocus = 2250;
      double yfocus = -2250;

      double conductivity = 10;
      double thickness = 10;

      std::vector<ObsRecord> obs = {
         ObsRecord{"01",1000,-1000,100,1},
         ObsRecord{"02",1000,-1500,105,1},
         ObsRecord{"03",1000,-2000,110,1},
         ObsRecord{"04",1000,-2500,115,1},
         ObsRecord{"05",1000,-3000,120,1},
         ObsRecord{"06",1500,-1000,95,1},
         ObsRecord{"07",1500,-1500,100,1},
         ObsRecord{"08",1500,-2000,105,1},
         ObsRecord{"09",1500,-2500,110,1},
         ObsRecord{"10",1500,-3000,115,1},
         ObsRecord{"11",2000,-1000,90,1},
         ObsRecord{"12",2000,-1500,95,1},
         ObsRecord{"13",2000,-2000,100,1},
         ObsRecord{"14",2000,-2500,105,1},
         ObsRecord{"15",2000,-3000,110,1},
         ObsRecord{"16",2500,-1000,85,1},
         ObsRecord{"17",2500,-1500,90,1},
         ObsRecord{"18",2500,-2000,95,1},
         ObsRecord{"19",2500,-2500,100,1},
         ObsRecord{"20",2500,-3000,105,1},
         ObsRecord{"21",3000,-1000,80,1},
         ObsRecord{"22",3000,-1500,85,1},
         ObsRecord{"23",3000,-2000,90,1},
         ObsRecord{"24",3000,-2500,95,1},
         ObsRecord{"25",3000,-3000,100,1}
      };

      std::vector<WellRecord> wells = {
         WellRecord{"12345",2250,-2250,0.25,750}
      };

      Matrix X, Vinv, Y;
      std::tie(X, Vinv, Y) = SetupQuadraticModel(xfocus, yfocus, conductivity, thickness, obs, wells);

      static const double X_data[] = {
        1562500.0,     1562500.0,    -1562500.0,       -1250.0,        1250.0,           1.0,
        1562500.0,      562500.0,     -937500.0,       -1250.0,         750.0,           1.0,
        1562500.0,       62500.0,     -312500.0,       -1250.0,         250.0,           1.0,
        1562500.0,       62500.0,      312500.0,       -1250.0,        -250.0,           1.0,
        1562500.0,      562500.0,      937500.0,       -1250.0,        -750.0,           1.0,
         562500.0,     1562500.0,     -937500.0,        -750.0,        1250.0,           1.0,
         562500.0,      562500.0,     -562500.0,        -750.0,         750.0,           1.0,
         562500.0,       62500.0,     -187500.0,        -750.0,         250.0,           1.0,
         562500.0,       62500.0,      187500.0,        -750.0,        -250.0,           1.0,
         562500.0,      562500.0,      562500.0,        -750.0,        -750.0,           1.0,
          62500.0,     1562500.0,     -312500.0,        -250.0,        1250.0,           1.0,
          62500.0,      562500.0,     -187500.0,        -250.0,         750.0,           1.0,
          62500.0,       62500.0,      -62500.0,        -250.0,         250.0,           1.0,
          62500.0,       62500.0,       62500.0,        -250.0,        -250.0,           1.0,
          62500.0,      562500.0,      187500.0,        -250.0,        -750.0,           1.0,
          62500.0,     1562500.0,      312500.0,         250.0,        1250.0,           1.0,
          62500.0,      562500.0,      187500.0,         250.0,         750.0,           1.0,
          62500.0,       62500.0,       62500.0,         250.0,         250.0,           1.0,
          62500.0,       62500.0,      -62500.0,         250.0,        -250.0,           1.0,
          62500.0,      562500.0,     -187500.0,         250.0,        -750.0,           1.0,
         562500.0,     1562500.0,      937500.0,         750.0,        1250.0,           1.0,
         562500.0,      562500.0,      562500.0,         750.0,         750.0,           1.0,
         562500.0,       62500.0,      187500.0,         750.0,         250.0,           1.0,
         562500.0,       62500.0,     -187500.0,         750.0,        -250.0,           1.0,
         562500.0,      562500.0,     -562500.0,         750.0,        -750.0,           1.0
      };
      Matrix X_true(25, 6, X_data);

      static const double Y_data[] = {
          8607.44247682397,
          9130.46001064384,
          9646.47083796387,
          10146.4708379639,
          10630.4600106438,
          8130.46001064384,
          8668.41779413424,
          9203.49872658524,
          9703.49872658524,
          10168.4177941342,
          7646.47083796387,
          8203.49872658524,
          8799.55497634651,
          9299.55497634651,
          9703.49872658524,
          7146.47083796387,
          7703.49872658524,
          8299.55497634651,
          8799.55497634651,
          9203.49872658524,
          6630.46001064384,
          7168.41779413424,
          7703.49872658524,
          8203.49872658524,
          8668.41779413424
      };
      Matrix Y_true(25, 1, Y_data);

      Matrix Eye, Vinv_true;
      Identity(Eye, 25);
      Multiply_aM(0.0001, Eye, Vinv_true);

      bool flag = true;
      flag &= CHECK( isClose(X, X_true, TOLERANCE) );
      flag &= CHECK( isClose(Y, Y_true, TOLERANCE) );
      flag &= CHECK( isClose(Vinv, Vinv_true, TOLERANCE) );
      return flag;
   }

   //--------------------------------------------------------------------------
   // TestFitQuadraticModel
   //
   //    This is simply an example problem.  The "correct" solution was
   //    computed using parallel Matlab code.
   //--------------------------------------------------------------------------
   bool TestFitQuadraticModel() {

      static const double X_data[] = {
        1562500.0,     1562500.0,    -1562500.0,       -1250.0,        1250.0,           1.0,
        1562500.0,      562500.0,     -937500.0,       -1250.0,         750.0,           1.0,
        1562500.0,       62500.0,     -312500.0,       -1250.0,         250.0,           1.0,
        1562500.0,       62500.0,      312500.0,       -1250.0,        -250.0,           1.0,
        1562500.0,      562500.0,      937500.0,       -1250.0,        -750.0,           1.0,
         562500.0,     1562500.0,     -937500.0,        -750.0,        1250.0,           1.0,
         562500.0,      562500.0,     -562500.0,        -750.0,         750.0,           1.0,
         562500.0,       62500.0,     -187500.0,        -750.0,         250.0,           1.0,
         562500.0,       62500.0,      187500.0,        -750.0,        -250.0,           1.0,
         562500.0,      562500.0,      562500.0,        -750.0,        -750.0,           1.0,
          62500.0,     1562500.0,     -312500.0,        -250.0,        1250.0,           1.0,
          62500.0,      562500.0,     -187500.0,        -250.0,         750.0,           1.0,
          62500.0,       62500.0,      -62500.0,        -250.0,         250.0,           1.0,
          62500.0,       62500.0,       62500.0,        -250.0,        -250.0,           1.0,
          62500.0,      562500.0,      187500.0,        -250.0,        -750.0,           1.0,
          62500.0,     1562500.0,      312500.0,         250.0,        1250.0,           1.0,
          62500.0,      562500.0,      187500.0,         250.0,         750.0,           1.0,
          62500.0,       62500.0,       62500.0,         250.0,         250.0,           1.0,
          62500.0,       62500.0,      -62500.0,         250.0,        -250.0,           1.0,
          62500.0,      562500.0,     -187500.0,         250.0,        -750.0,           1.0,
         562500.0,     1562500.0,      937500.0,         750.0,        1250.0,           1.0,
         562500.0,      562500.0,      562500.0,         750.0,         750.0,           1.0,
         562500.0,       62500.0,      187500.0,         750.0,         250.0,           1.0,
         562500.0,       62500.0,     -187500.0,         750.0,        -250.0,           1.0,
         562500.0,      562500.0,     -562500.0,         750.0,        -750.0,           1.0
      };
      Matrix X(25, 6, X_data);

      static const double Y_data[] = {
          8607.44247682397,
          9130.46001064384,
          9646.47083796387,
          10146.4708379639,
          10630.4600106438,
          8130.46001064384,
          8668.41779413424,
          9203.49872658524,
          9703.49872658524,
          10168.4177941342,
          7646.47083796387,
          8203.49872658524,
          8799.55497634651,
          9299.55497634651,
          9703.49872658524,
          7146.47083796387,
          7703.49872658524,
          8299.55497634651,
          8799.55497634651,
          9203.49872658524,
          6630.46001064384,
          7168.41779413424,
          7703.49872658524,
          8203.49872658524,
          8668.41779413424
      };
      Matrix Y(25, 1, Y_data);

      Matrix Eye, Vinv;
      Identity(Eye, 25);
      Multiply_aM(0.0001, Eye, Vinv);

      std::vector<int> is_active(25);
      std::fill(is_active.begin(), is_active.end(), 1);

      Matrix P_ev, P_cov;
      std::tie(P_ev, P_cov) = FitQuadraticModel(X, Vinv, Y, is_active);

      static const double P_ev_data[] = {
        -7.20476189326098e-05,
        -7.20476189326105e-05,
        -7.88066946065188e-06,
            -1.00588312418789,
           -0.994116875812116,
             8766.22133799834
      };
      Matrix P_ev_true(6, 1, P_ev_data);

      static const double P_cov_data[] = {
         2.28571428571429e-09,      5.16987882845642e-25,      2.58493941422821e-26,      1.14285714285714e-06,     -2.38228016415272e-22,                    -0.001,
         5.16987882845642e-25,      2.28571428571429e-09,     -5.16987882845642e-26,      3.70576914423756e-22,     -1.14285714285714e-06,                    -0.001,
         2.58493941422821e-26,     -5.16987882845642e-26,                   1.6e-09,                    -4e-07,                     4e-07,     -9.99999999999999e-05,
         1.14285714285714e-06,      3.70576914423756e-22,                    -4e-07,       0.00147142857142857,                   -0.0001,                    -0.275,
        -2.38228016415272e-22,     -1.14285714285714e-06,                     4e-07,                   -0.0001,       0.00147142857142857,                     0.275,
                       -0.001,                    -0.001,     -9.99999999999999e-05,                    -0.275,                     0.275,                   1381.25
      };
      Matrix P_cov_true(6, 6, P_cov_data);

      bool flag = true;
      flag &= CHECK( isClose(P_ev,  P_ev_true,  TOLERANCE) );
      flag &= CHECK( isClose(P_cov, P_cov_true, TOLERANCE) );
      return flag;
   }

   //--------------------------------------------------------------------------
   // TestComputeGeohydrologyStatistics
   //
   //    This is simply an example problem.  The "correct" solution was
   //    computed using parallel Matlab code.
   //--------------------------------------------------------------------------
   bool TestComputeGeohydrologyStatistics() {

      static const double P_ev_data[] = {
        -7.20476189326098e-05,
        -7.20476189326105e-05,
        -7.88066946065188e-06,
            -1.00588312418789,
           -0.994116875812116,
             8766.22133799834
      };
      Matrix P_ev_true(6, 1, P_ev_data);

      static const double P_cov_data[] = {
         2.28571428571429e-09,      5.16987882845642e-25,      2.58493941422821e-26,      1.14285714285714e-06,     -2.38228016415272e-22,                    -0.001,
         5.16987882845642e-25,      2.28571428571429e-09,     -5.16987882845642e-26,      3.70576914423756e-22,     -1.14285714285714e-06,                    -0.001,
         2.58493941422821e-26,     -5.16987882845642e-26,                   1.6e-09,                    -4e-07,                     4e-07,     -9.99999999999999e-05,
         1.14285714285714e-06,      3.70576914423756e-22,                    -4e-07,       0.00147142857142857,                   -0.0001,                    -0.275,
        -2.38228016415272e-22,     -1.14285714285714e-06,                     4e-07,                   -0.0001,       0.00147142857142857,                     0.275,
                       -0.001,                    -0.001,     -9.99999999999999e-05,                    -0.275,                     0.275,                   1381.25
      };
      Matrix P_cov_true(6, 6, P_cov_data);

      double R_ev_true = 0.000288190475730441;
      double R_sd_true = 0.000135224680756563;

      double M_ev_true = 1.41479360777816;
      double M_sd_true = 0.0370328974483369;

      double D_ev_true = 0.779515695353749;
      double D_sd_true = 0.0280300487210578;

      double R_ev, R_sd, M_ev, M_sd, D_ev, D_sd;
      std::tie(R_ev, R_sd, M_ev, M_sd, D_ev, D_sd) = ComputeGeohydrologyStatistics(P_ev_true, P_cov_true);

      bool flag = true;
      flag &= CHECK( isClose(R_ev, R_ev_true, TOLERANCE) );
      flag &= CHECK( isClose(R_sd, R_sd_true, TOLERANCE) );
      flag &= CHECK( isClose(M_ev, M_ev_true, TOLERANCE) );
      flag &= CHECK( isClose(M_sd, M_sd_true, TOLERANCE) );
      flag &= CHECK( isClose(D_ev, D_ev_true, TOLERANCE) );
      flag &= CHECK( isClose(D_sd, D_sd_true, TOLERANCE) );
      return flag;
   }

   //--------------------------------------------------------------------------
   // TestEngine
   //
   //    This is simply an example problem.  The "correct" solution was
   //    computed using parallel Matlab code.
   //--------------------------------------------------------------------------
   bool TestEngine() {

      double xfocus = 2250;
      double yfocus = -2250;

      double conductivity = 10;
      double thickness = 10;

      double buffer_radius = 100;

      std::vector<ObsRecord> obs = {
         ObsRecord{"01",1000,-1000,100,1},
         ObsRecord{"02",1000,-1500,105,1},
         ObsRecord{"03",1000,-2000,110,1},
         ObsRecord{"04",1000,-2500,115,1},
         ObsRecord{"05",1000,-3000,120,1},
         ObsRecord{"06",1500,-1000,95,1},
         ObsRecord{"07",1500,-1500,100,1},
         ObsRecord{"08",1500,-2000,105,1},
         ObsRecord{"09",1500,-2500,110,1},
         ObsRecord{"10",1500,-3000,115,1},
         ObsRecord{"11",2000,-1000,90,1},
         ObsRecord{"12",2000,-1500,95,1},
         ObsRecord{"13",2000,-2000,100,1},
         ObsRecord{"14",2000,-2500,105,1},
         ObsRecord{"15",2000,-3000,110,1},
         ObsRecord{"16",2500,-1000,85,1},
         ObsRecord{"17",2500,-1500,90,1},
         ObsRecord{"18",2500,-2000,95,1},
         ObsRecord{"19",2500,-2500,100,1},
         ObsRecord{"20",2500,-3000,105,1},
         ObsRecord{"21",3000,-1000,80,1},
         ObsRecord{"22",3000,-1500,85,1},
         ObsRecord{"23",3000,-2000,90,1},
         ObsRecord{"24",3000,-2500,95,1},
         ObsRecord{"25",3000,-3000,100,1}
      };

      std::vector<WellRecord> wells = {
         WellRecord{"12345",2250,-2250,0.25,750}
      };

      // All data.
      double recharge_ev  = 2.881904757304405e-04;
      double recharge_sd  = 1.352246807565627e-04;

      double magnitude_ev = 1.414793607778156;
      double magnitude_sd = 0.037032897448337;

      double direction_ev = 0.779515695353749;
      double direction_sd = 0.028030048721058;

      // Excluding the last obs.
      double recharge_ev_last  = 2.774442471611071e-04;
      double magnitude_ev_last = 1.415193300832738;
      double direction_ev_last = 0.783680402400208;

      double influence_recharge_last  = -0.397347159897507;
      double influence_magnitude_last =  0.053964593931656;
      double influence_direction_last =  0.742900429447129;

      Estimate estimate;
      std::vector<Boomerang> results;
      std::tie(estimate, results) = Engine(xfocus, yfocus, conductivity, thickness, buffer_radius, obs, wells);

      bool flag = true;
      flag &= CHECK( isClose(estimate.recharge_ev,  recharge_ev,   TOLERANCE) );
      flag &= CHECK( isClose(estimate.recharge_sd,  recharge_sd,   TOLERANCE) );

      flag &= CHECK( isClose(estimate.magnitude_ev, magnitude_ev,  TOLERANCE) );
      flag &= CHECK( isClose(estimate.magnitude_sd, magnitude_sd,  TOLERANCE) );

      flag &= CHECK( isClose(estimate.direction_ev, direction_ev,  TOLERANCE) );
      flag &= CHECK( isClose(estimate.direction_sd, direction_sd,  TOLERANCE) );

      flag &= CHECK( isClose(results[24].recharge,  recharge_ev_last,   TOLERANCE) );
      flag &= CHECK( isClose(results[24].magnitude, magnitude_ev_last,  TOLERANCE) );
      flag &= CHECK( isClose(results[24].direction, direction_ev_last,  TOLERANCE) );

      flag &= CHECK( isClose(results[24].influence_recharge,  influence_recharge_last,  TOLERANCE) );
      flag &= CHECK( isClose(results[24].influence_magnitude, influence_magnitude_last, TOLERANCE) );
      flag &= CHECK( isClose(results[24].influence_direction, influence_direction_last, TOLERANCE) );
      return flag;
   }
}

//-----------------------------------------------------------------------------
// test_Engine
//-----------------------------------------------------------------------------
std::pair<int,int> test_Engine()
{
   int nsucc = 0;
   int nfail = 0;

   TALLY( TestSetupQuadraticModel() );
   TALLY( TestFitQuadraticModel() );
   TALLY( TestComputeGeohydrologyStatistics() );
   TALLY( TestEngine() );

   return std::make_pair( nsucc, nfail );
}
