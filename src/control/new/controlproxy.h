#pragma once

#include "control/new/abstractcontrolproxy.h"

namespace NewControl {

// Used to read/write a ControlValue (guarantees the value is still alive while this object is alive)
template<typename Value, typename Parameter>
class ControlProxy: public AbstractControlProxy<Value, Parameter> {
  public:
    ControlProxy(ControlProxy<Value, Parameter>& other)
        : ControlProxy(other.m_pControlValue) {
    }

    // TODO: make private
    ControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
          : AbstractControlProxy<Value, Parameter>(pControlValue) {
        };

    // TODO: make private, this is just to make tests easier
    ControlProxy()
        : AbstractControlProxy<Value, Parameter>(std::shared_ptr<ControlValue<Value, Parameter>>()) {}

  private:

    friend class ControlFactory<Value, Parameter>;
};

} //namespace
