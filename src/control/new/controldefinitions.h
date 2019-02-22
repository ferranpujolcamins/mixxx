#pragma once

#include "control/new/controldefinition.h"

namespace NewControl {

class Channel {
  public:
    Channel(int index);

    class Play: public ControlDefinition<bool, bool> {

    };
    ControlDefinition<bool, bool> play() {
        return Play(m_iChannel, true);
    }
  private:
    int m_iChannel;
};

} // namespace
