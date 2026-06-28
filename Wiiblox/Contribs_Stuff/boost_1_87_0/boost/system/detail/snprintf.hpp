#ifndef BOOST_SYSTEM_DETAIL_SNPRINTF_HPP_INCLUDED
#define BOOST_SYSTEM_DETAIL_SNPRINTF_HPP_INCLUDED

// Copyright 2018, 2020, 2021 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See library home page at http://www.boost.org/libs/system

#include <cstdio>

// MSVC headers define snprintf as a macro expanding to _snprintf.
// This breaks "using std::snprintf;" below because the preprocessor
// expands it to "using std::_snprintf;".  Undefine to get the real name.
#ifdef snprintf
#undef snprintf
#endif

//

namespace boost
{
namespace system
{
namespace detail
{

using std::snprintf;

} // namespace detail
} // namespace system
} // namespace boost

#endif // #ifndef BOOST_SYSTEM_DETAIL_SNPRINTF_HPP_INCLUDED
