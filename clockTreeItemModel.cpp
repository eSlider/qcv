/*@@@**************************************************************************
 * \file  clockTreeItemModel
 * \date  Tue Apr 14 17:04:52 GMT 2009
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QCheckBox>

#include "clockTreeItemModel.h"
#include "clockTreeNode.h"

using namespace QCV;

CClockTreeItemModel::CClockTreeItemModel (  CClockOpNode *  f_rootNode_p,
                                            QObject *       f_parent_p )
        : QAbstractItemModel (      f_parent_p ),
          m_rootNode_p (          f_rootNode_p )
{
    //m_rootNode_p = new CClockOpNode ( NULL, NULL );
    
    //m_rootNode_p -> appendChild( f_rootNode_p );
    //printf("Created RootNode is %p child %i, added child is %p (%s)\n", m_rootNode_p, m_rootNode_p->getChildCount(),
    //       f_rootNode_p, f_rootNode_p?f_rootNode_p->getName().c_str():"rootNode is null");
}

CClockTreeItemModel::~CClockTreeItemModel ()
{
}

QVariant
CClockTreeItemModel::data ( const QModelIndex &f_index, 
                            int                f_role_i) const
{
    if ( ! f_index.isValid() )
        return QVariant();

    CClockTreeNodeAbstract *node_p = static_cast<CClockTreeNodeAbstract*>(f_index.internalPointer());

    if ( not node_p ) 
        return QVariant();

    if ( node_p == m_rootNode_p )
        return QVariant(tr("Containers"));

    if (f_role_i == Qt::DisplayRole)
    {
        if ( f_index.column() == 0 )
        {
            return QVariant(node_p -> getName().c_str());
        }

        if ( node_p -> getNodeType() == CClockTreeNodeAbstract::NT_CONTAINER )
            return QVariant();

        if ( f_index.column() == 1 )
        {
            QString str;
            str.setNum ( (int) ( ( (CClockNode *) node_p ) -> getCount() ) );
            return ( str );
        }

        if ( f_index.column() == 2 )
        {
            QString str;
            str.setNum( (double) ( ( (CClockNode *) node_p ) -> getTotalTime() ) );
            return ( str );
        }

        if ( f_index.column() == 3 )
        {
            QString str;
            str.setNum( (double) ( ( (CClockNode *) node_p ) -> getLoopTime() ) );
            return ( str );
        }
    }

    return QVariant();
}

Qt::ItemFlags
CClockTreeItemModel::flags ( const QModelIndex &f_index) const
{
    if (!f_index.isValid())
        return 0;

    Qt::ItemFlags flags_e = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    //CClockTreeNodeAbstract *node_p = static_cast<CClockTreeNodeAbstract*>(f_index.internalPointer());

    //if (node_p  -> getNodeType() != CClockTreeNodeAbstract::NT_CONTAINER )
    //    flags_e |= Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    
    //if (f_index.column() == 2)
    //    flags_e |= Qt::ItemIsUserCheckable;
    
    return flags_e;
}

QVariant
CClockTreeItemModel::headerData ( int              f_section_i, 
                                  Qt::Orientation  f_orientation_e,
                                  int              f_role_i ) const
{
    if (f_orientation_e == Qt::Horizontal && f_role_i == Qt::DisplayRole)
    {
        if (f_section_i == 0)
            return QVariant(tr("Clocks"));
    
        if (f_section_i == 1)
            return QVariant(tr("Loops"));
        
        if (f_section_i == 2)
            return QVariant(tr("Time"));
        
        return QVariant(tr("Time/Loop"));
    }
    
    return QVariant();
}

QModelIndex   
CClockTreeItemModel::index ( int f_row_i, 
                             int f_column_i,
                             const QModelIndex &f_parent ) const
{
    if ( ! hasIndex ( f_row_i, 
                      f_column_i, 
                      f_parent) )
        return QModelIndex();

    CClockTreeNodeAbstract *  parentNode_p;

    if ( !f_parent.isValid() )
        parentNode_p = m_rootNode_p;
    else
        parentNode_p = static_cast<CClockTreeNodeAbstract *>(f_parent.internalPointer());

    /// Create index with drawing list.
    if (parentNode_p)
    {
        CClockTreeNodeAbstract *  node_p = parentNode_p -> getChild ( f_row_i );

        return createIndex( f_row_i, f_column_i, node_p );
    }
    
    return QModelIndex();
}

QModelIndex   
CClockTreeItemModel::parent ( const QModelIndex &f_index ) const
{
    if ( !f_index.isValid() )
        return QModelIndex();

    CClockTreeNodeAbstract *  childNode_p =
        static_cast<CClockTreeNodeAbstract *>( f_index.internalPointer() );

    if ( not childNode_p )
        return QModelIndex();

    CClockTreeNodeAbstract *  parent_p = childNode_p -> getParent();
    
    // root node has no parent.
    if ( !parent_p || 
         parent_p == m_rootNode_p)
        return QModelIndex();

    return createIndex( parent_p->getIndexInParent(), 
                        0, 
                        parent_p );
}

int
CClockTreeItemModel::rowCount    ( const QModelIndex &f_parent ) const
{
    CClockTreeNodeAbstract *  node_p;
    
    if (!f_parent.isValid())
        node_p = m_rootNode_p;
    else
        node_p = static_cast<CClockTreeNodeAbstract *>(f_parent.internalPointer());

    if (not node_p) return 0;
    
    return node_p->getChildCount();
}

int
CClockTreeItemModel::columnCount ( const QModelIndex 
                                   &/*f_parent*/ ) const
{
    return 4;
}

bool 
CClockTreeItemModel::setData ( const QModelIndex & /*f_index*/, 
                               const QVariant    & /*f_value*/, 
                               int                 f_role_i )
{
    switch( f_role_i )
    {
	case Qt::CheckStateRole:
        {
            return false;
        }

	case Qt::EditRole:
        {
        }
    }
    
    return false;
}

void
CClockTreeItemModel::resetTimes()
{
    m_rootNode_p -> resetClock();    
}

void
CClockTreeItemModel::printTimes()
{
    m_rootNode_p -> printClock();    
}
