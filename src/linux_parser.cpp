#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <iostream>
#include <regex>


using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

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
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  string key, size;
  string mem_total_str = "MemTotal";
  float mem_total_flt = 0;
  string mem_avail_str = "MemFree";
  float mem_avail_flt = 0;
  float percent_mem_utilized = 0;
  std::vector<string> keys;
  keys.push_back(mem_total_str);
  keys.push_back(mem_avail_str);
  std::regex raw(mem_total_str);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> size;

      for (string key : keys) {
        std::regex reg_key(key);
        if(regex_search(line, reg_key)){
          if(key == mem_total_str){
            mem_total_flt = stof(size);
          }
          if(key == mem_avail_str){
            mem_avail_flt = stof(size);
          }
        }
      }
        }
  }
  if((mem_total_flt > 0) && (mem_avail_flt > 0)){
    percent_mem_utilized = (mem_avail_flt / mem_total_flt);
  }

  return percent_mem_utilized; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string system_uptime;
  long system_uptime_long = 0;
  string system_idle_time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> system_uptime >> system_idle_time;
      system_uptime_long = stol(system_uptime);
    }
  }
  return system_uptime_long; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
  }


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, cpu, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >>cpu;

    while (linestream >> value) {
      jiffies.push_back(value);
    }
  }
  return jiffies;
}



// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    std::ifstream filestream(kProcDirectory + "/stat");
    string line;
    string key;
    string value;
    int num_procs = 0;
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes"){
        num_procs = stoi(value);
      }
    }
    }
  return num_procs; 
  }


// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {     
    std::ifstream filestream(kProcDirectory + "/stat");
    string line;
    string key;
    string value;
    int run_procs = 0;
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running"){
        run_procs = stoi(value);
      }
    }
    }
  return run_procs; 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string cmd = "";
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
      string line;
      if (filestream.is_open()) {
          while (std::getline(filestream, line)) {
          std::istringstream linestream(line);
          cmd = line;
          }
      }
  return cmd; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/status");
    string line;
    string key;
    string value;
    string ram = "0 MB";
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        string name, size_str, unit;
        linestream >> name >> size_str >> unit;
        if(name == "VmSize:"){
            int size = stoi(size_str);
            ram = std::to_string(size) + " MB";
            break;
        }
      }
    }
  return ram; 
  
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    string line;
    string real_uid;
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            //fields taken from https://man7.org/linux/man-pages/man5/proc_pid_status.5.html
            string row_heading, real_uid_s, effective_uid, saved_set_uid, filesystem_uid;
            if(row_heading == "Uid:"){
            linestream >> row_heading >> real_uid >> effective_uid >> saved_set_uid >> filesystem_uid;
            break;
            }
        }
    }
    
    
  return real_uid; }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    std::ifstream filestream(kPasswordPath);
    string username;
    int uid;
    string line;
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            std::stringstream ss(line);
            vector<string> res;
            string token;
            char delimiter = ':';
            while (std::getline(ss, token, delimiter)) {
                res.push_back(token);
                }
            //std::cout << "The uid is " << res[2] << " the username is " << res[0];
            if(pid == stoi(res[2])){
                username = res[0];
                break;
            }
            for(string s : res){
                //std::cout << "The token is " << s;
            }
        }
    }

  return username; 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/stat");
    string line;
    long up_time = 0;
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        std::stringstream ss(line);
        vector<string> res;
        string token;
        char delimiter = ' ';
        while (std::getline(ss, token, delimiter)) {
            res.push_back(token);
            }
            up_time = stol(res[22]);
      }
    }

  
  return up_time; }
