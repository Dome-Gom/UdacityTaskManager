#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  Processor();
  float Utilization();  

 private:
 std::vector<std::string> processorDataParsed_;
 std::vector<long> processorData_, preProcessorData_;
};

#endif