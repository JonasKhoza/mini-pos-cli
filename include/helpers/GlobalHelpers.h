#pragma once
#include <string>
#include <chrono>

namespace GlobalHelpers 
{
    std::string to_string_timestamp(std::chrono::system_clock::time_point timestamp);
    std::chrono::system_clock::time_point from_string_timestamp(const std::string& tms);
}
