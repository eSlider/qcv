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

#ifndef __EVENTHANDLERBASE_H
#define __EVENTHANDLERBASE_H

#include <QtCore/QObject>

/*@@@**************************************************************************
 ** \file  eventhandler
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

namespace QCV
{    
    /// PROTOTYPES
    class CKeyEvent;
    class CMouseEvent;
    class CMouseEvent;
    class CMouseEvent;
    class CWheelEvent;
    class CTimerEvent;
    class CRegionSelectedEvent;
    class CDisplayWidget;

    class CEventHandlerBase : public QObject
    {
        Q_OBJECT
 
        public:

        CEventHandlerBase (  CDisplayWidget * f_disp_p, 
                             QObject * f_parent_p = NULL );
    
        virtual ~CEventHandlerBase ();
        
        /// Protected methods
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
        /// Associated display widget
        CDisplayWidget *  m_dispWidget_p;
    };
} /// QCV

#endif // __EVENTHANDLERBASE_H
