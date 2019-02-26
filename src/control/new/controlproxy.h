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

  private:
    ControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
          : AbstractControlProxy<Value, Parameter>(pControlValue) {
        };

    friend class ControlFactory<Value, Parameter>;
};

} //namespace
