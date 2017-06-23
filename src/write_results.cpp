//=============================================================================
// write_results.cpp
//
//    Read in the observation data from the user-specified file.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    23 June 2017
//=============================================================================
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "engine.h"
#include "numerical_constants.h"
#include "write_results.h"

//-----------------------------------------------------------------------------
void write_results( const std::string& outfilename, Estimate estimate, std::vector<Boomerang> results ) {

   // Open the specified output file.
   std::ofstream outfile( outfilename );
   if ( outfile.fail() ) {
      std::stringstream message;
      message << "Could not open <" << outfilename << "> for output.";
      throw InvalidOutputFile(message.str());
   }

   // Write out the header line to the output file.
   outfile << "id,x,y,"
      << "recharge,magnitude,direction,"
      << "influence_recharge,influence_magnitude,influence_direction"
      << std::endl;

   // Set the numeric output to maximum precision; i.e. all of the insignificant digits.
   outfile << std::setprecision(std::numeric_limits<long double>::digits10 + 1);

   // Write out the best estimates.
   outfile << "exp val,";
   outfile << estimate.x  << ',';
   outfile << estimate.y  << ',';
   outfile << estimate.recharge_ev  << ',';
   outfile << estimate.magnitude_ev << ',';
   outfile << estimate.direction_ev * RAD_TO_DEG << ',';
   outfile << ",," << std::endl;

   outfile << "std dev,";
   outfile << estimate.x  << ',';
   outfile << estimate.y  << ',';
   outfile << estimate.recharge_sd  << ',';
   outfile << estimate.magnitude_sd << ',';
   outfile << estimate.direction_sd * RAD_TO_DEG << ',';
   outfile << ",," << std::endl;

   // Fill the output file with the observation-by-observation results using
   // a maximum precision .csv format.

   for ( unsigned m = 0; m < results.size(); ++m ) {
      outfile << results[m].id << ',';
      outfile << results[m].x  << ',';
      outfile << results[m].y  << ',';

      if (results[m].is_active) {
         outfile << results[m].recharge  << ',';
         outfile << results[m].magnitude << ',';
         outfile << results[m].direction * RAD_TO_DEG << ',';

         outfile << results[m].influence_recharge  << ',';
         outfile << results[m].influence_magnitude << ',';
         outfile << results[m].influence_direction;

         outfile << std::endl;
      }
      else {
         outfile << ",,,,," << std::endl;
      }
   }

   outfile.close();
}
