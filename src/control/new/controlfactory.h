#pragma once

#include "control/new/controlvalueinterface.h"
#include "preferences/configobject.h"
#include "util/memory.h"

namespace NewControl {

template<typename Value, typename Parameter>
class ControlObject;

template<typename Value, typename Parameter>
class ControlProxy;

// Move only, we use this to create a ControlFactory, injecting itself into the created ControlFactory
class Group {
public:
    Group(QString sGroup)
        : m_sGroup(sGroup) {};

    virtual ~Group() {};
    virtual QString group() {
        return m_sGroup;
    }

private:
    QString m_sGroup;
};

template<typename Value, typename Parameter>
class ControlFactory: public ControlValueReadOnlyInterface<Value, Parameter> {
  public:
    // TODO: do we really want move semantics?
    ControlFactory(Group&& group, QString sItemKey, bool bPersistInConfiguration, Value initialValue, Value defaultValue)
      : m_group(group), m_sItemKey(sItemKey), m_bPersistInConfiguration(bPersistInConfiguration),
        m_initialValue(initialValue), m_defaultValue(defaultValue) {
    };
    virtual ~ControlFactory() {};

    // Create a ControlObject. Can return nullptr is the creation failed, for example because another CO with the same key is already created.
    // TODO: if getProxy doesn't ocnsume this, why then create should?
    std::unique_ptr<ControlObject<Value, Parameter>> create() &&;

    std::unique_ptr<ControlProxy<Value, Parameter>> getProxy() const;

    ConfigKey configKey() const {
        return ConfigKey(group().group(), itemKey());
    }

    // TODO: improve this
    Group group() const {
        return m_group;
    }

    QString itemKey() const {
        return m_sItemKey;
    }

    Value initialValue() {
        return m_initialValue;
    }

    // TODO: name for this
    Value defaultValue222() {
        return m_defaultValue;
    }

    // ControlValueReadOnlyInterface
    Value getValue() const;
    Parameter getParameter() const;
    Parameter getParameterForValue(Value value) const;
    Value defaultValue() const;

    //rxcpp::subjects::behavior<ValueType> subject;

    // Always called on the subscribing thread
    // Shortcut for get().subscribe()
    //Subscription subscribe(std::function<void(ValueType)>) { return Subscription(); }
  private:
    Group m_group;
    QString m_sItemKey;
    bool m_bPersistInConfiguration;
    Value m_initialValue;
    Value m_defaultValue;
};

} // namespace

#include "control/new/controlvaluestore.h"
#include "control/new/controlvalue.h"
#include "control/new/controlobject.h"
#include "control/new/controlproxy.h"

namespace NewControl {

template<typename Value, typename Parameter>
std::unique_ptr<ControlObject<Value, Parameter>> ControlFactory<Value, Parameter>::create() && {
    ControlValuePointer<Value, Parameter> value = std::make_shared<ControlValue<Value, Parameter>>(std::move(*this));
    if (!ControlValueStore<Value, Parameter>::insert(configKey(), value)) {
        return std::unique_ptr<ControlObject<Value, Parameter>>(nullptr);
    }
    return std::make_unique<ControlObject<Value, Parameter>>(value);
}

// TODO: use alias for pointers
template<typename Value, typename Parameter>
std::unique_ptr<ControlProxy<Value, Parameter>> ControlFactory<Value, Parameter>::getProxy() const {
    ControlValuePointer<Value, Parameter> value = ControlValueStore<Value, Parameter>::get(configKey());
    if (value == nullptr || value->isMarkedForDeletion()) {
        return std::unique_ptr<ControlProxy<Value, Parameter>>(nullptr);
    }
    return std::make_unique<ControlProxy<Value, Parameter>>(value);
}

template<typename Value, typename Parameter>
Value ControlFactory<Value, Parameter>::getValue() const {
//    return getProxy().get();
}

template<typename Value, typename Parameter>
Parameter ControlFactory<Value, Parameter>::getParameter() const {

}

template<typename Value, typename Parameter>
Parameter ControlFactory<Value, Parameter>::getParameterForValue(Value value) const {

}

template<typename Value, typename Parameter>
Value ControlFactory<Value, Parameter>::defaultValue() const {

}

} // namespace
