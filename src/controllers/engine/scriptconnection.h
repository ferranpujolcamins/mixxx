#ifndef MIXXX_SCRIPTCONNECTION_H
#define MIXXX_SCRIPTCONNECTION_H

#include <QtScript>

#include "preferences/usersettings.h"

class ControllerEngine;

// ScriptConnection represents a connection between
// a ControlObject and a script callback function that gets executed when
// the value of the ControlObject changes.
class ScriptConnection {
  public:
    ConfigKey key;
    QUuid id;
    QScriptValue callback;
    ControllerEngine *controllerEngine;
    QScriptValue context;

    void executeCallback(double value) const;

    // Required for various QList methods and iteration to work.
    inline bool operator==(const ScriptConnection& other) const {
        return id == other.id;
    }
    inline bool operator!=(const ScriptConnection& other) const {
        return !(*this == other);
    }
};

#endif //MIXXX_SCRIPTCONNECTION_H
