//
// Created by ferran on 30/03/18.
//

#include "controllerenginefactory.h"

// static
ControllerQScriptEngine* ControllerEngineFactory::getControllerEngineForController(Controller* controller) {
    return new ControllerQScriptEngine(controller);
}