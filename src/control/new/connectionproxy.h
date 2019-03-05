#pragma once

#include <QObject>
#include "control/new/voidsignal.h"
#include "util/assert.h"

namespace NewControl {

// ConnectionProxy classes encapsulate a VoidSignal that is connected to a source VoidSignal.
// Client QObjects can then connect to the ConnectionProxy and receive the source signal events:
//     [source] -> [proxy] -> [client]
// The ConnectionProxy controls what type of Qt connections are used.


// Connects the proxy to the source using Qt::DirectConnection.
class DirectConnectionProxy {
public:
    // The sender is owned by ControlValue, which is owned by Proxy/Object, which also own this class.
    // Thus we don't need to worry about the owenrship of pSender.
    DirectConnectionProxy(const VoidSignal* pSourceSignal)
        : m_pProxySignal(new VoidSignal()) {
        QObject::connect(pSourceSignal, &VoidSignal::signal,
                m_pProxySignal.get(), &VoidSignal::signal,
                Qt::DirectConnection);
    };

    ~DirectConnectionProxy() {};

    // Connect a client QObject to the proxy using the specified connectionType
    template<typename Client, typename Slot>
    QMetaObject::Connection connect(Client* client, Slot slot, Qt::ConnectionType connectionType = Qt::AutoConnection) {
        return QObject::connect(m_pProxySignal.get(), &VoidSignal::signal,
                client, slot,
                connectionType);
    };
private:
    std::unique_ptr<VoidSignal> m_pProxySignal;
};

// Connects the proxy to the source only after the client has been successfully connected to the proxy.
// The type of connection between the client and the proxy is automatically chosen.
// Can only be connected once. Subsequent connections always fail.
class SafeConnectionProxy {
public:
    SafeConnectionProxy(const VoidSignal* pSourceSignal)
        : m_pProxySignal(new VoidSignal()),
          m_pSourceSignal(pSourceSignal),
          m_bConnected(false) {
        // pSourceSignal is owned by ControlValue, which is owned by ControlProxy and/or ControlObject, which also own this class.
        // Thus pSourceSignal will be valid up until we are destroyed.
        QObject::connect(m_pSourceSignal, &VoidSignal::signal,
                m_pProxySignal.get(), &VoidSignal::signal,
                Qt::DirectConnection);
    };

    ~SafeConnectionProxy() {};

    bool connected() {
        return m_bConnected;
    }

    // Connect the source to the proxy using the specified connectionType
    // The connection type between the client QObject and the proxy is automatically chosen.
    template<typename Client, typename Slot>
    QMetaObject::Connection connect(Client* client, Slot slot, Qt::ConnectionType requestedConnectionType = Qt::AutoConnection) {
        DEBUG_ASSERT(!connected());
        if (connected()) {
            return QMetaObject::Connection();
        }

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

        QMetaObject::Connection connection = QObject::connect(m_pProxySignal.get(), &VoidSignal::signal, client, slot, scoConnection);
        if (!connection) {
            return QMetaObject::Connection();
        }

        // Connect to sender only if required. Do not allow
        // duplicate connections.

        // use only explicit direct connection if requested
        // the caller must not delete this until the all signals are
        // processed to avoid segfaults
        QObject::connect(m_pSourceSignal, &VoidSignal::signal,
                m_pProxySignal.get(), &VoidSignal::signal,
                static_cast<Qt::ConnectionType>(copConnection));
        m_bConnected = true;
        return connection;
    };
private:
    std::unique_ptr<VoidSignal> m_pProxySignal;
    const VoidSignal* m_pSourceSignal;
    bool m_bConnected;
};
} /* namespace NewControl */
