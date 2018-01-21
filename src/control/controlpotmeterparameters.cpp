#include "controlpotmeterparameters.h"

EffectKnobParameters::EffectKnobParameters()
        : m_dMinValue(0.0), m_dMaxValue(1.0) {
}

EffectKnobParameters::~EffectKnobParameters() {
}

double EffectKnobParameters::minValue() {
    return m_dMinValue;
}

double EffectKnobParameters::maxValue() {
    return m_dMaxValue;
}

void EffectKnobParameters::setMinValue(double value) {
    m_dMinValue = value;
}

void EffectKnobParameters::setMaxValue(double value) {
    m_dMaxValue = value;
}

ControlLinPotmeterParameters::ControlLinPotmeterParameters()
        : m_dMinValue(0.0), m_dMaxValue(1.0), m_dScaleStartParameter(0.0),
          m_bAllowOutOfBounds(false), m_dStep(0.0),
          m_dSmallStep(0.0) {
}

ControlLinPotmeterParameters::~ControlLinPotmeterParameters() {
}

double ControlLinPotmeterParameters::minValue() {
    return m_dMinValue;
}

double ControlLinPotmeterParameters::maxValue() {
    return m_dMaxValue;
}

double ControlLinPotmeterParameters::scaleStartParameter() {
    return m_dScaleStartParameter;
}

bool ControlLinPotmeterParameters::allowOutOfBounds() {
    return m_bAllowOutOfBounds;
}

double ControlLinPotmeterParameters::step() {
    return m_dStep;
}

double ControlLinPotmeterParameters::smallStep() {
    return m_dSmallStep;
}

void ControlLinPotmeterParameters::setMinValue(double value) {
    m_dMinValue = value;
}

void ControlLinPotmeterParameters::setMaxValue(double value) {
    m_dMaxValue = value;
}

void ControlLinPotmeterParameters::setScaleStartParameter(double value) {
    m_dScaleStartParameter = value;
}

void ControlLinPotmeterParameters::setAllowOutOfBounds(bool value) {
    m_bAllowOutOfBounds = value;
}

void ControlLinPotmeterParameters::setStep(double value) {
    m_dStep = value;
}

void ControlLinPotmeterParameters::setSmallStep(double value) {
    m_dSmallStep = value;
}

ControlLogPotmeterParameters::ControlLogPotmeterParameters()
        : m_dMaxValue(1.0), m_dScaleStartParameter(0.0),
          m_dMinDB(60.) {
}

ControlLogPotmeterParameters::~ControlLogPotmeterParameters() {
}

double ControlLogPotmeterParameters::maxValue() {
    return m_dMaxValue;
}

double ControlLogPotmeterParameters::scaleStartParameter() {
    return m_dScaleStartParameter;
}

double ControlLogPotmeterParameters::minDB() {
    return m_dMinDB;
}

void ControlLogPotmeterParameters::setMaxValue(double value) {
    m_dMaxValue = value;
}

void ControlLogPotmeterParameters::setScaleStartParameter(double value) {
    m_dScaleStartParameter = value;
}

void ControlLogPotmeterParameters::setMinDB(double value) {
    m_dMinDB = value;
}


ControlPotmeterParameters::ControlPotmeterParameters()
        : m_dMinValue(0.0), m_dMaxValue(1.0), m_dScaleStartParameter(0.0),
          m_bAllowOutOfBounds(false), m_bIgnoreNops(true),
          m_bTrack(false), m_bPersist(false) {
}

ControlPotmeterParameters::ControlPotmeterParameters(EffectKnobParameters& parameters)
        : m_dMinValue(parameters.minValue()),
          m_dMaxValue(parameters.maxValue()),
          m_dScaleStartParameter(0.0),
          m_bAllowOutOfBounds(false), m_bIgnoreNops(true),
          m_bTrack(false), m_bPersist(false) {
}

ControlPotmeterParameters::ControlPotmeterParameters(ControlLinPotmeterParameters& parameters)
        : m_dMinValue(parameters.minValue()),
          m_dMaxValue(parameters.maxValue()),
          m_dScaleStartParameter(parameters.scaleStartParameter()),
          m_bAllowOutOfBounds(parameters.allowOutOfBounds()),
          m_bIgnoreNops(true), m_bTrack(false), m_bPersist(false) {
}

ControlPotmeterParameters::ControlPotmeterParameters(ControlLogPotmeterParameters& parameters)
        : m_dMinValue(0.0), m_dMaxValue(parameters.maxValue()),
          m_dScaleStartParameter(parameters.scaleStartParameter()),
          m_bAllowOutOfBounds(false), m_bIgnoreNops(true),
          m_bTrack(false), m_bPersist(false) {
}

ControlPotmeterParameters::~ControlPotmeterParameters() {
}

double ControlPotmeterParameters::minValue() {
    return m_dMinValue;
}

double ControlPotmeterParameters::maxValue() {
    return m_dMaxValue;
}

double ControlPotmeterParameters::scaleStartParameter() {
    return m_dScaleStartParameter;
}

bool ControlPotmeterParameters::allowOutOfBounds() {
    return m_bAllowOutOfBounds;
}

bool ControlPotmeterParameters::ignoreNops() {
    return m_bIgnoreNops;
}

bool ControlPotmeterParameters::track() {
    return m_bTrack;
}

bool ControlPotmeterParameters::persist() {
    return m_bPersist;
}

void ControlPotmeterParameters::setMinValue(double value) {
    m_dMinValue = value;
}

void ControlPotmeterParameters::setMaxValue(double value) {
    m_dMaxValue = value;
}

void ControlPotmeterParameters::setScaleStartParameter(double value) {
    m_dScaleStartParameter = value;
}

void ControlPotmeterParameters::setAllowOutOfBounds(bool value) {
    m_bAllowOutOfBounds = value;
}

void ControlPotmeterParameters::setIgnoreNops(bool value) {
    m_bIgnoreNops = value;
}

void ControlPotmeterParameters::setTrack(bool value) {
    m_bTrack = value;
}

void ControlPotmeterParameters::setPersist(bool value) {
    m_bPersist = value;
}
