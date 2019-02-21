#pragma mark

namespace NewControl {

template<typename Value, typename Parameter>
class ControlValueInterface {
public:
    ControlValueInterface();
    virtual ~ControlValueInterface();

    virtual void set(Value value) = 0;
    virtual Value get() const = 0;

    virtual void setParameter(Parameter parameter) = 0;
    virtual Parameter getParameter() const = 0;
    virtual Parameter getParameterForValue(Value value) const = 0;
    //virtual Parameter getParameterForMidi(Value midiValue) const;

    // Reset to the default value
    virtual void reset() = 0;
    virtual void setDefaultValue(Value value) = 0;
    virtual Value defaultValue() const = 0;
};

} /* namespace NewControl */
