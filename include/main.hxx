#include <iostream>
#include <fstream>
#include <filesystem>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/program_options.hpp>

#include "application.hxx"
#include "mbtypes.hxx"
#include "mbsystem.hxx"


namespace pt = boost::property_tree;
namespace fs = std::filesystem;
namespace po = boost::program_options;
