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

#include <openscenario_interpreter/functional/equal_to.hpp>
#include <openscenario_interpreter/reader/attribute.hpp>
#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/syntax/speed_profile_action.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
SpeedProfileAction::SpeedProfileAction(const pugi::xml_node & node, Scope & scope)
: Scope(scope),
  entity_ref(readAttribute<EntityRef>("entityRef", node, scope, "")),
  following_mode(readAttribute<FollowingMode>("followingMode", node, scope)),
  dynamic_constraints(
    readElement<DynamicConstraints>("DynamicConstraints", node, scope, DynamicConstraints())),
  speed_profile_entry(readElements<SpeedProfileEntry, 1>("SpeedProfileEntry", node, scope))
{
}

auto SpeedProfileAction::apply(
  const EntityRef & actor, const SpeedProfileEntry & speed_profile_entry) -> void
{
  auto absolute_target_speed = [&]() { return speed_profile_entry.speed; };

  auto relative_target_speed = [&]() {
    return traffic_simulator::speed_change::RelativeTargetSpeed(
      entity_ref,                                                         //
      traffic_simulator::speed_change::RelativeTargetSpeed::Type::DELTA,  //
      speed_profile_entry.speed);
  };

  auto constraint = [&]() {
    if (std::isnan(speed_profile_entry.time)) {
      return traffic_simulator::speed_change::Constraint(
        traffic_simulator::speed_change::Constraint::Type::LONGITUDINAL_ACCELERATION,
        traffic_simulator_msgs::msg::BehaviorParameter().acceleration);
    } else {
      // TODO
      // return traffic_simulator::speed_change::Constraint(
      //   traffic_simulator::speed_change::Constraint::Type::TIME, speed_profile_entry.time);
      return traffic_simulator::speed_change::Constraint(
        traffic_simulator::speed_change::Constraint::Type::LONGITUDINAL_ACCELERATION,
        traffic_simulator_msgs::msg::BehaviorParameter().acceleration);
    }
  };

  if (entity_ref.empty()) {
    applySpeedProfileAction(
      actor,
      dynamic_constraints,      //
      absolute_target_speed(),  //
      traffic_simulator::speed_change::Transition::LINEAR,
      constraint(),  //
      true);
  } else {
    applySpeedProfileAction(
      actor,
      dynamic_constraints,      //
      relative_target_speed(),  //
      traffic_simulator::speed_change::Transition::LINEAR,
      constraint(),  //
      true);
  }
}

auto SpeedProfileAction::accomplished() -> bool
{
  // NOTE: Action ends on reaching the target speed of the last SpeedProfileEntry.

  return std::all_of(
    std::begin(accomplishments), std::end(accomplishments), [this](auto && actor_and_iter) {
      return std::get<1>(actor_and_iter) == std::end(speed_profile_entry);
    });
}

auto SpeedProfileAction::endsImmediately() const -> bool { return false; }

auto SpeedProfileAction::run() -> void
{
  auto compare = [this](const auto & actor, const auto & speed_profile_entry) {
    if (entity_ref.empty()) {
      return equal_to<double>()(evaluateSpeed(actor), speed_profile_entry.speed);
    } else {
      return equal_to<double>()(
        evaluateSpeed(actor), speed_profile_entry.speed + evaluateSpeed(entity_ref));
    }
  };

  for (auto && [actor, iter] : accomplishments) {
    if (
      iter != std::end(speed_profile_entry) and compare(actor, *iter) and
      ++iter != std::end(speed_profile_entry)) {
      apply(actor, *iter);
    }
  }

  for (auto && [actor, iter] : accomplishments) {
    auto target_speed = [&]() -> double {
      if (iter == std::end(speed_profile_entry)) {
        return Double::nan();
      } else if (entity_ref.empty()) {
        return iter->speed;
      } else {
        return evaluateSpeed(entity_ref) + iter->speed;
      }
    };

    // clang-format off
    std::cout << "actor " << std::quoted(actor) << "\n"
              << "  current speed = " << evaluateSpeed(actor) << "\n"
              << "  target speed = " << target_speed() << "\n"
              << "  entry = " << std::distance(std::begin(speed_profile_entry), iter) << "/" << speed_profile_entry.size() << "\n"
              << std::flush;
    // clang-format on
  }
}

auto SpeedProfileAction::start() -> void
{
  accomplishments.clear();

  assert(not speed_profile_entry.empty());

  for (const auto & actor : actors) {
    accomplishments.emplace(actor, std::begin(speed_profile_entry));
    apply(actor, *accomplishments[actor]);
  }
}
}  // namespace syntax
}  // namespace openscenario_interpreter
