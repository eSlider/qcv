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


#ifndef __PARAMTREEITEMMODEL_H
#define __PARAMTREEITEMMODEL_H

/**
 *******************************************************************************
 *
 * @file paramTreeItemModel.h
 *
 * \class CParamTreeItemModel
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Concrete interface class derived form QAbstractItemModel for handling
 * trees of parameter sets.
 *
 ******************************************************************************/

/* INCLUDES */

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

/* CONSTANTS */


namespace QCV
{

/* PROTOTYPES */
    class CParameterSet;

    class CParamTreeItemModel: public  QAbstractItemModel
    {
        Q_OBJECT
        
        public:
        CParamTreeItemModel ( CParameterSet *  f_rootNode_p,
                              QObject *        f_parent_p = 0 );

        ~CParamTreeItemModel ();
        
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

    protected:

    private:
        
        CParameterSet *          m_rootNode_p;
    };
}


#endif // __PARAMTREEITEMMODEL_H
