#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm> // for std::sort


#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
//intitlized as a empty vector, so need to populate the processes_ here, or is it better to put it as part of the constuctor? 
// for each pid, and try to parse the processes data, 
vector<Process>& System::Processes() { 
    
    //first clear the exsiting processes_ and then update
    if(!processes_.empty())
        processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        processes_.emplace_back(pid); //call process constructor
    }

    //sort the process based on the CPU utilization in the descending order, > operator has been overloaded
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());

    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }