#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    int hh, mm, ss;
    std::ostringstream oString;
    std::string output;
    hh = seconds/3600;
    mm = (seconds - (hh * 3600)) / 60;
    ss = (seconds - (hh * 3600) - (mm * 60));
    oString << std::setfill('0') << std::setw(2) << hh << ":";
    oString << std::setfill('0') << std::setw(2) << mm << ":";
    oString << std::setfill('0') << std::setw(2) << ss;
    output = oString.str();
    return output;
}