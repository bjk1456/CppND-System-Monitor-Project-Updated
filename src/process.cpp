#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include <processor.h>
#include <linux_parser.h>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float process_totaltime = (float) LinuxParser::ActiveJiffies(Pid()) / (float) sysconf(_SC_CLK_TCK);  
    float process_startime = (float) LinuxParser::UpTime(Pid()); 
    float system_uptime = (float) LinuxParser::UpTime(); 
    float process_uptime = system_uptime - process_startime;
    
    return process_totaltime / process_uptime;
}

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram(){ 
    return LinuxParser::Ram(pid); 
    }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpu_usage < a.cpu_usage;
     }