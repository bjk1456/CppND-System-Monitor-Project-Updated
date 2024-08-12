#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <dirent.h>
#include <filesystem>
#include <string>
#include <iostream>

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
    //vector<int> pids;

    //DIR* directory = opendir(kProcDirectory.c_str());
    //struct dirent* file;
    //while ((file = readdir(directory)) != nullptr) {

    vector<int> pids = LinuxParser::Pids();
    for(int pid: pids){
        Process proc;
        std::cout << "The pid is " << pid << "\n";
        proc.pid = pid;
        string pid_dir = kProcDirectory.c_str() + std::to_string(pid);
        DIR* directory = opendir(pid_dir.c_str());
        struct dirent* file;
        while ((file = readdir(directory)) != nullptr) {
            std::cout << file->d_name << "\n";
    // Is this a directory?
            if (strcmp(file->d_name, "cmdline") == 0){
                std::cout << "file->d_name: " << file->d_name;

                std::ifstream filestream(pid_dir + "/cmdline");
                string line;
                if (filestream.is_open()) {
                    while (std::getline(filestream, line)) {
                    std::istringstream linestream(line);
                    std::cout << pid << " PIZZZ";
                    std::cout << "Hello ... cmdline is " << line << "\n";
                    proc.command = line;
                    }
                }
            }
                if (strcmp(file->d_name, "stat") == 0){
                std::cout << "stat has been found";
                std::cout << file->d_name;

                std::ifstream filestream(pid_dir + "/stat");
                string line;
                if (filestream.is_open()) {
                    vector<string> tokens;
                    while (std::getline(filestream, line)) {
                    std::istringstream linestream(line);
                    std::cout << pid << " KARLLLLLLLL";
                    std::cout << "Hello ... cmdline is " << line << "\n";
                    std::string token;
                    std::stringstream ss(line);
                    while(std::getline(ss, token, ' ')) {
                        std::cout << token << '\n';
                        tokens.push_back(token);
                    }
                    //proc.command = line;
                    }
                    long int myint1 = stol(tokens[22]);
                    long int total_time = myint1 / sysconf(_SC_CLK_TCK);

                    std::cout << "The 22nd entry is " << tokens[22] << "\n";
                    std::cout << "myint1 is " << myint1 << "\n";
                    std::cout << "The total time is " << total_time << "\n";
                    proc.up_time = total_time;
                }
            }
  }
          processes_.push_back(proc);

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
std::string System::OperatingSystem() { return string(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }