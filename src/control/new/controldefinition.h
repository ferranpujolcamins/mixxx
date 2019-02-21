#pragma once

#include "control/new/controlobject.h"
#include "control/new/controlproxy.h"

namespace NewControl {

template<typename Value, typename Parameter>
class ControlDefinition {
  public:
    ControlDefinition(Value, Parameter initialValue);
    virtual ~ControlDefinition();

    ControlObject<Value, Parameter> create(Value initialValue) {
        // if control already created return null pointer
        return ControlObject<Value, Parameter>();
    }

    ControlProxy<Value, Parameter> get() {
        return ControlProxy<Value, Parameter>();
    }

    //rxcpp::subjects::behavior<ValueType> subject;

    // Always called on the subscribing thread
    // Shortcut for get().subscribe()
    //Subscription subscribe(std::function<void(ValueType)>) { return Subscription(); }
};

} // namespace
