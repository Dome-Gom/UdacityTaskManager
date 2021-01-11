#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using LinuxParser::ProcessCPUUsage;

Process::Process(int pid): pid_(pid), hertz_(sysconf(_SC_CLK_TCK)){
    processCpuUsage_ = {0, 0, 0, 0, 0};
    cpuUsage_ = CpuUtilization();
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { 
    float totalTime, upTimeProcess;
    LinuxParser::ProcessCpuUsage(processCpuUsage_, pid_);
    totalTime = processCpuUsage_[ProcessCPUUsage::kUtime_14] + processCpuUsage_[ProcessCPUUsage::kStime_15];
    upTimeProcess = LinuxParser::UpTime() - ((float)processCpuUsage_[ProcessCPUUsage::kStartTime_22]/(float)hertz_);
    cpuUsage_ = ((totalTime / hertz_)/ upTimeProcess); 
    return cpuUsage_;
}

string Process::Command() { 
    string fullCommand = LinuxParser::Command(pid_);
    return fullCommand.substr(0,40) + "...";
}

string Process::Ram() { 
    string ram_kb = LinuxParser::Ram(pid_);
    float ram_mb = 0;
    if (ram_kb != "") ram_mb = stof(ram_kb)/1000;
    std::stringstream ram_mb_low_precision;
    ram_mb_low_precision << std::setprecision(2) << ram_mb;
    return ram_mb_low_precision.str(); 
}

string Process::User() { 
    return LinuxParser::User(pid_);
}

long int Process::UpTime() { 
    return LinuxParser::UpTime()-(LinuxParser::UpTime(pid_)/hertz_); 
}

bool Process::operator<(const Process& a) const { 
    return cpuUsage_ > a.cpuUsage_;
}