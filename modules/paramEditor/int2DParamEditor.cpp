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
#include "int2DParamEditor.h"

#include <QLabel>
#include <QHBoxLayout>

#include "intEditor.h"

using namespace QCV;

CInt2DParameterEditor::CInt2DParameterEditor( CInt2DParameter *  f_parameter_p,
                                              QWidget *          f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_parameter_p (       f_parameter_p )
{    
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QHBoxLayout *layout_p = new QHBoxLayout( this );    
    layout_p -> setMargin( 0 );    

    const std::string * names_p = f_parameter_p -> getNames();

    /// Sping boxes now.
    for (int i = 0; i < 2; ++i)
    {
        m_qsb_p[i] = new CIntEditor( 0, this );
        m_qsb_p[i] -> setSizePolicy ( sizePolicy );
        if (not names_p[i].empty() )
        {    
            layout_p -> addWidget(new QLabel( names_p[i].c_str(), this ));
        }

        layout_p -> addWidget( m_qsb_p[i] );
    }

    updateWidget();

    connect( m_qsb_p[0],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );

    connect( m_qsb_p[1],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );
}

CInt2DParameterEditor::~CInt2DParameterEditor ()
{}

void
CInt2DParameterEditor::updateParameter()
{
    S2D<int> value( m_qsb_p[0]->getValue(),
                    m_qsb_p[1]->getValue() );
    

    if ( not (m_parameter_p -> getValue() == value ) )
        m_parameter_p -> setValue( value );
}

bool
CInt2DParameterEditor::updateWidget()
{
    S2D<int> value = m_parameter_p -> getValue();

    m_qsb_p[0] -> setValue(value.x);
    m_qsb_p[1] -> setValue(value.y);

    return true;    
}
