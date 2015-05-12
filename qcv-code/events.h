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

#ifndef __EVENTS_H
#define __EVENTS_H

/**
 *******************************************************************************
 *
 * @file events.h
 *
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Definition of event data structs.
 *
 * The events or signals are generated in the CDisplay class and provided to
 * the operators via the CSimpleWindow class.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QtGui/QMouseEvent>
#include <QtGui/QRegion>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>

#include "standardTypes.h"

/* CONSTANTS */

/* TYPE DEFINITION */
namespace QCV
{
    class CRegionSelectedEvent
    {
    public:
        QMouseEvent * mouseEvent;
        QRegion       region;
    };
    
    class CMouseEvent
    {
    public:
        QMouseEvent *qtMouseEvent_p;
        S2D<float>   posInDisplay;
        S2D<int>     displayScreen;
        S2D<float>   posInScreen;
    };

    class CKeyEvent
    {
    public:
        QKeyEvent   *qtKeyEvent_p;
    };

    class CWheelEvent
    {
    public:
        QWheelEvent *qtWheelEvent_p;
    };

    class CTimerEvent
    {
    public:
        QTimerEvent *qtTimerEvent_p;
    };
}


#endif // __EVENTS_H
