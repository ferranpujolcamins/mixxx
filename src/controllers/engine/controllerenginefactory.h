#ifndef MIXXX_CONTROLLERENGINEFACTORY_H
#define MIXXX_CONTROLLERENGINEFACTORY_H

class Controller;
class ControllerEngine;

class ControllerEngineFactory {
  public:
    static ControllerEngine* getControllerEngineForController(Controller* controller);
};

#endif //MIXXX_CONTROLLERENGINEFACTORY_H
