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

#ifndef OPENSCENARIO_INTERPRETER__HISTOGRAM_HPP_
#define OPENSCENARIO_INTERPRETER__HISTOGRAM_HPP_

#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/histogram_bin.hpp>
#include <openscenario_interpreter/utility/distribution.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- Histogram 1.2 ----------------------------------------------------------
 *
 *  <xsd:complexType name="Histogram">
 *    <xsd:sequence>
 *      <xsd:element name="Bin" type="HistogramBin" maxOccurs="unbounded"/>
 *    </xsd:sequence>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */

struct Histogram : public ComplexType
{
  /**
   * Note: HistogramBin must be stored in continuous range and ascending order, to `bins`
   */
  const std::list<HistogramBin> bins;

  struct BinAdaptor
  {
    explicit BinAdaptor(const std::list<HistogramBin> & bins)
    {
      intervals.emplace_back(bins.front().range.lower_limit.data);
      for (const auto & bin : bins) {
        intervals.emplace_back(bin.range.lower_limit.data);
        densities.emplace_back(bin.weight.data);
      }
      intervals.emplace_back(bins.back().range.upper_limit.data);
    }
    std::vector<double> intervals, densities;
  } bin_adaptor;

  StochasticDistributionClass<std::piecewise_constant_distribution<Double::value_type>>
    distribution;

  explicit Histogram(const pugi::xml_node &, Scope & scope);
};
}  // namespace syntax
}  // namespace openscenario_interpreter
#endif  // OPENSCENARIO_INTERPRETER__HISTOGRAM_HPP_
