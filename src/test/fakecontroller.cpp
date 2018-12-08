#include <test/fakecontroller.h>

FakeController::FakeController()
        : m_bMidiPreset(false),
          m_bHidPreset(false) {
}

FakeController::~FakeController() {
}

ControllerJSProxy* FakeController::jsProxy() {
    return new FakeControllerJSProxy(this);
}

QString FakeController::presetExtension() {
    // Doesn't affect anything at the moment.
    return ".test.xml";
}

ControllerPresetPointer FakeController::getPreset() const {
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

bool FakeController::savePreset(const QString fileName) const {
    Q_UNUSED(fileName);
    return true;
}

void FakeController::visit(const MidiControllerPreset* preset) {
    m_bMidiPreset = true;
    m_bHidPreset = false;
    m_midiPreset = *preset;
    m_hidPreset = HidControllerPreset();
}

void FakeController::visit(const HidControllerPreset* preset) {
    m_bMidiPreset = false;
    m_bHidPreset = true;
    m_midiPreset = MidiControllerPreset();
    m_hidPreset = *preset;
}

void FakeController::accept(ControllerVisitor* visitor) {
    // Do nothing since we aren't a normal controller.
    Q_UNUSED(visitor);
}

bool FakeController::isMappable() const {
    if (m_bMidiPreset) {
        return m_midiPreset.isMappable();
    } else if (m_bHidPreset) {
        return m_hidPreset.isMappable();
    }
    return false;
}

bool FakeController::matchPreset(const PresetInfo& preset) {
    // We're not testing product info matching in this test.
    Q_UNUSED(preset);
    return false;
}

ControllerPreset* FakeController::preset() {
    if (m_bHidPreset) {
        return &m_hidPreset;
    } else {
        // Default to MIDI.
        return &m_midiPreset;
    }
}
