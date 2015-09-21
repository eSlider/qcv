/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */

/*@@@**************************************************************************
* \file  mainWindow.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* CONSTANT */ 

/* INCLUDES */
#include <QSettings>
#include <QFileInfo>

#include "mainWindow.h"

#include "events.h"
#include "seqControlDlg.h"

#include "operator.h"

#include "seqController.h"
#include "seqDeviceControl.h"
#include "displayWidget.h"
#include "paramEditorDlg.h"
#include "clockTreeDlg.h"
#include "io.h"

#if defined HAVE_QGLVIEWER
#include "glViewer.h"
#endif

CMainWindow::CMainWindow ( CSeqDeviceControl * f_device_p,
                           COperator *         f_rootOp_p,
                           int                 f_sx_i, 
                           int                 f_sy_i )
    : CSimpleWindow (               ),
      m_device_p (       f_device_p ),
      m_rootOp_p (       f_rootOp_p ),
      m_controller_p (          NULL ),
      m_display_p (            NULL ),
      m_paramEditorDlg_p (     NULL ),
      m_clockTreeDlg_p (       NULL ),
      m_autoPlay_b (          false )
{
    QStringList list = QCoreApplication::arguments ();

    for (int i = 1; i < list.size(); ++i)
    {
        if ( list.at(i) == QString("--autoplay") )
            m_autoPlay_b = true;
    }
    
    setWindowTitle( tr("QCV Main Window") );
    setObjectName ( windowTitle() );

    createBaseWidgets();

    /// Load number of screens for this app
    QSettings qSettings;
    QString appName_str = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
    int sx_i = qSettings.value ( appName_str + 
                                 QString("/ScreenXCount"), "-1").toInt();
    int sy_i = qSettings.value ( appName_str + 
                                 QString("/ScreenYCount"), "-1").toInt();
    
    if (sx_i == -1) sx_i = f_sx_i;
    if (sy_i == -1) sy_i = f_sy_i;

    m_display_p -> setScreenCount ( S2D<unsigned int>(sx_i, sy_i) );


    m_controller_p -> start();    
}

CMainWindow::~CMainWindow( )
{    
    /// Save number of screens for this app
    QSettings qSettings;
    QString appName_str = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();
    qSettings.setValue ( appName_str + 
			 QString("/ScreenXCount"), m_display_p->getScreenCount().x);
    qSettings.setValue ( appName_str + 
			 QString("/ScreenYCount"), m_display_p->getScreenCount().y);
   

    /// Delete all operators.
    //if ( m_rootOp_p )
    //    delete m_rootOp_p;

    /// Delete controller.
    if (m_controller_p)
        delete m_controller_p;
    m_controller_p = NULL;

    /// Delete display.
    if (m_display_p)
        delete m_display_p;
    m_display_p = NULL;    

    /// Delete parameter editor,
    if (m_paramEditorDlg_p)
        delete m_paramEditorDlg_p;
    m_paramEditorDlg_p = NULL;

    if (m_clockTreeDlg_p)
        delete m_clockTreeDlg_p;
    m_clockTreeDlg_p = NULL;

#if defined HAVE_QGLVIEWER
    if (m_3dViewer_p)
        delete m_3dViewer_p;
    m_3dViewer_p = NULL;
#endif
}

void CMainWindow::createBaseWidgets()
{
    /// Create controller.
    m_controller_p      = new CSeqController ( m_device_p );

    /// Create display.
    m_display_p        = new CDisplayWidget ( 0,
                                              m_rootOp_p -> getDrawingListHandler(),
                                              640, 480 );    
    
    /// Create parameter editor.
    m_paramEditorDlg_p = new CParameterEditorDlg ( m_rootOp_p -> getParameterSet(),
                                                   NULL );

    /// Create clock tree dialog.
    m_clockTreeDlg_p   = new CClockTreeDlg ( NULL,
                                             m_rootOp_p -> getClockHandler() -> getRootNode() );

    /// Create the 3D viewer
#if defined HAVE_QGLVIEWER
    m_3dViewer_p       = new CGLViewer;
    m_rootOp_p -> set3DViewer ( m_3dViewer_p );
#endif


    CSimpleWindow::insertWindow ( m_clockTreeDlg_p );
    CSimpleWindow::insertWindow ( m_controller_p -> getDialog() );
    CSimpleWindow::insertWindow ( m_display_p );
    CSimpleWindow::insertWindow ( m_display_p -> getDialog() );
    CSimpleWindow::insertWindow ( m_paramEditorDlg_p );

#if defined HAVE_QGLVIEWER
    CSimpleWindow::insertWindow ( m_3dViewer_p );
#endif

    std::vector<QWidget *> opWidgets = m_rootOp_p -> getWidgets();

    for (unsigned int i = 0; i < opWidgets.size(); ++i)
        CSimpleWindow::insertWindow ( opWidgets[i] );

    // Connections with Controller.
    connect( m_controller_p, SIGNAL(cycle()),      this, SLOT(cycle()) );
    connect( m_controller_p, SIGNAL(stop()),       this, SLOT(stop()));    
    connect( m_controller_p, SIGNAL(initialize()), this, SLOT(initialize()) );
    connect( m_controller_p, SIGNAL(reset()),      this, SLOT(stop()) );

    /// Get display object pointer.
    CDisplay *disp_p = m_display_p -> getDisplay();    

    // Connections Display <-> This
    QObject::connect( disp_p, SIGNAL(mousePressed ( CMouseEvent *  )), 
                      this,   SLOT(  mousePressed ( CMouseEvent * )));
    
    QObject::connect( disp_p, SIGNAL(mouseReleased ( CMouseEvent * )), 
                      this,   SLOT(   mouseReleased ( CMouseEvent * )));

    QObject::connect( disp_p, SIGNAL(mouseMoved ( CMouseEvent * )), 
                      this,   SLOT(  mouseMoved ( CMouseEvent * )));

    QObject::connect( disp_p, SIGNAL(wheelTurned ( CWheelEvent * )), 
                      this,   SLOT(  wheelTurned ( CWheelEvent * )));

    QObject::connect( disp_p, SIGNAL(keyPressed ( CKeyEvent * )), 
                      this,   SLOT(  keyPressed ( CKeyEvent * )));

    QObject::connect( disp_p, SIGNAL(regionSelected ( CRegionSelectedEvent * )), 
                      this,   SLOT(  regionSelected ( CRegionSelectedEvent * )));

    ///////////////////

    m_device_p -> initializeDevice();

}

void CMainWindow::initialize() 
{     
    //exit(1);
    
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    bool success_b;
    
    std::map< std::string, CIOBase * > devOutput;
    success_b = m_device_p -> registerOutputs ( devOutput );

    m_rootOp_p -> startClock ( "OpenGL Display" );
    if ( m_display_p->isVisible() )
    m_display_p -> update(true);
    m_rootOp_p -> stopClock ( "OpenGL Display" );

    m_rootOp_p -> clearIOMap();
    
    if ( success_b )
    {
        m_rootOp_p -> registerOutputs ( devOutput );
        
        m_rootOp_p -> startClock ( "Initialize" );
        m_rootOp_p -> initialize();
        m_rootOp_p -> stopClock ( "Initialize" );

        m_rootOp_p -> startClock ( "Cycle" );
        m_rootOp_p -> cycle();
        m_rootOp_p -> stopClock ( "Cycle" );

        m_rootOp_p -> startClock ( "Show" );
        m_rootOp_p -> show();
        m_rootOp_p -> stopClock ( "Show" );
    }

    m_display_p -> setScreenSize ( m_rootOp_p -> getScreenSize() );

    m_rootOp_p -> startClock ( "OpenGL Display" );
    if ( m_display_p->isVisible() )
    m_display_p -> update(true);
    m_rootOp_p -> stopClock ( "OpenGL Display" );

#if defined HAVE_QGLVIEWER
    m_rootOp_p -> startClock ( "3D Viewer" );
    m_3dViewer_p -> update();
    m_rootOp_p -> stopClock ( "3D Viewer" );
#endif 
    
    
    if (m_autoPlay_b)
    {
        m_controller_p -> playClicked();
    }
    
    m_rootOp_p -> startClock ( "Clock Update" );
    m_clockTreeDlg_p -> updateTimes();    
    m_rootOp_p -> stopClock ( "Clock Update" );

    m_rootOp_p -> startClock ( "Device output update" );
    m_rootOp_p -> getOutputMap(devOutput);
    m_device_p -> updateOutput ( devOutput );
    m_rootOp_p -> stopClock ( "Device output update" );

}

void CMainWindow::cycle() 
{
    m_rootOp_p -> stopClock ( "Out of cycle" );
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    bool success_b;
    
    std::map< std::string, CIOBase * > devOutput;
    success_b = m_device_p -> registerOutputs ( devOutput );

    m_rootOp_p -> clearIOMap();

    if ( success_b )
    {
        m_rootOp_p -> registerOutputs ( devOutput );

        m_rootOp_p -> startClock ( "Cycle" );
        m_rootOp_p -> cycle();
        m_rootOp_p -> stopClock ( "Cycle" );

        m_rootOp_p -> startClock ( "Show" );
        m_rootOp_p -> show();
        m_rootOp_p -> stopClock ( "Show" );
    }

    m_display_p -> setScreenSize ( m_rootOp_p -> getScreenSize() );

    //printf("In CMainWindow::cycle updating display\n");
    m_rootOp_p -> startClock ( "OpenGL Display" );
    if ( m_display_p->isVisible() )
    m_display_p -> update();
    m_rootOp_p -> stopClock ( "OpenGL Display" );
    
#if defined HAVE_QGLVIEWER
        m_rootOp_p -> startClock ( "3D Viewer" );
        m_3dViewer_p -> update();
        m_rootOp_p -> stopClock ( "3D Viewer" );
#endif

    m_rootOp_p -> startClock ( "Clock Update" );
    m_clockTreeDlg_p -> updateTimes();
    m_rootOp_p -> stopClock ( "Clock Update" );	

    m_rootOp_p -> startClock ( "Device output update" );
    m_rootOp_p -> getOutputMap(devOutput);
    m_device_p -> updateOutput ( devOutput );
    m_rootOp_p -> stopClock ( "Device output update" );
    m_rootOp_p -> startClock ( "Out of cycle" );
}

void CMainWindow::stop() 
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    bool success_b;
    
    std::map< std::string, CIOBase * > devOutput;
    success_b = m_device_p -> registerOutputs ( devOutput );

    m_rootOp_p -> clearIOMap();
    
    if ( success_b )
    {
        m_rootOp_p -> registerOutputs ( devOutput );

        m_rootOp_p -> startClock ( "Reset" );
        m_rootOp_p -> reset();
        m_rootOp_p -> stopClock ( "Reset" );

        m_rootOp_p -> startClock ( "Initialize" );
        m_rootOp_p -> initialize();
        m_rootOp_p -> stopClock ( "Initialize" );

        m_rootOp_p -> startClock ( "Cycle" );
        m_rootOp_p -> cycle();
        m_rootOp_p -> stopClock ( "Cycle" );

        m_rootOp_p -> startClock ( "Show" );
        m_rootOp_p -> show();
        m_rootOp_p -> stopClock ( "Show" );
    } 

    m_display_p -> setScreenSize ( m_rootOp_p -> getScreenSize() );

    m_rootOp_p -> startClock ( "OpenGL Display" );
    if ( m_display_p->isVisible() )
    m_display_p -> update();
    m_rootOp_p -> stopClock ( "OpenGL Display" );

#if defined HAVE_QGLVIEWER
    m_rootOp_p -> startClock ( "3D Viewer" );
    m_3dViewer_p -> update();
    m_rootOp_p -> stopClock ( "3D Viewer" );
#endif

    m_rootOp_p -> startClock ( "Clock Update" );
    m_clockTreeDlg_p -> updateTimes();    
    m_rootOp_p -> stopClock ( "Clock Update" );

    m_rootOp_p -> startClock ( "Device output update" );
    m_rootOp_p -> getOutputMap(devOutput);
    m_device_p -> updateOutput ( devOutput );
    m_rootOp_p -> stopClock ( "Device output update" );
}

void CMainWindow::keyPressed ( CKeyEvent * const f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    QKeyEvent* keyEvent_p = f_event_p -> qtKeyEvent_p;
    
    if ( keyEvent_p -> key() == Qt::Key_P || 
         keyEvent_p -> key() == Qt::Key_Space )
    {
        if ( m_device_p -> getState() == CSeqDeviceControl::S_PLAYING )
        {
            m_controller_p -> pauseClicked();
        }
        else
        {
            m_controller_p -> playClicked();
        }       
    }
    else if ( keyEvent_p -> key() == Qt::Key_S )
    {
        m_controller_p -> stopClicked();
    }

    if ( m_device_p -> getState() != CSeqDeviceControl::S_PLAYING )
    {
        if ( keyEvent_p -> key() == Qt::Key_Right ||
             keyEvent_p -> key() == Qt::Key_P )
        {
            m_controller_p -> nextClicked();
        }
        else if ( keyEvent_p -> key() == Qt::Key_Left ||
             keyEvent_p -> key() == Qt::Key_N )
        {
            m_controller_p -> previousClicked();
        }
        else if ( keyEvent_p -> key() == Qt::Key_R )
        {
            m_controller_p -> reloadClicked();
        }
    }

    m_rootOp_p -> keyPressed ( f_event_p );
    if ( m_display_p->isVisible() )
    m_display_p -> update ( false );
}

void
CMainWindow::mousePressed   ( CMouseEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
        return;

    m_rootOp_p -> mousePressed ( f_event_p );

    if ( m_display_p->isVisible() )
    m_display_p -> update ( false );
}

void 
CMainWindow::mouseReleased  ( CMouseEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p -> mouseReleased ( f_event_p );

    if ( m_display_p->isVisible() )
    m_display_p -> update ( false );
}

void CMainWindow::mouseMoved     ( CMouseEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p -> mouseMoved ( f_event_p );

    if ( m_display_p->isVisible() )
    m_display_p -> update ( false );
}

void CMainWindow::wheelTurned    ( CWheelEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p ->wheelTurned  ( f_event_p );

    if ( m_display_p->isVisible() )
    m_display_p -> update ( false );
}

void CMainWindow::regionSelected ( CRegionSelectedEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("%s:%i Device %s not initialized\n", __FILE__, __LINE__, m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p -> regionSelected ( f_event_p );

    if ( m_display_p->isVisible() )
    m_display_p -> update ( false );
}

void CMainWindow::closeEvent ( QCloseEvent *  f_event_p)
{
    m_rootOp_p -> startClock ( "Exit" );
    m_rootOp_p -> exit();
    m_rootOp_p -> stopClock ( "Exit" );

    f_event_p->accept();
    qApp->quit();
}
