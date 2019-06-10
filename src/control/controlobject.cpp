/***************************************************************************
                          controlobject.cpp  -  description
                             -------------------
    begin                : Wed Feb 20 2002
    copyright            : (C) 2002 by Tue and Ken Haste Andersen
    email                :
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <QtDebug>
#include <QHash>
#include <QSet>
#include <QMutexLocker>

#include "control/controlobject.h"
#include "control/controlpushbutton.h"
#include "control/control.h"
#include "util/stat.h"
#include "util/timer.h"

ControlObject::ControlObject() {
}

ControlObject::ControlObject(ConfigKey key, bool bIgnoreNops, bool bTrack,
                             bool bPersist, double defaultValue)
        : m_key(key), m_bPersist(bPersist) {
    // Don't bother looking up the control if key is NULL. Prevents log spew.
    if (!m_key.isNull()) {
        m_pControl = ControlDoublePrivate::getControl(m_key, true, this,
                bIgnoreNops, bTrack, bPersist, defaultValue);
    }

    // getControl can fail and return a NULL control even with the create flag.
    if (m_pControl) {
        connect(m_pControl.data(), SIGNAL(valueChanged(double, QObject*)),
                this, SLOT(privateValueChanged(double, QObject*)),
                Qt::DirectConnection);
    }
}

ControlObject::~ControlObject() {
    if (m_pControl) {
        m_pControl->removeCreatorCO();
    }
}

void ControlObject::createEnabledCO() {
    m_pCOEnabled = std::make_unique<ControlPushButton>(
            ConfigKey(m_key.group, QString(m_key.item) + "_enabled"),
            m_bPersist,
            true
    );
    m_pCOEnabled->setButtonMode(ControlPushButton::ButtonMode::TOGGLE);
}

// slot
void ControlObject::privateValueChanged(double dValue, QObject* pSender) {
    // Only emit valueChanged() if we did not originate this change.
    if (pSender != this) {
        emit(valueChanged(dValue));
    }
}

// static
ControlObject* ControlObject::getControl(const ConfigKey& key, bool warn) {
    //qDebug() << "ControlObject::getControl for (" << key.group << "," << key.item << ")";
    QSharedPointer<ControlDoublePrivate> pCDP = ControlDoublePrivate::getControl(key, warn);
    if (pCDP) {
        return pCDP->getCreatorCO();
    }
    return NULL;
}

void ControlObject::setValueFromMidi(MidiOpCode o, double v) {
    if (m_pControl) {
        m_pControl->setValueFromMidi(o, v);
    }
}

double ControlObject::getMidiParameter() const {
    return m_pControl ? m_pControl->getMidiParameter() : 0.0;
}

// static
double ControlObject::get(const ConfigKey& key) {
    QSharedPointer<ControlDoublePrivate> pCop = ControlDoublePrivate::getControl(key);
    return pCop ? pCop->get() : 0.0;
}

double ControlObject::getParameter() const {
    return m_pControl ? m_pControl->getParameter() : 0.0;
}

double ControlObject::getParameterForValue(double value) const {
    return m_pControl ? m_pControl->getParameterForValue(value) : 0.0;
}

double ControlObject::getParameterForMidi(double midiParameter) const {
    return m_pControl ? m_pControl->getParameterForMidi(midiParameter) : 0.0;
}

void ControlObject::setParameter(double v) {
    if (m_pControl) {
        m_pControl->setParameter(v, this);
    }
}

void ControlObject::setParameterFrom(double v, QObject* pSender) {
    if (m_pControl) {
        m_pControl->setParameter(v, pSender);
    }
}

bool ControlObject::isEnabled() {
    if (m_pCOEnabled) {
        return m_pCOEnabled->toBool();
    }
    return true;
}

// If the CO does not have the associated enabled CO this is a NOOP.
void ControlObject::setIsEnabled(bool isEnabled) {
    if (m_pCOEnabled) {
        return m_pCOEnabled->set(isEnabled);
    }
}

// static
void ControlObject::set(const ConfigKey& key, const double& value) {
    QSharedPointer<ControlDoublePrivate> pCop = ControlDoublePrivate::getControl(key);
    if (pCop) {
        pCop->set(value, NULL);
    }
}

void ControlObject::setReadOnly() {
    connectValueChangeRequest(this, &ControlObject::readOnlyHandler,
                              Qt::DirectConnection);
}

void ControlObject::readOnlyHandler(double v) {
    qWarning() << m_key << "is read-only. Ignoring set of value:" << v;
}
