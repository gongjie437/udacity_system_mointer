#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int curPid)
{
    mPid = curPid;
    // then need to parse data
    setCommand();
    setRam();
    setUser();
    setUpTime();
    calcuateCpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return mPid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return mCpuUtilization; }

// TODO: Return the command that generated this process
string Process::Command() { return mCmd; }

// TODO: Return this process's memory utilization
string Process::Ram() { return mRam; }

// TODO: Return the user (name) that generated this process
string Process::User() { return mUser; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return mUpTime; }

//all the setter
void Process::setCommand() { 
    mCmd = LinuxParser::Command(mPid);
    }

void Process::setRam() { 
    mRam = LinuxParser::Ram(mPid); //LinuxParser::Ram(int pid) retun a string in kb
    }

void Process::setUser() { 
    mUser = LinuxParser::User(mPid); 
    }

void Process::setUpTime() { 
     mUpTime = LinuxParser::UpTime(mPid);
    }

//reference is here https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
void Process::calcuateCpuUtilization() { 
     long uptime =  LinuxParser::UpTime(mPid);
     auto cpuStats = LinuxParser::CpuUtilization(mPid);
     if(cpuStats.size() == 5) {
         float startT = cpuStats[4];
         float totalT = cpuStats[0] + cpuStats[1] + cpuStats[2] + cpuStats[3];
         mCpuUtilization = totalT / (uptime - startT);
     }
     else 
        mCpuUtilization = 0.0f;
    }

// TODO: Overload the "greater than" comparison operator for Process objects in descending order
bool Process::operator>(Process const& other) const { 
    return this->mCpuUtilization > other.mCpuUtilization; }