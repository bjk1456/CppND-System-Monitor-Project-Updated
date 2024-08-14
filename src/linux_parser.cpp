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
    //std::cout << "The line is " << line;
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
        //std::cout << "The pid is " << pid;
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
  //std::cout << "Yop";
  //std::regex value("[0-9]");
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
    //std::cout << "percent_mem_utilized" << percent_mem_utilized;

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
  //std::cout << "Yop";

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> system_uptime >> system_idle_time;
      system_uptime_long = stol(system_uptime);
      //std::cout << "Hello ... line is system_uptime" << system_uptime_long << "\n";
    }
  }
  //std::cout << "Going to return " << system_uptime_long << "\n";
  return system_uptime_long; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line;
  string system_uptime;
  string system_idle_time;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  //std::cout << "Yop";

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      //linestream >> system_uptime >> system_idle_time;
      //long system_uptime_long = stol(system_uptime);
      
      //std::cout << "Hello ... line is system_uptime" << line << "\n";
    }
  }
  return 0; 
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  float cpu_usage = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/stat");
        if (filestream.is_open()) {
            string line;
            
            vector<string> tokens;
            while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            //std::cout << pid << " KARLLLLLLLL";
            //std::cout << "Hello ... cmdline is " << line << "\n";
            std::string token;
            std::stringstream ss(line);
            while(std::getline(ss, token, ' ')) {
                //std::cout << token << '\n';
                tokens.push_back(token);
            }
            //proc.command = line;
            }
            long int start_time = stol(tokens[22]);
            long int aggregate_time = start_time / sysconf(_SC_CLK_TCK);
            /**
            std::cout << "The 22nd entry is " << tokens[22] << "\n";
            std::cout << "start_time is " << start_time << "\n";
            std::cout << "The aggregate time is = " << aggregate_time << "\n";
            std::cout << "ZIT";
            */
            //proc.up_time = aggregate_time;

            //Calculate Processor Utilization

            long up_time = LinuxParser::UpTime();
                        //std::cout << "Every";
            //std::cout << "Sir up_time is " << up_time;
            long int utime = stoul(tokens[14]);
            long int stime = stoul(tokens[15]);
            long int cutime = stoul(tokens[16]);
            long int cstime = stoul(tokens[17]);
            
            double total_time = 0;
            long int seconds = 0;
            //double cpu_usage;

            total_time = utime + stime + cutime + cstime;
            //std::cout << "total_time is " << total_time;
            seconds = aggregate_time - up_time;
            //std::cout << "up_time iz  " << up_time << " aggregate_time iz " << aggregate_time;
            //std::cout << "seconds is " << seconds;
            //std::cout << "feels are " <<  fella << " OK?";
            float cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds) * 100;
            //std::cout << "cpu_usage YANZ ... " << cpu_usage;
            return cpu_usage;
            //proc.cpu_usage = cpu_usage;
            
        }

            //#14 utime - CPU time spent in user code, measured in clock ticks
            //#15 stime - CPU time spent in kernel code, measured in clock ticks
            //#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
            //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
            //#22 starttime - Time when the process started, measured in clock ticks
  
  return cpu_usage; }



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
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

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
        //linestream >> key >> value;
        string name, size_str, unit;
        linestream >> name >> size_str >> unit;
        if(name == "VmSize:"){
            //std::cout << "99Cerl" << "\n";
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
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/stat");
    string line;
    long up_time = 0;
    //std::cout << "COUT IT HERE";
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        //std::cout << "Da LINE IS " << line;
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

  
  return 0; }
