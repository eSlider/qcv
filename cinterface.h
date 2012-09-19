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

#ifndef __CINTERFACE_H
#define __CINTERFACE_H

#include "displayWidget.h"
#include "drawingListHandler.h"
#include "node.h"
#include "eventHandler.h"
#include "clockHandler.h"
#include "clockTreeDlg.h"
#include "clock.h"
#include "simpleWindow.h"
#include <stdio.h>
#include <iostream> 

#include <cv.h>
#include <highgui.h>

namespace QCV
{
    CDisplayWidget *      g_disp_p         = NULL;
    CDrawingListHandler * g_drawHandler_p  = NULL;
    CNode *               g_rootNode_p     = NULL;
    CEventHandler *       g_evHandler_p    = NULL;
    CSimpleWindow *       g_mainWindow_p   = NULL;
    CClockTreeDlg *       g_clockTree_p    = NULL;
    CClockHandler *       g_clockHandler_p = NULL;

    static 
    void initialize( std::string f_title_str = "Application")
    {
        if ( !g_disp_p )
        {
            g_mainWindow_p = new CSimpleWindow();
            assert(g_mainWindow_p);

            //CNode * root_p = new CNode (NULL, "Real Root");
            g_rootNode_p = new CNode ( NULL, f_title_str );
            assert(g_rootNode_p);

            g_drawHandler_p = new CDrawingListHandler( g_rootNode_p );
            assert(g_drawHandler_p);

            g_clockHandler_p = new CClockHandler ( g_rootNode_p );
            assert(g_clockHandler_p);

            g_clockTree_p = new CClockTreeDlg ( NULL, g_clockHandler_p->getRootNode() );
            assert(g_clockTree_p);

            g_disp_p = new CDisplayWidget(NULL, g_drawHandler_p, 800, 600);
            assert(g_disp_p);

            g_evHandler_p = new CEventHandler( g_disp_p );
            assert(g_evHandler_p);

            g_mainWindow_p -> insertWindow ( g_disp_p );
            g_mainWindow_p -> insertWindow ( g_disp_p->getDialog() );
            g_mainWindow_p -> insertWindow ( g_clockTree_p );

            /// The window list now.
            g_mainWindow_p -> show();
        }
    }

    static 
    void terminate( )
    {
        if ( g_mainWindow_p )
            delete g_mainWindow_p;
        g_mainWindow_p = NULL;

        if ( g_clockTree_p )
            delete g_clockTree_p;
        g_clockTree_p = NULL;

        if ( g_clockHandler_p )
            delete g_clockHandler_p;
        g_clockHandler_p = NULL;
        
        if ( g_disp_p )
            delete g_disp_p;
        g_disp_p = NULL;
        
        if (g_drawHandler_p)
            delete g_drawHandler_p;
        g_drawHandler_p = NULL;
        
        if (g_evHandler_p)
            delete g_evHandler_p;
        g_evHandler_p = NULL;        
    }

    static 
    CDrawingList * 
    displayImage ( cv::Mat     f_img, 
                   std::string f_title_str, 
                   bool        f_clear_b = true,
                   int         f_sx_i = -1, 
                   int         f_sy_i = -1 )
    {
        if (!g_drawHandler_p) 
        {
            //char str[256];
            //sprintf(str, "%s:%d : ", __FILE__, __LINE__ );
            //throw std::runtime_error(std::string(str) + std::string("Drawing list handler has not been initialized\n"));
            return NULL;
        }

        CDrawingList *  list_p = g_drawHandler_p->getDrawingList ( f_title_str, g_rootNode_p );
        assert(list_p);

        if ( f_clear_b )
            list_p -> clear();

        if (f_sx_i != -1 && f_sy_i != -1)
        {    
            list_p->setPosition ( S2D<int>( f_sx_i, f_sy_i) );
        }
        
        list_p->setScreenSize ( f_img.size() );
        list_p->addImage ( f_img );

        return list_p;    
    }

    static 
    CDrawingList * 
    registerDrawingList ( std::string f_title_str, 
                          int         f_sx_i, 
                          int         f_sy_i )
    {
        if (!g_drawHandler_p) 
        {
            //char str[256];
            //sprintf(str, "%s:%d : ", __FILE__, __LINE__ );
            //throw std::runtime_error(std::string(str) + std::string("Drawing list handler has not been initialized\n"));
            return NULL;
        }

        CDrawingList *  list_p = g_drawHandler_p->getDrawingList ( f_title_str, g_rootNode_p );
        assert(list_p);

        list_p->setPosition ( S2D<int>( f_sx_i, f_sy_i) );

        return list_p;    
    }

    static 
    CDrawingList *
    getDrawingList( std::string f_title_str )
    {
        if (!g_drawHandler_p) 
        {
            //char str[256];
            //sprintf(str, "%s:%d : ", __FILE__, __LINE__ );
            //throw std::runtime_error(std::string(str) + std::string("Drawing list handler has not been initialized\n"));
            return NULL;
        }
    
        return g_drawHandler_p->getDrawingList ( f_title_str, g_rootNode_p );
    }

    static 
    void updateDisplay()
    {
        g_disp_p -> update ( true );
    }

    static 
    bool  setKeyPressedEventCBF(       bool (*f_keyEv_p)           (CKeyEvent *   ))
    {
        return  g_evHandler_p -> setKeyPressedEventCBF ( f_keyEv_p  );
    }    
    
    static 
    bool  setMousePressedEventCBF (    bool (*f_mousePressedEv_p)  (CMouseEvent * ))
    {
        return  g_evHandler_p -> setMousePressedEventCBF (  f_mousePressedEv_p );
    }
    
    static 
    bool  setMouseReleasedEventCBF (   bool (*f_mouseReleasedEv_p) (CMouseEvent * ))
    {
        return  g_evHandler_p -> setMouseReleasedEventCBF ( f_mouseReleasedEv_p );
    }
    
    static 
    bool  setMouseMovedEventCBF (      bool (*f_mouseMovedEv_p)    (CMouseEvent * ))
    {
        return  g_evHandler_p -> setMouseMovedEventCBF (    f_mouseMovedEv_p );
    }
    
    static 
    bool  setWheelTurnedEventCBF (     bool (*f_wheelTurnedEv_p)   (CWheelEvent * ))
    {
        return  g_evHandler_p -> setWheelTurnedEventCBF (   f_wheelTurnedEv_p );
    }

    static 
    bool  setTimerEventCBF (     bool (*f_timerEv_p)   (CTimerEvent * ), int f_mstime_i = 10 )
    {
        return  g_evHandler_p -> setTimerEventCBF (   f_timerEv_p, f_mstime_i );
    }
    
    static 
    bool  setRegionSelectedEventCBF (  bool (*f_regionSelectedEv_p)(CRegionSelectedEvent * ))
    {
        return  g_evHandler_p -> setRegionSelectedEventCBF (f_regionSelectedEv_p );
    }

    static 
    void setScreenSize( cv::Size size )
    {
        g_disp_p->setScreenSize( size );
    }

    static 
    int getScreenWidth()
    {
        return g_disp_p->getDisplay()->getScreenWidth();
    }
    
    static 
    int getScreenHeight()
    {
        return g_disp_p->getDisplay()->getScreenHeight();
    }
    
    static 
    void setScreenCount( cv::Size size )
    {
        g_disp_p->setScreenCount(size);
    }

    static 
    void startClock( std::string f_name_str )
    {
        g_clockHandler_p->getClock(f_name_str, g_rootNode_p)->start();
    }

    static 
    void stopClock( std::string f_name_str )
    {
        g_clockHandler_p->getClock(f_name_str, g_rootNode_p)->stop();
    }

    static 
    void updateClocks(  )
    {
        g_clockTree_p->updateTimes();
    }
}

#endif // __CINTERFACE_H
