#pragma once

#include <QObject>

// A QObject with only a single void signal.
// We use this object to provide non-QObjects with a signal they can emit and subscribe QObjects to it.
class VoidSignal: public QObject {
    Q_OBJECT
  public:
    ~VoidSignal();
  signals:
    void signal();
};

// TODO: find a better place for this, we use it only for tests
class VoidSlot: public QObject {
    Q_OBJECT
  public:
    VoidSlot(std::function<void(void)> fCallback);
    ~VoidSlot();
  public slots:
    void slot();
  private:
    std::function<void(void)> m_fCallback;
};
