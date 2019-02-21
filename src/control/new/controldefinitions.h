#pragma once

#include "control/new/controldefinition.h"

namespace NewControl {

class Channel {
  public:
    Channel(int index);
    ControlDefinition<bool, bool> play() {}
};

} // namespace
