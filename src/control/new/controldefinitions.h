#pragma once

#include "controlfactory.h"

namespace NewControl {

template class ControlFactory<bool, bool>;

class Channel: Group {
  public:
    Channel(int index)
  // TODO: Fix this: "[Channel" + std::to_string(index) + "]"
      : Group("[Channel1]") {};

    ControlFactory<bool, bool> play() {
        return ControlFactory<bool, bool>(*this, "play", false, true);
    }
};

} // namespace
