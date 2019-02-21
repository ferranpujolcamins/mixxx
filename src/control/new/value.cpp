#include "control/new/value.h"

namespace {

double Value::getMidiParameter() const {

    return m_behavior.valueToMidiParameter(get());
}

} // namespace
