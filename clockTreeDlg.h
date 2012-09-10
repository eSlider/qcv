#ifndef __CLOCKTREEDLG_H
#define __CLOCKTREEDLG_H

/*@@@**************************************************************************
** \file  clockTreeWidget
* \author Hernan Badino
* \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QDialog>

/* CONSTANTS */

#include "clockTreeView.h"

/* PROTOTYPES */
class QGLWidget;
class QCheckBox;

namespace QCV
{

    /* PROTOTYPES */
    class CClockTreeItemModel;
    class CClockOpNode;

    /* CLASS DEFINITION */

    class CClockTreeDlg: public QDialog //, private Ui::CClockTreeDlgDesign
    {
        Q_OBJECT
        
    public:
        CClockTreeDlg ( QWidget *           f_parent_p,
                        CClockOpNode *    f_rootNode_p );

        ~CClockTreeDlg ();
        
    public slots:
        void updateTimes();
        void printTimes();

    protected:
        
        //void startDrag( Qt::DropActions supportedActions );

    private:
        
        /// Tree View
        CClockTreeView *                m_qtvClocks_p;

        /// Tree Model
        CClockTreeItemModel  *          m_treeItemModel_p;

        /// QCheck Box for updating times.
        QCheckBox *                     m_qcbUpdate_p;
    };
}


#endif // __CLOCKTREEDLG_H
