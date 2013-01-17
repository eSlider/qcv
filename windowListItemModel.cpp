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
* \file  windowListItemModel
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>

#include "windowListItemModel.h"
#include <stdio.h>

using namespace QCV;

CWindowListModel::CWindowListModel ( std::vector<QWidget *> *  f_widgets_p,
                                     QObject *                 f_parent_p )
        : QAbstractListModel ( f_parent_p ),
          m_widgets_p (       f_widgets_p )
{
}

CWindowListModel::~CWindowListModel()
{
}
        
QVariant
CWindowListModel::data ( const QModelIndex &f_index, 
                         int                f_role_i) const
{
    unsigned int idx_ui = f_index.row();
    
    if ( ! f_index.isValid() ||
         (unsigned int) idx_ui >= m_widgets_p -> size() )
        return QVariant();

    if (f_role_i == Qt::CheckStateRole && f_index.column() == 0)
    {
        return (*m_widgets_p)[idx_ui] -> isVisible();
    }

    if ( f_role_i != Qt::DisplayRole )
        return QVariant();

    QString str = (*m_widgets_p)[idx_ui] -> objectName();

    return (*m_widgets_p)[idx_ui] -> objectName();
}

Qt::ItemFlags
CWindowListModel::flags ( const QModelIndex &f_index) const
{
    if (!f_index.isValid())
        return 0;

    Qt::ItemFlags flags_e = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;

    if (f_index.column() == 0)
        flags_e |= Qt::ItemIsUserCheckable;
    
    return flags_e;
}

int
CWindowListModel::rowCount    ( const QModelIndex 
                                & /* f_parent */ ) const
{
    return m_widgets_p -> size();
}

bool 
CWindowListModel::setData ( const QModelIndex & f_index, 
                            const QVariant    & f_value, 
                            int                 f_role_i )
{
    unsigned int idx_ui = f_index.row();
    
    switch( f_role_i )
    {
	case Qt::CheckStateRole:
        {
            printf ("f_value.toBool() %i (*m_widgets_p)[idx_ui] -> isVisible() %i\n", 
                    f_value.toBool(),
                    (*m_widgets_p)[idx_ui] -> isVisible());
            
            /// Check if value has really changed.
            //if ((*m_widgets_p)[idx_ui] -> isVisible() != f_value.toBool())
            //    return false;
            
            emit dataChanged(f_index, f_index);
            return true;
        }

	case Qt::EditRole:
        {
        }

        default:
        {
        }   
    }
    
    return false;
}
