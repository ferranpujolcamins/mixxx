#pragma once

#import "control/new/controlvalueinterface.h"

namespace NewControl {

// Implements the interface forwarding the calls to a ControlValuePointer
// TODO: this is not really abstract, find a better name
// TODO: Do we need this? OR is this actualyl just ControlProxy?
template<typename Value, typename Parameter>
class AbstractControlProxy: public ControlValueInterface<Value, Parameter> {
public:
    AbstractControlProxy(ControlValuePointer<Value, Parameter> pControlValue);

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

namespace NewControl {

template<typename Value, typename Parameter>
AbstractControlProxy<Value, Parameter>::AbstractControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
    : m_pControlValue(pControlValue) {

}

template<typename Value, typename Parameter>
void AbstractControlProxy<Value, Parameter>::setValue(Value value) {
    m_pControlValue->setValue(value);
}

template<typename Value, typename Parameter>
Value AbstractControlProxy<Value, Parameter>::getValue() const {
    return m_pControlValue->getValue();
}

template<typename Value, typename Parameter>
void AbstractControlProxy<Value, Parameter>::setParameter(Parameter parameter) {
    m_pControlValue->setParameter(parameter);
}

template<typename Value, typename Parameter>
Parameter AbstractControlProxy<Value, Parameter>::getParameter() const {
    return m_pControlValue->getParameter();
}

template<typename Value, typename Parameter>
Parameter AbstractControlProxy<Value, Parameter>::getParameterForValue(Value value) const {
    return m_pControlValue->getParameterForValue(value);
}

template<typename Value, typename Parameter>
void AbstractControlProxy<Value, Parameter>::reset() {}

template<typename Value, typename Parameter>
void AbstractControlProxy<Value, Parameter>::setDefaultValue(Value value) {
    m_pControlValue->setDefaultValue(value);
}

template<typename Value, typename Parameter>
Value AbstractControlProxy<Value, Parameter>::defaultValue() const {
    return m_pControlValue->defaultValue();
}

} // namespace
