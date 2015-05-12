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

#ifndef __WINDOWLISTITEMMODEL_H
#define __WINDOWLISTITEMMODEL_H

/**
 *******************************************************************************
 *
 * @file windowListItemModel.h
 *
 * \class CWindowListItemModel
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Concrete list model for handle windows.
 *
 * The class derives from QAbstractListModel to provide a concrete version
 * of a window list model. Instantiation of this class is used by CWindowListView
 * to handle visibility, position and size of windows.
 *
 *******************************************************************************/

/* INCLUDES */

#include <vector>
#include <map>
#include <QAbstractListModel>

/* CONSTANTS */

namespace QCV
{
    
    /* PROTOTYPES */
    class CWindowListModel: public QAbstractListModel
    {
        Q_OBJECT
        
        public:
        
        CWindowListModel (  std::vector<QWidget *> *  f_widgets_p,
                            QObject *                 f_parent_p = 0 );
        
        ~CWindowListModel ();
        
        QVariant      data ( const QModelIndex &f_index_i, 
                             int                f_role_i) const;
        
        Qt::ItemFlags flags ( const QModelIndex &f_index) const;
        
        int           rowCount    ( const QModelIndex 
                                    &f_parent = QModelIndex() ) const;
        
        bool          setData ( const QModelIndex  &f_index, 
                                const QVariant     &f_value, 
                                int                 f_role_i );
    private:
        
        /// Vector of widgets.
        std::vector<QWidget *> *              m_widgets_p;
    };
}


#endif // __WINDOWLISTITEMMODEL_H

