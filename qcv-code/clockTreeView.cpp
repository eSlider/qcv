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


/*@@@**************************************************************************
 * \file  clockTreeView
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QtGui>
#include <QMimeData>
#include "clockTreeView.h"
#include "clockTreeNode.h"

using namespace QCV;

CClockTreeView::CClockTreeView( QWidget *    f_parent_p )
        : QTreeView (                  f_parent_p )
{
    setSelectionMode (    QAbstractItemView::ExtendedSelection );
    setSelectionBehavior( QAbstractItemView::SelectRows );
    setDragEnabled (         false );
    setAcceptDrops (         false );
    setDropIndicatorShown (  false );
    //setMouseTracking (       true );
}

void 
CClockTreeView::loadParameters()
{
    QSettings qSettings;
 
    setColumnWidth ( 0,
                     qSettings.value(QString("ClockTreeView/Column0_Width"), 
                                     260 ).toInt() );
    setColumnWidth ( 1,
                     qSettings.value(QString("ClockTreeView/Column1_Width"), 
                                     83 ).toInt());
    setColumnWidth ( 2,
                     qSettings.value(QString("ClockTreeView/Column2_Width"), 
                                     75 ).toInt() );
}

CClockTreeView::~CClockTreeView ()
{    
    saveParameters();
}

void 
CClockTreeView::saveParameters()
{
    QSettings qSettings;

    qSettings.setValue(QString("ClockTreeView/Column0_Width"), columnWidth(0));
    qSettings.setValue(QString("ClockTreeView/Column1_Width"), columnWidth(1));
    qSettings.setValue(QString("ClockTreeView/Column2_Width"), columnWidth(2));
}

