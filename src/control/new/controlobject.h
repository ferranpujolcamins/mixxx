#pragma once

#include "control/new/abstractcontrolproxy.h"
#include "control/new/connectionproxy.h"

namespace NewControl {

//TODO: document how CO connects to the value
// Creates and owns a ControlValue (the control is still alive if there are proxies using it).
// TODO: ControlObject can be used to read / write the value for convenience. Make so by subclassing ControlProxy
template<typename Value, typename Parameter>
class ControlObject : public AbstractControlProxy<Value, Parameter, DirectConnectionProxy> {
public:
    // This should be private, but we need it public for make_unique
    ControlObject(ControlValuePointer<Value, Parameter> pControlValue);
    virtual ~ControlObject();
private:
    ControlObject(ControlObject& other) = delete;

    friend class ControlFactory<Value, Parameter>;
};

} // namespace

namespace NewControl {

template<typename Value, typename Parameter>
ControlObject<Value, Parameter>::ControlObject(ControlValuePointer<Value, Parameter> pControlValue)
    : AbstractControlProxy<Value, Parameter, DirectConnectionProxy>(pControlValue) {

}

template<typename Value, typename Parameter>
ControlObject<Value, Parameter>::~ControlObject() {
    this->m_pControlValue->markForDeletion();
}

} // namespace
