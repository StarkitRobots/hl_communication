#pragma once

#include <string>

namespace hl_communication
{

#define HL_DEBUG                                   \
  (std::string(__FUNCTION__) + ":"                 \
   + hl_communication::getBaseName(__FILE__) + ":" \
   + std::to_string(__LINE__)  + ": ")

/**
 * Return the name of the file at the given path:
 * e.g getBaseName("toto/file.cpp") returns "file.cpp"
 */
std::string getBaseName(const std::string & path);

/**
 * Return time_since_epoch in a integer value (unit: microseconds)
 */
uint64_t getTimeStamp();

/**
 * Convert a human readable string to a 8 bytes ip address
 */
uint64_t stringToIP(const std::string & str);

/**
 * Convert a 8 bytes ip address to a human readable string
 */
std::string ipToString(uint64_t ip_address);


}
