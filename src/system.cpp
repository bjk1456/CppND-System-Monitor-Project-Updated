#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <dirent.h>
#include <string>
#include <iostream>
#include <map>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"
#include "system.h"
#include <iostream>
#include <sstream>

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::stoi;
//using std::filesystem;


// TODO: Return the system's CPU
Processor& System::Cpu() {
    return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    std::map<int, std::string> users;
    vector<int> pids = LinuxParser::Pids();
    for(int pid: pids){
        Process proc;
        proc.pid = pid;
        string pid_dir = kProcDirectory.c_str() + std::to_string(pid);
        DIR* directory = opendir(pid_dir.c_str());
               if( directory == NULL)
    {
            string error_msg = "Could not open directory:" + kProcDirectory + std::to_string(pid);
            fprintf(stderr, "%s", error_msg.c_str());
            break;

    }
          
          proc.command = LinuxParser::Command(pid);
          proc.user = LinuxParser::User(pid);
          //proc.cpu_usage = LinuxParser::CpuUtilization();
          proc.ram = LinuxParser::Ram(pid);
          proc.up_time = LinuxParser::UpTime();
          processes_.emplace_back(proc);
          std::sort(processes_.rbegin(), processes_.rend());
    }
    
    return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
     return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }