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

#ifndef OPENSCENARIO_INTERPRETER__POISSON_DISTRIBUTION_HPP_
#define OPENSCENARIO_INTERPRETER__POISSON_DISTRIBUTION_HPP_

#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/double.hpp>
#include <openscenario_interpreter/syntax/range.hpp>
#include <openscenario_interpreter/utility/distribution.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- PoissonDistribution 1.2 ------------------------------------------------
 *
 *  <xsd:complexType name="PoissonDistribution">
 *    <xsd:sequence>
 *      <xsd:element name="Range" type="Range"/>
 *    </xsd:sequence>
 *    <xsd:attribute name="expectedValue" type="Double" use="required"/>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct PoissonDistribution : public ComplexType
{
  const Range range;

  const Double expected_value;

  StochasticDistributionClass<std::poisson_distribution<>> distribution;

  explicit PoissonDistribution(const pugi::xml_node &, Scope & scope);
};
}  // namespace syntax
}  // namespace openscenario_interpreter
#endif  // OPENSCENARIO_INTERPRETER__POISSON_DISTRIBUTION_HPP_
