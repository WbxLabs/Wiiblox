
#pragma once

#include <vector>
#include <map>

#include "wbx/boost.hpp"
#include "wbx/threadsafe.h"
#include "wbx/signal.h"
#include "wbx/TaskScheduler.Job.h"

#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

#include "util/Name.h"
#include "util/Region3.h"
#include "Reflection/YieldFunction.h"
#include "Tree/Instance.h"
#include "Datamodel/DataModel.h"

// Make Base library types (RBX and rbx namespaces) visible within WBX
// so that references like wbx::mutex -> RBX::mutex resolve correctly.
// MSVC is case-insensitive for identifiers, but Clang is not.
namespace WBX {
    using namespace ::RBX;
    using namespace ::rbx;
}

// Allow wbx:: to resolve to WBX:: (MSVC case-insensitivity compat for Clang)
namespace wbx = WBX;


