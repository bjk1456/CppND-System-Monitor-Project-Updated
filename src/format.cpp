#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hr = seconds/3600;                          //Number of seconds in an hour
  int mins = (seconds-hr*3600)/60;              //Remove the number of hours and calculate the minutes.
  int sec = seconds-hr*3600-mins*60;            //Remove the number of hours and minutes, leaving only seconds.
  sec %= 60;
  mins %= 60;
  hr %= 24; 
    return std::to_string(hr) + ":" + std::to_string(mins) + ":" + std::to_string(sec); 
}