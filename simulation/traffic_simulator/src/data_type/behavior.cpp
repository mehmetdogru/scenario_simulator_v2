// Copyright 2015 TIER IV, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <traffic_simulator/data_type/behavior.hpp>

namespace traffic_simulator
{
namespace behavior
{
std::ostream & operator<<(std::ostream & stream, const Request & value)
{
  switch (value) {
    default:
    case Request::NONE:
      return stream << "request : NONE" << std::endl;
    case Request::LANE_CHANGE:
      return stream << "request : LANE_CHANGE" << std::endl;
    case Request::FOLLOW_LANE:
      return stream << "request : FOLLOW_LANE" << std::endl;
    case Request::WALK_STRAIGHT:
      return stream << "request : WALK_STRAIGHT" << std::endl;
  }
}

std::string getRequestString(const Request & request)
{
  switch (request) {
    case Request::NONE:
      return "none";
    case Request::LANE_CHANGE:
      return "lane_change";
    case Request::FOLLOW_LANE:
      return "follow_lane";
    case Request::WALK_STRAIGHT:
      return "walk_straight";
    default:
      THROW_SEMANTIC_ERROR(request, " is invalid");
  }
}
}  // namespace behavior
}  // namespace traffic_simulator
