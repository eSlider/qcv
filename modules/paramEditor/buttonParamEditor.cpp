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
* \file  buttonParamEditor
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "buttonParamEditor.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <limits>


using namespace QCV;

CButtonParameterEditor::CButtonParameterEditor( CButtonParameter *  f_parameter_p,
                                                QWidget *           f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_qpb_p (                      NULL ),
          m_parameter_p (       f_parameter_p )
{
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    m_qpb_p = new ::QPushButton ( NULL );

    QGridLayout *layout_p = new QGridLayout( this );

    layout_p -> setMargin( 0 );    
    layout_p -> addWidget( m_qpb_p );
    
    if ( not f_parameter_p ->getComment().empty() )
    {
        m_qpb_p -> setToolTip ( QString(f_parameter_p ->getComment().c_str() ) );
        m_qpb_p -> setText    ( QString(f_parameter_p ->getName().c_str() ) );
    }
    
    sizePolicy.setHeightForWidth(m_qpb_p->sizePolicy().hasHeightForWidth());
    m_qpb_p -> setSizePolicy ( sizePolicy );    
           
    connect( m_qpb_p,   SIGNAL( clicked (  ) ),
             this,      SLOT(   clicked (  ) ) );
}

CButtonParameterEditor::~CButtonParameterEditor ()
{}

void
CButtonParameterEditor::clicked()
{
    m_parameter_p -> clicked ( );
}
