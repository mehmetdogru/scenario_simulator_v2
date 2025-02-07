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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__VALUE_CONSTRAINT_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__VALUE_CONSTRAINT_HPP_

#include <nlohmann/json.hpp>
#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/double.hpp>
#include <openscenario_interpreter/syntax/rule.hpp>
#include <openscenario_interpreter/syntax/string.hpp>
#include <openscenario_msgs/msg/value_constraint.hpp>
#include <pugixml.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- ValueConstraint --------------------------------------------------------------
 *
 *  <xsd:complexType name="ValueConstraint">
 *    <xsd:attribute name="rule" type="Rule" use="required"/>
 *    <xsd:attribute name="value" type="String" use="required"/>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct ValueConstraint : public ComplexType
{
  const Rule rule;

  const String value;

  explicit ValueConstraint(const pugi::xml_node &, Scope &);

  explicit ValueConstraint(const openscenario_msgs::msg::ValueConstraint &);

  auto evaluate(const Object &) const -> bool;
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__VALUE_CONSTRAINT_HPP_
