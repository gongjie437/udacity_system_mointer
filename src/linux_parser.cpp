#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream> //for istringstream

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::getline;


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memUtilization = 0.0f;
  string line;
  string memType, unit;
  long memTotal, memFree, memUsed;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> memType >> memTotal >> unit;

    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> memType >> memFree >> unit;
    memUtilization = (memTotal - memFree ) / memTotal;
  }
  return memUtilization; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime = 0;
  long idleTime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return upTime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    long activeJiffies, user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string cpu, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line); //get the first line
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >>sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    activeJiffies = user + nice + sys;
  }
  return activeJiffies; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idleJiffies, user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string cpu, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line); //get the first line
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >>sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    idleJiffies = idle + iowait;
  }

  return idleJiffies; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  //return the first line of /proc/stat in a vector string line
  vector<string> ret;
  string line, processes;
  string cpuKey, user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename); 
  if (stream.is_open()) {
    while(getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpuKey >> user >> nice >>sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
        if (cpuKey == "cpu")
        {
          ret.push_back(user);
          ret.push_back(nice);
          ret.push_back(sys);
          ret.push_back(idle); 
          ret.push_back(iowait);
          ret.push_back(irq);
          ret.push_back(softirq);
          ret.push_back(steal); 
          ret.push_back(guest);
          ret.push_back(guest_nice);
          return ret;                          
        }
      }
    }
  } 
  return ret; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProc = 0;
  string line, processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line)) {
      if (line.substr(0, 9) == "processes")
      {
        std::istringstream linestream(line);
        linestream >> processes >> totalProc;
        break;
      }
    }
  }
   return totalProc; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  int totalRunningProc = 0;
  string line, processes;
  const string key = "procs_running";
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    while(std::getline(stream, line)) {
      if (line.substr(0, key.length()) == key)
      {
        std::istringstream linestream(line);
        linestream >> processes >> totalRunningProc;
        return totalRunningProc;
      }
    }
  }
   return totalRunningProc; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
    std::getline(stream, line);
  return line;  // is stream is not present, line will be ""; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string ram; 
  string line, vmsizeString;
  const string key = "Vmsize";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
     while(std::getline(stream, line)) {
      if (line.substr(0, key.length()) == key)
      {
        std::istringstream linestream(line);
        linestream >> vmsizeString >> ram;
        break;
      }
  }
  }
  //the unit is KB, convert to MB for better display
  if(!ram.empty())
  {
    int ram_MB = std::stol(ram)/1000;
    ram = to_string(ram_MB);
  }
  return ram; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//For the purposes of this project, you simply need to capture the first integer on the "Uid:" line.
string LinuxParser::Uid(int pid) { 
  string ret;
  int uid = -1;
  string line, uidString;
  const string key = "Uid:";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
     while(std::getline(stream, line)) {
       if (line.substr(0, key.length()) == key) {
        std::istringstream linestream(line);
        linestream >> uidString >> uid;
        break;
      }
     }
  }
  if(uid != -1)
  {
    ret = to_string(uid);
  }
  return ret; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  //first get Uid string, and then find the username
  string uid = Uid(pid);
  string line, ret, tmp1, tmp2;
  // find user name in /etc/passwd
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while(getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' '); //replace : with space
      std::istringstream linestream(line);
      while(linestream >> ret >> tmp1 >> tmp2 ) {
        if(tmp2 == uid)
          return ret;
      }
    }
  }
  return ret; }

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, str;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  long uptime = 0, tmp;
  const int index = 21; //it is the 21th of the long line
  if (stream.is_open())
  {
    // such a long line
    std::getline(stream, line);
    std::istringstream linestream(line);
    //parse the string by space
    for(int i = 0; i < index; i++) {
      linestream >> str;
    }
    uptime = std::stol(str) / sysconf(_SC_CLK_TCK); //convert from "clock ticks" to seconds
  }
  
  return uptime; }


//newly added function, return cpu usage of a specific process
//reference is here https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
vector<float> LinuxParser::CpuUtilization(int pid)
{
  vector<float> cpuStats;
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  const int index1 = 14, index2 = 15, index3 = 16, index4 = 17, index5 = 21;
  if (stream.is_open()) {
     // such a long line
    getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 0; i < index5; ++i ) {
      linestream >> value;
      if(i == index1 || i == index2 || i == index3 || i == index4 || i == index5 ) {
        cpuStats.push_back(std::stof(value) / sysconf(_SC_CLK_TCK));
      }
    } 
  }
  return cpuStats; }

