#include "controllers/engine/scriptconnection.h"
#include "controllers/engine/controllerqscriptengine.h"

/* -------- ------------------------------------------------------
   Purpose: Execute a ScriptConnection's callback
   Input:   the value of the connected ControlObject to pass to the callback
   -------- ------------------------------------------------------ */
void ScriptConnection::executeCallback(double value) const {
    QScriptValueList args;
    args << QScriptValue(value);
    args << QScriptValue(key.group);
    args << QScriptValue(key.item);
    QScriptValue func = callback; // copy function because QScriptValue::call is not const
    QScriptValue result = func.call(context, args);
    if (result.isError()) {
        qWarning() << "ControllerQScriptEngine: Invocation of connection " << id.toString()
                   << "connected to (" + key.group + ", " + key.item + ") failed:"
                   << result.toString();
    }
}