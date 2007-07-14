/***************************************************************************
                          mixxxview.h  -  description
                             -------------------
    begin                : Mon Feb 18 09:48:17 CET 2002
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

#ifndef MIXXXVIEW_H
#define MIXXXVIEW_H

#include <qwidget.h>
#include <qlabel.h>
#include <qstring.h>
#include <qptrlist.h>
#include <qpushbutton.h>

#include "configobject.h"
#include "configobject.h"
#include "imgsource.h"

class ControlObject;
class WSlider;
class WSliderComposed;
class WPushButton;
class WTrackTable;
class WDisplay;
class WKnob;
class WVisual;
class WOverview;
class WNumberPos;
class QDomNode;
class QDomElement;
class MixxxKeyboard;
/*new classes for new visual layout*/
class QComboBox;
class QLineEdit;
class QPushButton;
/**
 * This class provides an incomplete base for your application view.
 */


class MixxxView : public QWidget
{
    Q_OBJECT
public:
    /** Construtor. Tries to open visuals if bVisuals is true. */
    MixxxView(QWidget *parent, ConfigObject<ConfigValueKbd> *kbdconfig, bool bVisualsWaveform, QString qSkinPath, ConfigObject<ConfigValue> *pConfig);
    ~MixxxView();

    /** Check if direct rendering is not available, and display warning */
    void checkDirectRendering();
    /** Return true if WVisualWaveform has been instantiated. */
    bool activeWaveform();

    WTrackTable *m_pTrackTable;

    QLabel *m_pTextCh1, *m_pTextCh2;
    /** Pointer to WVisual widgets */
    QObject *m_pVisualCh1, *m_pVisualCh2;
    /** Pointer to absolute file position widgets */
    WNumberPos *m_pNumberPosCh1, *m_pNumberPosCh2;
    /** Pointer to rate slider widgets */
    WSliderComposed *m_pSliderRateCh1, *m_pSliderRateCh2;
    /** Allow dynamic zoom on visuals */
    bool m_bZoom;
	/** Pointer to ComboBox*/
	QComboBox *m_pComboBox;
	/** Pointer to SearchBox */
	QLineEdit *m_pLineEditSearch;
	/** Pointer to Search PushButton*/
	QPushButton *m_pPushButton;
    /** Pointer to overview displays */
    WOverview *m_pOverviewCh1, *m_pOverviewCh2;
	
	void rebootGUI(QWidget* parent, bool bVisualsWaveform, ConfigObject<ConfigValue> *pConfig, QString qSkinPath);

	static QValueList<QString> getSchemeList(QString qSkinPath);

private:
	void setupColorScheme(QDomElement docElem, ConfigObject<ConfigValue> *pConfig);
	void createAllWidgets(QDomElement docElem, QWidget* parent, bool bVisualsWaveform, ConfigObject<ConfigValue> *pConfig);
	
	/*temp to change view*/
	WTrackTable *m_pTmpPlaylist;
	WTrackTable *m_pTmpPlaylist2;

	ImgSource* parseFilters(QDomNode filt);
	static QDomElement openSkin(QString qSkinPath);
	/**Used for comboBox change*/
	int view;
    // True if m_pVisualChX is instantiated as WVisualWaveform
    bool m_bVisualWaveform;
    bool compareConfigKeys(QDomNode node, QString key);
    QPtrList<QObject> m_qWidgetList;
    /** Pointer to keyboard handler */
    MixxxKeyboard *m_pKeyboard;
};

#endif
