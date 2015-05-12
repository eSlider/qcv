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

/*@@@**************************************************************************
 * \file  displayTreeWidget
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QGridLayout>
#include <QPushButton>
#include <QSettings>

#include "displayTreeDlg.h"
#include "displayTreeItemModel.h"
#include "displayTreeNode.h"


using namespace QCV;

CDisplayTreeDlg::CDisplayTreeDlg  ( QWidget *           f_parent_p,
                                    QGLWidget *         f_shareGLWidget_p,
                                    CDisplayOpNode *    f_rootNode_p )
        : QWidget (                      f_parent_p ),
          m_treeItemModel_p (                  NULL )
{
    setMouseTracking(true);
    

    setWindowTitle ( tr("Drawing Lists") );
    setObjectName  ( windowTitle() );

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/symbols/pencil.png")), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    /// Tree View
    m_qtvDisplays_p = new CDisplayTreeView( this, f_shareGLWidget_p );
    m_qtvDisplays_p->setObjectName(QString::fromUtf8("Display Tree View Object"));

    /// Tree View Model
    m_treeItemModel_p = new CDisplayTreeItemModel ( f_rootNode_p );
    m_qtvDisplays_p -> setModel ( m_treeItemModel_p );

    
    /// Expand all button.
    QPushButton *pushButton1_p = new QPushButton(this);
    pushButton1_p->setObjectName(QString::fromUtf8("ExpandAllPushButton"));
    pushButton1_p->setText(tr("Expand All"));
    connect(pushButton1_p, SIGNAL(clicked(bool)), m_qtvDisplays_p, SLOT(expandAll()));

    /// Collapse all button.
    QPushButton *pushButton2_p = new QPushButton(this);
    pushButton2_p->setObjectName(QString::fromUtf8("CollapseAllPushButton"));
    pushButton2_p->setText(tr("Collapse All"));
    connect(pushButton2_p, SIGNAL(clicked(bool)), m_qtvDisplays_p, SLOT(collapseAll()));   
    
    /// Layout
    QGridLayout * gridLayout_p = new QGridLayout(this);

    gridLayout_p->setObjectName(QString::fromUtf8("GridLayout"));

    gridLayout_p->addWidget(pushButton1_p, 0, 0, 1, 1);
    gridLayout_p->addWidget(pushButton2_p, 0, 1, 1, 1);
    gridLayout_p->addWidget(m_qtvDisplays_p, 1, 0, 1, 2);
    
    m_qtvDisplays_p -> loadParameters ( );
    m_qtvDisplays_p -> expandAll ( );

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(m_qtvDisplays_p);

    QSettings settings;
    QString name = QString("CDisplayTreeDlg/geometry/") + (f_parent_p?f_parent_p->objectName():QString("default"));
    restoreGeometry(settings.value(name).toByteArray());

}

void CDisplayTreeDlg::closeEvent(QCloseEvent *f_event_p)
{
    QSettings settings;
    QString name = QString("CDisplayTreeDlg/geometry/") + (parent()?parent()->objectName():QString("default"));
    settings.setValue(name, saveGeometry());

    QWidget::closeEvent(f_event_p);
}

void CDisplayTreeDlg::focusInEvent(QFocusEvent * /* f_event_p */)
{
    m_qtvDisplays_p -> updateTree();
}

void CDisplayTreeDlg::update()
{
    m_qtvDisplays_p -> updateTree();
    QWidget::update();
}

CDisplayTreeDlg::~CDisplayTreeDlg ()
{
    if ( m_treeItemModel_p )
        delete m_treeItemModel_p;

    if (m_qtvDisplays_p)
      delete m_qtvDisplays_p;

    m_treeItemModel_p = NULL;
}

void
CDisplayTreeDlg::mouseMoveEvent ( QMouseEvent *  /*f_event_p*/ )
{
    activateWindow();
}

