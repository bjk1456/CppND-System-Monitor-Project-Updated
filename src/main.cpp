#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <linux_parser.h>
using std::vector;

int main() {
  System system;
  //system.Kernel();
  //std::cout << "The memory is " << system.MemoryUtilization();
  std::cout << "The system upTime ips " << system.UpTime();
  //system.Jiffies();
  LinuxParser::Pids();

  vector<Process>processes = system.Processes();
  std::cout << "Hello";
  for(Process p : processes){
    std::cout << "p.id == " << p.Pid() << "\n";
    std::cout << "p.Command() == " << p.Command() << "\n";
    std::cout << "p.UpTime) == " << p.UpTime() << "\n";
    std::cout << "p.CpuUtilization() == " << p.CpuUtilization() << "\n";
    std::cout << "p.User() == " << p.User() << "\n";
    std::cout << "p.Ram() == " << p.Ram() << "\n";
  }
  std::cout << "Da TotalProcesses is " << system.TotalProcesses();
  std::cout << "Da RunningProcesses is " << system.RunningProcesses();
  //NCursesDisplay::Display(system);
}