#include "waveformrenderersignalbase.h"

#include <QDomNode>

#include "control/controlobject.h"
#include "control/controlproxy.h"
#include "util/colorcomponents.h"
#include "waveform/waveformwidgetfactory.h"
#include "waveformwidgetrenderer.h"
#include "widget/wskincolor.h"
#include "widget/wwidget.h"

WaveformRendererSignalBase::WaveformRendererSignalBase(
        WaveformWidgetRenderer* waveformWidgetRenderer)
        : WaveformRendererAbstract(waveformWidgetRenderer),
          m_pEQEnabled(nullptr),
          m_pLowFilterControlObject(nullptr),
          m_pMidFilterControlObject(nullptr),
          m_pHighFilterControlObject(nullptr),
          m_pLowButtonControlObject(nullptr),
          m_pMidButtonControlObject(nullptr),
          m_pHighButtonControlObject(nullptr),
          m_pEQButtonMode(nullptr),
          m_alignment(Qt::AlignCenter),
          m_orientation(Qt::Horizontal),
          m_pColors(nullptr),
          m_axesColor_r(0),
          m_axesColor_g(0),
          m_axesColor_b(0),
          m_axesColor_a(0),
          m_signalColor_r(0),
          m_signalColor_g(0),
          m_signalColor_b(0),
          m_lowColor_r(0),
          m_lowColor_g(0),
          m_lowColor_b(0),
          m_midColor_r(0),
          m_midColor_g(0),
          m_midColor_b(0),
          m_highColor_r(0),
          m_highColor_g(0),
          m_highColor_b(0),
          m_rgbLowColor_r(0),
          m_rgbLowColor_g(0),
          m_rgbLowColor_b(0),
          m_rgbMidColor_r(0),
          m_rgbMidColor_g(0),
          m_rgbMidColor_b(0),
          m_rgbHighColor_r(0),
          m_rgbHighColor_g(0),
          m_rgbHighColor_b(0) {
}

WaveformRendererSignalBase::~WaveformRendererSignalBase() {
    deleteControls();
}

void WaveformRendererSignalBase::deleteControls() {
    delete m_pEQEnabled;
    delete m_pLowFilterControlObject;
    delete m_pMidFilterControlObject;
    delete m_pHighFilterControlObject;
    delete m_pLowButtonControlObject;
    delete m_pMidButtonControlObject;
    delete m_pHighButtonControlObject;
    delete m_pEQButtonMode;
}

bool WaveformRendererSignalBase::init() {
    deleteControls();

    //create controls
    const QString eqGroup = QString("[EqualizerRack1_%1_Effect1]")
                                    .arg(m_waveformRenderer->getGroup());
    m_pEQEnabled = new ControlProxy(
            m_waveformRenderer->getGroup(), "filterWaveformEnable");
    m_pLowFilterControlObject = new ControlProxy(
            eqGroup, "parameter1");
    m_pMidFilterControlObject = new ControlProxy(
            eqGroup, "parameter2");
    m_pHighFilterControlObject = new ControlProxy(
            eqGroup, "parameter3");
    m_pLowButtonControlObject = new ControlProxy(
            eqGroup, "button_parameter1");
    m_pMidButtonControlObject = new ControlProxy(
            eqGroup, "button_parameter2");
    m_pHighButtonControlObject = new ControlProxy(
            eqGroup, "button_parameter3");
    m_pEQButtonMode = new ControlProxy(
            "[Mixer Profile]", "EQButtonMode");

    return onInit();
}

void WaveformRendererSignalBase::setup(const QDomNode& node,
                                       const SkinContext& context) {
    QString orientationString = context.selectString(node, "Orientation").toLower();
    if (orientationString == "vertical") {
        m_orientation = Qt::Vertical;
    } else {
        m_orientation = Qt::Horizontal;
    }

    QString alignString = context.selectString(node, "Align").toLower();
    if (m_orientation == Qt::Horizontal) {
        if (alignString == "bottom") {
            m_alignment = Qt::AlignBottom;
        } else if (alignString == "top") {
            m_alignment = Qt::AlignTop;
        } else {
            m_alignment = Qt::AlignCenter;
        }
    } else {
        if (alignString == "left") {
            m_alignment = Qt::AlignLeft;
        } else if (alignString == "right") {
            m_alignment = Qt::AlignRight;
        } else {
            m_alignment = Qt::AlignCenter;
        }
    }

    m_pColors = m_waveformRenderer->getWaveformSignalColors();

    const QColor& l = m_pColors->getLowColor();
    getRgbF(l, &m_lowColor_r, &m_lowColor_g, &m_lowColor_b);

    const QColor& m = m_pColors->getMidColor();
    getRgbF(m, &m_midColor_r, &m_midColor_g, &m_midColor_b);

    const QColor& h = m_pColors->getHighColor();
    getRgbF(h, &m_highColor_r, &m_highColor_g, &m_highColor_b);

    const QColor& rgbLow = m_pColors->getRgbLowColor();
    getRgbF(rgbLow, &m_rgbLowColor_r, &m_rgbLowColor_g, &m_rgbLowColor_b);

    const QColor& rgbMid = m_pColors->getRgbMidColor();
    getRgbF(rgbMid, &m_rgbMidColor_r, &m_rgbMidColor_g, &m_rgbMidColor_b);

    const QColor& rgbHigh = m_pColors->getRgbHighColor();
    getRgbF(rgbHigh, &m_rgbHighColor_r, &m_rgbHighColor_g, &m_rgbHighColor_b);

    const QColor& rgbFilteredLow = m_pColors->getRgbLowFilteredColor();
    getRgbF(rgbFilteredLow,
            &m_rgbLowFilteredColor_r,
            &m_rgbLowFilteredColor_g,
            &m_rgbLowFilteredColor_b);

    const QColor& rgbFilteredMid = m_pColors->getRgbMidFilteredColor();
    getRgbF(rgbFilteredMid,
            &m_rgbMidFilteredColor_r,
            &m_rgbMidFilteredColor_g,
            &m_rgbMidFilteredColor_b);

    const QColor& rgbFilteredHigh = m_pColors->getRgbHighFilteredColor();
    getRgbF(rgbFilteredHigh,
            &m_rgbHighFilteredColor_r,
            &m_rgbHighFilteredColor_g,
            &m_rgbHighFilteredColor_b);

    const QColor& axes = m_pColors->getAxesColor();
    getRgbF(axes, &m_axesColor_r, &m_axesColor_g, &m_axesColor_b, &m_axesColor_a);

    const QColor& signal = m_pColors->getSignalColor();
    getRgbF(signal, &m_signalColor_r, &m_signalColor_g, &m_signalColor_b);

    onSetup(node);
}

void WaveformRendererSignalBase::getGains(float* pAllGain, float* pLowGain,
                                          float* pMidGain, float* pHighGain) {
    WaveformWidgetFactory* factory = WaveformWidgetFactory::instance();
    if (pAllGain) {
        *pAllGain = static_cast<CSAMPLE_GAIN>(m_waveformRenderer->getGain()) *
                static_cast<CSAMPLE_GAIN>(factory->getVisualGain(WaveformWidgetFactory::All));
        ;
    }

    if (pLowGain || pMidGain || pHighGain) {
        // Per-band gain from the EQ knobs.
        CSAMPLE_GAIN lowGain = 1.0, midGain = 1.0, highGain = 1.0;

        // Only adjust low/mid/high gains if EQs are enabled.
        if (m_pEQEnabled->get() > 0.0) {
            if (m_pLowFilterControlObject &&
                m_pMidFilterControlObject &&
                m_pHighFilterControlObject) {
                lowGain = static_cast<CSAMPLE_GAIN>(m_pLowFilterControlObject->get());
                midGain = static_cast<CSAMPLE_GAIN>(m_pMidFilterControlObject->get());
                highGain = static_cast<CSAMPLE_GAIN>(m_pHighFilterControlObject->get());
            }

            lowGain *= static_cast<CSAMPLE_GAIN>(
                    factory->getVisualGain(WaveformWidgetFactory::Low));
            midGain *= static_cast<CSAMPLE_GAIN>(
                    factory->getVisualGain(WaveformWidgetFactory::Mid));
            highGain *= static_cast<CSAMPLE_GAIN>(
                    factory->getVisualGain(WaveformWidgetFactory::High));

            if (m_pLowButtonControlObject && m_pLowButtonControlObject->get() > 0.0) {
                lowGain = static_cast<CSAMPLE_GAIN>(m_pEQButtonMode->get());
            }

            if (m_pMidButtonControlObject && m_pMidButtonControlObject->get() > 0.0) {
                midGain = static_cast<CSAMPLE_GAIN>(m_pEQButtonMode->get());
            }

            if (m_pHighButtonControlObject && m_pHighButtonControlObject->get() > 0.0) {
                highGain = static_cast<CSAMPLE_GAIN>(m_pEQButtonMode->get());
            }
        }

        if (pLowGain != nullptr) {
            *pLowGain = lowGain;
        }
        if (pMidGain != nullptr) {
            *pMidGain = midGain;
        }
        if (pHighGain != nullptr) {
            *pHighGain = highGain;
        }
    }
}
