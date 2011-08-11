#include "waveformwidgetfactory.h"
#include "waveform/widget/waveformwidgetabstract.h"
#include "widget/wwaveformviewer.h"

//WaveformWidgets
#include "waveform/widget/emptywaveformwidget.h"
#include "waveform/widget/glwaveformwidget.h"
#include "waveform/widget/softwarewaveformwidget.h"

#include <QTimer>
#include <QWidget>
#include <QTime>
#include <QStringList>
#include <QtOpenGL/QGLFormat>

#include <QDebug>

WaveformWidgetFactory::WaveformWidgetFactory()
{
    m_timer = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(refresh()));

    m_time = new QTime();

    //TODO let default and config file to setup this
    setFrameRate(40);
    m_lastFrameTime = 0;
    m_actualFrameRate = 0;

    //setup the opengl default format
    m_openGLAvailable = false;
    if( QGLFormat::hasOpenGL())
    {
        QGLFormat glFormat;
        glFormat.setDirectRendering(true);
        glFormat.setDoubleBuffer(true);
        glFormat.setDepth(false);
        glFormat.setSwapInterval(1); //enable vertical sync to avoid cue line to be cut
        QGLFormat::setDefaultFormat(glFormat);
        m_openGLVersion = QString::number(glFormat.majorVersion()) + "." + QString::number(glFormat.minorVersion());
        m_openGLAvailable = true;
    }

    if( m_openGLAvailable)
        m_type = WaveformWidgetType::FilteredOpenGlWaveform;
    else
        m_type = WaveformWidgetType::FilteredSoftwareWaveform;

    evaluateWidgets();
    start();
}

WaveformWidgetFactory::~WaveformWidgetFactory()
{
    delete m_timer;
    delete m_time;
}

void WaveformWidgetFactory::start()
{
    qDebug() << "WaveformWidgetFactory::start";
    m_timer->start();
}

void WaveformWidgetFactory::stop()
{
    m_timer->stop();
}

void WaveformWidgetFactory::destroyWidgets()
{
    for( int i = 0; i < m_waveformWidgets.size(); i++)
        delete m_waveformWidgets[i];
    m_waveformWidgets.clear();
}

bool WaveformWidgetFactory::setWaveformWidget( WWaveformViewer* viewer)
{
    int index = -1;
    if( viewer->getWaveformWidget())
    {
        //it already have a WaveformeWidget
        for( int i = 0; i < m_waveformWidgets.size(); i++)
        {
            if( m_waveformWidgets[i] == viewer->getWaveformWidget())
            {
                index = i;
                break;
            }
        }
        if( index == -1)
        {
            qDebug() << "WaveformWidgetFactory::setWaveformWidget - "\
                        "viewer already have a waveform widget but it's not found by the factory !";
        }
        delete viewer->getWaveformWidget();
    }

    //Cast to widget done just after creation because it can't be perform in constructor (pure virtual)
    WaveformWidgetAbstract* waveformWidget = createWaveformWidget(m_type,viewer);
    waveformWidget->castToQWidget();
    viewer->setWaveformWidget( waveformWidget);
    viewer->setup();

    m_waveformWidgets.push_back(waveformWidget);
    index = m_waveformWidgets.size()-1;

    qDebug() << "WaveformWidgetFactory::setWaveformWidget - waveform widget added in factory index" << index;

    return true;
}

void WaveformWidgetFactory::setFrameRate( int frameRate)
{
    m_frameRate = std::min(60,frameRate);
    m_timer->setInterval((int)(1000.0/(double)m_frameRate));
}

bool WaveformWidgetFactory::setWidgetType( int handleIndex)
{
    if( handleIndex < 0 && handleIndex > m_waveformWidgetHandles.size())
    {
        qDebug() << "WaveformWidgetFactory::setWidgetType - invalid handle";
        return false;
    }

    WaveformWidgetAbstractHandle& handle = m_waveformWidgetHandles[handleIndex];
    if( handle.m_type == m_type)
    {
        qDebug() << "WaveformWidgetFactory::setWidgetType - type already in use";
        return false;
    }

    //change the type
    m_type = handle.m_type;

    /*
      vRince
      I can't just recreate waveform widgets (even if its nicer !!)
      Waveform widget creation works but it the complete setup (color etc ...) from the skin
      need to be re-run ! I tried to implement some int the skin loader be it became "spagetti"
      code :( ... So for the moment a mixxx restart will do ...

    //retrieve existing viewers
    QVector<WWaveformViewer*> viewers;
    for( int i = 0; i < m_waveformWidgets.size(); i++)
    {
        if( !m_waveformWidgets[i]->isValid())
        {
            //should never happend the casting must be check into the setWaveformWidget
            //method to ensre we never store mis-formed widget in the factory !!
            continue;
        }

        //it should be safe since only the factory can build WaveformWidget and we know
        //we give them a WWaveformViewer as a parent
        WWaveformViewer* viewer = static_cast<WWaveformViewer*>(m_waveformWidgets[i]->getWidget()->parent());
        viewers.push_back(viewer);
    }

    //re-create them with the current type
    for( int i = 0; i < viewers.size(); i++)
        setWaveformWidget(viewers[i]);
    */

    return true;
}

void WaveformWidgetFactory::refresh()
{
    for( int i = 0; i < m_waveformWidgets.size(); i++)
        m_waveformWidgets[i]->prepare();

    for( int i = 0; i < m_waveformWidgets.size(); i++)
        m_waveformWidgets[i]->refresh();

    m_lastFrameTime = m_time->elapsed();
    m_time->restart();
    m_actualFrameRate = 1000.0/(double)(m_lastFrameTime);
}

void WaveformWidgetFactory::evaluateWidgets()
{
    m_waveformWidgetHandles.clear();
    for( int type = 0; type < WaveformWidgetType::Count_WaveformwidgetType; type++)
    {
        WaveformWidgetAbstract* widget = 0;
        switch(type)
        {
        case WaveformWidgetType::EmptyWaveform : widget = new EmptyWaveformWidget(); break;
        case WaveformWidgetType::SimpleSoftwareWaveform : break; //TODO
        case WaveformWidgetType::SimpleOpenGlWaveform : break; //TODO
        case WaveformWidgetType::FilteredSoftwareWaveform : widget = new SoftwareWaveformWidget(); break;
        case WaveformWidgetType::FilteredOpenGlWaveform : widget = new GLWaveformWidget(); break;
        }

        if( widget)
        {
            QString widgetName = widget->getWaveformWidgetName();
            if( widget->useOpenGl())
                widgetName += " " + WaveformWidgetAbstract::s_openGlFlag;

            //add new handle for each available widget type
            WaveformWidgetAbstractHandle handle;
            handle.m_displayString = widgetName;
            handle.m_type = (WaveformWidgetType::Type)type;

            //NOTE: For the moment non active widget are not added to available handle
            //but it could be useful to have them anyway but not selectable in the combo box
            if( widget->useOpenGl() && !isOpenGLAvailable())
            {
                handle.m_active = false;
                continue;
            }
            m_waveformWidgetHandles.push_back( handle);
        }
        delete widget;
    }
}

WaveformWidgetAbstract* WaveformWidgetFactory::createWaveformWidget( WaveformWidgetType::Type type, WWaveformViewer* viewer)
{
    if( viewer)
    {
        switch(type)
        {
        case WaveformWidgetType::EmptyWaveform : return new EmptyWaveformWidget( viewer->getGroup(), viewer);
        case WaveformWidgetType::SimpleSoftwareWaveform : return 0; //TODO
        case WaveformWidgetType::SimpleOpenGlWaveform : return 0; //TODO
        case WaveformWidgetType::FilteredSoftwareWaveform : return new SoftwareWaveformWidget( viewer->getGroup(), viewer);
        case WaveformWidgetType::FilteredOpenGlWaveform : return new GLWaveformWidget( viewer->getGroup(), viewer);
        default : return 0;
        }
    }
}
