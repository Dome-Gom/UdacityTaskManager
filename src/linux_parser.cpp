#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
    filestream.close();
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memTotal = 1;
  float memFree = 1;
  std::ifstream filestream (kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "MemTotal:" && value != "") memTotal = std::stof(value);
        if (key == "MemFree:"  && value != "") memFree = std::stof(value);
      }
    }
    filestream.close();
  }
  return ((memTotal-memFree)/memTotal);
}

long LinuxParser::UpTime() { 
  string line;
  string value;
  long uptime = 0;
  std::ifstream filestream (kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    if(value != "") uptime = std::stoi(value);
    filestream.close();
  }
  return uptime;
}

void LinuxParser::ProcessCpuUsage(std::vector<long> &cpuUsage, int pid){
  string line;
  string value;
  int nrValue = 1;
  std::ifstream filestream (kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(nrValue < 23){
      linestream >> value;
      if(value != ""){
        if (nrValue == 14) cpuUsage[ProcessCPUUsage::kUtime_14] = stol(value);
        else if (nrValue == 15) cpuUsage[ProcessCPUUsage::kStime_15] = stol(value);
        else if (nrValue == 16) cpuUsage[ProcessCPUUsage::kCutime_16] = stol(value);
        else if (nrValue == 17) cpuUsage[ProcessCPUUsage::kCstime_17] = stol(value);
        else if (nrValue == 22) cpuUsage[ProcessCPUUsage::kStartTime_22] = stol(value);
        nrValue++;
      }
    }
    filestream.close();
  }
  return;
}

vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string temp;
  vector<string> values;
  std::ifstream filestream (kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu"){
        for(int i=0; i<10; i++){
          linestream >> temp;
          values.push_back(temp);
        }
      }
    }
    filestream.close();
  }
  return values;
}

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int totalProcesses = -1;
  std::ifstream filestream (kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes" && value != "") totalProcesses = std::stoi(value);
    }
    filestream.close();
  }
  return totalProcesses;
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int runningProcesses = -1;
  std::ifstream filestream (kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running" && value != "") runningProcesses = std::stoi(value);
    }
    filestream.close();
  }
  return runningProcesses;
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream (kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    filestream.close();
  }
  return line; 
}

string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  string ramSize;
  std::ifstream filestream (kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") ramSize = value;
    }
    filestream.close();
  }
  return ramSize;
}

string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  string uid {};
  std::ifstream filestream (kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") uid = value;
    }
    filestream.close();
  }
  return uid;
}

string LinuxParser::User(int pid) { 
  string line;
  string name;
  string placeholder;
  string currentUid;
  string user {};
  string uid = Uid(pid);
  std::ifstream filestream (kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> placeholder >> currentUid) {
        if (currentUid == uid) {
          user = name;
        }
      }
    }
    filestream.close();
  }
  return user;
}

long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  int nrValue = 1;
  long upTime = -1;
  std::ifstream filestream (kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(nrValue < 23){
      linestream >> value;
      if (nrValue == 22 && value != "") upTime = stol(value);
      nrValue++;
    }
    filestream.close();
  }
  return upTime; 
}