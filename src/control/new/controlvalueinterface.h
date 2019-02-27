#pragma once

namespace NewControl {

// TODO: Do we really need the interfaces? Do we abstractly use objects implementing them somewhere?

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