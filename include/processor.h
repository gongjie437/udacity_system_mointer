#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor() : prevTotalT(0), prevIdleT(0) {}; //constructor

  // TODO: Declare any necessary private members
 private:
 long prevTotalT;
 long prevIdleT;
};

#endif