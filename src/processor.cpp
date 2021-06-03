#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    //call LinuxParser::CpuUtilization()
    vector<string> cpuStats_str = LinuxParser::CpuUtilization();
    //conver the string to the long
    vector<long> cpuStats;
    for(const auto & str : cpuStats_str) {
        cpuStats.push_back(std::stol(str));
    }
    long totalT = cpuStats[LinuxParser::kUser_] + cpuStats[LinuxParser::kNice_] +
      cpuStats[LinuxParser::kSystem_] + cpuStats[LinuxParser::kIdle_] +
      cpuStats[LinuxParser::kIOwait_] + cpuStats[LinuxParser::kIRQ_] +
      cpuStats[LinuxParser::kSoftIRQ_] + cpuStats[LinuxParser::kSteal_];

    long idleT = cpuStats[LinuxParser::kIdle_] + cpuStats[LinuxParser::kIOwait_];
    
    long diffTotalT = totalT - prevTotalT;
    long diffIdle = idleT - prevIdleT;
    long upTime = diffTotalT - diffIdle;
    float utilization = upTime / diffTotalT;
    
    //update the member for more accurate calcuation 
    prevTotalT = totalT;
    prevIdleT = idleT;
    return utilization; }