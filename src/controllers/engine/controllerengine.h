#ifndef MIXXX_CONTROLLERENGINE_H
#define MIXXX_CONTROLLERENGINE_H

#include <QObject>
#include <QtScript/QScriptValue>

#include "controllers/controllerpreset.h"
#include "util/duration.h"

class ScriptConnection;

class ControllerEngine: public QObject {
    Q_OBJECT

  public:
    virtual ~ControllerEngine() {};

    // TODO: Can we assume script can be represented by QString? Probably yes by now. Also, we ssould not assume js here.
    // Wrap a snippet of JS code in an anonymous function
    virtual QScriptValue wrapFunctionCode(const QString& codeSnippet, int numberOfArgs) = 0;

    // Look up registered script function prefixes
    virtual const QList<QString>& getScriptFunctionPrefixes() = 0;

    virtual void triggerScriptConnection(const ScriptConnection connection) = 0;
    virtual void removeScriptConnection(const ScriptConnection connection) = 0;

    // TODO: check if these are actually connected to a signal
  public slots:
    // Execute a basic MIDI message callback.
    virtual bool execute(QScriptValue function,
                 unsigned char channel,
                 unsigned char control,
                 unsigned char value,
                 unsigned char status,
                 const QString& group,
                 mixxx::Duration timestamp) = 0;
    // Execute a byte array callback.
    virtual bool execute(QScriptValue function, const QByteArray data,
                 mixxx::Duration timestamp) = 0;

    // Evaluates all provided script files and returns true if no script errors
    // occurred while evaluating them.
    virtual bool loadScriptFiles(const QList<QString>& scriptPaths,
                                 const QList<ControllerPreset::ScriptFileInfo>& scripts) = 0;
    virtual void initializeScripts(const QList<ControllerPreset::ScriptFileInfo>& scripts) = 0;
    virtual void gracefulShutdown() = 0;
};

#endif //MIXXX_CONTROLLERENGINE_H
