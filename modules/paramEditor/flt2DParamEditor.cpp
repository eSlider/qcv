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
#include "flt2DParamEditor.h"

#include <QLabel>
#include <QGridLayout>

#include "doubleEditor.h"

using namespace QCV;

const unsigned int CFlt2DParameterEditor::m_maxDecimals_cui = 6;

CFlt2DParameterEditor::CFlt2DParameterEditor( CFlt2DParameter *  f_parameter_p,
                                              QWidget *          f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_parameter_p (       f_parameter_p )
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QGridLayout *layout_p = new QGridLayout( this );    
    layout_p -> setMargin( 0 );    

    const std::string * names_p = f_parameter_p -> getNames();
    
    unsigned int pos_i = 0;
    
    /// Sping boxes now.
    for (int i = 0; i < 2; ++i)
    {
        m_qdsb_p[i] = new CDoubleEditor ( 0, this );
        m_qdsb_p[i] -> setDecimals ( m_maxDecimals_cui );
        sizePolicy.setHeightForWidth(m_qdsb_p[i]->sizePolicy().hasHeightForWidth());
        m_qdsb_p[i] -> setSizePolicy ( sizePolicy );

        if (not names_p[i].empty() )
        {    
            layout_p -> addWidget(new QLabel( names_p[i].c_str(), this ), 0, pos_i++, 1, 1 );
        }

        layout_p -> addWidget( m_qdsb_p[i], 0, pos_i++, 1, 1 );
    }

    updateWidget();

    connect( m_qdsb_p[0],  SIGNAL( valueChanged ( double ) ),
             this,        SLOT(   updateParameter() ) );

    connect( m_qdsb_p[1],  SIGNAL( valueChanged ( double ) ),
             this,        SLOT(   updateParameter() ) );
}

CFlt2DParameterEditor::~CFlt2DParameterEditor ()
{}

void
CFlt2DParameterEditor::updateParameter()
{
    S2D<float> value( m_qdsb_p[0]->getValue(),
                      m_qdsb_p[1]->getValue() );

    if ( not (m_parameter_p -> getValue() == value ) )
        m_parameter_p -> setValue( value );
}

bool
CFlt2DParameterEditor::updateWidget()
{
    S2D<float> value = m_parameter_p -> getValue();
    
    m_qdsb_p[0] -> setValue(value.x);
    m_qdsb_p[1] -> setValue(value.y);
    
    return true;    
}
