#pragma once

#include "controlvalueinterface.h"

namespace NewControl {

// Used to read/write a ControlValue (guarantees the value is still alive while this object is alive)
template<typename Value, typename Parameter>
class ControlProxy: public ControlValueInterface<Value, Parameter> {
  public:
    Value value() {};
    // Subscription subscribe(std::function<void(ValueType)>) { return Subscription(); }

    // ControlValueInterface
    void set(Value value);
    Value get() const;
    void setParameter(Parameter parameter);
    Parameter getParameter() const;
    Parameter getParameterForValue(Value value) const;
    void reset();
    void setDefaultValue(Value value);
    Value defaultValue() const;
};

} //namespace
