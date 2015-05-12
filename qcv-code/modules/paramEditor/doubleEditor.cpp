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
* \file  doubleEditor
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "doubleEditor.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <math.h>

using namespace QCV;

CDoubleEditor::CDoubleEditor( double        f_value_d,
                              QWidget *     f_parent_p )
        : QWidget (               f_parent_p ),
          m_qle_p (                     NULL ),
          m_value_d (              f_value_d ),
          m_maxDecimals_ui (              25 )
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    m_qle_p = new QLineEdit ( this );

    sizePolicy.setHeightForWidth(m_qle_p->sizePolicy().hasHeightForWidth());
    m_qle_p -> setSizePolicy ( sizePolicy );    

    m_qle_p -> setText ( QString::number(m_value_d, 'g', m_maxDecimals_ui ) );    

    QGridLayout *layout_p = new QGridLayout( this );
    layout_p -> addWidget ( m_qle_p );
    
    connect( m_qle_p,   SIGNAL( editingFinished() ),
             this,      SLOT(   lineEditChanged() ) );
}

CDoubleEditor::~CDoubleEditor ()
{}

void
CDoubleEditor::lineEditChanged()
{
    QString str = m_qle_p->text();
    bool ok;
    double newVal_d = str.toDouble(&ok);

    if ( ok && fabs(newVal_d - m_value_d) > 1.e-11 )
    {
        m_value_d = newVal_d;
        emit valueChanged( m_value_d );
    }

    m_qle_p -> setText ( QString::number(m_value_d, 'g', m_maxDecimals_ui ) );
}

bool
CDoubleEditor::setValue( double f_value_d )
{
    m_value_d = f_value_d;
    m_qle_p -> setText ( QString::number(m_value_d, 'g', m_maxDecimals_ui ) );
    return true;
}

QSize CDoubleEditor::sizeHint () const
{
    return QSize(10,10);
}
