/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU General Public License, version 2.
 * See the GNU General Public License, version 2 for details.
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
#include <stdio.h>
#include <iostream> 

namespace QCV
{
    CDisplayWidget *      g_disp_p         = NULL;
    CDrawingListHandler * g_drawHandler_p  = NULL;
    CNode *               g_parentNode_p   = NULL;
    CEventHandler *       g_evHandler_p    = NULL;

    static 
    void initialize( std::string f_title_str = "Application")
    {
        if ( !g_disp_p )
        {
            g_parentNode_p = new CNode ( new CNode (NULL, "Real Root"), f_title_str );
            assert(g_parentNode_p);

            g_drawHandler_p = new CDrawingListHandler(g_parentNode_p);
            assert(g_drawHandler_p);

            g_disp_p = new CDisplayWidget(NULL, g_drawHandler_p, 800, 600);
            assert(g_disp_p);

            g_evHandler_p = new CEventHandler( g_disp_p );
            assert(g_evHandler_p);

            g_disp_p -> show();
            g_disp_p->getDialog() -> show();        
        }
    }

    static 
    void terminate( )
    {
        if ( !g_disp_p )
            delete g_disp_p;
        g_disp_p = NULL;
        
        if (g_drawHandler_p)
            delete g_drawHandler_p;
        g_drawHandler_p = NULL;
        
        if (g_evHandler_p)
            delete g_evHandler_p;
        g_evHandler_p = NULL;        
    }

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

        CDrawingList *  list_p = g_drawHandler_p->getDrawingList ( f_title_str, g_parentNode_p );
        assert(list_p);

        if ( f_clear_b )
            list_p -> clear();

        if (f_sx_i != -1 && f_sy_i != -1)
        {    
            list_p->setPosition ( S2D<int>( f_sx_i, f_sy_i) );
        }
        
        list_p->setScreenSize ( S2D<unsigned int> (f_img.size().width, f_img.size().height ) );
        list_p->addImage ( f_img );

        return list_p;    
    }

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

        CDrawingList *  list_p = g_drawHandler_p->getDrawingList ( f_title_str, g_parentNode_p );
        assert(list_p);

        list_p->setPosition ( S2D<int>( f_sx_i, f_sy_i) );

        return list_p;    
    }

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
    
        return g_drawHandler_p->getDrawingList ( f_title_str, g_parentNode_p );
    }

    void updateDisplay()
    {
        g_disp_p -> update ( true );
    }

    bool  setKeyPressedEventCBF(       bool (*f_keyEv_p)           (CKeyEvent *   ))
    {
        return  g_evHandler_p -> setKeyPressedEventCBF ( f_keyEv_p  );
    }    
    
    bool  setMousePressedEventCBF (    bool (*f_mousePressedEv_p)  (CMouseEvent * ))
    {
        return  g_evHandler_p -> setMousePressedEventCBF (  f_mousePressedEv_p );
    }
    
    bool  setMouseReleasedEventCBF (   bool (*f_mouseReleasedEv_p) (CMouseEvent * ))
    {
        return  g_evHandler_p -> setMouseReleasedEventCBF ( f_mouseReleasedEv_p );
    }
    
    bool  setMouseMovedEventCBF (      bool (*f_mouseMovedEv_p)    (CMouseEvent * ))
    {
        return  g_evHandler_p -> setMouseMovedEventCBF (    f_mouseMovedEv_p );
    }
    
    bool  setWheelTurnedEventCBF (     bool (*f_wheelTurnedEv_p)   (CWheelEvent * ))
    {
        return  g_evHandler_p -> setWheelTurnedEventCBF (   f_wheelTurnedEv_p );
    }

    bool  setTimerEventCBF (     bool (*f_timerEv_p)   (CTimerEvent * ), int f_mstime_i = 10 )
    {
        return  g_evHandler_p -> setTimerEventCBF (   f_timerEv_p, f_mstime_i );
    }
    
    bool  setRegionSelectedEventCBF (  bool (*f_regionSelectedEv_p)(CRegionSelectedEvent * ))
    {
        return  g_evHandler_p -> setRegionSelectedEventCBF (f_regionSelectedEv_p );
    }

    void setScreenSize( cv::Size size )
    {
        g_disp_p->setScreenSize(S2D<unsigned int> (size.width, size.height) );
    }

    int getScreenWidth()
    {
        return g_disp_p->getDisplay()->getScreenWidth();
    }
    
    int getScreenHeight()
    {
        return g_disp_p->getDisplay()->getScreenHeight();
    }
    
    void setScreenCount( cv::Size size )
    {
        g_disp_p->setScreenCount(S2D<unsigned int> (size.width, size.height) );
    }
}

#endif // __CINTERFACE_H
