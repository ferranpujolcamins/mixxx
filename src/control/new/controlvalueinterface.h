#pragma once

namespace NewControl {

// This interface defines the methods that can be used to read the value of a ControlValue.
// All classes that somehow allow a client to read a ControlValue implement this interface.
template<typename Value, typename Parameter>
class ControlValueReadOnlyInterface {
public:
    virtual ~ControlValueReadOnlyInterface() {};

    virtual Value getValue() const = 0;

    virtual Parameter getParameter() const = 0;
    virtual Parameter getParameterForValue(Value value) const = 0;
    //virtual Parameter getParameterForMidi(Value midiValue) const;

    // Reset to the default value
    virtual Value defaultValue() const = 0;
};

// This interface defines the methods that can be used to read and write the value of a ControlValue.
// All classes that somehow allow a client to read and write a ControlValue implement this interface.
template<typename Value, typename Parameter>
class ControlValueInterface: public ControlValueReadOnlyInterface<Value, Parameter> {
public:
    virtual ~ControlValueInterface() {};

    virtual void setValue(Value value) = 0;

    virtual void setParameter(Parameter parameter) = 0;
    //virtual Parameter getParameterForMidi(Value midiValue) const;

    // Reset to the default value
    virtual void reset() = 0;
    virtual void setDefaultValue(Value value) = 0;
};

} /* namespace NewControl */
