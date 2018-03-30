#ifndef MIXXX_CONTROLLERENGINEFACTORY_H
#define MIXXX_CONTROLLERENGINEFACTORY_H

#include "controllers/controller.h"
#include "controllers/engine/controllerengine.h"
#include "util/singleton.h"

class ControllerEngineFactory {
  public:
    static ControllerQScriptEngine* getControllerEngineForController(Controller* controller);
};

#endif //MIXXX_CONTROLLERENGINEFACTORY_H
