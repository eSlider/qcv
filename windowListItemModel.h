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

