#pragma once

#include "controlfactory.h"

namespace NewControl {

template class ControlFactory<bool, bool>;

class Channel: public Group {
  public:
    Channel(int index)
  // TODO: Fix this: "[Channel" + std::to_string(index) + "]"
      : Group("[Channel1]") {};

    ControlFactory<bool, bool> play() {
        // TODO: builder pattern or parameter object
        return ControlFactory<bool, bool>(std::move(*this), "play", false, true, false);
    }
};

} // namespace
