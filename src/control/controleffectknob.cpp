#include "control/controleffectknob.h"

#include "util/math.h"
#include "effects/effectmanifestparameter.h"

ControlEffectKnob::ControlEffectKnob(ConfigKey key, EffectKnobParameters parameters)
        : ControlPotmeter(key, parameters) {
}

void ControlEffectKnob::setBehavior(EffectManifestParameter::ControlHint type,
                                     double dMinValue, double dMaxValue,
                                     double dScaleStartParameter) {
    if (m_pControl == NULL) {
        return;
    }

    if (type == EffectManifestParameter::ControlHint::KNOB_LINEAR) {
            m_pControl->setBehavior(new ControlLinPotmeterBehavior(
                    dMinValue, dMaxValue, dScaleStartParameter, false));
    } else if (type == EffectManifestParameter::ControlHint::KNOB_LINEAR_INVERSE) {
            m_pControl->setBehavior(new ControlLinInvPotmeterBehavior(
                    dMinValue, dMaxValue, dScaleStartParameter, false));
    } else if (type == EffectManifestParameter::ControlHint::KNOB_LOGARITHMIC) {
        if (dMinValue == 0) {
            if (dMaxValue == 1.0) {
                // Volume like control
                m_pControl->setBehavior(
                        new ControlAudioTaperPotBehavior(-20, 0, 1, dScaleStartParameter));
            } else if (dMaxValue > 1.0) {
                // Gain like control
                m_pControl->setBehavior(
                        new ControlAudioTaperPotBehavior(-12, ratio2db(dMaxValue), 0.5, dScaleStartParameter));
            } else {
                m_pControl->setBehavior(
                        new ControlLogPotmeterBehavior(dMinValue, dMaxValue, dScaleStartParameter, -40));
            }
        } else {
            m_pControl->setBehavior(
                    new ControlLogPotmeterBehavior(dMinValue, dMaxValue, dScaleStartParameter, -40));
        }
    } else if (type == EffectManifestParameter::ControlHint::KNOB_LOGARITHMIC_INVERSE) {
        m_pControl->setBehavior(
                new ControlLogInvPotmeterBehavior(dMinValue, dMaxValue, dScaleStartParameter, -40));
    }
}
