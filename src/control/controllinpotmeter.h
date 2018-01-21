#ifndef CONTROLLINPOTMETER_H
#define CONTROLLINPOTMETER_H

#include "control/controlpotmeter.h"

class ControlLinPotmeter : public ControlPotmeter {
    Q_OBJECT
  public:
    // dStep = 0 and dSmallStep = 0 defaults to 10 and 100 steps
    ControlLinPotmeter(ConfigKey key,
                        ControlLinPotmeterParameters parameters = ControlLinPotmeterParameters());
};

#endif // CONTROLLINPOTMETER_H
