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

#include "eventHandlerBase.h"
#include "events.h"
#include "displayWidget.h"

#include <stdio.h>
#include <QEvent>

#include "displayWidget.h"
#include "display.h"

using namespace QCV;


CEventHandlerBase::CEventHandlerBase( CDisplayWidget *  f_disp_p, 
                                      QObject *         f_parent_p ):
    QObject (                  f_parent_p ),
    m_dispWidget_p (             f_disp_p )
{
    assert(f_disp_p);
    
    CDisplay *disp_p = m_dispWidget_p -> getDisplay();
    
    // Connections Display <-> This
    QObject::connect( disp_p, SIGNAL(mousePressed ( CMouseEvent *  )), 
                      this,   SLOT(  mousePressed ( CMouseEvent * )));
    
    QObject::connect( disp_p, SIGNAL(mouseReleased ( CMouseEvent * )), 
                      this,   SLOT(   mouseReleased ( CMouseEvent * )));

    QObject::connect( disp_p, SIGNAL(mouseMoved ( CMouseEvent * )), 
                      this,   SLOT(  mouseMoved ( CMouseEvent * )));

    QObject::connect( disp_p, SIGNAL(wheelTurned ( CWheelEvent * )), 
                      this,   SLOT(  wheelTurned ( CWheelEvent * )));

    QObject::connect( disp_p, SIGNAL(keyPressed ( CKeyEvent * )), 
                      this,   SLOT(  keyPressed ( CKeyEvent * )));
    
    QObject::connect( disp_p, SIGNAL(regionSelected ( CRegionSelectedEvent * )), 
                      this,   SLOT(  regionSelected ( CRegionSelectedEvent * )));
}

CEventHandlerBase::~CEventHandlerBase( )
{
}

void 
CEventHandlerBase::keyPressed     ( CKeyEvent *    /*f_event_p*/ )
{
}

void 
CEventHandlerBase::mousePressed   ( CMouseEvent *  /* f_event_p */ )
{
}

void 
CEventHandlerBase::mouseReleased  ( CMouseEvent *  /* f_event_p */ )
{
}


void 
CEventHandlerBase::mouseMoved     ( CMouseEvent *  /* f_event_p */ )
{
}

void 
CEventHandlerBase::wheelTurned    ( CWheelEvent *  /* f_event_p */ )
{
}

void 
CEventHandlerBase::timerEvent    ( QTimerEvent *  /* f_event_p */ )
{
}

void 
CEventHandlerBase::regionSelected ( CRegionSelectedEvent * /* f_event_p */ )
{
}

