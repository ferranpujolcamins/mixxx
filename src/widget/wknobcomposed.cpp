#include <QStylePainter>
#include <QStyleOption>
#include <QTransform>

#include "widget/wknobcomposed.h"
#include "widget/controlwidgetconnection.h"

WKnobComposed::WKnobComposed(QWidget* pParent)
        : WWidget(pParent),
          m_dCurrentAngle(140.0),
          m_dScaleStartParameter(0.0),
          m_dMinAngle(-230.0),
          m_dMaxAngle(50.0),
          m_dKnobCenterXOffset(0),
          m_dKnobCenterYOffset(0),
          m_dMaskXOffset(0),
          m_dMaskYOffset(0) {
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
        setPixmapKnob(
                context.getPixmapSource(ringNode),
                context.selectScaleMode(ringNode, Paintable::STRETCH),
                scaleFactor);
    }

    context.hasNodeSelectDouble(node, "MinAngle", &m_dMinAngle);
    context.hasNodeSelectDouble(node, "MaxAngle", &m_dMaxAngle);
    context.hasNodeSelectDouble(node, "KnobCenterXOffset", &m_dKnobCenterXOffset);
    context.hasNodeSelectDouble(node, "KnobCenterYOffset", &m_dKnobCenterYOffset);
    context.hasNodeSelectDouble(node, "RingMaskXOffset", &m_dMaskXOffset);
    context.hasNodeSelectDouble(node, "RingMaskYOffset", &m_dMaskYOffset);

    m_dKnobCenterXOffset *= scaleFactor;
    m_dKnobCenterYOffset *= scaleFactor;
    m_dMaskXOffset *= scaleFactor;
    m_dMaskYOffset *= scaleFactor;
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

void WKnobComposed::addConnection(ControlParameterWidgetConnection* pConnection) {
    WBaseWidget::addConnection(pConnection);
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

    if (m_pPixmapBack) {
        m_pPixmapBack->draw(rect(), &p, m_pPixmapBack->rect());
    }

    ControlParameterWidgetConnection* defaultConnection = m_connections.at(0);
    if (defaultConnection) {
        m_dScaleStartParameter = defaultConnection->scaleStartParameter();
    }

    // We update m_dCurrentAngle since onConnectedControlChanged uses it for
    // no-op detection.
    m_dCurrentAngle = m_dMinAngle + (m_dMaxAngle - m_dMinAngle) * getControlParameterDisplay();

    // Qt measures angles in degrees from 3 o'clock counterclockwise.
    // In Mixxx we measure angles also in degrees but from 12 o'clock clockwise.
    // So: QtAngle = 90 - MixxxAngle
    if (m_pRing) {
        QPainterPath path;
        int w = width();
        int h = height();
        path.moveTo(w/2.0 + m_dMaskXOffset, h/2.0 + m_dMaskYOffset);
        double d = sqrt(pow(w+abs(m_dMaskXOffset),2) + pow(h+abs(m_dMaskYOffset),2));
        double dScaleStartAngle = m_dMinAngle + (m_dMaxAngle - m_dMinAngle) * m_dScaleStartParameter;
        path.arcTo(QRectF((w-d)/2.0,(h-d)/2.0,d,d), 90 - dScaleStartAngle, dScaleStartAngle - m_dCurrentAngle);
        path.closeSubpath();
        p.save();
        p.setClipPath(path);
        m_pRing->draw(rect(), &p, m_pRing->rect());
        p.restore();
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
