#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  Process(int pid);  //constructor
bool operator<(Process const& other) const; 
//all the setters, better to put them in the private for protection of attribute change
void setCommand();
void setRam();
void setUser();
void setUpTime();
void calcuateCpuUtilization();

  // TODO: Declare any necessary private members
 private:
 int mPid;
 std::string mUser;
 std::string mCmd;
 std::string mRam;
 long int mUpTime;    
 float mCpuUtilization;
};

#endif