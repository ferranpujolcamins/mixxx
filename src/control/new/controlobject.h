#pragma once

#include "controlvalueinterface.h"

namespace NewControl {

//class Subscription {
//  public:
//    void disposedBy() {}
//};

// Creates and owns a ControlValue (the control is still alive if there are proxies using it).
// TODO: ControlObject can be used to read / write the value for convenience. Make so by subclassing ControlProxy
template<typename Value, typename Parameter>
class ControlObject : public ControlValueInterface<Value, Parameter> {
  public:
    ControlObject() {}

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

} // namespace
