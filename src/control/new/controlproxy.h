#pragma once

#include "basecontrolproxy.h"

namespace NewControl {

// Used to read/write a ControlValue (guarantees the value is still alive while this object is alive)
template<typename Value, typename Parameter>
class ControlProxy: public BaseControlProxy<Value, Parameter> {
  public:
    ControlProxy(ControlProxy<Value, Parameter>& other)
        : ControlProxy(other.m_pControlValue) {
    }

    // TODO: make private
    ControlProxy(ControlValuePointer<Value, Parameter> pControlValue)
          : BaseControlProxy<Value, Parameter>(pControlValue),
            m_connectionProxy(pControlValue->signal()) {};

    // TODO: make private, this is just to make tests easier
    ControlProxy()
        : BaseControlProxy<Value, Parameter>(std::shared_ptr<ControlValue<Value, Parameter>>()) {}

    // Connect a client QObject so it is notified when the ControlValue changes.
    // Can only be connected once. Subsequent connections always fail.
    template<typename Receiver, typename Slot>
    QMetaObject::Connection connect(Receiver* receiver, Slot slot, Qt::ConnectionType connectionType = Qt::AutoConnection) {
        return m_connectionProxy.connect(receiver, slot, connectionType);
    }

  private:
    SafeConnectionProxy m_connectionProxy;
    friend class ControlFactory<Value, Parameter>;
};

} //namespace
