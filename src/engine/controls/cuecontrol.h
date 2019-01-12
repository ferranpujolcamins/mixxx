// cuecontrol.h
// Created 11/5/2009 by RJ Ryan (rryan@mit.edu)

#ifndef CUECONTROL_H
#define CUECONTROL_H

#include <QList>
#include <QMutex>

#include "engine/controls/enginecontrol.h"
#include "preferences/usersettings.h"
#include "control/controlproxy.h"
#include "track/track.h"
#include "util/memory.h"

#define NUM_HOT_CUES 37

class ControlObject;
class ControlPushButton;
class ControlIndicator;

class HotcueControl : public QObject {
    Q_OBJECT
  public:
    HotcueControl(QString group, int hotcueNumber);
    virtual ~HotcueControl();

    inline int getHotcueNumber() { return m_iHotcueNumber; }
    inline CuePointer getCue() { return m_pCue; }
    double getPosition() const;
    void setPosition(double position);
    // Some CueTypes represent a range rather than a single position. This is the lenght of the range.
    double getLength() const;
    void setLength(double length);
    void setCue(CuePointer pCue);
    void resetCue();
    Cue::CueType getType() const;
    void setType(Cue::CueType newType);

    // Used for caching the preview state of this hotcue control.
    inline bool isPreviewing() {
        return m_bPreviewing;
    }
    inline void setPreviewing(bool bPreviewing) {
        m_bPreviewing = bPreviewing;
    }
    inline double getPreviewingPosition() {
        return m_previewingPosition;
    }
    inline void setPreviewingPosition(double position) {
        m_previewingPosition = position;
    }

  private slots:
    void slotHotcueSet(double v);
    void slotHotcueGoto(double v);
    void slotHotcueGotoAndPlay(double v);
    void slotHotcueGotoAndStop(double v);
    void slotHotcueActivate(double v);
    void slotHotcueActivatePreview(double v);
    void slotHotcueClear(double v);
    void slotHotcuePositionChanged(double newPosition);
    void slotHotcueLengthChanged(double newLength);
    void slotHotcueTypeChanged(double newType);

  signals:
    void hotcueSet(HotcueControl* pHotcue, double v);
    void hotcueGoto(HotcueControl* pHotcue, double v);
    void hotcueGotoAndPlay(HotcueControl* pHotcue, double v);
    void hotcueGotoAndStop(HotcueControl* pHotcue, double v);
    void hotcueActivate(HotcueControl* pHotcue, double v);
    void hotcueActivatePreview(HotcueControl* pHotcue, double v);
    void hotcueClear(HotcueControl* pHotcue, double v);
    void hotcuePositionChanged(HotcueControl* pHotcue, double newPosition);
    void hotcueLengthChanged(HotcueControl* pHotcue, double newLength);
    void hotcueTypeChanged(HotcueControl* pHotcue, double newType);
    void hotcuePlay(double v);

  private:
    ConfigKey keyForControl(int hotcue, const char* name);

    QString m_group;
    int m_iHotcueNumber;
    CuePointer m_pCue;

    // Hotcue state controls
    ControlObject* m_hotcuePosition;
    std::unique_ptr<ControlObject> m_hotcueLength;
    ControlObject* m_hotcueEnabled;
    std::unique_ptr<ControlObject> m_hotcueType;
    // Hotcue button controls
    ControlObject* m_hotcueSet;
    ControlObject* m_hotcueGoto;
    ControlObject* m_hotcueGotoAndPlay;
    ControlObject* m_hotcueGotoAndStop;
    ControlObject* m_hotcueActivate;
    ControlObject* m_hotcueActivatePreview;
    ControlObject* m_hotcueClear;

    bool m_bPreviewing;
    double m_previewingPosition;
};

class CueControl : public EngineControl {
    Q_OBJECT
  public:
    CueControl(QString group,
               UserSettingsPointer pConfig);
    ~CueControl() override;

    virtual void hintReader(HintVector* pHintList) override;
    bool updateIndicatorsAndModifyPlay(bool newPlay, bool playPossible);
    void updateIndicators();
    void resetIndicators();
    bool isPlayingByPlayButton();
    bool getPlayFlashingAtPause();
    void trackLoaded(TrackPointer pNewTrack) override;

  private slots:
    void cueUpdated();
    void trackCuesUpdated();
    void hotcueSet(HotcueControl* pControl, double v);
    void hotcueGoto(HotcueControl* pControl, double v);
    void hotcueGotoAndPlay(HotcueControl* pControl, double v);
    void hotcueGotoAndStop(HotcueControl* pControl, double v);
    void hotcueActivate(HotcueControl* pControl, double v);
    void hotcueActivatePreview(HotcueControl* pControl, double v);
    void hotcueClear(HotcueControl* pControl, double v);
    void hotcuePositionChanged(HotcueControl* pControl, double newPosition);
    void hotcueLengthChanged(HotcueControl* pControl, double newLength);
    void hotcueTypeChanged(HotcueControl* pControl, int newType);

    void cueSet(double v);
    void cueGoto(double v);
    void cueGotoAndPlay(double v);
    void cueGotoAndStop(double v);
    void cuePreview(double v);
    void cueCDJ(double v);
    void cueDenon(double v);
    void cuePlay(double v);
    void cueDefault(double v);
    void pause(double v);
    void playStutter(double v);

  private:
    enum class TrackAt {
        Cue,
        End,
        ElseWhere
    };

    // These methods are not thread safe, only call them when the lock is held.
    void createControls();
    void attachCue(CuePointer pCue, int hotcueNumber);
    void detachCue(int hotcueNumber);
    TrackAt getTrackAt() const;

    bool m_bPreviewing;
    ControlObject* m_pPlay;
    ControlObject* m_pStopButton;
    int m_iCurrentlyPreviewingHotcues;
    ControlObject* m_pQuantizeEnabled;
    ControlObject* m_pNextBeat;
    ControlObject* m_pClosestBeat;
    bool m_bypassCueSetByPlay;

    const int m_iNumHotCues;
    QList<HotcueControl*> m_hotcueControls;

    ControlObject* m_pTrackSamples;
    ControlObject* m_pCuePoint;
    ControlObject* m_pCueMode;
    ControlPushButton* m_pCueSet;
    ControlPushButton* m_pCueCDJ;
    ControlPushButton* m_pCueDefault;
    ControlPushButton* m_pPlayStutter;
    ControlIndicator* m_pCueIndicator;
    ControlIndicator* m_pPlayIndicator;
    ControlPushButton* m_pCueGoto;
    ControlPushButton* m_pCueGotoAndPlay;
    ControlPushButton* m_pCuePlay;
    ControlPushButton* m_pCueGotoAndStop;
    ControlPushButton* m_pCuePreview;
    ControlProxy* m_pVinylControlEnabled;
    ControlProxy* m_pVinylControlMode;
    std::unique_ptr<ControlProxy> m_pLoopEnabled;
    std::unique_ptr<ControlProxy> m_pReloop;
    std::unique_ptr<ControlProxy> m_pReloopAndPlay;
    std::unique_ptr<ControlProxy> m_pReloopAndStop;
    std::unique_ptr<ControlProxy> m_pLoopStartPosition;
    std::unique_ptr<ControlProxy> m_pLoopEndPosition;

    TrackPointer m_pLoadedTrack; // is written from an engine worker thread

    // Tells us which controls map to which hotcue
    QMap<QObject*, int> m_controlMap;

    QMutex m_mutex;
};


#endif /* CUECONTROL_H */
