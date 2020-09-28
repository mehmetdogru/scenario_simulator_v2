// Copyright 2015-2020 Autoware Foundation. All rights reserved.
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

#ifndef SCENARIO_RUNNER__SYNTAX__PROPERTIES_HPP_
#define SCENARIO_RUNNER__SYNTAX__PROPERTIES_HPP_

namespace scenario_runner
{
inline namespace syntax
{
/* ==== Properties ===========================================================
 *
 * <xsd:complexType name="Properties">
 *   <xsd:sequence>
 *     <xsd:element name="Property" minOccurs="0" maxOccurs="unbounded" type="Property"/>
 *     <xsd:element name="File" type="File" minOccurs="0" maxOccurs="unbounded"/>
 *   </xsd:sequence>
 * </xsd:complexType>
 *
 * ======================================================================== */
struct Properties
{
  Properties() = default;

  template<typename Node, typename Scope>
  explicit Properties(const Node &, Scope &)
  {}
};
}
}  // namespace scenario_runner

#endif  // SCENARIO_RUNNER__SYNTAX__PROPERTIES_HPP_
