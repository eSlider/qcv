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

        void insertWindow( QWidget * f_widget_p, bool f_showNow_b = true );
        void clear ( );
        void showWindows( );
        void saveWindowsGeometry() const;
        
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
