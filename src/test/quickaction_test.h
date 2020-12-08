#include <gtest/gtest.h>

#include <QDebug>
#include <QObject>

#include "control/controlobject.h"
#include "control/quickaction.h"
#include "control/threadlocalquickaction.h"
#include "mixxxtest.h"
#include "util/memory.h"

class Value : public QObject {
    Q_OBJECT
  public:
    Value()
            : m_value(0) {
    }

    void slotSetValue(double value) {
        m_value = value;
    }

    double m_value;
};

class Counter : public QObject {
    Q_OBJECT
  public:
    Counter()
            : m_value(0) {
    }

    void slotSetValue(double) {
        m_value += 1;
    }

    double m_value;
};

class QuickActionTest : public MixxxTest {
  public:
    QuickActionTest();

    std::shared_ptr<ThreadLocalQuickAction> pQuickAction;
    ControlObject co1;
    ControlObject co2;
    ControlProxy coRecording;
    ControlProxy coTrigger;
    Value result;
    Counter setCount;
};