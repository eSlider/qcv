#ifndef __CLOCKTREEVIEW_H
#define __CLOCKTREEVIEW_H

/*@@@**************************************************************************
** \file  clockTreeView
* \author Hernan Badino
* \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QTreeView>

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
    class CClockTreeView : public QTreeView
    {
        Q_OBJECT
        
        public:
        CClockTreeView ( QWidget *    f_parent_p = 0 );
        
        virtual ~CClockTreeView ();

        void     saveParameters();
        void     loadParameters();

        /// Public slots.
    public slots:
        
        /// Inherited virtual methods
    protected:

        /// Help methods.
    protected:
        
        /// Members
    protected:
    };
}


#endif // __CLOCKTREEVIEW_H

