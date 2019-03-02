#pragma once

#include <QObject>
#include "control/new/voidsignal.h"
#include "util/assert.h"

namespace NewControl {

// TODO: Do we really need the interfaces? Do we abstractly use objects implementing them somewhere?

class DirectConnectionProxy {
public:
    // The sender is owned by ControlValue, which is owned by Proxy/Object, which also own this class.
    // Thus we don't need to worry about the owenrship of pSender.
    DirectConnectionProxy(VoidSignal* pSender)
        : m_pSignal(new VoidSignal()) {
        QObject::connect(pSender, &VoidSignal::signal,
                m_pSignal.get(), &VoidSignal::signal,
                Qt::DirectConnection);
    };

    ~DirectConnectionProxy() {};

    template<typename Receiver, typename Slot>
    QMetaObject::Connection connect(Receiver* receiver, Slot slot, Qt::ConnectionType connectionType = Qt::AutoConnection) {
        return QObject::connect(m_pSignal.get(), &VoidSignal::signal,
                receiver, slot,
                connectionType);
    };
private:
    std::unique_ptr<VoidSignal> m_pSignal;
};

class MaybeDirectConnectionProxy {
public:
    // The sender is owned by ControlValue, which is owned by Proxy/Object, which also own this class.
    // Thus we don't need to worry about the owenrship of pSender.
    MaybeDirectConnectionProxy(VoidSignal* pSender)
        : m_pSignal(new VoidSignal()),
          m_pSender(pSender) {
        QObject::connect(m_pSender, &VoidSignal::signal,
                m_pSignal.get(), &VoidSignal::signal,
                Qt::DirectConnection);
    };

    ~MaybeDirectConnectionProxy() {};

    template<typename Receiver, typename Slot>
    QMetaObject::Connection connect(Receiver* receiver, Slot slot, Qt::ConnectionType requestedConnectionType = Qt::AutoConnection) {
        // We connect to the
        // sender only once and in a way that
        // the requested ConnectionType is working as desired.
        // We try to avoid direct connections if not requested
        // since you cannot safely delete an object with a pending
        // direct connection. This fixes bug Bug #1406124
        // requested: Auto -> COP = Auto / SCO = Auto
        // requested: Direct -> COP = Direct / SCO = Direct
        // requested: Queued -> COP = Queued / SCO = Auto
        // requested: BlockingQueued -> Assert(false)

        Qt::ConnectionType copConnection;
        Qt::ConnectionType scoConnection;
        switch(requestedConnectionType) {
        case Qt::AutoConnection:
            copConnection = Qt::AutoConnection;
            scoConnection = Qt::AutoConnection;
            break;
        case Qt::DirectConnection:
            copConnection = Qt::DirectConnection;
            scoConnection = Qt::DirectConnection;
            break;
        case Qt::QueuedConnection:
            copConnection = Qt::QueuedConnection;
            scoConnection = Qt::AutoConnection;
            break;
        case Qt::BlockingQueuedConnection:
            // We must not block the signal source by a blocking connection
            DEBUG_ASSERT(false);
            return QMetaObject::Connection();
        default:
            DEBUG_ASSERT(false);
            return QMetaObject::Connection();
        }

        QMetaObject::Connection connection = QObject::connect(m_pSignal.get(), &VoidSignal::signal, receiver, slot, scoConnection);
        if (!connection) {
            return QMetaObject::Connection();
        }

        // Connect to sender only if required. Do not allow
        // duplicate connections.

        // use only explicit direct connection if requested
        // the caller must not delete this until the all signals are
        // processed to avoid segfaults
        QObject::connect(m_pSender, &VoidSignal::signal,
                m_pSignal.get(), &VoidSignal::signal,
                static_cast<Qt::ConnectionType>(copConnection | Qt::UniqueConnection));
        return connection;
    };
private:
    std::unique_ptr<VoidSignal> m_pSignal;
    VoidSignal* m_pSender;
};
} /* namespace NewControl */
