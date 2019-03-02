#pragma once

#include "control/new/controlvalueinterface.h"

namespace NewControl {

// This is the base for both ControlProxy and ControlObject.
// BaseControlProxy implements ControlValueInterface, it forwards all the method calls to the associated ControlValue.
template<typename Value, typename Parameter, typename ConnectionProxy>
class BaseControlProxy: public ControlValueInterface<Value, Parameter> {
public:
    BaseControlProxy(ControlValuePointer<Value, Parameter> pControlValue);
    virtual ~BaseControlProxy() {};

    template<typename Receiver, typename Slot>
    QMetaObject::Connection connect(Receiver* receiver, Slot slot, Qt::ConnectionType connectionType = Qt::AutoConnection) {
        return m_connectionProxy.connect(receiver, slot, connectionType);
    }

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
    ConnectionProxy m_connectionProxy;
};

} // namespace

// ------------------------Implementation -------------------------------------

namespace NewControl {

template<typename Value, typename Parameter, typename ConnectionProxy>
BaseControlProxy<Value, Parameter, ConnectionProxy>::BaseControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
    : m_pControlValue(pControlValue),
      m_connectionProxy(pControlValue->signal()) {}

template<typename Value, typename Parameter, typename ConnectionProxy>
void BaseControlProxy<Value, Parameter, ConnectionProxy>::setValue(Value value) {
    m_pControlValue->setValue(value);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Value BaseControlProxy<Value, Parameter, ConnectionProxy>::getValue() const {
    return m_pControlValue->getValue();
}

template<typename Value, typename Parameter, typename ConnectionProxy>
void BaseControlProxy<Value, Parameter, ConnectionProxy>::setParameter(Parameter parameter) {
    m_pControlValue->setParameter(parameter);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Parameter BaseControlProxy<Value, Parameter, ConnectionProxy>::getParameter() const {
    return m_pControlValue->getParameter();
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Parameter BaseControlProxy<Value, Parameter, ConnectionProxy>::getParameterForValue(Value value) const {
    return m_pControlValue->getParameterForValue(value);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
void BaseControlProxy<Value, Parameter, ConnectionProxy>::reset() {
    m_pControlValue->reset();
}

template<typename Value, typename Parameter, typename ConnectionProxy>
void BaseControlProxy<Value, Parameter, ConnectionProxy>::setDefaultValue(Value value) {
    m_pControlValue->setDefaultValue(value);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Value BaseControlProxy<Value, Parameter, ConnectionProxy>::defaultValue() const {
    return m_pControlValue->defaultValue();
}

} // namespace
