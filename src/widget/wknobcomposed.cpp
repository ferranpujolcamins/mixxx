#include <algorithm> // std::swap

#include <QStylePainter>
#include <QStyleOption>
#include <QTransform>

#include "widget/wknobcomposed.h"
#include "widget/controlwidgetconnection.h"

WKnobComposed::WKnobComposed(QWidget* pParent)
        : WWidget(pParent),
          m_dScaleStartParameter(0.0),
          m_dMinAngle(-135),
          m_dMaxAngle(135),
          m_dKnobCenterXOffset(0),
          m_dKnobCenterYOffset(0),
          m_dRingMaskXOffset(0),
          m_dRingMaskYOffset(0),
          m_dRingMinSpan(0),
          m_dRingSpanOffset(0),
          m_bMaskBackground(false) {
    m_dCurrentAngle = m_dMinAngle + (m_dMaxAngle - m_dMinAngle) * 0.5;
}

void WKnobComposed::setup(const QDomNode& node, const SkinContext& context) {
    clear();

    double scaleFactor = context.getScaleFactor();

    // Set background pixmap if available
    QDomElement backPathElement = context.selectElement(node, "BackPath");
    if (!backPathElement.isNull()) {
        setPixmapBackground(
                context.getPixmapSource(backPathElement),
                context.selectScaleMode(backPathElement, Paintable::STRETCH),
                scaleFactor);
    }

    // Set knob pixmap if available
    QDomElement knobNode = context.selectElement(node, "Knob");
    if (!knobNode.isNull()) {
        setPixmapKnob(
                context.getPixmapSource(knobNode),
                context.selectScaleMode(knobNode, Paintable::STRETCH),
                scaleFactor);
    }

    // Set ring pixmap if available
    QDomElement ringNode = context.selectElement(node, "Ring");
    if (!ringNode.isNull()) {
        setPixmapRing(
                context.getPixmapSource(ringNode),
                context.selectScaleMode(ringNode, Paintable::STRETCH),
                scaleFactor);
    }

    context.hasNodeSelectDouble(node, "MinAngle", &m_dMinAngle);
    context.hasNodeSelectDouble(node, "MaxAngle", &m_dMaxAngle);
    context.hasNodeSelectDouble(node, "KnobCenterXOffset", &m_dKnobCenterXOffset);
    context.hasNodeSelectDouble(node, "KnobCenterYOffset", &m_dKnobCenterYOffset);
    context.hasNodeSelectDouble(node, "RingMaskXOffset", &m_dRingMaskXOffset);
    context.hasNodeSelectDouble(node, "RingMaskYOffset", &m_dRingMaskYOffset);
    context.hasNodeSelectDouble(node, "RingMinSpan", &m_dRingMinSpan);
    context.hasNodeSelectDouble(node, "RingSpanOffset", &m_dRingSpanOffset);
    context.hasNodeSelectBool(node, "MaskBackground", &m_bMaskBackground);
    
    m_dMinAngle = math_clamp<double>(m_dMinAngle, -180, 180);
    m_dMaxAngle = math_clamp<double>(m_dMaxAngle, -180, 180);
    if (m_dMinAngle > m_dMaxAngle) {
        std::swap(m_dMinAngle, m_dMaxAngle);
    }
    m_dRingMinSpan = math_clamp<double>(m_dRingMinSpan, 0, 180);
    m_dRingSpanOffset = math_clamp<double>(m_dRingMinSpan, 0, 180);

    m_dKnobCenterXOffset *= scaleFactor;
    m_dKnobCenterYOffset *= scaleFactor;
    m_dRingMaskXOffset *= scaleFactor;
    m_dRingMaskYOffset *= scaleFactor;
}

void WKnobComposed::clear() {
    m_pPixmapBack.clear();
    m_pKnob.clear();
}

void WKnobComposed::setPixmapBackground(PixmapSource source,
                                        Paintable::DrawMode mode,
                                        double scaleFactor) {
    m_pPixmapBack = WPixmapStore::getPaintable(source, mode, scaleFactor);
    if (m_pPixmapBack.isNull() || m_pPixmapBack->isNull()) {
        qDebug() << metaObject()->className()
                 << "Error loading background pixmap:" << source.getPath();
    }
}

void WKnobComposed::setPixmapKnob(PixmapSource source,
                                  Paintable::DrawMode mode,
                                  double scaleFactor) {
    m_pKnob = WPixmapStore::getPaintable(source, mode, scaleFactor);
    if (m_pKnob.isNull() || m_pKnob->isNull()) {
        qDebug() << metaObject()->className()
                 << "Error loading knob pixmap:" << source.getPath();
    }
}

void WKnobComposed::setPixmapRing(PixmapSource source,
                                  Paintable::DrawMode mode,
                                  double scaleFactor) {
    m_pRing = WPixmapStore::getPaintable(source, mode, scaleFactor);
    if (m_pRing.isNull() || m_pRing->isNull()) {
        qDebug() << metaObject()->className()
                 << "Error loading ring pixmap:" << source.getPath();
    }
}

void WKnobComposed::onConnectedControlChanged(double dParameter, double dValue) {
    Q_UNUSED(dValue);
    // dParameter is in the range [0, 1].
    double angle = m_dMinAngle + (m_dMaxAngle - m_dMinAngle) * dParameter;

    // TODO(rryan): What's a good epsilon? Should it be dependent on the min/max
    // angle range? Right now it's just 1/100th of a degree.
    if (fabs(angle - m_dCurrentAngle) > 0.01) {
        // paintEvent updates m_dCurrentAngle
        update();
    }
}

void WKnobComposed::paintEvent(QPaintEvent* e) {
    Q_UNUSED(e);
    QStyleOption option;
    option.initFrom(this);
    QStylePainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.drawPrimitive(QStyle::PE_Widget, option);

    bool bScaleStartParameterIsDefined = false;
    ControlParameterWidgetConnection* defaultConnection;
    if (!m_connections.isEmpty()) {
        defaultConnection = m_connections.at(0);
        if (defaultConnection) {
            bScaleStartParameterIsDefined = true;
            m_dScaleStartParameter = defaultConnection->scaleStartParameter();
        }
    }

    // We update m_dCurrentAngle since onConnectedControlChanged uses it for
    // no-op detection.
    m_dCurrentAngle = m_dMinAngle + (m_dMaxAngle - m_dMinAngle) * getControlParameterDisplay();

    if (m_pRing && bScaleStartParameterIsDefined) {
        // The ring is masked so it's only painted between the control's scale start parameter and the knob's current value.
        
        QPainterPath ringMaskPath;
        int w = width();
        int h = height();
        
        // Move ring painter to ring center position
        double dRingCenterX = w/2.0 + m_dRingMaskXOffset;
        double dRingCenterY = h/2.0 + m_dRingMaskYOffset;
        ringMaskPath.moveTo(dRingCenterX, dRingCenterY);
        
        // The ring mask is a circular sector. We set its diameter for the mask to cover all the widget.
        double dRingMaskDiameter = sqrt(pow(w,2) + pow(h,2));
        
        // Set the rectangle where the ring mask will be painted
        QPointF ringMaskRectTopLeftCorner = QPointF(dRingCenterX-dRingMaskDiameter/2.0, dRingCenterY-dRingMaskDiameter/2.0);
        QSizeF ringMaskRectSize = QSizeF(dRingMaskDiameter, dRingMaskDiameter);
        QRectF ringMaskBoundingRectangle = QRectF(ringMaskRectTopLeftCorner, ringMaskRectSize);
        
        // Qt measures angles in degrees from 3 o'clock counterclockwise.
        // In Mixxx we measure angles also in degrees but from 12 o'clock clockwise.
        // So: QtAngle = 90 - MixxxAngle
        // We do all calculations in Mixxx angle units. We convert to Qt Angle units later.
        double dScaleStartAngle = m_dMinAngle + (m_dMaxAngle - m_dMinAngle) * m_dScaleStartParameter;
        
        // The angle where the mask circular sector starts
        double dRingMaskBaseAngle;
        // How wide the ring mask must be
        double dRingMaskSpan;
        if (m_dCurrentAngle < dScaleStartAngle) {
            dRingMaskBaseAngle = math_max(dScaleStartAngle + m_dRingSpanOffset, dScaleStartAngle + m_dRingMinSpan);
            dRingMaskSpan = math_min(m_dCurrentAngle - dRingMaskBaseAngle - m_dRingSpanOffset, -2*m_dRingMinSpan);
        } else {
            dRingMaskBaseAngle = math_min(dScaleStartAngle - m_dRingSpanOffset, dScaleStartAngle - m_dRingMinSpan);
            dRingMaskSpan = math_max(m_dCurrentAngle - dRingMaskBaseAngle + m_dRingSpanOffset, 2*m_dRingMinSpan);
        }
        
        // Here we convert to Qt angles and draw the mask
        ringMaskPath.arcTo(ringMaskBoundingRectangle, 90 - dRingMaskBaseAngle, -dRingMaskSpan);
        ringMaskPath.closeSubpath();
        p.save();
        
        if (m_bMaskBackground) {
            // Do not paint the background pixmap where the ring will be painted.
            // It greatly improves the render quality if the ring and the background
            // have the same shape and position (for example in Shade).
            QPainterPath canvasPath;
            canvasPath.addRect(0,0,w,h);
            // The fill area of backMaskPath is the complementary of canvasPath.
            QPainterPath backMaskPath = canvasPath.subtracted(ringMaskPath);
            p.setClipPath(backMaskPath);
        }
        if (m_pPixmapBack) {
            m_pPixmapBack->draw(rect(), &p, m_pPixmapBack->rect());
        }

        p.setClipPath(ringMaskPath);
        m_pRing->draw(rect(), &p, m_pRing->rect());
        p.restore();
    } else if (m_pPixmapBack) {
        // If we don't need to paint the ring, we paint the background without further transformations.
        m_pPixmapBack->draw(rect(), &p, m_pPixmapBack->rect());
    }

    QTransform transform;
    if (!m_pKnob.isNull() && !m_pKnob->isNull()) {
        qreal tx = m_dKnobCenterXOffset + width() / 2.0;
        qreal ty = m_dKnobCenterYOffset + height() / 2.0;
        transform.translate(-tx, -ty);
        p.translate(tx, ty);
        p.rotate(m_dCurrentAngle);

        // Need to convert from QRect to a QRectF to avoid losing precison.
        QRectF targetRect = rect();
        m_pKnob->drawCentered(transform.mapRect(targetRect), &p,
                              m_pKnob->rect());
    }
}

void WKnobComposed::mouseMoveEvent(QMouseEvent* e) {
    m_handler.mouseMoveEvent(this, e);
}

void WKnobComposed::mousePressEvent(QMouseEvent* e) {
    m_handler.mousePressEvent(this, e);
}

void WKnobComposed::mouseReleaseEvent(QMouseEvent* e) {
    m_handler.mouseReleaseEvent(this, e);
}

void WKnobComposed::wheelEvent(QWheelEvent* e) {
    m_handler.wheelEvent(this, e);
}
