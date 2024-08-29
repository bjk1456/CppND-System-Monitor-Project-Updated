#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds_value) {
  long sec_tot = seconds_value;
  double hour = (double) sec_tot / 3600.0; // seconds in hours
  double hour_int;
  double hour_fra = modf(hour, &hour_int); //split integer and decimal part of hours

  double minutes = hour_fra * 60; // decimal hours in minutes
  double minutes_int;
  double minutes_fra = modf(minutes, &minutes_int); //split integer and decimal part of minutes

  double seconds = minutes_fra * 60; //decimal minutes in seconds
  double seconds_int;
  modf(seconds, &seconds_int); //split integer and decimal part of minutes

  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << hour_int << ":" << std::setfill('0') << minutes_int << ":" << std::setfill('0') << std::setw(2) << seconds_int;
  string time_obs_def = ss.str();
  return time_obs_def;
}