#include "effects/chains/equalizereffectchain.h"

#include "effects/effectslot.h"

EqualizerEffectChain::EqualizerEffectChain(
        const ChannelHandleAndGroup& handleAndGroup,
        EffectsManager* pEffectsManager,
        EffectsMessengerPointer pEffectsMessenger)
        : PerGroupEffectChain(
                  handleAndGroup,
                  formatEffectChainGroup(handleAndGroup.name()),
                  SignalProcessingStage::Prefader,
                  pEffectsManager,
                  pEffectsMessenger),
          m_eqButtonMode(ConfigKey("[Mixer Profile]", "EQButtonMode")),
          m_pCOFilterWaveform(
                  new ControlObject(ConfigKey(handleAndGroup.name(), "filterWaveformEnable"))),
          m_pCOFilterLowKill(
                  new ControlPushButton(ConfigKey(handleAndGroup.name(), "filterLowKill"))),
          m_pCOFilterMidKill(
                  new ControlPushButton(ConfigKey(handleAndGroup.name(), "filterMidKill"))),
          m_pCOFilterHighKill(
                  new ControlPushButton(ConfigKey(handleAndGroup.name(), "filterHighKill"))) {
    // Add a single effect slot
    addEffectSlot(formatEffectSlotGroup(handleAndGroup.name()));
    enableForInputChannel(handleAndGroup);
    m_effectSlots[0]->setEnabled(true);

    QObject::connect(this,
            &EffectChain::chainPresetChanged,
            this,
            [this](const QString& presetname) {
                Q_UNUSED(presetname);
                setFilterWaveform(
                        m_effectSlots.at(0)->getManifest()->isMixingEQ());
            });
    // DlgPrefEq loads the Effect with loadEffectToGroup

    m_pCOFilterLowKill->setButtonMode(ControlPushButton::POWERWINDOW);
    m_pCOFilterMidKill->setButtonMode(ControlPushButton::POWERWINDOW);
    m_pCOFilterHighKill->setButtonMode(ControlPushButton::POWERWINDOW);

    setupAliasesForGroup(handleAndGroup.name());
}

void EqualizerEffectChain::setFilterWaveform(bool state) {
    m_pCOFilterWaveform->set(state);
}

QString EqualizerEffectChain::formatEffectChainGroup(const QString& group) {
    return QString("[EqualizerRack1_%1]").arg(group);
}

QString EqualizerEffectChain::formatEffectSlotGroup(const QString& group) {
    return QString("[EqualizerRack1_%1_Effect1]")
            .arg(group);
}

void EqualizerEffectChain::slotFilterLowKillChanged(double value) {
    EffectSlotPointer pEffectSlot = getEffectSlot(0);
    if (pEffectSlot) {
        const QString& effectSlotGroup = pEffectSlot->getGroup();
        PollingControlProxy effectButton(ConfigKey(effectSlotGroup,
                m_eqButtonMode.toBool() ? "button_parameter2"
                                        : "button_parameter1"));
        VERIFY_OR_DEBUG_ASSERT(effectButton.valid()) {
            return;
        }
        effectButton.set(value);
    }
}

void EqualizerEffectChain::slotFilterMidKillChanged(double value) {
    EffectSlotPointer pEffectSlot = getEffectSlot(0);
    if (pEffectSlot) {
        const QString& effectSlotGroup = pEffectSlot->getGroup();
        PollingControlProxy effectButton(ConfigKey(effectSlotGroup,
                m_eqButtonMode.toBool() ? "button_parameter4"
                                        : "button_parameter3"));
        VERIFY_OR_DEBUG_ASSERT(effectButton.valid()) {
            return;
        }
        effectButton.set(value);
    }
}

void EqualizerEffectChain::slotFilterHighKillChanged(double value) {
    EffectSlotPointer pEffectSlot = getEffectSlot(0);
    if (pEffectSlot) {
        const QString& effectSlotGroup = pEffectSlot->getGroup();
        PollingControlProxy effectButton(ConfigKey(effectSlotGroup,
                m_eqButtonMode.toBool() ? "button_parameter6"
                                        : "button_parameter5"));
        VERIFY_OR_DEBUG_ASSERT(effectButton.valid()) {
            return;
        }
        effectButton.set(value);
    }
}

void EqualizerEffectChain::setupAliasesForGroup(const QString& group) {
    // Create aliases for controller EQ controls.
    EffectSlotPointer pEffectSlot = getEffectSlot(0);
    if (pEffectSlot) {
        const QString& effectSlotGroup = pEffectSlot->getGroup();
        PollingControlProxy eqButtonMode(ConfigKey("[Mixer Profile]", "EQButtonMode"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterLow"),
                ConfigKey(effectSlotGroup, "parameter1"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterMid"),
                ConfigKey(effectSlotGroup, "parameter2"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterHigh"),
                ConfigKey(effectSlotGroup, "parameter3"));

        connect(m_pCOFilterLowKill.get(),
                &ControlObject::valueChanged,
                this,
                &EqualizerEffectChain::slotFilterLowKillChanged);
        connect(m_pCOFilterMidKill.get(),
                &ControlObject::valueChanged,
                this,
                &EqualizerEffectChain::slotFilterMidKillChanged);
        connect(m_pCOFilterHighKill.get(),
                &ControlObject::valueChanged,
                this,
                &EqualizerEffectChain::slotFilterHighKillChanged);

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterLow_loaded"),
                ConfigKey(effectSlotGroup, "parameter1_loaded"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterMid_loaded"),
                ConfigKey(effectSlotGroup, "parameter2_loaded"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterHigh_loaded"),
                ConfigKey(effectSlotGroup, "parameter3_loaded"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterLowKill_loaded"),
                ConfigKey(effectSlotGroup, "button_parameter1_loaded"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterMidKill_loaded"),
                ConfigKey(effectSlotGroup, "button_parameter2_loaded"));

        ControlDoublePrivate::insertAlias(ConfigKey(group, "filterHighKill_loaded"),
                ConfigKey(effectSlotGroup, "button_parameter3_loaded"));
    }
}
