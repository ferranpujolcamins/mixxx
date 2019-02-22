#include "control/new/controldefinition.h"

namespace NewControl {

template<typename Value, typename Parameter>
Value ControlDefinition<Value, Parameter>::getValue() const {
    return getProxy().get();
}

template<typename Value, typename Parameter>
Parameter ControlDefinition<Value, Parameter>::getParameter() const {

}

template<typename Value, typename Parameter>
Parameter ControlDefinition<Value, Parameter>::getParameterForValue(Value value) const {

}

template<typename Value, typename Parameter>
Value ControlDefinition<Value, Parameter>::defaultValue() const {

}

} /* namespace NewControl */
