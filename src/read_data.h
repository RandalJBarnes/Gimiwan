//=============================================================================
// read_data.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#ifndef READ_DATA_H
#define READ_DATA_H

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

//-----------------------------------------------------------------------------
class InvalidObsFile : public std::runtime_error {
   public :
      InvalidObsFile( const std::string& message ) : std::runtime_error(message) {
      }
};

class InvalidObsRecord : public std::runtime_error {
   public :
      InvalidObsRecord( const std::string& message ) : std::runtime_error(message) {
      }
};

class InvalidWellFile : public std::runtime_error {
   public :
      InvalidWellFile( const std::string& message ) : std::runtime_error(message) {
      }
};

class InvalidWellRecord : public std::runtime_error {
   public :
      InvalidWellRecord( const std::string& message ) : std::runtime_error(message) {
      }
};

//-----------------------------------------------------------------------------
struct ObsRecord{
   std::string id;
   double x;
   double y;
   double head_ev;
   double head_sd;
};

std::vector<ObsRecord> read_obs_data( const std::string& inpfilename );

//-----------------------------------------------------------------------------
struct WellRecord{
   std::string id;
   double x;
   double y;
   double r;
   double q;
};

std::vector<WellRecord> read_well_data( const std::string& inpfilename );

//=============================================================================
#endif  // READ_DATA_H
