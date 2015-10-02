
#ifndef CONTROLAUDIOTAPERPOT_H
#define CONTROLAUDIOTAPERPOT_H

#include "control/controlpotmeter.h"
#include "preferences/usersettings.h"

class ControlAudioTaperPot : public ControlPotmeter {
    Q_OBJECT
  public:
    // The AudioTaperPot has a log scale, starting at -Infinity
    // minDB is the Start value of the pure db scale it cranked to -Infinity by the linear part of the AudioTaperPot
    // maxDB is the Upper gain Value
    // scaleStartParameter is a knob position between 0 and 1 where the gain is 1 (0dB)
    ControlAudioTaperPot(ConfigKey key, double minDB, double maxDB,
                         double zeroDbParameter);
    ControlAudioTaperPot(ConfigKey key, double minDB, double maxDB,
                         double zeroDbParameter, double scaleStartParameter);
};

#endif // CONTROLAUDIOTAPERPOT_H
