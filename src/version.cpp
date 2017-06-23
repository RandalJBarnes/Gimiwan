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
//    23 June 2017
//=============================================================================
#include <iostream>

#include "version.h"

namespace {
const char* VERSION = "23 June 2017 [Beta]";
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
   std::cout << "   Identify influential observation head data using a boomerang approach "     << std::endl;
   std::cout << "   and the quadratic discharge potential model. "                              << std::endl;
   std::cout                                                                                    << std::endl;
/*
   std::cout << "Method: "                                                                      << std::endl;
   std::cout << "   With the boomerang approach we march through our set of N observation "     << std::endl;
   std::cout << "   head data. One by one, we temporarily remove the observation from the "     << std::endl;
   std::cout << "   data set. With this reduced data set we fit the quadratic discharge "       << std::endl;
   std::cout << "   potential model using least squares. We quantify the influence of an "      << std::endl;
   std::cout << "   observation by comparing the fitted models computed with and without "      << std::endl;
   std::cout << "   the observation. "                                                          << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Aquifer: "                                                                     << std::endl;
   std::cout << "   The model aquifer is homogeneous, isotropic, and rests on a horizontal "    << std::endl;
   std::cout << "   impermeable base. "                                                         << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Heads and Discharge Potentials: "                                              << std::endl;
   std::cout << "   All piezometric heads are measured from the base of the aquifer. A "        << std::endl;
   std::cout << "   measured head at location (x,y), head(x,y) [L], is characterized by an "    << std::endl;
   std::cout << "   expected value, E[head(x,y)], and a standard deviation, Std[head(x,y)]. "   << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The discharge potential at location (x,y), Phi(x,y) [L^3/T], is a "         << std::endl;
   std::cout << "   function of the head, head(x,y) [L], the hydraulic conductivity, k [L/T], " << std::endl;
   std::cout << "   and the aquifer thickness, H [L]. If the head is greater than the aquifer " << std::endl;
   std::cout << "   thickness, the aquifer is confined and "                                    << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      Phi(x,y) = k*H*(head(x,y) - H/2) "                                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   If the head is less than the aquifer thickness, the aquifer is unconfined " << std::endl;
   std::cout << "   and "                                                                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      Phi(x,y) = (1/2)*k*head(x,y)^2 "                                         << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   See Strack et al. (2006). "                                                 << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Using a first-order second moment approximation (see Benjamin and "         << std::endl;
   std::cout << "   Cornell, 1970, Section 2.4.4) we can write the expect values and standard " << std::endl;
   std::cout << "   deviations of the discharge potentails as functions of the expect values "  << std::endl;
   std::cout << "   and standard deviations of the head. If the head is greater than the "      << std::endl;
   std::cout << "   aquifer thickness, the aquifer is confined and "                            << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      E[Phi(x,y)] = k*H*(E[head(x,y)] - H/2) "                                 << std::endl;
   std::cout << "      Std[Phi(x,y)] = k*H*Std[head(x,y)] "                                     << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   If the head is less than the aquifer thickness, the aquifer is unconfined " << std::endl;
   std::cout << "   and "                                                                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      E[Phi(x,y)] = (1/2)*k*(E(head(x,y))^2 + Std[head(x,y)]^2 "               << std::endl;
   std::cout << "      Std[Phi(x,y)] = k*E[head(x,y)]*Std[head(x,y)] "                          << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Model: "                                                                       << std::endl;
   std::cout << "   The quadratic discharge potential model combines regional uniform "         << std::endl;
   std::cout << "   recharge, regional uniform flow, and a set of M pumping wells. "            << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      Phi(x,y) = AX^2 + BY^2 + CXY + DX + EY + F + Wells(x,y) "                << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   where "                                                                     << std::endl;
   std::cout << "      -- Phi(x,y) is the discharge potential at location (x,y), "              << std::endl;
   std::cout << "      -- {A, B, C, D, E, F} are six fitted parameters, "                       << std::endl;
   std::cout << "      -- X = x - xo, where (xo,yo) is the model origin, "                      << std::endl;
   std::cout << "      -- Y = y - yo, and "                                                     << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "                       M  Qw(j)                                  "             << std::endl;
   std::cout << "      -- Wells(x,y) = sum ---- log( (x-xw(j))^2 + (y-yw(j))^2 )  "             << std::endl;
   std::cout << "                      j=1 4*pi                                   "             << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   with Qw(j) = the discharge of well (j), and [xw(j),yw(j)] is the "          << std::endl;
   std::cout << "   location of well (j). "                                                     << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Weighted Least Squares: "                                                      << std::endl;
   std::cout << "   The six model parameters, {A, B, C, D, E, F}, are fitted using weighted "   << std::endl;
   std::cout << "   least squares. The residuals are computed in terms of the discharge "       << std::endl;
   std::cout << "   potentials "                                                                << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      residual = Phi - E[Phi] "                                                << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   and the weights are one over the Std[Phi]^2. "                              << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Influence: "                                                                   << std::endl;
   std::cout << "   The influences of an observation are characterized using three "            << std::endl;
   std::cout << "   geohydrologic descriptors evaluated at the model origin (xo,yo): "          << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      -- regional uniform recharge, "                                          << std::endl;
   std::cout << "      -- magnitude of the regional uniform flow, and "                         << std::endl;
   std::cout << "      -- direction of the regional uniform flow. "                             << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Each of these three geohydrologic descriptors are computed as simple "      << std::endl;
   std::cout << "   functions of the six fitted parameters. "                                   << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      recharge = -2*(A+B) "                                                    << std::endl;
   std::cout << "      magnitude = sqrt(D^2 + E^2) "                                            << std::endl;
   std::cout << "      direction = atan2(-E, -D) "                                              << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The influence of observation 'j' on descriptor 'D' is computed as "         << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "      Influence = sqrt(N) * (D(j) - D()) / Std(D) "                            << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   where "                                                                     << std::endl;
   std::cout << "      --- N is the number of observation head data. "                          << std::endl;
   std::cout << "      --- D(j) denotes the expected value of descriptor 'D' computed "         << std::endl;
   std::cout << "          excluding observation 'j'. "                                         << std::endl;
   std::cout << "      --- D() denotes the expected value of descriptor 'D' computed using "    << std::endl;
   std::cout << "          all of the observations. "                                           << std::endl;
   std::cout << "      --- Std(D) is the standard deviation of descriptor 'D' computed using "  << std::endl;
   std::cout << "          all of the observations. "                                           << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   A influence value that is less than -2 or greater than +2 is significant."  << std::endl;
   std::cout << "   See Belsey et al. (1980), Section 2.1, p. 27-29. "                          << std::endl;
   std::cout                                                                                    << std::endl;

   Usage();
   std::cout << "Arguments: "                                                                   << std::endl;
   std::cout << "   <xo>            The x-coordinate [L] of model origin. (xo,yo) is the "      << std::endl;
   std::cout << "                   location at which the magnitude and direction of the "      << std::endl;
   std::cout << "                   regional uniform flow are computed. "                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <yo>            The y-coordinate [L] of model origin. (xo,yo) is the "      << std::endl;
   std::cout << "                   location at which the magnitude and direction of the "      << std::endl;
   std::cout << "                   regional uniform flow are computed. "                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <conductivity>  The hydraulic conductivity [L/T] of the aquifer. "          << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <thickness>     The thickness of the aquifer. Where the head is greater "   << std::endl;
   std::cout << "                   the thickness the model is confined. Where the head is "    << std::endl;
   std::cout << "                   less than the thickness the model is unconfined. "          << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <radius>        The radius [L] of the buffer circle around of each "        << std::endl;
   std::cout << "                   pumping well. Any observation head data located within "    << std::endl;
   std::cout << "                   any well's buffer circle is excluded from the analysis."    << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <obs file>      The name of the file (including any necessary path "        << std::endl;
   std::cout << "                   information and the .csv file extension) containing the "   << std::endl;
   std::cout << "                   observation head data. (See below for details.) "           << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <well file>     The name of the file (including any necessary path "        << std::endl;
   std::cout << "                   information and the .csv file extension) containing the "   << std::endl;
   std::cout << "                   well data. (See below for details.) "                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <output file>   The name of the file (including any necessary path "        << std::endl;
   std::cout << "                   information and the file extension) where Nagadan will "    << std::endl;
   std::cout << "                   write all of the program results. If the specified file "   << std::endl;
   std::cout << "                   already exists, it will overwritten without warning. "      << std::endl;
   std::cout << "                   (See below for details.) "                                  << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Example: "                                                                     << std::endl;
   std::cout << "   Nagadan 2250 -2250 10 10 100 my_obs.csv my_wells.csv my_output.csv"         << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "<obs file>: "                                                                  << std::endl;
   std::cout << "   All of the observation head data are supplied by the <obs file>. The "      << std::endl;
   std::cout << "   <obs file> must be a valid .csv format, using a single comma as the "       << std::endl;
   std::cout << "   field delimiter."                                                           << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The <obs file> contains NO header lines. The <obs file> may include "       << std::endl;
   std::cout << "   blank lines. The <obs file> may include comment lines. Comment lines "      << std::endl;
   std::cout << "   are identified by an octothorpe (#) or an exclamation mark (!) in the "     << std::endl;
   std::cout << "   first column of the line."                                                  << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The <obs file> contains one line for each observation. Each observation "   << std::endl;
   std::cout << "   line has five fields. "                                                     << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <ID>            The specified observation unique identification string."    << std::endl;
   std::cout << "                   The ID string can contain numbers, letters, underscores, "  << std::endl;
   std::cout << "                   and internal spaces. The ID may NOT contain commas."        << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <x>             The x-coordinate [L] of observation location. "             << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <y>             The y-coordinate [L] of observation location. "             << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <head>          The measured head [L] at location (x,y). The head is "      << std::endl;
   std::cout << "                   is measured from the based of the aquifer. The <head> "     << std::endl;
   std::cout << "                   must be a strictly positive."                               << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <std dev>       The standard deviation of the measured head [L] at "        << std::endl;
   std::cout << "                   location (x,y). The <std dev> must be strictly positive. "  << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Each of the five fields must delimited by a single comma, or a single "     << std::endl;
   std::cout << "   comma and one or more spaces. Spaces and tabs at the start and end of "     << std::endl;
   std::cout << "   fields are trimmed. "                                                       << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "<well file>: "                                                                 << std::endl;
   std::cout << "   All of the pumping well data are supplied by the <well file>. The "         << std::endl;
   std::cout << "   <well file> must be a valid .csv format, using a single comma as the "      << std::endl;
   std::cout << "   field delimiter."                                                           << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The <well file> contains NO header lines. The <well file> may include "     << std::endl;
   std::cout << "   blank lines. The <well file> may include comment lines. Comment lines "     << std::endl;
   std::cout << "   are identified by an octothorpe (#) or an exclamation mark (!) in the "     << std::endl;
   std::cout << "   first column of the line."                                                  << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The <well file> contains one line for each pumping well. Each line has "    << std::endl;
   std::cout << "   five fields. "                                                              << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <ID>            The specified observation unique identification string."    << std::endl;
   std::cout << "                   The ID string can contain numbers, letters, underscores, "  << std::endl;
   std::cout << "                   and internal spaces. The ID may NOT contain commas."        << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <x>             The x-coordinate [L] of pumping well location. "            << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <y>             The y-coordinate [L] of pumping well location. "            << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <r>             The radius [L] of the well casing. The <r> must be "        << std::endl;
   std::cout << "                   strictly positive."                                         << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <Q>             The discharge [L^3/T] (pumping rate) of the well. "         << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Each of the five fields must delimited by a single comma, or a single "     << std::endl;
   std::cout << "   comma and one or more spaces. Spaces and tabs at the start and end of "     << std::endl;
   std::cout << "   fields are trimmed. "                                                       << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "<output file>:"                                                                << std::endl;
   std::cout << "   All of the program results go to the <output file>. The <output file> "     << std::endl;
   std::cout << "   is written in a valid .csv format, using a single comma as the field "      << std::endl;
   std::cout << "   the field delimiter."                                                       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The <output file> contains one standard header line with nine comma "       << std::endl;
   std::cout << "   separated text fields containing the field titles. "                         << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Following the header line there are two lines reporting the expected "      << std::endl;
   std::cout << "   values (exp val) and standard deviations (std dev) for the three "          << std::endl;
   std::cout << "   geohydrologic descriptors computed using all of the observation data. "     << std::endl;
   std::cout << "   The <x> and <y> entries for these two rows are the model origin (xo,yo). "  << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   The rest of the <output file> comprises one line for each observation."     << std::endl;
   std::cout << "   Each line has ten fields. "                                                 << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <ID>            The observation identification string. "                    << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <x>             The x-coordinate for the location of the observation. "     << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <y>             The y-coordinate for the location of the observation. "     << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <recharge>      The expected value of the regional uniform recharge "       << std::endl;
   std::cout << "                   [L/T], computed with the observation excluded. "            << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <magnitude>     The expected value of the magnitude of the regional "       << std::endl;
   std::cout << "                   uniform flow [L^2/T] evaluated at the model origin "        << std::endl;
   std::cout << "                   (xo,yo) computed with the observation excluded. "           << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <direction>     The expected value of the direction of the regional "       << std::endl;
   std::cout << "                   uniform flow [deg] evaluated at the model origin (xo,yo), " << std::endl;
   std::cout << "                   computed with the observation excluded. "                   << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <influence_recharge>  The influence of the observation on recharge []. "    << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <influence_magnitude> The influence of the observation on magnitude []. "   << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   <influence_direction> The influence of the observation on direction []. "   << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   If an observation is not active (i.e. the observation location falls "      << std::endl;
   std::cout << "   inside the buffer circle of one or more pumping well) only the first "      << std::endl;
   std::cout << "   three field (<ID>, <x>, <y>) are filled. "                                  << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Notes: "                                                                       << std::endl;
   std::cout << "   o  The set of observations may include 'duplicates', i.e. multiple "        << std::endl;
   std::cout << "      observation values at the same location. However, there must be at "     << std::endl;
   std::cout << "      at least 10 unique observation locations remaining after excluding "     << std::endl;
   std::cout << "      all of the observations within any pumping well's buffer circle. "       << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   o  The project name 'Nagadan' is the Ojibwe word for the inanimate "        << std::endl;
   std::cout << "      transitive verb 'leave it behind'. See [http://ojibwe.lib.umn.edu]. "    << std::endl;
   std::cout << "      This name seems appropriate for a program using a boomerang approach "   << std::endl;
   std::cout << "      to identify influential data."                                           << std::endl;
   std::cout                                                                                    << std::endl;

   std::cout << "Reference: " << std::endl;
   std::cout << "   David A. Belsey, Edwin Kuh, and Roy E. Welsch, 1980, 'Regression "          << std::endl;
   std::cout << "   Diagnostics: Identifying Influential Data and Sources of Collinearity', "   << std::endl;
   std::cout << "   John Wiley & Sons, 292 pp., ISBN 0-471-05856-4. "                           << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Jack R. Benjamin and C. Allin Cornell, 1970, 'Proabability, Statistics, "   << std::endl;
   std::cout << "   and Decision for Civil Engineers', McGraw-Hill Book Company, 684 pp., "     << std::endl;
   std::cout << "   ISBN 07-004549-6. "                                                         << std::endl;
   std::cout                                                                                    << std::endl;
   std::cout << "   Otto D.L. Strack, Randal J. Barnes, and Arnold Verruijt, 2006, "            << std::endl;
   std::cout << "   'Vertically Integrated FLows, Discharge Potential, and the Dupuit- "        << std::endl;
   std::cout << "   Forchheimer Approximation', Groundwater, v. 44, n. 1, p. 72-75."            << std::endl;
   std::cout                                                                                    << std::endl;
*/

   std::cout << "Authors:" << std::endl;
   std::cout << "   R. Barnes, University of Minnesota          "  << std::endl;
   std::cout << "   R. Soule,  Minnesota Department of Health   "  << std::endl;
   std::cout << std::endl;
}

//-----------------------------------------------------------------------------
void Usage() {
   std::cout << "Usage:" << std::endl;
   std::cout << "   Gimiwan <xo> <yo> <k alpha> <k beta> <k count> <h alpha> <h beta> <h count> <radius> <obs file> <wells file> <output root>" << std::endl;
   std::cout << "   Gimiwan --help" << std::endl;
   std::cout << "   Gimiwan --version" << std::endl;
   std::cout << std::endl;
}

//-----------------------------------------------------------------------------
void Version() {
   std::cout << std::endl;
   std::cout << "Gimiwan (" << VERSION << ")" << std::endl;
   std::cout << std::endl;
}
