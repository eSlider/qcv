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

#include "eventHandler.h"
#include "events.h"

#include <stdio.h>
#include <QEvent>

using namespace QCV;


CEventHandler::CEventHandler( CDisplayWidget *  f_disp_p, 
                              QObject *         f_parent_p ):
    CEventHandlerBase ( f_disp_p, f_parent_p ),
    m_timerId_i (                          0 ),
    m_keyEv_p (                         NULL ),
    m_mousePressedEv_p (                NULL ),
    m_mouseReleasedEv_p (               NULL ),
    m_mouseMovedEv_p (                  NULL ),
    m_wheelTurnedEv_p (                 NULL ),
    m_timerEv_p (                       NULL ),
    m_regionSelectedEv_p (              NULL )
{
}

void 
CEventHandler::keyPressed     ( CKeyEvent *    f_event_p )
{
    if ( m_keyEv_p )
        (*m_keyEv_p)(f_event_p);
}

void 
CEventHandler::mousePressed   ( CMouseEvent *  f_event_p )
{
    if ( m_mousePressedEv_p )
        (*m_mousePressedEv_p)(f_event_p);
}

void 
CEventHandler::mouseReleased  ( CMouseEvent *  f_event_p )
{
    if ( m_mouseReleasedEv_p )
        (*m_mouseReleasedEv_p)(f_event_p);
}


void 
CEventHandler::mouseMoved     ( CMouseEvent *  f_event_p )
{
    if ( m_mouseMovedEv_p )
        (*m_mouseMovedEv_p)(f_event_p);
}

void 
CEventHandler::wheelTurned    ( CWheelEvent *  f_event_p )
{
    if ( m_wheelTurnedEv_p )
        (*m_wheelTurnedEv_p)(f_event_p);
}

void 
CEventHandler::timerEvent    ( QTimerEvent *  f_event_p )
{
    static CTimerEvent s_event;
    s_event.qtTimerEvent_p = f_event_p;
    
    if ( m_timerEv_p )
        (*m_timerEv_p)(&s_event);
}

void 
CEventHandler::regionSelected ( CRegionSelectedEvent * f_event_p )
{
    if ( m_regionSelectedEv_p )
        (*m_regionSelectedEv_p)(f_event_p);
}

bool
CEventHandler::setKeyPressedEventCBF(       bool (*f_keyEv_p)           (CKeyEvent *   ))
{
    m_keyEv_p = f_keyEv_p;
    
    return true;
}

bool
CEventHandler::setMousePressedEventCBF (    bool (*f_mousePressedEv_p)  (CMouseEvent * ))
{
    m_mousePressedEv_p = f_mousePressedEv_p;
    
    return true;
}

bool
CEventHandler::setMouseReleasedEventCBF (   bool (*f_mouseReleasedEv_p) (CMouseEvent * ))
{
    m_mouseReleasedEv_p = f_mouseReleasedEv_p;
    
    return true;
}

bool
CEventHandler::setMouseMovedEventCBF (      bool (*f_mouseMovedEv_p)    (CMouseEvent * ))
{
    m_mouseMovedEv_p = f_mouseMovedEv_p;
    
    return true;
}

bool
CEventHandler::setWheelTurnedEventCBF (     bool (*f_wheelTurnedEv_p)   (CWheelEvent * ))
{
    m_wheelTurnedEv_p = f_wheelTurnedEv_p;
    
    return true;
}

bool
CEventHandler::setTimerEventCBF (  bool (*f_timerEv_p)(CTimerEvent * ),
                                   const int f_ms_i )
{
    m_timerEv_p = f_timerEv_p;

    if ( m_timerId_i != 0 )
        killTimer ( m_timerId_i );
    
    if ( m_timerEv_p != NULL && f_ms_i > 0 )
        m_timerId_i = startTimer ( f_ms_i );
    
    return true;
}

bool
CEventHandler::setRegionSelectedEventCBF (  bool (*f_regionSelectedEv_p)(CRegionSelectedEvent * ))
{
    m_regionSelectedEv_p = f_regionSelectedEv_p;
    
    return true;
}

