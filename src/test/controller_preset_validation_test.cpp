#include <gtest/gtest.h>

#include <QDir>
#include <QUrl>
#include <QScopedPointer>
#include <QtDebug>

#include "controllers/controller.h"
#include "controllers/controllerpresetfilehandler.h"
#include "controllers/controllerpresetinfoenumerator.h"
#include "controllers/midi/midicontrollerpreset.h"
#include "controllers/hid/hidcontrollerpreset.h"
#include "controllers/defs_controllers.h"
#include "test/mixxxtest.h"

class FakeControllerJSProxy : public ControllerJSProxy {
  Q_OBJECT
  public:
    FakeControllerJSProxy();

    Q_INVOKABLE void send(QList<int> data, unsigned int length = 0) override {
        Q_UNUSED(data);
        Q_UNUSED(length);
    }

    Q_INVOKABLE void sendSysexMsg(QList<int> data, unsigned int length = 0) {
        Q_UNUSED(data);
        Q_UNUSED(length);
    }

    Q_INVOKABLE void sendShortMsg(unsigned char status,
            unsigned char byte1, unsigned char byte2) {
        Q_UNUSED(status);
        Q_UNUSED(byte1);
        Q_UNUSED(byte2);
    }
};

FakeControllerJSProxy::FakeControllerJSProxy()
        : ControllerJSProxy(nullptr) {
}

class FakeController : public Controller {
  public:
    FakeController();
    ~FakeController() override;

    QString presetExtension() override {
        // Doesn't affect anything at the moment.
        return ".test.xml";
    }

    ControllerJSProxy* jsProxy() override {
        return new FakeControllerJSProxy();
    }

    ControllerPresetPointer getPreset() const override {
        if (m_bHidPreset) {
            HidControllerPreset* pClone = new HidControllerPreset();
            *pClone = m_hidPreset;
            return ControllerPresetPointer(pClone);
        } else {
            MidiControllerPreset* pClone = new MidiControllerPreset();
            *pClone = m_midiPreset;
            return ControllerPresetPointer(pClone);
        }
    }

    bool savePreset(const QString fileName) const override {
        Q_UNUSED(fileName);
        return true;
    }

    void visit(const MidiControllerPreset* preset) override {
        m_bMidiPreset = true;
        m_bHidPreset = false;
        m_midiPreset = *preset;
        m_hidPreset = HidControllerPreset();
    }
    void visit(const HidControllerPreset* preset) override {
        m_bMidiPreset = false;
        m_bHidPreset = true;
        m_midiPreset = MidiControllerPreset();
        m_hidPreset = *preset;
    }

    void accept(ControllerVisitor* visitor) override {
        // Do nothing since we aren't a normal controller.
        Q_UNUSED(visitor);
    }

    bool isMappable() const override {
        if (m_bMidiPreset) {
            return m_midiPreset.isMappable();
        } else if (m_bHidPreset) {
            return m_hidPreset.isMappable();
        }
        return false;
    }

    bool matchPreset(const PresetInfo& preset) override {
        // We're not testing product info matching in this test.
        Q_UNUSED(preset);
        return false;
    }

  protected:
    void send(QList<int> data, unsigned int length) override {
        Q_UNUSED(data);
        Q_UNUSED(length);
    }

    void send(QByteArray data) {
        Q_UNUSED(data);
    }

  private slots:
    int open() override {
        return 0;
    }
    int close() override {
        return 0;
    }

  private:
    ControllerPreset* preset() override {
        if (m_bHidPreset) {
            return &m_hidPreset;
        } else {
            // Default to MIDI.
            return &m_midiPreset;
        }
    }

    bool m_bMidiPreset;
    bool m_bHidPreset;
    MidiControllerPreset m_midiPreset;
    HidControllerPreset m_hidPreset;
};

FakeController::FakeController()
        : m_bMidiPreset(false),
          m_bHidPreset(false) {
    startEngine();
    getEngine()->setTesting(true);
}

FakeController::~FakeController() {
}

class ControllerPresetValidationTest : public MixxxTest {
  protected:
    void SetUp() override {
        m_presetPaths << QDir::currentPath() + "/res/controllers";
        m_pEnumerator.reset(new PresetInfoEnumerator(m_presetPaths));
    }

    bool testLoadPreset(const PresetInfo& preset) {
        ControllerPresetPointer pPreset =
                ControllerPresetFileHandler::loadPreset(preset.getPath(),
                                                        m_presetPaths);
        if (pPreset.isNull()) {
            return false;
        }

        FakeController controller;
        controller.setDeviceName("Test Controller");
        controller.setPreset(*pPreset);
        // Do not initialize the scripts.
        bool result = controller.applyPreset(m_presetPaths, false);
        controller.stopEngine();
        return result;
    }


    QStringList m_presetPaths;
    QScopedPointer<PresetInfoEnumerator> m_pEnumerator;
};

bool checkUrl(const QString& url) {
    return QUrl(url).isValid();
}

bool lintPresetInfo(const PresetInfo& preset) {
    bool result = true;
    if (preset.getName().trimmed().isEmpty()) {
        qWarning() << "LINT:" << preset.getPath() << "has no name.";
        result = false;
    }

    if (preset.getAuthor().trimmed().isEmpty()) {
        qWarning() << "LINT:" << preset.getPath() << "has no author.";
    }

    if (preset.getDescription().trimmed().isEmpty()) {
        qWarning() << "LINT:" << preset.getPath() << "has no description.";
    }

    if (preset.getForumLink().trimmed().isEmpty()) {
        qWarning() << "LINT:" << preset.getPath() << "has no forum link.";
    } else if (!checkUrl(preset.getForumLink())) {
        qWarning() << "LINT:" << preset.getPath() << "has invalid forum link";
        result = false;
    }

    if (preset.getWikiLink().trimmed().isEmpty()) {
        qWarning() << "LINT:" << preset.getPath() << "has no wiki link.";
    } else if (!checkUrl(preset.getWikiLink())) {
        qWarning() << "LINT:" << preset.getPath() << "has invalid wiki link";
        result = false;
    }
    return result;
}

TEST_F(ControllerPresetValidationTest, MidiPresetsValid) {
    foreach (const PresetInfo& preset,
             m_pEnumerator->getPresetsByExtension(MIDI_PRESET_EXTENSION)) {
        qDebug() << "Validating " << preset.getPath();
        std::string errorDescription = "Error while validating " + preset.getPath().toStdString();
        EXPECT_TRUE(preset.isValid()) << errorDescription;
        EXPECT_TRUE(lintPresetInfo(preset)) << errorDescription;
        EXPECT_TRUE(testLoadPreset(preset)) << errorDescription;
    }
}

TEST_F(ControllerPresetValidationTest, HidPresetsValid) {
    foreach (const PresetInfo& preset,
             m_pEnumerator->getPresetsByExtension(HID_PRESET_EXTENSION)) {
        qDebug() << "Validating" << preset.getPath();
        std::string errorDescription = "Error while validating " + preset.getPath().toStdString();
        EXPECT_TRUE(preset.isValid()) << errorDescription;
        EXPECT_TRUE(lintPresetInfo(preset)) << errorDescription;
        EXPECT_TRUE(testLoadPreset(preset)) << errorDescription;
    }
}

TEST_F(ControllerPresetValidationTest, BulkPresetsValid) {
    foreach (const PresetInfo& preset,
             m_pEnumerator->getPresetsByExtension(BULK_PRESET_EXTENSION)) {
        qDebug() << "Validating" << preset.getPath();
        std::string errorDescription = "Error while validating " + preset.getPath().toStdString();
        EXPECT_TRUE(preset.isValid()) << errorDescription;
        EXPECT_TRUE(lintPresetInfo(preset)) << errorDescription;
        EXPECT_TRUE(testLoadPreset(preset)) << errorDescription;
    }
}
