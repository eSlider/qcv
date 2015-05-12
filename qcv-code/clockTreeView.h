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

