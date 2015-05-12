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

#ifndef __EVENTHANDLER_H
#define __EVENTHANDLER_H

#include <QtGui/QWidget>

/*@@@**************************************************************************
 ** \file  eventhandler
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

#include "eventHandlerBase.h"

namespace QCV
{
    class CEventHandler: public CEventHandlerBase
    {
        Q_OBJECT
 
        public:
        CEventHandler (  CDisplayWidget * f_disp_p, 
                         QObject * f_parent_p = NULL );
    
        ~CEventHandler () 
        {
        }

    public:
        /// Set callback functions pointers.
        bool  setKeyPressedEventCBF(       bool (*f_keyEv_p)           (CKeyEvent *   ));
        bool  setMousePressedEventCBF (    bool (*f_mousePressedEv_p)  (CMouseEvent * ));
        bool  setMouseReleasedEventCBF (   bool (*f_mouseReleasedEv_p) (CMouseEvent * ));
        bool  setMouseMovedEventCBF (      bool (*f_mouseMovedEv_p)    (CMouseEvent * ));
        bool  setWheelTurnedEventCBF (     bool (*f_wheelTurnedEv_p)   (CWheelEvent * ));
        bool  setTimerEventCBF (           bool (*f_timerEv_p)         (CTimerEvent * ),
                                           const int f_ms_s = 1 );    
        /// Not implemented et
        bool  setRegionSelectedEventCBF (  bool (*f_regionSelectedEv_p)(CRegionSelectedEvent * ));

    /// Protected slots
    protected slots:
    
    
        /// Key Pressed event.
        virtual void keyPressed     ( CKeyEvent *    f_event_p );
        virtual void mousePressed   ( CMouseEvent *  f_event_p );
        virtual void mouseReleased  ( CMouseEvent *  f_event_p );
        virtual void mouseMoved     ( CMouseEvent *  f_event_p );
        virtual void wheelTurned    ( CWheelEvent *  f_event_p );
        virtual void regionSelected ( CRegionSelectedEvent * 
                                      f_event_p );
        virtual void timerEvent     ( QTimerEvent  *  f_event_p );
        
    /// Members
    protected:
        /// Timer Id
        int               m_timerId_i;

        /// Call back function pointers.
        bool              (*m_keyEv_p)            (CKeyEvent *   );
        bool              (*m_mousePressedEv_p)   (CMouseEvent * );
        bool              (*m_mouseReleasedEv_p)  (CMouseEvent * );
        bool              (*m_mouseMovedEv_p)     (CMouseEvent * );
        bool              (*m_wheelTurnedEv_p)    (CWheelEvent * );
        bool              (*m_timerEv_p)          (CTimerEvent * );    
        bool              (*m_regionSelectedEv_p) (CRegionSelectedEvent *   );    

    };
} /// QCV

#endif
