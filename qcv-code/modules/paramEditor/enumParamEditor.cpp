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
#include "enumParamEditor.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <limits>
#include <stdio.h>

using namespace QCV;

CEnumParameterEditor::CEnumParameterEditor( CEnumParameterBase *  f_parameter_p,
                                            QWidget *             f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_qcb_p (                      NULL ),
          m_parameter_p (       f_parameter_p ),
	  m_updatingWidget_b (          false )
{
    m_qcb_p = new QComboBox( this );

    QHBoxLayout *layout_p = new QHBoxLayout( this );

    layout_p -> setMargin( 0 );    
    layout_p -> addWidget( m_qcb_p );
    
    if ( not f_parameter_p ->getComment().empty() )
        m_qcb_p -> setToolTip ( QString(f_parameter_p ->getComment().c_str() ) );

    m_qcb_p->setEditable ( false );
    
    
    updateContent ();
    
    
    connect( m_qcb_p,   SIGNAL( currentIndexChanged ( int  ) ),
             this,      SLOT(   updateParameter() ) );
}

CEnumParameterEditor::~CEnumParameterEditor ()
{}

void
CEnumParameterEditor::updateContent  ()
{
    int currIdx_i = 0;

    m_qcb_p -> clear();
    
    for (int i = 0; i < m_parameter_p -> getCount(); ++i)
    {
        std::string str;
        int val_i;

        m_parameter_p -> getPair ( i, val_i, str );
        m_qcb_p -> addItem ( str.c_str(), QVariant(val_i) );

        if (val_i == m_parameter_p->getIntValue())
            currIdx_i = i;
    }

    m_qcb_p -> setCurrentIndex ( currIdx_i );
}

void
CEnumParameterEditor::updateParameter()
{
    int val_i = m_qcb_p -> itemData( m_qcb_p -> currentIndex() ).toInt();
    
    if ( m_parameter_p -> getIntValue() != val_i )
    {
        //printf("Value changed to %i\n", val_i);
        
        m_parameter_p -> setValueFromInt ( val_i );
    }
    
}

bool
CEnumParameterEditor::updateWidget()
{
    int val_i = m_qcb_p -> itemData( m_qcb_p -> currentIndex() ).toInt();

    if ( m_parameter_p -> getIntValue() != val_i  )
    {    
        int currIdx_i = -1;
        
        for (int i = 0; m_parameter_p -> getCount(); ++i)
        {
            std::string str;
            int val_i;
            
            m_parameter_p -> getPair ( i, val_i, str  );
            //m_qcb_p -> addItem ( str.c_str(), QVariant(val_i) );
            
            if (val_i == m_parameter_p->getIntValue())
            {
                currIdx_i = i;
                break;
            }
        }

        if (currIdx_i == -1)
            return false;
        
        m_qcb_p -> setCurrentIndex ( currIdx_i );
    }

    return true;
}
