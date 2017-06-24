//=============================================================================
// write_results.cpp
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    24 June 2017
//=============================================================================
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "engine.h"
#include "matrix.h"
#include "numerical_constants.h"
#include "write_results.h"

//-----------------------------------------------------------------------------
namespace {

   //--------------------------------------------------------------------------
   void write_result_matrix( const std::string& outfilename,
      std::vector<double> k, std::vector<double> h, Matrix V)
   {
      std::ofstream outfile( outfilename );
      if ( outfile.fail() ) {
         std::stringstream message;
         message << "Could not open <" << outfilename << "> for output.";
         throw InvalidOutputFile(message.str());
      }

      outfile << std::setprecision(std::numeric_limits<long double>::digits10 + 1);

      int k_count = k.size();
      int h_count = h.size();

      for (int j = 0; j < h_count; ++j) {
         outfile << ',' << h[j];
      }
      outfile << std::endl;

      for (int i = 0; i < k_count; ++i ) {
         outfile << k[i];
         for (int j = 0; j < h_count; ++j) {
            outfile << ',' << V(i,j);
         }
         outfile << std::endl;
      }

      outfile.close();
   }
}

//-----------------------------------------------------------------------------
void write_results( const std::string& outfileroot, Results results ) {
   std::string outfilename;

   // Write out the recharge expected value.
   outfilename = outfileroot + "_recharge_ev.csv";
   write_result_matrix(outfilename, results.k, results.h, results.R_ev);

   // Write out the recharge standard deviation.
   outfilename = outfileroot + "_recharge_sd.csv";
   write_result_matrix(outfilename, results.k, results.h, results.R_sd);

   // Write out the magnitude expected value.
   outfilename = outfileroot + "_magnitude_ev.csv";
   write_result_matrix(outfilename, results.k, results.h, results.M_ev);

   // Write out the magnitude standard deviation.
   outfilename = outfileroot + "_magnitude_sd.csv";
   write_result_matrix(outfilename, results.k, results.h, results.M_sd);

   // Write out the direction expected value.
   Matrix D_ev_deg;
   Multiply_aM(RAD_TO_DEG, results.D_ev, D_ev_deg);

   outfilename = outfileroot + "_direction_ev.csv";
   write_result_matrix(outfilename, results.k, results.h, D_ev_deg);

   // Write out the direction standard deviation.
   Matrix D_sd_deg;
   Multiply_aM(RAD_TO_DEG, results.D_sd, D_sd_deg);

   outfilename = outfileroot + "_direction_sd.csv";
   write_result_matrix(outfilename, results.k, results.h, D_sd_deg);
}
