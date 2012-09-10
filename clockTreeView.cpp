/*@@@**************************************************************************
 * \file  clockTreeView
 * \date  Tue May 19 16:53:20 EDT 2009
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
                                     20 ).toInt() );
    setColumnWidth ( 1,
                     qSettings.value(QString("ClockTreeView/Column1_Width"), 
                                     20 ).toInt());
    setColumnWidth ( 2,
                     qSettings.value(QString("ClockTreeView/Column2_Width"), 
                                     20 ).toInt() );
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

