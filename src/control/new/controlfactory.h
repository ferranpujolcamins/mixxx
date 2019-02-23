#pragma once

#include "control/new/controlvalueinterface.h"
#include "preferences/configobject.h"

namespace NewControl {

template<typename Value, typename Parameter>
class ControlObject;

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
    ControlFactory(Group group, QString sItemKey, bool bPersistInConfiguration, Value initialValue)
      : m_group(group), m_sItemKey(sItemKey), m_bPersistInConfiguration(bPersistInConfiguration) {
    };
    virtual ~ControlFactory() {};

    std::unique_ptr<ControlObject<Value, Parameter> > create();

//    ControlProxy<Value, Parameter> getProxy() {
//        return ControlProxy<Value, Parameter>();
//    }

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
};

} // namespace

#include "control/new/controlvaluestore.h"
#include "control/new/controlvalue.h"

namespace NewControl {

template<typename Value, typename Parameter>
std::unique_ptr<ControlObject<Value, Parameter> > ControlFactory<Value, Parameter>::create() {
//    ControlValuePointer<Value, Parameter> value = std::make_shared<ControlValue<Value, Parameter> >(*this);
//    if (!ControlValueStore<Value, Parameter>::insert(configKey(), value)) {
//        // TODO: handle error. careful, we have already consumed this
//    }
//    return ControlObject<Value, Parameter>(value);
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
