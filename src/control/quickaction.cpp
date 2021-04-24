#include "quickaction.h"

#include "control/controlproxy.h"
#include "preferences/usersettings.h"

QuickAction::QuickAction(QObject* parent)
        : QObject(parent),
          m_coRecordingMacro(ConfigKey("[QuickAction]", "recording")),
          m_coTrigger(ConfigKey("[QuickAction]", "trigger")),
          m_recordedValues(100),
          m_recordingSession(0) {
}

bool QuickAction::recordCOValue(const ConfigKey& key, double value) {
    RecordingSessionId recordingSession =
            m_recordingSession.load() if (recordingSession.recording) {
        // TODO: what happens if the sessions ends before we emplace?
        // TODO: we immediately trigger right?
        // TODO: how to gracefully handle the case where the queue is full?
        if (m_recordedValues.try_emplace(key, value)) {
            if (m_recordingSession.load() != recordingSession) {
                return false;
            }
        }
        //
    }
    return false;
}

void QuickAction::trigger(double) {
    m_recordingBlocked.store(true);
    m_recordingSession.stop();

    QHash<ConfigKey, double> values;
    QueueElement recordedValue;
    // We need to make room for the placeholder, since the queue could be full.
    while (!m_recordedValues.try_emplace()) {
        // Since try_emplace failed, we know there's something on the queue, thus pop won't block.
        // TODO: can this invariant break here? because someone else pops maybe?

        m_recordedValues.pop(recordedValue);
        // TODO: can the poped value be from the wrong recording session?
        values[recordedValue.m_key] = recordedValue.m_dValue;
    }

    // TODO: could someone else change the recording session value while doing all this ^ ???
    // TODO: What happens if elements are added to the queue while doing this??

    m_recordingSession.increase();

    QHash<ConfigKey, double> values;

    while (m_recordedValues.try_pop(recordedValue) &&
            !recordedValue.m_bEndOfRecordingSessionPlaceholder) {
        if (recordedValue.m_recordingSessionId == recordingSessionId) {
            values[recordedValue.m_key] = recordedValue.m_dValue;
        }
    }

    for (ConfigKey configKey : values.keys()) {
        double value = values[configKey];
        ControlProxy(configKey).set(value);
    }
}
