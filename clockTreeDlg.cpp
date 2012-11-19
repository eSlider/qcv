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
 * \file  clockTreeWidget
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QGridLayout>
//#include <QGLWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>

#include "clockTreeDlg.h"
#include "clockTreeItemModel.h"
#include "clockTreeNode.h"


using namespace QCV;

CClockTreeDlg::CClockTreeDlg  ( QWidget *         f_parent_p,
                                CClockOpNode *    f_rootNode_p )
        : QWidget (                      f_parent_p ),
          m_treeItemModel_p (                  NULL ),
          m_qcbUpdate_p (                      NULL )
{
    setWindowTitle ( tr("Clocks") );
    setObjectName  ( windowTitle() );

    QIcon icon;
    icon.addPixmap(QPixmap(QString::fromUtf8(":/symbols/player_time.png")), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);

    /// Tree View
    m_qtvClocks_p = new CClockTreeView( this );
    m_qtvClocks_p->setObjectName(QString::fromUtf8("Clock Tree View Object"));

    /// Tree View Model
    m_treeItemModel_p = new CClockTreeItemModel ( f_rootNode_p );
    m_qtvClocks_p -> setModel ( m_treeItemModel_p );

    
    /// Expand all button.
    QPushButton *pushButton1_p = new QPushButton(this);
    pushButton1_p->setObjectName(QString::fromUtf8("ExpandAllPushButton"));
    pushButton1_p->setText(tr("Expand All"));
    connect(pushButton1_p, SIGNAL(clicked(bool)), m_qtvClocks_p, SLOT(expandAll()));

    /// Collapse all button.
    QPushButton *pushButton2_p = new QPushButton(this);
    pushButton2_p->setObjectName(QString::fromUtf8("CollapseAllPushButton"));
    pushButton2_p->setText(tr("Collapse All"));
    connect(pushButton2_p, SIGNAL(clicked(bool)), m_qtvClocks_p, SLOT(collapseAll()));   
    
    /// Reset Times.
    QPushButton *pushButton3_p = new QPushButton(this);
    pushButton3_p->setObjectName(QString::fromUtf8("ResetTimesPushButton"));
    pushButton3_p->setText(tr("Reset"));
    connect(pushButton3_p, SIGNAL(clicked(bool)), m_treeItemModel_p, SLOT(resetTimes()));
    connect(pushButton3_p, SIGNAL(clicked(bool)), this, SLOT(updateTimes()));

    /// Reset Times.
    QPushButton *pushButton4_p = new QPushButton(this);
    pushButton4_p->setObjectName(QString::fromUtf8("PrintTimesPushButton"));
    pushButton4_p->setText(tr("Print"));
    //connect(pushButton4_p, SIGNAL(clicked(bool)), m_treeItemModel_p, SLOT(resetTimes()));
    connect(pushButton4_p, SIGNAL(clicked(bool)), this, SLOT(printTimes ()));

    /// Reset Times.
    m_qcbUpdate_p = new QCheckBox(this);
    m_qcbUpdate_p->setObjectName(QString::fromUtf8("UpdateTimesCheckBox"));
    m_qcbUpdate_p->setText(tr("Update Times"));
    connect(m_qcbUpdate_p, SIGNAL(stateChanged(int)), this, SLOT(updateTimes ()));

    /// Layout
    QGridLayout * gridLayout_p = new QGridLayout(this);

    gridLayout_p->setObjectName(QString::fromUtf8("GridLayout"));

    gridLayout_p->addWidget(pushButton1_p, 0, 0, 1, 1);
    gridLayout_p->addWidget(pushButton2_p, 0, 1, 1, 1);
    gridLayout_p->addWidget(pushButton3_p, 0, 2, 1, 1);

    gridLayout_p->addWidget(m_qtvClocks_p, 1, 0, 1, 3);

    gridLayout_p->addWidget(m_qcbUpdate_p, 2, 0, 1, 2);
    gridLayout_p->addWidget(pushButton4_p, 2, 2, 1, 1);

    m_qtvClocks_p -> loadParameters ( );
    m_qtvClocks_p -> expandAll ( );

    QSettings qSettings;
 
    bool state_b = qSettings.value( 
            QString("ClockTreeDlg/qcbUpdate"), false).toBool();
    
    m_qcbUpdate_p->setChecked ( state_b );
}

CClockTreeDlg::~CClockTreeDlg ()
{
    /// Save check box state.
    QSettings qSettings;
    qSettings.setValue(QString("ClockTreeDlg/qcbUpdate"), m_qcbUpdate_p -> isChecked() );

    if ( m_treeItemModel_p )
        delete m_treeItemModel_p;

    if ( m_qtvClocks_p )
      delete m_qtvClocks_p;

    m_treeItemModel_p = NULL;
}

void
CClockTreeDlg::updateTimes ()
{
    if ( m_qcbUpdate_p -> isChecked() )
    {
        m_qtvClocks_p->expandAll();
    }
}

void
CClockTreeDlg::printTimes ()
{
    //printf("save times has been clicked\n");

    m_treeItemModel_p -> printTimes ();
}

void 
CClockTreeDlg::closeEvent(QCloseEvent *f_event_p)
{
    QSettings settings;
    QString name = QString("CClockTreeDlg/geometry/") + (parent()?parent()->objectName():QString("default"));
    settings.setValue(name, saveGeometry());

    QWidget::closeEvent(f_event_p);
}

void 
CClockTreeDlg::focusInEvent(QFocusEvent * /* f_event_p */)
{
    m_qtvClocks_p -> expandAll();
}

void
CClockTreeDlg::mouseMoveEvent ( QMouseEvent *  /*f_event_p*/ )
{
    activateWindow();
}

