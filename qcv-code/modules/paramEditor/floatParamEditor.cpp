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
* \file  floatParamEditor
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "floatParamEditor.h"

#include <QHBoxLayout>
#include <limits>

#include "doubleEditor.h"

using namespace QCV;

const unsigned int CFloatParameterEditor::m_maxDecimals_cui = 6;

CFloatParameterEditor::CFloatParameterEditor( CFloatParameter *  f_parameter_p,
                                                QWidget *           f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_qdsb_p (                     NULL ),
          m_parameter_p (       f_parameter_p )
{
    m_qdsb_p = new CDoubleEditor( 0., this );

    QHBoxLayout *layout_p = new QHBoxLayout( this );

    layout_p -> setMargin( 0 );    
    layout_p -> addWidget( m_qdsb_p );
    
    if ( not f_parameter_p ->getComment().empty() )
        m_qdsb_p -> setToolTip ( QString(f_parameter_p ->getComment().c_str() ) );
   
    float value_f =  (float)(f_parameter_p -> getValue());    
    /// (float)std::numeric_limits<float>::min() is not working therefore:
    //float min_f   = -(float)std::numeric_limits<float>::max();
    //float max_f   = (float)std::numeric_limits<float>::max();

    //float min_f   = -1.e10;
    //float max_f   =  1.e10;

    m_qdsb_p -> setDecimals ( m_maxDecimals_cui );
    /*
    m_qdsb_p -> setMinimum ( min_f );
    m_qdsb_p -> setMaximum ( max_f );
    */

    m_qdsb_p -> setValue ( value_f );
    connect( m_qdsb_p,   SIGNAL( valueChanged(  double ) ),
             this,       SLOT(   updateParameter() ) );
}

CFloatParameterEditor::~CFloatParameterEditor ()
{}

void
CFloatParameterEditor::updateParameter()
{
    if ( m_parameter_p -> getValue() != m_qdsb_p -> getValue() )
    {
        m_parameter_p -> setValue( m_qdsb_p -> getValue() );
    }
    
}

bool
CFloatParameterEditor::updateWidget()
{
    if ( m_parameter_p -> getValue() != ((float)m_qdsb_p -> getValue()) )
        m_qdsb_p -> setValue ( m_parameter_p -> getValue() );

    return true;
}
