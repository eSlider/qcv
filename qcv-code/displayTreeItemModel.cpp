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
 * \file  displayTreeItemModel
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QCheckBox>

#include "displayTreeItemModel.h"
#include "displayTreeNode.h"
#include <stdio.h>

using namespace QCV;

CDisplayTreeItemModel::CDisplayTreeItemModel (  CDisplayOpNode *  f_rootNode_p,
                                                QObject *         f_parent_p )
        : QAbstractItemModel (      f_parent_p ),
          m_rootNode_p (                  NULL )
{
    m_rootNode_p = new CDisplayOpNode ( NULL, NULL );
    m_rootNode_p -> appendChild( f_rootNode_p );
}

CDisplayTreeItemModel::~CDisplayTreeItemModel ()
{
}

QVariant
CDisplayTreeItemModel::data ( const QModelIndex &f_index, 
                              int                f_role_i) const
{
    if ( ! f_index.isValid() )
        return QVariant();

    CDisplayTreeNodeAbstract *node_p = static_cast<CDisplayTreeNodeAbstract*>(f_index.internalPointer());


    if (f_role_i == Qt::CheckStateRole && f_index.column() == 2)
    {
        if ( node_p -> getNodeType() == CDisplayTreeNodeAbstract::NT_DISPLAY )
            return ( static_cast<CDisplayNode *>( node_p ) -> isVisible()?Qt::Checked:Qt::Unchecked );
        else
            return QVariant();
    }
 
    if (f_role_i != Qt::DisplayRole )
        return QVariant();
    
    if (node_p == m_rootNode_p)
        return QVariant(tr("Operators"));

    if ( not node_p ) return QVariant();

    if ( f_index.column() == 0 )
    {
        return QVariant(node_p -> getName().c_str());
    }

    if ( node_p -> getNodeType() == CDisplayTreeNodeAbstract::NT_CONTAINER )
        return QVariant();

    CDisplayNode *  dispNode_p = static_cast<CDisplayNode *> (node_p);

    if ( f_index.column() == 1 )
    {
        S2D<int> screen = dispNode_p -> getPosition();    
        char str[16];
        sprintf(str, "(%i,%i)", screen.x, screen.y );
        return QVariant(str);
    }
    
    if ( f_index.column() == 3 )
    {
        char str[16];
        int elemCount_i = dispNode_p -> getElementsCount();

        if ( elemCount_i < 0)
            return QVariant(tr("Invalid"));        

        sprintf(str, "%i", elemCount_i );
        return QVariant(str);
    }

    return QVariant(dispNode_p -> isVisible());
}

Qt::ItemFlags
CDisplayTreeItemModel::flags ( const QModelIndex &f_index) const
{
    if (!f_index.isValid())
        return 0;

    Qt::ItemFlags flags_e = Qt::ItemIsEnabled;

    CDisplayTreeNodeAbstract *node_p = static_cast<CDisplayTreeNodeAbstract*>(f_index.internalPointer());

    if (node_p  -> getNodeType() != CDisplayTreeNodeAbstract::NT_CONTAINER )
        flags_e |= Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    
    if (f_index.column() == 2)
        flags_e |= Qt::ItemIsUserCheckable;
    
    return flags_e;
}

QVariant
CDisplayTreeItemModel::headerData ( int              f_section_i, 
                                    Qt::Orientation  f_orientation_e,
                                    int              f_role_i ) const
{
    if (f_orientation_e == Qt::Horizontal && f_role_i == Qt::DisplayRole)
    {
        if (f_section_i == 0)
            return QVariant(tr("Operator"));
    
        if (f_section_i == 1)
            return QVariant(tr("Screen"));
        
        if (f_section_i == 2)
            return QVariant(tr("Show"));
        
        return QVariant(tr("Elements"));
    }
    
    return QVariant();
}

QModelIndex   
CDisplayTreeItemModel::index ( int f_row_i, 
                               int f_column_i,
                               const QModelIndex &f_parent ) const
{
    if ( ! hasIndex ( f_row_i, 
                      f_column_i, 
                      f_parent) )
        return QModelIndex();

    CDisplayTreeNodeAbstract *  parentNode_p;

    if ( !f_parent.isValid() )
        parentNode_p = m_rootNode_p;
    else
        parentNode_p = static_cast<CDisplayTreeNodeAbstract *>(f_parent.internalPointer());

    /// Create index with drawing list.
    if (parentNode_p)
    {
        CDisplayTreeNodeAbstract *  node_p = parentNode_p -> getChild ( f_row_i );

        return createIndex( f_row_i, f_column_i, node_p );
    }

    return QModelIndex();
}

QModelIndex   
CDisplayTreeItemModel::parent ( const QModelIndex &f_index ) const
{
    if ( !f_index.isValid() )
        return QModelIndex();

    CDisplayTreeNodeAbstract *  childNode_p =
        static_cast<CDisplayTreeNodeAbstract *>( f_index.internalPointer() );

    if ( not childNode_p )
        return QModelIndex();

    CDisplayTreeNodeAbstract *  parent_p = childNode_p -> getParent();
    
    // root node has no parent.
    if ( !parent_p || 
         parent_p == m_rootNode_p)
        return QModelIndex();

    return createIndex( parent_p->getIndexInParent(), 
                        0, 
                        parent_p );
}

int
CDisplayTreeItemModel::rowCount    ( const QModelIndex 
                                     &f_parent ) const
{
    CDisplayTreeNodeAbstract *  node_p;
    
    if (!f_parent.isValid())
        node_p = m_rootNode_p;
    else
        node_p = static_cast<CDisplayTreeNodeAbstract *>(f_parent.internalPointer());

    if (not node_p) return 0;

    return node_p->getChildCount();
}

int
CDisplayTreeItemModel::columnCount ( const QModelIndex 
                                     &/*f_parent*/ ) const
{
    return 4;
}

bool 
CDisplayTreeItemModel::setData ( const QModelIndex & f_index, 
                                 const QVariant    & f_value, 
                                 int                 f_role_i )
{
    switch( f_role_i )
    {
	case Qt::CheckStateRole:
        {
            //myEntry->SetEnabled( i_rValue.toBool() );
            CDisplayNode *  node_p =
                static_cast<CDisplayNode *>( f_index.internalPointer() );

            node_p ->  setVisibility( f_value.toBool() );

            emit dataChanged(f_index, f_index);
            return true;
        }

	case Qt::EditRole:
        {
        }
    }
    
    return false;
}
