#pragma once

#include <QHash>
#include <QSet>
#include "util/mutex.h"

// TODO: make final everything that we can
namespace NewControl {

// Since each control type has a dedicated ControlValueStore, we need to register config keys here to avoid
// two controls of different types to register with the same key.
class ControlConfigKeyRegister {
protected:
    // Return false if a key was already there
    static bool insertKey(ConfigKey key) {
        MMutexLocker locker(&s_mutex);
        return s_keySet.insert(key) != s_keySet.end();
    }
    static bool keyExists(ConfigKey key) {
        MMutexLocker locker(&s_mutex);
        return s_keySet.contains(key);
    }
    // Return true if something was actually removed
    static bool removeKey(ConfigKey key) {
        MMutexLocker locker(&s_mutex);
        return s_keySet.remove(key);
    }

    static QSet<ConfigKey> s_keySet;
    static MMutex s_mutex;
};

QSet<ConfigKey> ControlConfigKeyRegister::s_keySet
GUARDED_BY(ControlConfigKeyRegister::s_mutex);

// TODO: we can probably make the lock non recursive
MMutex ControlConfigKeyRegister::s_mutex(QMutex::Recursive);

template<typename Value, typename Parameter>
class ControlValue;

template<typename Value, typename Parameter>
class ControlValueStore final: private ControlConfigKeyRegister {
public:
    // Return false if the key is already registered
    static bool insert(ConfigKey key, std::shared_ptr<ControlValue<Value, Parameter> > value);
    // Returns nullptr if the key does not have a registered CO
    static std::shared_ptr<ControlValue<Value, Parameter> > get(ConfigKey key);
    static bool remove(ConfigKey key);

    // TODO: make this pricate
    // Test helpers
    static int count() {
        MMutexLocker locker(&s_mutex);
        return s_controlValueHash.count();
    }

private:
    static QHash<ConfigKey, std::weak_ptr<ControlValue<Value, Parameter> >>  s_controlValueHash;
    friend class NewControlTest;
};

} /* namespace NewControl */


#include "control/new/controlvalue.h"

namespace NewControl {

template<typename Value, typename Parameter>
QHash<ConfigKey, std::weak_ptr<ControlValue<Value, Parameter>>> ControlValueStore<Value,Parameter>::s_controlValueHash
GUARDED_BY(ControlConfigKeyRegister::s_mutex);

template<typename Value, typename Parameter>
bool ControlValueStore<Value, Parameter>::insert(ConfigKey key, std::shared_ptr<ControlValue<Value, Parameter> > value) {
    MMutexLocker locker(&s_mutex);
    if (keyExists(key)) {
        qWarning() << "A control with the following key is already created: " << key;
        return false;
    }
    bool inserted = s_controlValueHash.insert(key, value) != s_controlValueHash.end();
    if (inserted) {
        insertKey(key);
    }
    return inserted;
}


template<typename Value, typename Parameter>
std::shared_ptr<ControlValue<Value, Parameter>> ControlValueStore<Value, Parameter>::get(ConfigKey key) {
    MMutexLocker locker(&s_mutex);
    auto it = s_controlValueHash.constFind(key);
    if (it == s_controlValueHash.constEnd()) {
        return std::shared_ptr<ControlValue<Value, Parameter>>(nullptr);
    }
    return std::shared_ptr<ControlValue<Value, Parameter>>(*it);
}
template<typename Value, typename Parameter>
bool ControlValueStore<Value, Parameter>::remove(ConfigKey key) {
    MMutexLocker locker(&s_mutex);
    if (s_controlValueHash.remove(key) > 0) {
        removeKey(key);
        return true;
    }
    return false;
}

} /* namespace NewControl */
