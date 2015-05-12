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
* \file  stringParamEditor
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "displayStateParamEditor.h"

#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>

using namespace QCV;

CDisplayStateParameterEditor::CDisplayStateParameterEditor( CDisplayStateParameter *  f_parameter_p,
                                                            QWidget *                 f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_qcb_p (                      NULL ),
          m_parameter_p (       f_parameter_p )
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QGridLayout *layout_p = new QGridLayout( this );    
    layout_p -> setMargin( 0 );    

    /// Check box first.
    m_qcb_p = new QCheckBox(this);
    m_qcb_p  -> setToolTip ( tr( "List visibility" ));
    //m_qcb_p  -> setSizePolicy ( sizePolicy );
    m_qcb_p  -> setText ( tr("Visible") );
    m_qcb_p  -> setLayoutDirection(Qt::RightToLeft);
    layout_p -> addWidget( m_qcb_p, 0, 0, 1, 1 );

    /// Sping boxes now.
    for (int i = 0; i < 2; ++i)
    {
        m_qsb_p[i] = new QSpinBox( this );
        m_qsb_p[i] -> setMaximum(255);
        m_qsb_p[i] -> setMinimum(0);
        sizePolicy.setHeightForWidth(m_qsb_p[i]->sizePolicy().hasHeightForWidth());
        m_qsb_p[i] -> setSizePolicy ( sizePolicy );
        QLabel * ql_p = new QLabel( i==0?"X:":"Y:", this );
        //ql_p -> setSizePolicy ( QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed) );
        layout_p -> addWidget(ql_p, 0, 1+i*2, 1, 1);
        layout_p -> addWidget( m_qsb_p[i], 0, 2+i*2, 1, 1 );
    }

    m_qsb_p[0] -> setToolTip ( QString(tr("X Position") ) );
    m_qsb_p[1] -> setToolTip ( QString(tr("Y Position") ) );

    updateWidget();

    connect( m_qsb_p[0],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );

    connect( m_qsb_p[1],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );

    connect( m_qcb_p,  SIGNAL(  toggled(     bool ) ),
             this,     SLOT(    updateParameter() ) );

}

CDisplayStateParameterEditor::~CDisplayStateParameterEditor ()
{}

void
CDisplayStateParameterEditor::updateParameter()
{
    SDisplayState displayState (S2D<int> ( m_qsb_p[0]->value(),
                                           m_qsb_p[1]->value() ),
                                m_qcb_p -> isChecked() );
    

    if ( not (m_parameter_p -> getValue() == displayState ) )
        m_parameter_p -> setValue( displayState );
}

bool
CDisplayStateParameterEditor::updateWidget()
{
    SDisplayState displayState = m_parameter_p -> getValue();

    m_qsb_p[0] -> setValue(displayState.position.x);
    m_qsb_p[1] -> setValue(displayState.position.y);
    m_qcb_p    -> setCheckState ( displayState.visible_b?Qt::Checked:Qt::Unchecked );

    return true;    
}
