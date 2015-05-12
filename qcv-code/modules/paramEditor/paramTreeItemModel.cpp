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
 * \file  paramTreeItemModel
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QCheckBox>

#include "paramTreeItemModel.h"
#include "parameterSet.h"

using namespace QCV;

CParamTreeItemModel::CParamTreeItemModel (  CParameterSet *   f_rootNode_p,
                                            QObject *         f_parent_p )
        : QAbstractItemModel (      f_parent_p ),
          m_rootNode_p (                  NULL )
{
    m_rootNode_p = new CParameterSet ( NULL );
    m_rootNode_p -> setName ( "Application Parameters" );
    m_rootNode_p -> addSubset (  f_rootNode_p );    
}

CParamTreeItemModel::~CParamTreeItemModel ()
{   
    if ( m_rootNode_p )
    {
        m_rootNode_p -> orphanSubsets();
        //delete m_rootNode_p;
    }
}

QVariant
CParamTreeItemModel::data ( const QModelIndex &f_index, 
                            int                f_role_i) const
{
    if ( ! f_index.isValid() )
        return QVariant();

    CParameterSet *node_p = static_cast<CParameterSet*>(f_index.internalPointer());

    if ( !node_p || f_role_i != Qt::DisplayRole || f_index.column() != 0 )
        return QVariant();

    return QVariant(node_p -> getName().c_str());
}

Qt::ItemFlags
CParamTreeItemModel::flags ( const QModelIndex &f_index) const
{
    if (!f_index.isValid())
        return 0;

    Qt::ItemFlags flags_e = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return flags_e;
}

QVariant
CParamTreeItemModel::headerData ( int              f_section_i, 
                                  Qt::Orientation  f_orientation_e,
                                  int              f_role_i ) const
{
    if (f_orientation_e == Qt::Horizontal && f_role_i == Qt::DisplayRole)
    {
        if (f_section_i == 0)
            return QVariant("Parameter Category");    
    }
    
    return QVariant();
}

QModelIndex   
CParamTreeItemModel::index ( int f_row_i, 
                             int f_column_i,
                             const QModelIndex &f_parent ) const
{
    if ( ! hasIndex ( f_row_i, 
                      f_column_i, 
                      f_parent) )
        return QModelIndex();

    CParameterSet *  parentNode_p;

    if ( !f_parent.isValid() )
        parentNode_p = m_rootNode_p;
    else
        parentNode_p = static_cast<CParameterSet *>(f_parent.internalPointer());

    /// Create index with drawing list.
    if (parentNode_p)
    {
        CParameterSet *  node_p = parentNode_p -> getSubset ( f_row_i );

        return createIndex( f_row_i, f_column_i, node_p );
    }
    
    return QModelIndex();
}

QModelIndex   
CParamTreeItemModel::parent ( const QModelIndex &f_index ) const
{
    
    if ( !f_index.isValid() )
        return QModelIndex();

    CParameterSet *  childNode_p =
        static_cast<CParameterSet *>( f_index.internalPointer() );

    if ( not childNode_p )
        return QModelIndex();

    CParameterSet *  parent_p = childNode_p -> getParent();
    
    // root node has no parent.
    if ( !parent_p || 
         parent_p == m_rootNode_p)
        return QModelIndex();

    childNode_p = parent_p;
    parent_p    = childNode_p -> getParent();
    
    for (unsigned int i = 0; i < parent_p->getSubsetCount(); ++i)
        if ( parent_p -> getSubset ( i ) == childNode_p )
            return createIndex( i, 
                                0, 
                                childNode_p );

    return QModelIndex();
}

int
CParamTreeItemModel::rowCount    ( const QModelIndex 
                                   &f_parent ) const
{
    CParameterSet *  node_p;
    

    if (!f_parent.isValid())
        node_p = m_rootNode_p;
    else
        node_p = static_cast<CParameterSet *>(f_parent.internalPointer());

    if (not node_p) return 0;
    
    
    return node_p->getSubsetCount();
}

int
CParamTreeItemModel::columnCount ( const QModelIndex 
                                   &/*f_parent*/ ) const
{
    return 1;
}

bool 
CParamTreeItemModel::setData ( const QModelIndex & /*f_index*/, 
                               const QVariant    & /*f_value*/, 
                               int                 /*f_role_i*/ )
{    
    return false;
}
