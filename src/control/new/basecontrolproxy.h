#pragma once

#include "control/new/controlvalueinterface.h"

namespace NewControl {

// This is the base for both ControlProxy and ControlObject.
// BaseControlProxy implements ControlValueInterface, it forwards all the method calls to the ControlValue member.
// The methods in this class are thread-safe because the calls are forwarded to ControlValue, which is thread-safe.
template<typename Value, typename Parameter>
class BaseControlProxy: public ControlValueInterface<Value, Parameter> {
public:
    BaseControlProxy(ControlValuePointer<Value, Parameter> pControlValue);
    virtual ~BaseControlProxy() {};

    // ControlValueInterface
    void setValue(Value value) override;
    Value getValue() const override;
    void setParameter(Parameter parameter) override;
    Parameter getParameter() const override;
    Parameter getParameterForValue(Value value) const override;
    void reset() override;
    void setDefaultValue(Value value) override;
    Value defaultValue() const override;

protected:
    ControlValuePointer<Value, Parameter> m_pControlValue;
};

} // namespace

// ------------------------Implementation -------------------------------------

namespace NewControl {

template<typename Value, typename Parameter>
BaseControlProxy<Value, Parameter>::BaseControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
    : m_pControlValue(pControlValue) {}

template<typename Value, typename Parameter>
void BaseControlProxy<Value, Parameter>::setValue(Value value) {
    m_pControlValue->setValue(value);
}

template<typename Value, typename Parameter>
Value BaseControlProxy<Value, Parameter>::getValue() const {
    return m_pControlValue->getValue();
}

template<typename Value, typename Parameter>
void BaseControlProxy<Value, Parameter>::setParameter(Parameter parameter) {
    m_pControlValue->setParameter(parameter);
}

template<typename Value, typename Parameter>
Parameter BaseControlProxy<Value, Parameter>::getParameter() const {
    return m_pControlValue->getParameter();
}

template<typename Value, typename Parameter>
Parameter BaseControlProxy<Value, Parameter>::getParameterForValue(Value value) const {
    return m_pControlValue->getParameterForValue(value);
}

template<typename Value, typename Parameter>
void BaseControlProxy<Value, Parameter>::reset() {
    m_pControlValue->reset();
}

template<typename Value, typename Parameter>
void BaseControlProxy<Value, Parameter>::setDefaultValue(Value value) {
    m_pControlValue->setDefaultValue(value);
}

template<typename Value, typename Parameter>
Value BaseControlProxy<Value, Parameter>::defaultValue() const {
    return m_pControlValue->defaultValue();
}

} // namespace
