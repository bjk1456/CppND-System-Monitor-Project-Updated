#include "processor.h"
#include <linux_parser.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long value_total = LinuxParser::Jiffies();
    long value_active = LinuxParser::ActiveJiffies();
    return value_active * (1.f / value_total);
}