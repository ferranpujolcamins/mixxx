#ifndef SRC_TEST_FAKECONTROLLER_H
#define SRC_TEST_FAKECONTROLLER_H

#include "controllers/controller.h"
#include "controllers/midi/midicontrollerpreset.h"
#include "controllers/hid/hidcontrollerpreset.h"

class FakeController : public Controller {
    Q_OBJECT
  public:
    FakeController();
    ~FakeController() override;

    virtual ControllerJSProxy* jsProxy();

    QString presetExtension() override;

    ControllerPresetPointer getPreset() const override;

    bool savePreset(const QString fileName) const override;

    void visit(const MidiControllerPreset* preset) override;

    void visit(const HidControllerPreset* preset) override;

    void accept(ControllerVisitor* visitor) override;

    bool isMappable() const override;

    bool matchPreset(const PresetInfo& preset) override;

  protected:
    using Controller::send;

  private slots:
    int open() override {
        return 0;
    }
    int close() override {
        return 0;
    }

  private:
    void send(QByteArray data) override {
        Q_UNUSED(data);
    }
    virtual void send(QByteArray data, unsigned int reportID) {
        Q_UNUSED(data);
        Q_UNUSED(reportID);
    }

    ControllerPreset* preset() override;

    bool m_bMidiPreset;
    bool m_bHidPreset;
    MidiControllerPreset m_midiPreset;
    HidControllerPreset m_hidPreset;
};

class FakeControllerJSProxy: public ControllerJSProxy {
  Q_OBJECT
  public:
    FakeControllerJSProxy(FakeController* m_pController)
        : ControllerJSProxy(m_pController) {
    }

    Q_INVOKABLE void sendShortMsg(unsigned char status,
            unsigned char byte1, unsigned char byte2) {
        Q_UNUSED(status);
        Q_UNUSED(byte1);
        Q_UNUSED(byte2);
    }

    Q_INVOKABLE void sendSysexMsg(QList<int> data, unsigned int length) {
        Q_UNUSED(data);
        Q_UNUSED(length);
    }
};

#endif /* SRC_TEST_FAKECONTROLLER_H */
