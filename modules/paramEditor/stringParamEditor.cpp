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
#include "stringParamEditor.h"

#include <QLineEdit>
#include <QHBoxLayout>

using namespace QCV;

CStringParameterEditor::CStringParameterEditor( CStringParameter *  f_parameter_p,
                                                QWidget        *  f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_qle_p (                      NULL ),
          m_parameter_p (       f_parameter_p )
{
    m_qle_p = new QLineEdit( this );

    m_qle_p -> setText ( f_parameter_p -> getValue().c_str() );
    
    QHBoxLayout *layout_p = new QHBoxLayout( this );
    
    layout_p -> setMargin( 0 );    
    layout_p -> addWidget( m_qle_p );
    
    if ( not f_parameter_p ->getComment().empty() )
        m_qle_p -> setToolTip ( QString(f_parameter_p ->getComment().c_str() ) );
    
    connect( m_qle_p,   SIGNAL( editingFinished () ),
             this,      SLOT(   updateParameter() ) );
}

CStringParameterEditor::~CStringParameterEditor ()
{}

void
CStringParameterEditor::updateParameter()
{
    if ( m_parameter_p -> getValue() != m_qle_p -> text().toStdString() )
    {
        m_parameter_p -> setValue( m_qle_p -> text().toStdString() );
    }
}

bool
CStringParameterEditor::updateWidget()
{
    if ( m_parameter_p -> getValue() != m_qle_p -> text().toStdString() )
        m_qle_p -> setText( QString( m_parameter_p -> getValue().c_str() ) );
                             
    return true;
}
