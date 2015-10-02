#ifndef CONTROLBEHAVIOR_H
#define CONTROLBEHAVIOR_H

#include <QTimer>
#include <QScopedPointer>

#include "controllers/midi/midimessage.h"

class ControlDoublePrivate;

// A linear 0 .. 1 control without Midi representation
class ControlNumericBehavior {
  public:
    virtual ~ControlNumericBehavior() { };

    // this may change the dValue in place before it is adopted
    // Returns false to reject the new value entirely
    virtual bool setFilter(double* dValue);

    // returns the normalized parameter range 0..1
    virtual double valueToParameter(double dValue);
    // returns the normalized parameter range 0..1
    virtual double midiToParameter(double midiValue);
    // returns the scaled user visible value
    virtual double parameterToValue(double dParam);
    // returns the midi range parameter 0..127
    virtual double valueToMidiParameter(double dValue);

    virtual void setValueFromMidi(
            MidiOpCode o, double dParam, ControlDoublePrivate* pControl);
    
    // This value is used for the skin system to determine a position
    // of the control where its visual feedback should be off.
    virtual double scaleStartParameter();
};

class ControlPotmeterBehavior : public ControlNumericBehavior {
  public:
    ControlPotmeterBehavior(double dMinValue, double dMaxValue,
                            double dScaleStartParameter, bool allowOutOfBounds);

    bool setFilter(double* dValue) override;
    double valueToParameter(double dValue) override;
    double midiToParameter(double midiValue) override;
    double parameterToValue(double dParam) override;
    double valueToMidiParameter(double dValue) override;
    double scaleStartParameter() override;

  protected:
    double m_dMinValue;
    double m_dMaxValue;
    double m_dValueRange;
    double m_dScaleStartParameter;
    bool m_bAllowOutOfBounds;
};

class ControlLogPotmeterBehavior : public ControlPotmeterBehavior {
  public:
    ControlLogPotmeterBehavior(double dMinValue, double dMaxValue,
                               double dScaleStartParameter, double minDB);

    double valueToParameter(double dValue) override;
    double parameterToValue(double dParam) override;

  protected:
    double m_minDB;
    double m_minOffset;
};

class ControlLogInvPotmeterBehavior : public ControlLogPotmeterBehavior {
  public:
    ControlLogInvPotmeterBehavior(double dMinValue, double dMaxValue, double dScaleStartParameter, double minDB);

    double valueToParameter(double dValue) override;
    double parameterToValue(double dParam) override;
};

class ControlLinPotmeterBehavior : public ControlPotmeterBehavior {
  public:
    ControlLinPotmeterBehavior(
            double dMinValue, double dMaxValue, double dScaleStartParameter, bool allowOutOfBounds);
};

class ControlLinInvPotmeterBehavior : public ControlPotmeterBehavior {
  public:
    ControlLinInvPotmeterBehavior(
            double dMinValue, double dMaxValue, double dScaleStartParameter, bool allowOutOfBounds);
    double valueToParameter(double dValue) override;
    double parameterToValue(double dParam) override;
};

class ControlAudioTaperPotBehavior : public ControlPotmeterBehavior {
  public:
    ControlAudioTaperPotBehavior(double minDB, double maxDB,
                                 double zeroDbParameter,
                                 double scaleStartParameter);

    double valueToParameter(double dValue) override;
    double parameterToValue(double dParam) override;
    double midiToParameter(double midiValue) override;
    double valueToMidiParameter(double dValue) override;
    void setValueFromMidi(
            MidiOpCode o, double dParam, ControlDoublePrivate* pControl)
                    override;


  protected:
    // a knob position between 0 and 1 where the gain is 1 (0dB)
    double m_dZeroDbParameter;
    // the Start value of the pure db scale it cranked to -Infinity by the
    // linear part of the AudioTaperPot
    double m_minDB;
    // maxDB is the upper gain Value
    double m_maxDB;
    // offset at knob position 0 (Parameter = 0) to reach -Infinity
    double m_offset;
    // ensures that the neutral position on a integer midi value
    // This value is subtracted from the Midi value at neutral position
    // and is allways < 1
    double m_midiCorrection;
};

class ControlTTRotaryBehavior : public ControlNumericBehavior {
  public:
    double valueToParameter(double dValue) override;
    double parameterToValue(double dParam) override;
};

class ControlPushButtonBehavior : public ControlNumericBehavior {
  public:
    static const int kPowerWindowTimeMillis;
    static const int kLongPressLatchingTimeMillis;

    // TODO(XXX) Duplicated from ControlPushButton. It's complicated and
    // annoying to share them so I just copied them.
    enum ButtonMode {
         PUSH = 0,
         TOGGLE,
         POWERWINDOW,
         LONGPRESSLATCHING,
         TRIGGER
    };

    ControlPushButtonBehavior(ButtonMode buttonMode, int iNumStates);
    void setValueFromMidi(
            MidiOpCode o, double dParam, ControlDoublePrivate* pControl)
                override;

  private:
    // We create many hundreds of push buttons at Mixxx startup and most of them
    // never use their timer. Delay creation of the timer until it's needed.
    QTimer* getTimer() {
        if (m_pushTimer.isNull()) {
            m_pushTimer.reset(new QTimer());
        }
        return m_pushTimer.data();
    }
    ButtonMode m_buttonMode;
    int m_iNumStates;
    QScopedPointer<QTimer> m_pushTimer;
};

#endif /* CONTROLBEHAVIOR_H */
