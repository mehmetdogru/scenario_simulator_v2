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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__USER_DEFINED_DISTRIBUTION_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__USER_DEFINED_DISTRIBUTION_HPP_

#include <openscenario_interpreter/scope.hpp>
#include <pugixml.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- UserDefinedDistribution 1.2 --------------------------------------------
 *
 *  <xsd:complexType name="UserDefinedDistribution">
 *    <xsd:simpleContent>
 *      <xsd:extension base="xsd:string">
 *        <xsd:attribute name="type" type="String" use="required"/>
 *      </xsd:extension>
 *    </xsd:simpleContent>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct UserDefinedDistribution : private Scope, public ComplexType
{
  const String type;

  const String content;

  explicit UserDefinedDistribution(const pugi::xml_node &, const Scope &);

  // TODO: implement evaluate()?`
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__USER_DEFINED_DISTRIBUTION_HPP_
