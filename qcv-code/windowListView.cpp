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
* \file  windowListView
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QSettings>

#include "windowListView.h"
#include "windowListItemModel.h"
#include <stdio.h>

using namespace QCV;

CWindowListView::CWindowListView ( QWidget *                m_parent_p )
        : QListView (     m_parent_p )
{
    //initializeWindows();
    setModel ( (QAbstractItemModel *)
               new CWindowListModel ( &m_window_v ) );

}

CWindowListView::~CWindowListView()
{
}

void
CWindowListView::dataChanged( const QModelIndex & f_topLeft, 
                              const QModelIndex & /*f_bottomRight*/ )
{
    if (f_topLeft.row() >= (int)m_window_v.size())
    {
        printf("Unexpected error: the index is larger than the amount of items in the vector\n");
    }
    
    QSettings qSettings;

    QString name_str = m_window_v[f_topLeft.row()] -> objectName();

    bool visible_b = m_window_v[f_topLeft.row()] -> isHidden();

    m_window_v[f_topLeft.row()] -> setVisible ( visible_b );

    if ( visible_b )
    {
        m_window_v[f_topLeft.row()] -> restoreGeometry ( 
                qSettings.value(name_str + QString("/geometry")).toByteArray() );
        
    }
    else
    { 
        qSettings.setValue(name_str + QString("/geometry"), 
                           m_window_v[f_topLeft.row()] -> saveGeometry());
    }
    
    qSettings.setValue( name_str + QString("/visible"), 
                        visible_b );       
}

void 
CWindowListView::insertWindow( QWidget * f_widget_p, bool f_showNow_b )
{
    if ( !f_widget_p ) return;

    QString name_str = f_widget_p -> objectName();

    /// Check if already inserted.
    for (unsigned int i = 0; i < m_window_v.size(); ++i)
    {
        if ( m_window_v[i] == f_widget_p )
            return;
    }    
    
    QSettings qSettings;
    f_widget_p -> setVisible ( false );
    
    f_widget_p -> restoreGeometry ( 
            qSettings.value(name_str + QString("/geometry")).toByteArray() );

    if ( f_showNow_b )
    {
        bool visible_b = qSettings.value(name_str + QString("/visible"), true).toBool();
        f_widget_p -> setVisible ( visible_b );
    }
    
    m_window_v.push_back ( f_widget_p );
}

void 
CWindowListView::showWindows ()
{
    for (unsigned int i = 0; i < m_window_v.size(); ++i)
    {
        QString name_str = m_window_v[i] -> objectName();

        QSettings qSettings;        

        restoreGeometry ( 
            qSettings.value(name_str + QString("/geometry")).toByteArray() );

        bool visible_b = qSettings.value(name_str + QString("/visible"), true).toBool();
        m_window_v[i] -> setVisible ( visible_b );    
    }
}

void 
CWindowListView::saveWindowsGeometry () const
{
    QSettings qSettings;

    for (unsigned int i = 0; i < m_window_v.size(); ++i)
    {
        QString name_str = m_window_v[i] -> objectName();
        
        qSettings.setValue( name_str + QString("/geometry"), m_window_v[i] -> saveGeometry());
        qSettings.setValue( name_str + QString("/visible"), m_window_v[i] -> isVisible() );
    }
}

void 
CWindowListView::clear ()
{
    m_window_v.clear();    
}

void
CWindowListView::showAllWindows()
{
    for (unsigned int i = 0; i < m_window_v.size(); ++i)
    {
        m_window_v[i]->show();
    }
}

void 
CWindowListView::hideAllWindows()
{
    for (unsigned int i = 0; i < m_window_v.size(); ++i)
    {
        m_window_v[i]->hide();
    }
}
    

