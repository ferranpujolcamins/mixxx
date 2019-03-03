#pragma once

#include <QAtomicInt>

#include "control/new/controlbehavior.h"
#include "control/new/controlfactory.h"
#include "control/new/voidsignal.h"
#include "control/controlvalue.h"

namespace NewControl {

// TODO: document what is thread safe and what not
// TODO: think what needs to be a pointer
// TODO: explicitly write the thread safety requirements and later check that the implementaiton fulfills them
template<typename Value, typename Parameter>
class ControlValue final: public ControlValueInterface<Value, Parameter> {
public:
    ControlValue(ControlFactory<Value, Parameter>&& controlFactory);
    ~ControlValue();

    VoidSignal* signal() {
        return m_pSignal.get();
    }

    // ControlValueInterface
    void setValue(Value value);
    Value getValue() const;
    void setParameter(Parameter parameter);
    Parameter getParameter() const;
    Parameter getParameterForValue(Value value) const;
    void reset();
    void setDefaultValue(Value value);
    Value defaultValue() const;

    bool isMarkedForDeletion() const;
    void markForDeletion();

    ConfigKey configKey() const {
        return m_controlFactory.configKey();
    }

private:
    // TODO: is this member needed?
    ControlFactory<Value, Parameter> m_controlFactory;
    ControlValueAtomic<Value> m_value;
    ControlValueAtomic<Value> m_defaultValue;

    // TODO: this is a sharedpointer so the methods using behaviour are thread safe, each invokation retains the old behaviour
    // in case a new one is set
    // But do we really need to mutate this?
    ControlBehavior<Value, Parameter> m_behavior;

    QAtomicInt m_deletionMark;
    std::unique_ptr<VoidSignal> m_pSignal;
};

template<typename Value, typename Parameter>
using ControlValuePointer = std::shared_ptr<ControlValue<Value, Parameter>>;

template<typename Value, typename Parameter>
using ControlValueWeakPointer = std::weak_ptr<ControlValue<Value, Parameter>>;

} // namespace

// ------------------------Implementation -------------------------------------

#import "control/new/controlvaluestore.h"

namespace NewControl {

template<typename Value, typename Parameter>
ControlValue<Value, Parameter>::ControlValue(ControlFactory<Value, Parameter>&& controlFactory)
    : m_controlFactory(controlFactory), m_deletionMark(0), m_pSignal(new VoidSignal()) {
    m_value.setValue(m_controlFactory.initialValue());
    m_defaultValue.setValue(m_controlFactory.defaultValue222());
}

template<typename Value, typename Parameter>
ControlValue<Value, Parameter>::~ControlValue() {
    ControlValueStore<Value, Parameter>::remove(configKey());
}

template<typename Value, typename Parameter>
void ControlValue<Value, Parameter>::setValue(Value value) {
    // TODO: check if we use the atomic specialization
    m_value.setValue(value);
    m_pSignal->signal();
}

template<typename Value, typename Parameter>
Value ControlValue<Value, Parameter>::getValue() const {
    return m_value.getValue();
}

template<typename Value, typename Parameter>
void ControlValue<Value, Parameter>::setParameter(Parameter parameter) {}

template<typename Value, typename Parameter>
Parameter ControlValue<Value, Parameter>::getParameter() const {}

template<typename Value, typename Parameter>
Parameter ControlValue<Value, Parameter>::getParameterForValue(Value value) const {}

template<typename Value, typename Parameter>
void ControlValue<Value, Parameter>::reset() {
    Value defaultValue = m_defaultValue.getValue();
    m_value.setValue(defaultValue);
}

template<typename Value, typename Parameter>
void ControlValue<Value, Parameter>::setDefaultValue(Value value) {
    m_defaultValue.setValue(value);
}

template<typename Value, typename Parameter>
Value ControlValue<Value, Parameter>::defaultValue() const {
    return m_defaultValue.getValue();
}

template<typename Value, typename Parameter>
bool ControlValue<Value, Parameter>::isMarkedForDeletion() const {
    return m_deletionMark.loadAcquire();
}

template<typename Value, typename Parameter>
void ControlValue<Value, Parameter>::markForDeletion() {
    m_deletionMark.storeRelease(1);
}


} // namespace
