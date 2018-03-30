//
// Created by ferran on 30/03/18.
//

#include "controllerenginefactory.h"
#include "controllers/engine/controllerqscriptengine.h"

// static
ControllerEngine* ControllerEngineFactory::getControllerEngineForController(Controller* controller) {
    return new ControllerQScriptEngine(controller);
}