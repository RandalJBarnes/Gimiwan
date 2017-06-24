//=============================================================================
// main.cpp
//
//    The driver for the command line version of the user interface.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    24 June 2017
//=============================================================================
#include <cstring>
#include <ctime>
#include <iostream>

#include "engine.h"
#include "now.h"
#include "numerical_constants.h"
#include "read_data.h"
#include "version.h"
#include "write_results.h"


//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
   // Check the command line.
   switch (argc) {
      case 1: {
         Usage();
         return 0;
      }
      case 2: {
         if ( strcmp(argv[1], "--help") == 0 )
            Help();
         else if ( strcmp(argv[1], "--version") == 0 )
            Version();
         else
            Usage();
         return 0;
      }
      case 13: {
         Banner( std::cout );
         break;
      }
      default: {
         Usage();
         return 1;
      }
   }

   // Gimiwan <xo> <yo> <k alpha> <k beta> <k count> <h alpha> <h beta> <h count> <radius> <obs file> <wells file> <output root>
   // [0]     [1]  [2]  [3]       [4]      [5]       [6]       [7]      [8]       [9]      [10]       [11]         [12]

   // Get <xo> and <yo>.
   double xo = atof( argv[1] );
   double yo = atof( argv[2] );

   // Get and check the hydraulic conductivity distribution.
   double k_alpha = atof( argv[3] );

   double k_beta  = atof( argv[4] );
   if ( k_beta <= EPS ) {
      std::cerr << "ERROR: k_beta = " << argv[4] << " is not valid;  0 < k_beta." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   int k_count = atoi( argv[5] );
   if ( k_count < 1 ) {
      std::cerr << "ERROR: k_count = " << argv[5] << " is not valid;  0 < k_count." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Get and check the aquifer thickness distribution.
   double h_alpha = atof( argv[6] );

   double h_beta = atof( argv[7] );
   if ( h_beta <= EPS ) {
      std::cerr << "ERROR: h_beta = " << argv[7] << " is not valid;  0 < h_beta." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   int h_count = atoi( argv[8] );
   if ( h_count < 1 ) {
      std::cerr << "ERROR: h_count = " << argv[5] << " is not valid;  0 < h_count." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Get and check the buffer radius.
   double radius = atof( argv[9] );
   if ( radius < 0 ) {
      std::cerr << "ERROR: buffer radius = " << argv[9] << " is not valid;  0 <= buffer radius." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Read in the observation data from the specified <obs file>.
   std::vector<ObsRecord> obs;

   try {
      obs = read_obs_data( argv[10] );
      std::cout << obs.size() << " observation data records read from <" << argv[10] << ">." << std::endl;
   }
   catch (InvalidObsFile& e) {
      std::cerr << e.what() << std::endl;
      return 3;
   }
   catch (InvalidObsRecord& e) {
      std::cerr << e.what() << std::endl;
      return 3;
   }

   // Read in the well data from the specified <well file>.
   std::vector<WellRecord> wells;

   try {
      wells = read_well_data( argv[11] );
      std::cout << wells.size() << " well data records read from <" << argv[11] << ">." << std::endl;
   }
   catch (InvalidWellFile& e) {
      std::cerr << e.what() << std::endl;
      return 3;
   }
   catch (InvalidWellRecord& e) {
      std::cerr << e.what() << std::endl;
      return 3;
   }

   // Execute all of the computations.
   Results results;

   try {
       results = Engine(xo, yo, k_alpha, k_beta, k_count, h_alpha, h_beta, h_count, radius, obs, wells);
   }
   catch (TooFewObservations& e) {
      std::cerr << e.what() << std::endl;
      return 4;
   }
   catch (CholeskyDecompositionFailed& e) {
      std::cerr << e.what() << std::endl;
      return 4;
   }
   catch (...) {
      std::cerr << "The Gimiwan Engine failed for an unknown reason." << std::endl;
      throw;
   }

   // Write out the results to the specified output data file.
   try {
      write_results( argv[12], results );
      std::cout << "Six output files with root name <" << argv[12] << "> created. " << std::endl;
   }
   catch (InvalidOutputFile& e) {
      std::cerr << e.what() << std::endl;
      return 4;
   }

   // Successful termination.
   double elapsed = static_cast<double>(clock())/CLOCKS_PER_SEC;
   std::cout << "elapsed time: " << std::fixed << elapsed << " seconds." << std::endl;
   std::cout << std::endl;

   // Terminate execution.
	return 0;
}
