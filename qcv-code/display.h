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

#ifndef __CVGLDISPLAY_H
#define __CVGLDISPLAY_H

/*@@@**************************************************************************
 * \file  drawingList
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QGLWidget>
#include "drawingList.h"
#include "events.h"
//#include "rgbImage.h"

namespace QCV
{

    /* PROTOTYPES */
    class CDisplayOpNode;

    class CDisplay : public QGLWidget
    {
        Q_OBJECT

        public:
        CDisplay( CDisplayOpNode * const  f_rootNode_p,
                  QWidget *               parent = 0 );

        ~CDisplay();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

    /// Zoom methods.
    public:
        
        bool   zoomToScreen ( const S2D<float> screen,
                              const bool       f_update_b = true );
        
        bool   zoomToScreen ( const unsigned int f_screenX_ui, 
                              const unsigned int f_screenY_ui,
                              const bool         f_update_b = true );
        
        bool   setScreenCount ( S2D<unsigned int> count );
        S2D<unsigned int>
               getScreenCount (  ) const
        {
            return m_screenCount;
        }
        
        
        QImage renderGL ();

        bool setScreenSize ( const S2D<unsigned int> f_size );
        S2D<unsigned int>   
             getScreenSize (  ) const;

        int    getScreenWidth   (  ) const { return m_screenSize.width;  }
        int    getScreenHeight  (  ) const { return m_screenSize.height;  }
        
    public slots:
        bool   showAllScreens ( );
        void   show ( );

    protected slots:
        virtual void   showEvent ( QShowEvent * f_event_p );
        virtual void   timerEvent ( QTimerEvent * f_event_p ); 

    signals:
        void clicked();
        void fullScreenSwitched();
        void exitFullScreen();

        void keyPressed    ( CKeyEvent   *  f_event_p );
        void mousePressed  ( CMouseEvent *  f_event_p );
        void mouseReleased ( CMouseEvent *  f_event_p );
        void mouseMoved    ( CMouseEvent *  f_event_p );
        void wheelTurned   ( CWheelEvent *  f_event_p );

        void regionSelected  ( CRegionSelectedEvent * 
                                            f_event_p );
        void glPainted     ( );
        
    /// Inherited members.
    protected:
        
        virtual void initializeGL();
        virtual void paintGL();
        virtual void resizeGL ( int width, 
                                int height );
        virtual void mousePressEvent   ( QMouseEvent *  f_event_p );
        virtual void mouseMoveEvent    ( QMouseEvent *  f_event_p );
        virtual void mouseReleaseEvent ( QMouseEvent *  f_event_p );
        virtual void wheelEvent        ( QWheelEvent *  f_event_p );
        virtual void keyPressEvent     ( QKeyEvent *    f_event_p );

        virtual void dragEnterEvent    ( QDragEnterEvent *f_event_p );
        virtual void dragMoveEvent     ( QDragMoveEvent *f_event_p );
        virtual void dropEvent         ( QDropEvent *f_event_p );

        virtual void highlightScreen   (  );

    /// Own help methods.
    protected:

        virtual void zoom ( float f_x_f, 
                            float f_y_f, 
                            float f_newZoomFactor_f,
                            bool f_updateGL_b );
        virtual void zoomIn();
        virtual void zoomOut();
        

        void displayScreens ( CDisplayOpNode * const f_parent_p,
                              const int              f_level_i );
        
        bool isOneScreenMode   ( ) const { return m_fsScreen.isValid(); }
        void exitOneScreenMode ( ) { m_fsScreen.invalidate(); }
        
        CMouseEvent* getMouseEventData( QMouseEvent * f_event_p );

    /// Data members.
    private:

        /// Root node of display lists tree.
        CDisplayOpNode *           m_rootNode_p;

        /// Size of each screen.
        S2D<int>                   m_screenSize;

        /// Screen count.
        S2D<unsigned int>          m_screenCount;
        
        /// Position of zoom.
        S2D<float>                 m_zoomTL;

        /// Position of zoom when mouse was pressed.
        S2D<float>                 m_prevZoomTL;

        /// Zoom factor.
        float                      m_zoomFactor_f;

        /// Position of mouse when mouse was pressed.
        S2D<float>                 m_prevMousePos;
        
        /// Scale of display.
        S2D<float>                 m_roiSize;

        /// Scale of display.
        S2D<float>                 m_roiPos;

        /// Curren screen being display in one screen mode.
        S2D<int>                   m_fsScreen;

        /// Snapshot captured
        cv::Mat                    m_snapshot;

        /// Auxiliar image for flipping the snapshot vertically.
        cv::Mat                    m_aux;

	/// Flag
	bool                       m_initialized_b;

        /// Highligh screen
        S2D<int>                   m_highlightScreen;
    };
}


#endif // #define __CVGLDISPLAY_H
