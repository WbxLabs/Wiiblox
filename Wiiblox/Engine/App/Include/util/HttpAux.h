#pragma once

#include <string>
#include <boost/unordered_map.hpp>

namespace WBX
{
namespace HttpAux
{
typedef boost::unordered_map<std::string, std::string> AdditionalHeaders;
} // namespace HttpAux
} // namespace WBX
