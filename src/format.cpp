#include <string>

#include "format.h"

using std::string;
using std::to_string;


// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    
    long hr = (seconds / 60) / 60;
    string hr_str = time2str(hr);
    long min = (seconds / 60) % 60;
    string min_str = time2str(min);
    long sec = seconds % 60;
    string sec_str = time2str(sec);
    string ret = hr_str + ":" + min_str + ":" + sec_str;
    return ret; }

string Format::time2str(long t) {
    if( t < 10) {
        return "0" + to_string(t);
    }
    else
        return to_string(t);
}