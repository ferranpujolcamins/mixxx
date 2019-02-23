#pragma once

#include "control/new/controlbehavior.h"
#include "control/new/controlfactory.h"
#include "control/controlvalue.h"

namespace NewControl {


// TODO: think what needs to be a pointer
// TODO: explicitly write the thread safety requirements and later check that the implementaiton fulfills them
template<typename Value, typename Parameter>
class ControlValue final: public ControlValueInterface<Value, Parameter> {
public:
    ControlValue(ControlFactory<Value, Parameter> controlFactory);
    ~ControlValue();

    // ControlValueInterface
    void setValue(Value value);
    Value getValue() const;
    void setParameter(Parameter parameter);
    Parameter getParameter() const;
    Parameter getParameterForValue(Value value) const;
    void reset();
    void setDefaultValue(Value value);
    Value defaultValue() const;

    ConfigKey configKey() const {
        return m_controlFactory.configKey();
    }

private:
    ControlFactory<Value, Parameter> m_controlFactory;
    ControlValueAtomic<Value> m_value;
    ControlValueAtomic<Value> m_defaultValue;

    // TODO: this is a sharedpointer so the methods using behaviour are thread safe, each invokation retains the old behaviour
    // in case a new one is set
    // But do we really need to mutate this?
    ControlBehavior<Value, Parameter> m_behavior;
};

template<typename Value, typename Parameter>
using ControlValuePointer = std::shared_ptr<ControlValue<Value, Parameter> >;

template<typename Value, typename Parameter>
using ControlValueWeakPointer = std::weak_ptr<ControlValue<Value, Parameter> >;


} // namespace


#import "control/new/controlvaluestore.h"

namespace NewControl {

template<typename Value, typename Parameter>
ControlValue<Value, Parameter>::ControlValue(ControlFactory<Value, Parameter> controlFactory)
    : m_controlFactory(controlFactory) {

}

template<typename Value, typename Parameter>
ControlValue<Value, Parameter>::~ControlValue() {
    ControlValueStore<Value, Parameter>::remove(configKey());
}

} // namespace
