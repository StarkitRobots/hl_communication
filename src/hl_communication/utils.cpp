#include <hl_communication/utils.h>

#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std::chrono;

namespace hl_communication
{
void readFromFile(const std::string& path, google::protobuf::Message* msg)
{
  msg->Clear();
  std::ifstream in(path);
  if (!in.good())
  {
    throw std::runtime_error(HL_DEBUG + " failed to open file '" + path + "'");
  }
  msg->ParseFromIstream(&in);
}

void writeToFile(const std::string& path, const google::protobuf::Message& msg)
{
  std::ofstream out(path, std::ios::binary);
  if (!out.good())
  {
    throw std::runtime_error(HL_DEBUG + " failed to open file '" + path + "'");
  }
  msg.SerializeToOstream(&out);
}

uint64_t getTimeStamp()
{
  return duration_cast<duration<uint64_t, std::micro>>(steady_clock::now().time_since_epoch()).count();
}

uint64_t getUTCTimeStamp()
{
  return duration_cast<duration<uint64_t, std::micro>>(system_clock::now().time_since_epoch()).count();
}

int64_t getSteadyClockOffset()
{
  int64_t steady_ts = getTimeStamp();
  int64_t system_ts = getUTCTimeStamp();
  return system_ts - steady_ts;
}

std::string getBaseName(const std::string& path)
{
  size_t idx = path.find_last_of('/');
  if (idx == std::string::npos)
  {
    return path;
  }
  return path.substr(idx + 1);
}

uint64_t stringToIP(const std::string& str)
{
  std::stringstream ss(str);
  uint64_t result = 0;
  std::string element;
  while (getline(ss, element, '.'))
  {
    uint64_t elem_value = std::stoi(element);
    result = (result << 8) + elem_value;
  }
  return result;
}

std::string ipToString(uint64_t ip)
{
  std::ostringstream oss;
  oss << (ip >> 24 & 0xFF) << "." << (ip >> 16 & 0xFF) << "." << (ip >> 8 & 0xFF) << "." << (ip & 0xFF);
  return oss.str();
}

void invertPosition(PositionDistribution* position)
{
  position->set_x(-position->x());
  position->set_y(-position->y());
}

void invertAngle(AngleDistribution* angle)
{
  double alpha = angle->mean() + M_PI;
  if (alpha > M_PI)
  {
    alpha -= 2 * M_PI;
  }
  angle->set_mean(alpha);
}

void invertPose(PoseDistribution* pose)
{
  if (pose->has_position())
  {
    invertPosition(pose->mutable_position());
  }
  if (pose->has_dir())
  {
    invertAngle(pose->mutable_dir());
  }
}

bool isPenalized(const GCMsg& msg, int team_id, int robot_id)
{
  for (const GCTeamMsg& team : msg.teams())
  {
    if (!team.has_team_number() || team.team_number() != team_id)
    {
      continue;
    }
    int idx = robot_id - 1;//Robots are numbered from 1
    return team.robots(idx).has_penalty() && team.robots(idx).penalty() != 0;
  }
  return false;
}

}  // namespace hl_communication
