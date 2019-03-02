#pragma once

#include "control/new/controlvalueinterface.h"

namespace NewControl {

// Implements the interface forwarding the calls to a ControlValuePointer
// TODO: this is not really abstract, find a better name
// TODO: Do we need this? OR is this actualyl just ControlProxy?
template<typename Value, typename Parameter, typename ConnectionProxy>
class AbstractControlProxy: public ControlValueInterface<Value, Parameter> {
public:
    AbstractControlProxy(ControlValuePointer<Value, Parameter> pControlValue);
    virtual ~AbstractControlProxy() {};

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

namespace NewControl {

template<typename Value, typename Parameter, typename ConnectionProxy>
AbstractControlProxy<Value, Parameter, ConnectionProxy>::AbstractControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
    : m_pControlValue(pControlValue),
      m_connectionProxy(pControlValue->signal()) {

}

template<typename Value, typename Parameter, typename ConnectionProxy>
void AbstractControlProxy<Value, Parameter, ConnectionProxy>::setValue(Value value) {
    m_pControlValue->setValue(value);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Value AbstractControlProxy<Value, Parameter, ConnectionProxy>::getValue() const {
    return m_pControlValue->getValue();
}

template<typename Value, typename Parameter, typename ConnectionProxy>
void AbstractControlProxy<Value, Parameter, ConnectionProxy>::setParameter(Parameter parameter) {
    m_pControlValue->setParameter(parameter);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Parameter AbstractControlProxy<Value, Parameter, ConnectionProxy>::getParameter() const {
    return m_pControlValue->getParameter();
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Parameter AbstractControlProxy<Value, Parameter, ConnectionProxy>::getParameterForValue(Value value) const {
    return m_pControlValue->getParameterForValue(value);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
void AbstractControlProxy<Value, Parameter, ConnectionProxy>::reset() {}

template<typename Value, typename Parameter, typename ConnectionProxy>
void AbstractControlProxy<Value, Parameter, ConnectionProxy>::setDefaultValue(Value value) {
    m_pControlValue->setDefaultValue(value);
}

template<typename Value, typename Parameter, typename ConnectionProxy>
Value AbstractControlProxy<Value, Parameter, ConnectionProxy>::defaultValue() const {
    return m_pControlValue->defaultValue();
}

} // namespace
