#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <linux_parser.h>
using std::vector;

int main() {
  System system;
  /**
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
  */
  NCursesDisplay::Display(system);
}