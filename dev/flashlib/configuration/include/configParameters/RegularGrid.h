#pragma once

#include <basic/types.h>

#include "ArrayParameter.h"
#include "Node.h"
#include "Parameter.h"

namespace flash {
namespace configParameters {

class RegularGrid : configParameters::Node {
    ArrayParameter<basic::types::mySize, 3> sampling;
    ArrayParameter<basic::types::myDouble, 3> offset;
};

}  // namespace configParameters
}  // namespace flash
