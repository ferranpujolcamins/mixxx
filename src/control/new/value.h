#pragma once

#include "control/new/controlvalueinterface.h"

namespace NewControl {

class ValueInterface {

};
// TODO: think what needs to be a pointer
// TODO: explicitly write the thread safety requirements and later check that the implementaiton fulfills them
template<typename Value, typename Parameter, template <typename> class Behavior>
class ControlValue final: public ControlValueInterface<Value, Parameter> {
public:
    ControlValue(Value defaultValue, Behavior<Parameter>);
    ~ControlValue();

    // ControlValueInterface
    void set(Value value);
    Value get() const;
    void setParameter(Parameter parameter);
    Parameter getParameter() const;
    Parameter getParameterForValue(Value value) const;
    void reset();
    void setDefaultValue(Value value);
    Value defaultValue() const;

private:
    ControlValueAtomic<Value> m_value;
    ControlValueAtomic<Value> m_defaultValue;

    // TODO: this is a sharedpointer so the methods using behaviour are thread safe, each invokation retains the old behaviour
    // in case a new one is set
    // But do we really need to mutate this?
    Behavior m_behavior;
};

} // namespace
