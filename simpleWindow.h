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


#ifndef __SIMPLEWINDOW_H
#define __SIMPLEWINDOW_H

/**
 *******************************************************************************
 *
 * @file simplewindow.h
 *
 * \class CSimpleWindow
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief The class implements the main window dialog of the framework.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QtGui/QMainWindow>
#include "events.h"

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
/* NAMESPACE PROTOTYPES */
    class CWindowListView;
    
    class CSimpleWindow: public QMainWindow
    {
        Q_OBJECT

    public:
                
    public:
        
        CSimpleWindow ( );
        virtual ~CSimpleWindow();
        
    public slots:
        /// Hide all windows.
        virtual void hideAllWindows();

        /// Show all windows.
        virtual void showAllWindows();

        /// Show about dialog.
        virtual void about();

        /// Insert window
        virtual void insertWindow( QWidget * f_widget_p, 
                                   bool f_showNow_b = false );
        // Show 
        virtual void show();
        
    protected:
        virtual void closeEvent ( QCloseEvent *  f_event_p);
        
    //// Protected methods.
    protected:
        
        /// Create Menues.
        void createMenues();

    /// Protected members
    protected:

        /// Vector of windows.
        CWindowListView *         m_listView_p;
    };
}

#endif // __SIMPLEWINDOW_H
