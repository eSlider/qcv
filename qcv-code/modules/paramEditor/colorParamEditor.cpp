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
#include "colorParamEditor.h"

#include <QSpinBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QColorDialog>

using namespace QCV;

CColorParameterEditor::CColorParameterEditor( CColorParameter *  f_parameter_p,
                                              QWidget *          f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_qpb_p (                      NULL ),
          m_parameter_p (       f_parameter_p )
{
    QHBoxLayout *layout_p = new QHBoxLayout( this );    
    layout_p -> setMargin( 0 );    

    int channels_i = m_parameter_p -> useAlpha()?4:3;

    for (int i = 0; i < channels_i; ++ i)
    {
        m_qsb_p[i] = new QSpinBox( this );
        m_qsb_p[i] -> setMaximum(255);
        m_qsb_p[i] -> setMinimum(0);
        //m_qsb_p[i] -> setSizePolicy ( sizePolicy );
        layout_p -> addWidget( m_qsb_p[i] );
    }

    m_qsb_p[0] -> setToolTip ( QString(tr("Red Component") ) );
    m_qsb_p[1] -> setToolTip ( QString(tr("Gree Component") ) );
    m_qsb_p[2] -> setToolTip ( QString(tr("Blue Component") ) );

    if ( m_parameter_p -> useAlpha() )
        m_qsb_p[3] -> setToolTip ( QString(tr("Alpha Component") ) );

    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    /// Now the button
    m_qpb_p = new QPushButton(this);
    m_qpb_p  -> setMinimumSize(QSize(m_qpb_p->height(), m_qpb_p->height()));
    m_qpb_p  -> setToolTip ( tr( "Open the selection dialog" ));
    m_qpb_p  -> setSizePolicy ( sizePolicy );
    m_qpb_p  -> setText ( "..." );
    layout_p -> addWidget( m_qpb_p );

    updateWidget();

    connect( m_qsb_p[0],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );

    connect( m_qsb_p[1],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );

    connect( m_qsb_p[2],  SIGNAL( valueChanged ( int ) ),
             this,        SLOT(   updateParameter() ) );

    if (m_parameter_p -> useAlpha())
        connect( m_qsb_p[3],  SIGNAL( valueChanged ( int ) ),
                 this,        SLOT(   updateParameter() ) );

    connect( m_qpb_p,  SIGNAL( clicked ( ) ),
             this,     SLOT(   openColorDialog() ) );

}

CColorParameterEditor::~CColorParameterEditor ()
{}

void
CColorParameterEditor::updateParameter()
{
    SRgba color;
    color.r = m_qsb_p[0]->value();
    color.g = m_qsb_p[1]->value();
    color.b = m_qsb_p[2]->value();

    if ( m_parameter_p -> useAlpha() )
        color.a = m_qsb_p[3]->value();
    else
        color.a = 255;
    
    QPalette palette;
    QBrush   brush(QColor(color.r, color.g, color.b, color.a));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    m_qpb_p -> setPalette(palette);

    if ( not (m_parameter_p -> getValue() == color ) )
        m_parameter_p -> setValue( color );
}

bool
CColorParameterEditor::updateWidget()
{
    SRgba color = m_parameter_p -> getValue();

    m_qsb_p[0]->setValue(color.r);
    m_qsb_p[1]->setValue(color.g);
    m_qsb_p[2]->setValue(color.b);

    if ( m_parameter_p -> useAlpha() )
        m_qsb_p[3]->setValue(color.a);

    QPalette palette;
    QBrush   brush(QColor(color.r, color.g, color.b, color.a));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
    m_qpb_p -> setPalette(palette);

    return true;
}

/// Button was clicked.
void
CColorParameterEditor::openColorDialog()
{
    
    QColor initial;

    if ( m_parameter_p -> useAlpha() )
        initial = QColor ( m_qsb_p[0]->value(), 
                           m_qsb_p[1]->value(), 
                           m_qsb_p[2]->value(),
                           m_qsb_p[3]->value() );
    else
        initial =  QColor ( m_qsb_p[0]->value(), 
                            m_qsb_p[1]->value(), 
                            m_qsb_p[2]->value(),
                            255  );
    
    QColorDialog::ColorDialogOptions flags_e = 0;
    
    if (m_parameter_p -> useAlpha())
        flags_e = QColorDialog::ShowAlphaChannel;
    
    QColor res = QColorDialog::getColor ( initial, 
                                          this, 
                                          "Select a new color", 
                                           flags_e );
    if (res.isValid())
    {
        m_qsb_p[0] -> setValue ( res.red()   );
        m_qsb_p[1] -> setValue ( res.green() );
        m_qsb_p[2] -> setValue ( res.blue()  );

        if ( m_parameter_p -> useAlpha() )
            m_qsb_p[3] -> setValue ( res.alpha() );
        
        updateParameter();
    }    
}
