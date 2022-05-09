#include "quickaction.h"

#include "control/controlproxy.h"
#include "preferences/usersettings.h"

QuickAction::QuickAction(int iIndex)
        : QObject(),
          m_coRecording(ConfigKey(QString("[QuickAction%1]").arg(iIndex), "recording")),
          m_coTrigger(ConfigKey(QString("[QuickAction%1]").arg(iIndex), "trigger")),
          m_coClear(ConfigKey(QString("[QuickAction%1]").arg(iIndex), "clear")),
          m_iIndex(iIndex),
          m_recordedValues() {
    m_coRecording.setButtonMode(ControlPushButton::TOGGLE);
    m_coTrigger.setButtonMode(ControlPushButton::TRIGGER);
    m_coClear.setButtonMode(ControlPushButton::TRIGGER);

    connect(&m_coTrigger,
            &ControlObject::valueChanged,
            this,
            &QuickAction::slotTriggered);
    connect(&m_coClear,
            &ControlObject::valueChanged,
            this,
            &QuickAction::slotCleared);
}

bool QuickAction::recordCOValue(const ConfigKey& key, double value) {
    if (m_coRecording.toBool()) {
        MWriteLocker lock(&m_recordedValuesLock);
        m_recordedValues.append({key, value});
        return true;
    }
    return false;
}

void QuickAction::trigger() {
    MWriteLocker lock(&m_recordedValuesLock);
    QHash<ConfigKey, unsigned int> validValue;

    int numValues = m_recordedValues.size();
    for (int i = 0; i < numValues; ++i) {
        validValue[m_recordedValues[i].m_key] = i;
    }

    for (int i = 0; i < numValues; ++i) {
        const ConfigKey& key = m_recordedValues[i].m_key;
        double value = m_recordedValues[i].m_dValue;
        if (validValue[key] == i) {
            ControlProxy(key).set(value);
        }
    }
}

void QuickAction::clear() {
    MWriteLocker lock(&m_recordedValuesLock);
    m_recordedValues.clear();
}

void QuickAction::slotTriggered(double d) {
    Q_UNUSED(d);
    m_coRecording.set(0);
    trigger();
}

void QuickAction::slotCleared(double d) {
    Q_UNUSED(d);
    clear();
}
