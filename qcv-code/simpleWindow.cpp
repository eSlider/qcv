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
* \file  mainWindow
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* CONSTANT */ 
/* INCLUDES */
#include <QSettings>
#include <QMessageBox>
#include <QCoreApplication>
#include <QToolBar>
#include <QMenuBar>

#include "simpleWindow.h"
#include "windowListView.h"

#include <stdio.h>

using namespace QCV;

CSimpleWindow::CSimpleWindow ( )
        : m_listView_p (           NULL )
{
    QStringList list = QCoreApplication::arguments ();
    
    setWindowTitle( tr("QCV Window Control") );
    setObjectName ( windowTitle() );

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/symbols/showAllWindows.png")), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    /// Read saved geometry.
    QSettings qSettings;
    restoreGeometry ( 
            qSettings.value(objectName() + QString("/geometry")).toByteArray() );

    /// Create central widget
    m_listView_p = new CWindowListView ( this );

    setCentralWidget( m_listView_p );

    /// Create menues.
    createMenues();
}

CSimpleWindow::~CSimpleWindow( )
{   
    /// Write geometry.
    QSettings qSettings;
    qSettings.setValue(objectName() + QString("/geometry"), 
                       saveGeometry());

    /// Iterate over windows and close them.

    delete m_listView_p;
}

void CSimpleWindow::createMenues()
{
    // Create first menu and toolbar entries.
    QToolBar *qtbWindow_p = this -> addToolBar ( tr("Windows") );
    QMenu    *qmWindows_p = this -> menuBar() -> addMenu( tr("&Windows") );
    QMenu    *qmHelp_p    = this -> menuBar() -> addMenu( tr("&Help") );

    /// First set toolbar not moveable.
    qtbWindow_p -> setMovable ( false );
    qtbWindow_p -> setIconSize ( QSize(32,32) );

    /// Create Show All Windows Action.
    QAction *qaShowAllWindows_p = new QAction ( QIcon(":/symbols/showAllWindows.png"),
                                                tr("&Show all windows"),
                                                this );
    qaShowAllWindows_p -> setStatusTip( tr("Make all windows visible") );
    qtbWindow_p -> addAction ( qaShowAllWindows_p );
    qmWindows_p -> addAction ( qaShowAllWindows_p );
    // Connect action to corresponding slot.
    connect( qaShowAllWindows_p, SIGNAL(triggered(bool)), m_listView_p, SLOT(showAllWindows()) );

    /// Create Hide All Windows Action.
    //QAction *qaHideAllWindows_p = new QAction ( QIcon(":/symbols/hideAllWindows.png"),
    QAction *qaHideAllWindows_p = new QAction ( QIcon(":/symbols/hideAllWindows.png"),
                                                tr("&Hide all windows"),
                                                this );
    qaHideAllWindows_p -> setStatusTip( tr("Make all windows invisible") );
    qtbWindow_p -> addAction ( qaHideAllWindows_p );
    qmWindows_p -> addAction ( qaHideAllWindows_p );
    // Connect action to corresponding slot.
    connect( qaHideAllWindows_p, SIGNAL(triggered(bool)), m_listView_p, SLOT(hideAllWindows()) );


    /// Create Exit Action.
    QAction *qaExit_p = new QAction ( QIcon(":/symbols/exit.png"),
                                      tr("&Exit."),
                                      this );

    qaExit_p -> setStatusTip( tr("Exit application saving preferences.") );
    qtbWindow_p -> addAction ( qaExit_p );
    qmWindows_p -> addAction ( qaExit_p );
    // Connect action to quit slot.
    connect( qaExit_p, SIGNAL(triggered(bool)), m_listView_p, SLOT(saveWindowsGeometry()) );
    connect( qaExit_p, SIGNAL(triggered(bool)), this, SLOT(close()) );

    /// Create About Action
    QAction *qaAbout_p = new QAction ( QIcon(":/symbols/about.png"),
                                       tr("&About CVFramework..."),
                                       this );
    qaAbout_p -> setStatusTip( tr("Show CVFramework information.") );
    qmHelp_p -> addAction ( qaAbout_p );
    // Connect action to quit slot.
    connect( qaAbout_p, SIGNAL(triggered(bool)), this, SLOT(about()) );
}

void
CSimpleWindow::show()
{   
    m_listView_p -> showWindows();

    QMainWindow::show();
}

void CSimpleWindow::about() 
{
    QMessageBox::about( this,
                        "About CVFramework ",
                        tr("This app was coded for educational purposes") );
}

void CSimpleWindow::hideAllWindows() 
{
    QMessageBox::about( this,"Hiding all windows.", "Bye" );
}

void CSimpleWindow::showAllWindows() 
{
    QMessageBox::about( this,
                        "Showing all windows.",
                        "Bye" );
}

void 
CSimpleWindow::insertWindow( QWidget * f_widget_p, 
                             bool      f_showNow_b )
{
    m_listView_p -> insertWindow ( f_widget_p, f_showNow_b );
}

void CSimpleWindow::closeEvent ( QCloseEvent *  f_event_p)
{
    f_event_p->accept();    
    qApp->quit();
}

