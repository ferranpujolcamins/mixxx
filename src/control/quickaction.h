#pragma once

#include <QObject>

#include "control/controlobject.h"
#include "rigtorp/MPMCQueue.h"

class ConfigKey;

// TODO: memory order & memory barriers?

class QuickAction : public QObject {
    Q_OBJECT
  public:
    explicit QuickAction(QObject* parent = nullptr);

    // Record a new value for a CO.
    //
    // You must pass to the recordingSession parameter the value you read from .... XXXX
    //
    // Returns true if the macro recorder was in recording state and the new value was recorded,
    // returns false if the new value was not recorded.
    bool recordCOValue(const ConfigKey& key, double value);

    void trigger(double value);

    // A positive value means we are recording. A negative value indicates we are not recording.
    // The magnitude of the value is used to determine what "recording session" a control change belongs to.
    //
    // COs read this value to determine whether they should call the recordCOValue method (positive value) or not (negative value).
    // When COs call the recordCOValue method, they must pass back the recordingSession value they got. This allows us to detect CO values
    // that we receive after the QuickAction has already been triggered.
    RecordingSessionId recordingSession() {
        return RecordingSessionId(m_recordingSession.load());
    }

  private:
    using RecordingSessionId = unsigned int;

    void toggleRecording() {
        m_recordingSession += QuickAction::recordingBoundary;
    }

    // Ends recording session and increases m_recordingSession so its ready for next session.
    // Returns the id of the just ended session
    unsigned int endRecordingSession() {
        // Add placeholder to queue
        m_recordedValues.try_emplace();
        // TODO: what if a recording is inserted after the placeholder but before the recording session is increased??

        // TODO: can we handle gracefully the case when the queue is full?
        // If we could get the nª of elements in the queue we could count how many we pop so we wouldn't need the placeholder.
        // Or modify MPMC queue to have a better api to consume the queue up to the current last element

        return m_recordingSession.fetch_add(std::numeric_limits<unsigned int>::digits);
    }

    ControlObject m_coRecordingMacro;
    ControlObject m_coTrigger;

    // If m_recordingSession >= boundary, it means the quick action is recording. Otherwise, the quick action is not recording.
    static constexpr int recordingBoundary = std::numeric_limits<unsigned int>::digits - 1;
    std::atomic_uint m_recordingSession;
    static_assert(std::numeric_limits<unsigned int>::is_modulo);
    static_assert(std::numeric_limits<unsigned int>::radix == 2);

    std::atomic_bool m_recordingBlocked;

    class QueueElement {
      public:
        QueueElement() noexcept
                : m_bEndOfRecordingSessionPlaceholder(true) {
        }

        QueueElement(ConfigKey key, double dValue, RecordingSessionId recordingSessionId) noexcept
                : m_key(key),
                  m_dValue(dValue),
                  m_bEndOfRecordingSessionPlaceholder(false),
                  m_recordingSessionId(recordingSessionId) {
        }

        ConfigKey m_key;
        double m_dValue;
        bool m_bEndOfRecordingSessionPlaceholder;
        RecordingSessionId m_recordingSessionId;
    };

    rigtorp::MPMCQueue<QueueElement> m_recordedValues;
};
