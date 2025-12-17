#include "errors/Error.h"

#include <chrono>
#include <sstream>
#include <string>
#include <iomanip>

namespace GlobalHelpers 
{
    std::string to_string_timestamp( std::chrono::system_clock::time_point timestamp )
    {
        std::time_t t = std::chrono::system_clock::to_time_t(timestamp);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");

        return  oss.str();
        
    }

    std::chrono::system_clock::time_point from_string_timestamp(const std::string& tms)
    {
        std::tm tm{};
        std::istringstream iss{tms};
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

        if (iss.fail())
        throw Error("Failed to parse timestamp: " + tms);

        std::time_t t {std::mktime(&tm)};

        return std::chrono::system_clock::from_time_t(t);
    }
}