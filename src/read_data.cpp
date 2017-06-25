//=============================================================================
// read_data.cpp
//
//    Read in the observation data from the user-specified file.
//
// notes:
// o  This function uses Ben Strasser's "fast-cpp-csv-parser" to read in the
//    .csv input file. See
//
//       https://github.com/ben-strasser/fast-cpp-csv-parser
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    25 June 2017
//=============================================================================
#include <algorithm>
#include <fstream>
#include <sstream>

#include "../include/csv.h"
#include "numerical_constants.h"
#include "read_data.h"

//-----------------------------------------------------------------------------
std::vector<ObsRecord> read_obs_data( const std::string& obsfilename ) {
   std::vector<ObsRecord> obs;

   try {
      io::CSVReader<5,
      io::trim_chars<' ', '\t'>,
      io::no_quote_escape<','>,
      io::throw_on_overflow,
      io::single_and_empty_line_comment<'!','#'>> in(obsfilename);

      std::string id;
      double x, y, head_ev, head_sd;

      while (in.read_row(id, x, y, head_ev, head_sd)) {
         if (head_ev < EPS) {
            std::stringstream message;
            message << "Invalid observation head_ev on line " << obs.size()+1 << " of file " << obsfilename << ".";
            throw InvalidObsRecord(message.str());
         }

         if (head_sd < EPS) {
            std::stringstream message;
            message << "Invalid observation head_sd on line " << obs.size()+1 << " of file " << obsfilename << ".";
            throw InvalidObsRecord(message.str());
         }

         ObsRecord s = {id, x, y, head_ev, head_sd};
         obs.push_back(s);
      }
   } catch (io::error::can_not_open_file& e) {
      std::stringstream message;
      message << "Could not open <" << obsfilename << "> for input.";
      throw InvalidObsFile(message.str());
   } catch (...) {
      std::stringstream message;
      message << "Reading the observation data failed on line " << obs.size()+1 << " of file " << obsfilename << ".";
      throw InvalidObsRecord(message.str());
   }

   return obs;
}

//-----------------------------------------------------------------------------
std::vector<WellRecord> read_well_data( const std::string& wellfilename ) {
   std::vector<WellRecord> wells;

   try {
      io::CSVReader<5,
      io::trim_chars<' ', '\t'>,
      io::no_quote_escape<','>,
      io::throw_on_overflow,
      io::single_and_empty_line_comment<'!','#'>> in(wellfilename);

      std::string id;
      double x, y, r, q;

      while (in.read_row(id, x, y, r, q)) {
         WellRecord w = {id, x, y, r, q};
         wells.push_back(w);
      }
   } catch (io::error::can_not_open_file& e) {
      std::stringstream message;
      message << "Could not open <" << wellfilename << "> for input.";
      throw InvalidWellFile(message.str());
   } catch (...) {
      std::stringstream message;
      message << "Reading the well data failed on line " << wells.size()+1 << " of file " << wellfilename << ".";
      throw InvalidWellRecord(message.str());
   }

   return wells;
}
