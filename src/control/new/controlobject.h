#pragma once

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
    // ControlValueInterface
    void setValue(Value value) override;
    Value getValue() const override;
    void setParameter(Parameter parameter) override;
    Parameter getParameter() const override;
    Parameter getParameterForValue(Value value) const override;
    void reset() override;
    void setDefaultValue(Value value) override;
    Value defaultValue() const override;

private:
  ControlObject(ControlValuePointer<Value, Parameter> pControlValue);

  ControlValuePointer<Value, Parameter> m_pControlValue;

  friend class ControlFactory<Value, Parameter>;
};


} // namespace

namespace NewControl {

template<typename Value, typename Parameter>
ControlObject<Value, Parameter>::ControlObject(ControlValuePointer<Value, Parameter> pControlValue)
    : m_pControlValue(pControlValue) {

}

template<typename Value, typename Parameter>
void ControlObject<Value, Parameter>::setValue(Value value) {
    m_pControlValue->setValue(value);
}

template<typename Value, typename Parameter>
Value ControlObject<Value, Parameter>::getValue() const {
    return m_pControlValue->getValue();
}

template<typename Value, typename Parameter>
void ControlObject<Value, Parameter>::setParameter(Parameter parameter) {
    m_pControlValue->setParameter(parameter);
}

template<typename Value, typename Parameter>
Parameter ControlObject<Value, Parameter>::getParameter() const {
    return m_pControlValue->getParameter();
}

template<typename Value, typename Parameter>
Parameter ControlObject<Value, Parameter>::getParameterForValue(Value value) const {
    return m_pControlValue->getParameterForValue(value);
}

template<typename Value, typename Parameter>
void ControlObject<Value, Parameter>::reset() {}

template<typename Value, typename Parameter>
void ControlObject<Value, Parameter>::setDefaultValue(Value value) {
    m_pControlValue->setDefaultValue(value);
}

template<typename Value, typename Parameter>
Value ControlObject<Value, Parameter>::defaultValue() const {
    return m_pControlValue->defaultValue();
}

} // namespace
