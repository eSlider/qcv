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

