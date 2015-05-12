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

#ifndef __DISPLAYTREEDLG_H
#define __DISPLAYTREEDLG_H

/*@@@**************************************************************************
 ** \file  displayTreeWidget
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>

/* CONSTANTS */

#include "displayTreeView.h"

/* PROTOTYPES */
class QGLWidget;

namespace QCV
{

/* PROTOTYPES */
    class CDisplayTreeItemModel;
    class CDisplayOpNode;

/* CLASS DEFINITION */

    class CDisplayTreeDlg: public QWidget //, private Ui::CDisplayTreeDlgDesign
    {
        Q_OBJECT
        
        public:
        CDisplayTreeDlg ( QWidget *           f_parent_p,
                          QGLWidget *         f_shareGLWidget_p,
                          CDisplayOpNode *    f_rootNode_p );

        ~CDisplayTreeDlg ();
        
    protected:
        
        //void startDrag( Qt::DropActions supportedActions );
        void closeEvent(QCloseEvent *f_event_p);
        virtual void focusInEvent ( QFocusEvent * f_event_p );
        virtual void mouseMoveEvent ( QMouseEvent * f_event_p );

    public:
        virtual void update ( );
        

    private:
        
        CDisplayTreeView *                m_qtvDisplays_p;
        CDisplayTreeItemModel  *          m_treeItemModel_p;
    };
}


#endif // __DISPLAYTREEDLG_H
