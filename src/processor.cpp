#include "processor.h"
#include "linux_parser.h"

Processor::Processor(){
 processorData_ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 preProcessorData_ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

float Processor::Utilization() { 
    /*Processor Data description
    ---
    [0]     [1]     [2]     [3]     [4]     [5] [6]     [7]     [8]     [9]
    user    nice    system  idle    iowait  irq softirq steal   guest   guest_nice
    */
    long preIdle, idle, preNonIdle, nonIdle, preTotal, total, totalD, idleD;

    processorDataParsed_ = LinuxParser::CpuUtilization();
    preProcessorData_ = processorData_;
    processorData_.clear();

    for(int i = 0; i < 10; i++){
        processorData_.push_back(std::stol(processorDataParsed_[i]));
    }

    preIdle = preProcessorData_[3]+preProcessorData_[4];
    idle = processorData_[3]+processorData_[4];

    preNonIdle = preProcessorData_[0], preProcessorData_[1], preProcessorData_[2], preProcessorData_[5], preProcessorData_[6], preProcessorData_[7];
    nonIdle = processorData_[0], processorData_[1], processorData_[2], processorData_[5], processorData_[6], processorData_[7];

    preTotal = preIdle + preNonIdle;
    total = idle + nonIdle;

    totalD = total - preTotal;
    idleD = idle - preIdle;

    return (float)(totalD-idleD)/(float)totalD;
}