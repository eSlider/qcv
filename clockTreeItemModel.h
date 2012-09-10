#ifndef __CLOCKTREEITEMMODEL_H
#define __CLOCKTREEITEMMODEL_H

/*@@@**************************************************************************
** \file  clockTreeItemModel
* \author Hernan Badino
* \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

/* CONSTANTS */


namespace QCV
{

    /* PROTOTYPES */
    class CClockOpNode;

    class CClockTreeItemModel: public  QAbstractItemModel
    {
        Q_OBJECT
        
        public:
        CClockTreeItemModel ( CClockOpNode *  f_rootNode_p,
                              QObject *       f_parent_p = 0 );

        ~CClockTreeItemModel ();
        
        QVariant      data ( const QModelIndex &f_index_i, 
                             int                f_role_i) const;
        Qt::ItemFlags flags ( const QModelIndex &f_index) const;
        
        QVariant      headerData ( int              f_section_i, 
                                   Qt::Orientation  f_orientation_e,
                                   int              f_role_i = Qt::DisplayRole) const;
        
        QModelIndex   index ( int f_row_i, 
                              int f_column_i,
                              const QModelIndex &f_parent = QModelIndex()) const;
        
        QModelIndex   parent ( const QModelIndex &f_index_i) const;
        
        int           rowCount    ( const QModelIndex 
                                    &f_parent = QModelIndex() ) const;
        
        int           columnCount ( const QModelIndex 
                                    &f_parent = QModelIndex() ) const;        
 
        bool          setData ( const QModelIndex &f_index, 
                                const QVariant    &f_value, 
                                int                f_role_i );
    /// Reset
    public slots:
        void          resetTimes();
        void          printTimes();

    protected:

    private:
        
        CClockOpNode *          m_rootNode_p;
    };
}


#endif // __CLOCKTREEITEMMODEL_H

