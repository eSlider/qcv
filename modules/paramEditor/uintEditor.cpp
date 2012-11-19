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
 * \file  uintEditor
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "uintEditor.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <math.h>

using namespace QCV;

CUIntEditor::CUIntEditor( unsigned int   f_value_ui,
                          QWidget *      f_parent_p )
        : QWidget (               f_parent_p ),
          m_qle_p (                     NULL ),
          m_value_ui (            f_value_ui )
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    m_qle_p = new QLineEdit ( this );

    sizePolicy.setHeightForWidth(m_qle_p->sizePolicy().hasHeightForWidth());
    m_qle_p -> setSizePolicy ( sizePolicy );    

    m_qle_p -> setText ( QString::number(m_value_ui) );    

    QGridLayout *layout_p = new QGridLayout( this );
    layout_p -> addWidget ( m_qle_p );
    
    connect( m_qle_p,   SIGNAL( editingFinished() ),
             this,      SLOT(   lineEditChanged() ) );
}

CUIntEditor::~CUIntEditor ()
{}

void
CUIntEditor::lineEditChanged()
{
    QString str = m_qle_p->text();
    bool ok;
    unsigned int newVal_ui = str.toUInt(&ok);

    if ( ok && newVal_ui != m_value_ui )
    {
        m_value_ui = newVal_ui;
        emit valueChanged( m_value_ui );
    }

    m_qle_p -> setText ( QString::number( m_value_ui ) );
}

bool
CUIntEditor::setValue( unsigned int f_value_ui )
{
    m_value_ui = f_value_ui;
    m_qle_p -> setText ( QString::number( m_value_ui ) );
    return true;
}

QSize CUIntEditor::sizeHint () const
{
    return QSize(10,10);
}
