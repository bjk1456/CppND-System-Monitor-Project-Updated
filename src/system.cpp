#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <dirent.h>
#include <filesystem>
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
                    long int start_time = stol(tokens[22]);
                    long int aggregate_time = start_time / sysconf(_SC_CLK_TCK);

                    std::cout << "The 22nd entry is " << tokens[22] << "\n";
                    std::cout << "start_time is " << start_time << "\n";
                    std::cout << "The aggregate time is = " << aggregate_time << "\n";
                    std::cout << "ZIT";
                    proc.up_time = aggregate_time;

                    //Calculate Processor Utilization
 
                    long up_time = LinuxParser::UpTime();
                    //std::cout << "Every";
                    std::cout << "Sir up_time is " << up_time;
                    long int utime = stoul(tokens[14]);
                    std::cout << "Good";
                    long int stime = stoul(tokens[15]);
                    std::cout << "Boy";
                    long int cutime = stoul(tokens[16]);
                    std::cout << "Deserves";
                    long int cstime = stoul(tokens[17]);
                    std::cout << "Fudge";
                    
                    double total_time = 0;
                    long int seconds = 0;
                    //double cpu_usage;

                    total_time = utime + stime + cutime + cstime;
                    std::cout << "total_time is " << total_time;
                    seconds = aggregate_time - up_time;
                    std::cout << "up_time iz  " << up_time << " aggregate_time iz " << aggregate_time;
                    std::cout << "seconds is " << seconds;
                    double fella = (total_time / sysconf(_SC_CLK_TCK));
                    std::cout << "feels are " <<  fella << " OK?";
                    double sum_ting = (fella / seconds) * 100;
                    float cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds) * 100;
                    double tst = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
                    double sex = tst / seconds;
                    std::cout << "(total_time / sysconf(_SC_CLK_TCK)) == " << tst << " sum_ting " << sum_ting;
                    std::cout << "sex is " << 100 * sex;
                    std::cout << "cpu_usage YANZ ... " << cpu_usage;
                    proc.cpu_usage = cpu_usage;
                    


                    //#14 utime - CPU time spent in user code, measured in clock ticks
                    //#15 stime - CPU time spent in kernel code, measured in clock ticks
                    //#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
                    //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
                    //#22 starttime - Time when the process started, measured in clock ticks
                }
            }
            if (strcmp(file->d_name, "status") == 0){
                std::cout << "ITZZZZZZZZZZ STATUS!!!!";
                std::ifstream filestream(pid_dir + "/status");
                string name, size_str, unit;
                string line;
                if (filestream.is_open()) {
                    while (std::getline(filestream, line)) {
                        std::istringstream linestream(line);
                        linestream >> name >> size_str >> unit;
                        if(name == "VmSize:"){
                            std::cout << "Cerl" << "\n";
                            int size = stoi(size_str);
                            std::cout << size << "\n";
                            proc.ram = std::to_string(size) + " MB";
                        }
                        if(name == "Uid:"){
                            std::cout << "UIDDDDDD<< DD" << "\n";
                            std::cout << "size (which is the second value)" << size_str;
                            //map check
                            bool user_exists = 0;
                            for (const auto& [k, v] : users) {
                                if (users.count(stoi(size_str)) == 0){
                                    std::cout << "found value " << k << "\n";

                                    user_exists = 1;
                                    proc.user = users[stoi(size_str)];
                                }
                            }
                            if(!user_exists){
/**
                            if(users.count(size_str) > 0){

                            }
                            */
                        std::ifstream filestream("/etc/passwd");
                        string username, psswd;
                        int uid;
                        string line;
                        if (filestream.is_open()) {
                            while (std::getline(filestream, line)) {
                                std::istringstream linestream(line);
                                //linestream >> username >> psswd >> uid;
                                //std::cout << "NOMBRE is " << username << " uid is " << uid;

                                std::stringstream ss(line);
                                vector<string> res;
                                string token;
                                char delimiter = ':';
                                while (std::getline(ss, token, delimiter)) {
                                    res.push_back(token);
                                    }
                                std::cout << "The uid is " << res[2] << " the username is " << res[0];
                                if(size_str == res[2]){
                                    users[stoi(res[2])] = res[0];
                                    proc.user = res[0];
                                    std::cout << " It's ADDDDDDDDDDDDDDDDDDDDDD";
                                    user_exists = 1;
                                    break;
                                }
                                for(string s : res){
                                    std::cout << "The token is " << s;
                                }
                            }
                                }
                            }
                            
                        }
                        }
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