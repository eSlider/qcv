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

#ifndef __WINDOWLISTVIEW_H
#define __WINDOWLISTVIEW_H

/**
 *******************************************************************************
 * @file windowListView.h
 *
 * \class CWindowListView
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements a QListView for registered windows of the framework.
 *
 * The list handles the visibility of the windows and stores and restore
 * the window position and visibility when the program is restarted.
 *******************************************************************************/

/* INCLUDES */

#include <QListView>

/* CONSTANTS */

namespace QCV
{
    class CWindowListView: public QListView
    {
        Q_OBJECT
        
     public:
        CWindowListView ( QWidget * m_parent_p = 0 );
        ~CWindowListView();

        void insertWindow( QWidget * f_widget_p, 
                           bool f_showNow_b = false );

    public slots:
        void clear ( );
        void showWindows( );
        void saveWindowsGeometry() const;

    public slots:

        void showAllWindows();
        void hideAllWindows();
        
    protected:
        void dataChanged( const QModelIndex & f_topLeft, 
                          const QModelIndex & f_bottomRight );

    protected:
        
        typedef enum
        {
            WS_UNINITIALIZED = 0,
            WS_INVISIBLE,
            WS_VISIBLE
        } EWidgetState;        
        
        /// Vector of handled windows.
        std::vector<QWidget *>       m_window_v;
    };
    
}

    
    

#endif // __WINDOWLISTVIEW_H
