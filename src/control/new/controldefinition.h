#pragma once

#include "control/new/controlobject.h"
#include "control/new/controlproxy.h"
#include "control/new/controlvalueinterface.h"

namespace NewControl {

template<typename Value, typename Parameter>
class ControlDefinition: public ControlValueReadOnlyInterface<Value, Parameter> {
  public:
    ControlDefinition(int iChannel, Value initialValue);
    virtual ~ControlDefinition();

    ControlObject<Value, Parameter> create(Value initialValue) {
        // if control already created return null pointer
        return ControlObject<Value, Parameter>();
    }

    ControlProxy<Value, Parameter> getProxy() {
        return ControlProxy<Value, Parameter>();
    }

    // ControlValueReadOnlyInterface
    Value getValue() const;
    Parameter getParameter() const;
    Parameter getParameterForValue(Value value) const;
    Value defaultValue() const;

    //rxcpp::subjects::behavior<ValueType> subject;

    // Always called on the subscribing thread
    // Shortcut for get().subscribe()
    //Subscription subscribe(std::function<void(ValueType)>) { return Subscription(); }
  private:
    int m_iChannel;
};

} // namespace
