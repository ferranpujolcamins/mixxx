#include "control/controllinpotmeter.h"

ControlLinPotmeter::ControlLinPotmeter(ConfigKey key, ControlLinPotmeterParameters parameters)
        : ControlPotmeter(key, parameters) {
    if (m_pControl) {
        m_pControl->setBehavior(
            new ControlLinPotmeterBehavior(parameters.minValue(),
                parameters.maxValue(), parameters.scaleStartParameter(),
                parameters.allowOutOfBounds()));
    }
    if (parameters.step()) {
        setStepCount((parameters.maxValue() - parameters.minValue())
                        / parameters.step());
    }
    if (parameters.smallStep()) {
        setSmallStepCount((parameters.maxValue() - parameters.minValue())
                        / parameters.smallStep());
    }
}
