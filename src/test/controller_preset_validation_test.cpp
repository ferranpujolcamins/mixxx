#include <gtest/gtest.h>

#include <QDir>
#include <QUrl>
#include <QScopedPointer>
#include <QtDebug>

#include "controllers/controllerpresetfilehandler.h"
#include "controllers/controllerpresetinfoenumerator.h"
#include "controllers/midi/midicontroller.h"

#include "controllers/defs_controllers.h"
#include "test/mixxxtest.h"
#include "test/fakecontroller.h"

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
        controller.startEngine();
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
