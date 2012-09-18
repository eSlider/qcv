/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

/* CONSTANT */ 

/* INCLUDES */
#include "mainWindow.h"
#include "deviceOpBinder.h"

#include "events.h"
#include "seqControlDlg.h"

#include "operator.h"

#include "seqControler.h"
#include "displayWidget.h"
#include "paramEditorDlg.h"
#include "clockTreeDlg.h"

//#include "displayTreeNode.h"
//#include "clockTreeNode.h"

CMainWindow::CMainWindow ( CDeviceOpBinderBase * f_binder_p )
    : CSimpleWindow (               ),
      m_binder_p (       f_binder_p ),
      m_device_p (             NULL ),
      m_rootOp_p (             NULL ),
      m_controler_p (          NULL ),
      m_display_p (            NULL ),
      m_paramEditorDlg_p (     NULL ),
      m_clockTreeDlg_p (       NULL )
{
    //QStringList list = QCoreApplication::arguments ();
    
    assert( f_binder_p );
    m_device_p = f_binder_p -> getDeviceCtrl();
    m_rootOp_p = f_binder_p -> getRootOperator();

    assert( m_device_p );
    assert( m_rootOp_p );    

    setWindowTitle( tr("QCV Main Window") );
    setObjectName ( windowTitle() );

    createBaseWidgets();

    m_controler_p -> start();    
}

CMainWindow::~CMainWindow( )
{    
    /// Delete all operators.
    if ( m_rootOp_p )
        delete m_rootOp_p;

    /// Delete controler.
    if (m_controler_p)
        delete m_controler_p;
    m_controler_p = NULL;

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
}

void CMainWindow::createBaseWidgets()
{
    /// Create controler.
    m_controler_p      = new CSeqControler ( m_device_p );

    /// Create display.
    m_display_p        = new CDisplayWidget ( 0,
                                              m_rootOp_p -> getDrawingListHandler(),
                                              640, 480 );    
    
    /// Create parameter editor.
    m_paramEditorDlg_p = new CParameterEditorDlg ( m_rootOp_p -> getParameterSet(),
                                                   NULL );

    /// Create clock tree dialog.
    m_clockTreeDlg_p   = new CClockTreeDlg ( this,
                                             m_rootOp_p -> getClockHandler() -> getRootNode() );

    CSimpleWindow::insertWindow ( m_controler_p -> getDialog() );
    CSimpleWindow::insertWindow ( m_display_p );
    CSimpleWindow::insertWindow ( m_display_p -> getDialog() );
    CSimpleWindow::insertWindow ( m_paramEditorDlg_p );
    CSimpleWindow::insertWindow ( m_clockTreeDlg_p );

    std::vector<QWidget *> opWidgets = m_rootOp_p -> getWidgets();

    for (unsigned int i = 0; i < opWidgets.size(); ++i)
        CSimpleWindow::insertWindow ( opWidgets[i] );

    // Connections with Controler.
    connect( m_controler_p, SIGNAL(cycle()),      this, SLOT(cycle()) );
    connect( m_controler_p, SIGNAL(stop()),       this, SLOT(stop()));    
    connect( m_controler_p, SIGNAL(initialize()), this, SLOT(initialize()) );
    connect( m_controler_p, SIGNAL(reset()),      this, SLOT(stop()) );

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
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    bool success_b;
    
    success_b = m_binder_p -> setOperatorInput ( );

    m_display_p -> update(true);

    if ( success_b )
    {
        m_rootOp_p -> startClock ( "Initialize" );
        success_b = m_rootOp_p -> initialize();
        m_rootOp_p -> stopClock ( "Initialize" );

        m_rootOp_p -> startClock ( "Cycle" );
        success_b = m_rootOp_p -> cycle();
        m_rootOp_p -> stopClock ( "Cycle" );

        m_rootOp_p -> startClock ( "Show" );
        success_b = m_rootOp_p -> show();
        m_rootOp_p -> stopClock ( "Show" );
    }

    m_rootOp_p -> startClock ( "OpenGL Display" );
    m_display_p -> update(true);
    m_rootOp_p -> stopClock ( "OpenGL Display" );


    // std::vector<QWidget *> opWidgets = m_rootOp_p -> getWidgets();

    // for (unsigned int i = 0; i < opWidgets.size(); ++i)
    // {
    //     CSimpleWindow::insertWindow ( opWidgets[i] );
    // }
    
    m_rootOp_p -> startClock ( "Clock Update" );
    m_clockTreeDlg_p -> updateTimes();    
    m_rootOp_p -> stopClock ( "Clock Update" );

}

void CMainWindow::cycle() 
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    bool success_b;
    
    success_b = m_binder_p -> setOperatorInput (  );

    m_display_p -> setScreenSize ( m_rootOp_p -> getScreenSize() );

    if ( success_b )
    {
        //printf("In CMainWindow::cycle calling cycle for root operator\n");
        m_rootOp_p -> startClock ( "Cycle" );
        success_b = m_rootOp_p -> cycle();
        m_rootOp_p -> stopClock ( "Cycle" );

        //printf("In CMainWindow::cycle calling show for root operator\n");
        m_rootOp_p -> startClock ( "Show" );
        success_b = m_rootOp_p -> show();
        m_rootOp_p -> stopClock ( "Show" );
    }

    //printf("In CMainWindow::cycle updating display\n");
    m_rootOp_p -> startClock ( "OpenGL Display" );
    m_display_p -> update();
    m_rootOp_p -> stopClock ( "OpenGL Display" );

    m_rootOp_p -> startClock ( "Clock Update" );
    m_clockTreeDlg_p -> updateTimes();
    m_rootOp_p -> stopClock ( "Clock Update" );	
}

void CMainWindow::stop() 
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }


    bool success_b;
    
    success_b = m_binder_p -> setOperatorInput (  );

    if ( success_b )
    {
        m_rootOp_p -> startClock ( "Reset" );
        success_b = m_rootOp_p -> reset();
        m_rootOp_p -> stopClock ( "Reset" );

        m_rootOp_p -> startClock ( "Initialize" );
        success_b = m_rootOp_p -> initialize();
        m_rootOp_p -> stopClock ( "Initialize" );

        m_rootOp_p -> startClock ( "Cycle" );
        success_b = m_rootOp_p -> cycle();
        m_rootOp_p -> stopClock ( "Cycle" );

        m_rootOp_p -> startClock ( "Cycle" );
        success_b = m_rootOp_p -> show();
        m_rootOp_p -> stopClock ( "Show" );
#if defined HAVE_QGLVIEWER
#endif
    }

    m_rootOp_p -> startClock ( "OpenGL Display" );
    m_display_p -> update();
    m_rootOp_p -> stopClock ( "OpenGL Display" );

    m_rootOp_p -> startClock ( "Clock Update" );
    m_clockTreeDlg_p -> updateTimes();    
    m_rootOp_p -> stopClock ( "Clock Update" );

#if defined HAVE_QGLVIEWER
    m_rootOp_p -> startClock ( "3D Viewer" );
    m_3dViewer_p -> update();
    m_rootOp_p -> stopClock ( "3D Viewer" );
#endif

}

void CMainWindow::keyPressed ( CKeyEvent * const f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    QKeyEvent* keyEvent_p = f_event_p -> qtKeyEvent_p;
    
    if ( keyEvent_p -> key() == Qt::Key_P || 
         keyEvent_p -> key() == Qt::Key_Space )
    {
        if ( m_device_p -> getState() == CSeqDeviceControlBase::S_PLAYING )
        {
            m_controler_p -> pauseClicked();
        }
        else
        {
            m_controler_p -> playClicked();
        }       
    }
    else if ( keyEvent_p -> key() == Qt::Key_S )
    {
        m_controler_p -> stopClicked();
    }

    if ( m_device_p -> getState() != CSeqDeviceControlBase::S_PLAYING )
    {
        if ( keyEvent_p -> key() == Qt::Key_Right ||
             keyEvent_p -> key() == Qt::Key_P )
        {
            m_controler_p -> nextClicked();
        }
        else if ( keyEvent_p -> key() == Qt::Key_Left ||
             keyEvent_p -> key() == Qt::Key_N )
        {
            m_controler_p -> previousClicked();
        }
        else if ( keyEvent_p -> key() == Qt::Key_R )
        {
            m_controler_p -> reloadClicked();
        }
    }

    m_rootOp_p -> keyPressed ( f_event_p );
    m_display_p -> update ( false );
}

void
CMainWindow::mousePressed   ( CMouseEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
        return;

    m_rootOp_p -> mousePressed ( f_event_p );

    m_display_p -> update ( false );
}

void 
CMainWindow::mouseReleased  ( CMouseEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p -> mouseReleased ( f_event_p );

    m_display_p -> update ( false );
}

void CMainWindow::mouseMoved     ( CMouseEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p -> mouseMoved ( f_event_p );

    m_display_p -> update ( false );
}

void CMainWindow::wheelTurned    ( CWheelEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p ->wheelTurned  ( f_event_p );

    m_display_p -> update ( false );
}

void CMainWindow::regionSelected ( CRegionSelectedEvent *  f_event_p )
{
    if ( not m_device_p -> isInitialized() )
    {
        printf("Device %s not initialized\n", m_device_p ->getName().c_str());
        return;
    }

    m_rootOp_p -> regionSelected ( f_event_p );

    m_display_p -> update ( false );
}

void CMainWindow::closeEvent ( QCloseEvent *  f_event_p)
{
    //printf("Close event was called\n");
    m_rootOp_p -> startClock ( "Exit" );
    m_rootOp_p -> exit();
    m_rootOp_p -> stopClock ( "Exit" );
    //f_event_p->ignore();
    f_event_p->accept();

    /////////////// THIS FUNCTION IS NOT BEING CALLED!!!

#if 0
    if (maybeSave()) 
    {
        writeSettings();
        event->accept();
    } else 
    {
        event->ignore();
     }
#endif
}
