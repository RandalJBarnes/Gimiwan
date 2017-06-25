//=============================================================================
// version.cpp
//
//    A simple version reporting class.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    25 June 2017
//=============================================================================
#include <iostream>

#include "version.h"

namespace {
const char* VERSION = "25 June 2017 [Beta]";
}

//-----------------------------------------------------------------------------
void Banner( std::ostream& ost) {
   ost                                                    << std::endl;
   ost << "--------------------------------------------"  << std::endl;
   ost << "Gimiwan (" << VERSION << ")"                   << std::endl;
   ost                                                    << std::endl;
   ost << "R. Barnes, University of Minnesota          "  << std::endl;
   ost << "R. Soule,  Minnesota Department of Health   "  << std::endl;
   ost << "--------------------------------------------"  << std::endl;
}

//-----------------------------------------------------------------------------
void Help() {
   Version();

   std::cout <<
      "   For ranges of conductivities and thicknesses, compute the expected values \n"
      "   and standard deviations of the regional uniform recharge, the magnitude of \n"
      "   the regional uniform flow, and the direction of the regional uniform flow, \n"
      "   using the quadratic discharge potential model and observed heads. \n"
   << std::endl;

   Usage();

   std::cout <<
      "Arguments: \n"
      "   <xo>            The x-coordinate [L] of model origin. (xo,yo) is the \n"
      "                   location at which the magnitude and direction of the \n"
      "                   regional uniform flow are computed. \n"
      "\n"
      "   <yo>            The y-coordinate [L] of model origin. (xo,yo) is the \n"
      "                   location at which the magnitude and direction of the \n"
      "                   regional uniform flow are computed. \n"
      "\n"
      "   <k_alpha>       The log-mean of the aquifer hydraulic conductivity [ln(L/T)]. \n"
      "\n"
      "   <k_beta>        The log-standard deviation of the aquifer hydraulic \n"
      "                   conductivity [ln(L/T)]. \n"
      "\n"
      "   <k_count>       The number of equal probability intervals for k. \n"
      "\n"
      "   <h_alpha>       The log-mean of the aquifer thickness [ln(L)]. \n"
      "\n"
      "   <h_beta>        The log-standard deviation of the aquifer thickness [ln(L)]. \n"
      "\n"
      "   <h_count>       The number of equal probability intervals for h. \n"
      "\n"
      "   <radius>        The radius [L] of the buffer circle around of each pumping \n"
      "                   well. Any observations located within one or more well's \n"
      "                   buffer circle are excluded from the analysis. \n"
      "\n"
      "   <obs filename>  The name of the input observation file. \n"
      "\n"
      "   <well filename> The name of the input well file. \n"
      "\n"
      "   <out fileroot>  The filename root of the six output files. \n"
      "\n"
   << std::endl;

   std::cout <<
      "Example: \n"
      "   Gimiwan 100 200 2.2 0.2 10  2.3 0.10 10 100 obs.csv wells.csv results \n"
   << std::endl;

   std::cout <<
      "Observation File: \n"
      "   All of the observation head data are supplied by this .csv file. \n"
      "\n"
      "   The observation file contains no header line. The observation file may \n"
      "   include blank lines, which are ignored. The observation may include \n"
      "   comment lines, which are identified by an octothorpe (#) in the first \n"
      "   column of the line. \n"
      "\n"
      "   The observation file contains one line for each head observation. Each \n"
      "   line in the observation file has five fields. \n"
      "\n"
      "   <ID>            The observation identification string. The ID string can \n"
      "                   contain numbers, letters, underscores, and internal spaces. \n"
      "                   The ID may not contain commas. \n"
      "\n"
      "   <x>             The x-coordinate [L] of observation location. \n"
      "\n"
      "   <y>             The y-coordinate [L] of observation location. \n"
      "\n"
      "   <head>          The measured head [L]. The head is measured from the base \n"
      "                   of the aquifer. The head must be strictly positive. \n"
      "\n"
      "   <std dev>       The standard deviation of the measured head [L]. The \n"
      "                   standard deviation must be strictly positive. \n"
      "\n"
      "   Each of the five fields must separated by a single comma. Spaces and tabs \n"
      "   at the start and end of fields are trimmed. \n"
   << std::endl;

   std::cout <<
      "Well File: \n"
      "   All of the pumping well data are supplied by this .csv file. \n"
      "\n"
      "   The well file contains no header lines. The well file may include blank \n"
      "   lines, which are ignored. The well file may include comment lines, which \n"
      "   are identified by an octothorpe (#) in the first column of the line. \n"
      "\n"
      "   The well file contains one line for each pumping well. Each line in the \n"
      "   well file has five fields. \n"
      "\n"
      "   <ID>            The well identification string. The ID string can contain \n"
      "                   numbers, letters, underscores, and internal spaces. The ID \n"
      "                   may not contain commas. \n"
      "\n"
      "   <x>             The x-coordinate [L] of pumping well location. \n"
      "\n"
      "   <y>             The y-coordinate [L] of pumping well location. \n"
      "\n"
      "   <r>             The radius [L] of the well casing. The radius must be \n"
      "                   strictly positive. \n"
      "\n"
      "   <Q>             The discharge [L^3/T] (pumping rate) of the well. \n"
      "\n"
      "   Each of the five fields must separated by a single comma. Spaces and tabs \n"
      "   at the start and end of fields are trimmed. \n"
   << std::endl;

   std::cout <<
      "Output Files: \n"
      "   Gimiwan generates six identically formatted .csv files. Two files, \n"
      "\n"
      "      -- expected value file (_ev), and \n"
      "      -- standard deviation file (_sd), \n"
      "\n"
      "   are generated for each of the three hydrogrologic descriptors, \n"
      "\n"
      "      -- regional uniform recharge [L/T], \n"
      "      -- magnitude of the regional uniform flow [L^2/T] evaluated at (xo,yo), and \n"
      "      -- direction of the regional uniform flow [deg] evaluated at (xo,yo). \n"
      "\n"
      "   The names of the six file are constructed from the <out fileroot>. \n"
      "\n"
      "      <out fileroot>_recharge_ev.csv \n"
      "      <out fileroot>_recharge_sd.csv \n"
      "\n"
      "      <out fileroot>_magnitude_ev.csv \n"
      "      <out fileroot>_magnitude_sd.csv \n"
      "\n"
      "      <out fileroot>_direction_ev.csv \n"
      "      <out fileroot>_direction_sd.csv \n"
      "\n"
      "   Each file contains a recangular array of values, with k_count+1 rows \n"
      "   h_count+1 columns. The first row contains the h values (thicknesses) \n"
      "   associated with each column. The first column contains the k values \n"
      "   (conductivities) associated with each row. \n"
   << std::endl;

   std::cout <<
      "Notes: \n"
      "   o  The set of observations may include 'duplicates', i.e. multiple \n"
      "      observation values at the same location. However, there must be at \n"
      "      least 10 unique observation locations remaining after excluding all \n"
      "      of the observations within any pumping well's buffer circle. \n"
      "\n"
      "   o  The project name 'Gimiwan' is the Ojibwe word for the inanimate \n"
      "      intransitive verb 'it rains'. See [http://ojibwe.lib.umn.edu]. \n"
   << std::endl;

   std::cout <<
      "Authors: \n"
      "   R. Barnes, University of Minnesota \n"
      "   R. Soule,  Minnesota Department of Health \n"
   << std::endl;
}

//-----------------------------------------------------------------------------
void Usage() {
   std::cout <<
      "Usage: \n"
      "   Gimiwan <xo> <yo> <k alpha> <k beta> <k count> <h alpha> <h beta> <h count> <radius> <obs filename> <wells filename> <out fileroot> \n"
      "   Gimiwan --help \n"
      "   Gimiwan --version \n"
   << std::endl;
}

//-----------------------------------------------------------------------------
void Version() {
   std::cout << std::endl;
   std::cout << "Gimiwan (" << VERSION << ")" << std::endl;
   std::cout << std::endl;
}
