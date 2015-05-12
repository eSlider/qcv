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
#include "filePathParamEditor.h"

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

using namespace QCV;

CFilePathParameterEditor::CFilePathParameterEditor( CStringParameter *    f_parameter_p,
                                                    EFileType_t           f_fileType_e,
                                                    QWidget *             f_parent_p )
        : CBaseParamEditorWidget ( f_parent_p ),
          m_fileType_e (         f_fileType_e ),
          m_qle_p (                      NULL ),
          m_qpb_p (                      NULL ),
          m_parameter_p (       f_parameter_p )
{
    /// First the line edit.
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    m_qle_p = new QLineEdit( this );
    m_qle_p -> setSizePolicy ( sizePolicy );
    m_qle_p -> setText ( f_parameter_p -> getValue().c_str() );
    
    QHBoxLayout *layout_p = new QHBoxLayout( this );
    
    layout_p -> setMargin( 0 );    
    layout_p -> addWidget( m_qle_p );
    

    /// Now the button
    m_qpb_p = new QPushButton(this);
    m_qpb_p  -> setMinimumSize(QSize(m_qpb_p->height(), m_qpb_p->height()));
    m_qpb_p  -> setMaximumSize(QSize(m_qpb_p->height(), m_qpb_p->height()));
    m_qpb_p  -> setText ( "..." );
    m_qpb_p  -> setToolTip ( tr( "Open the selection dialog" ));

    layout_p -> addWidget(m_qpb_p);

    if ( not f_parameter_p ->getComment().empty() )
        m_qle_p -> setToolTip ( QString(f_parameter_p ->getComment().c_str() ) );
    
    connect( m_qle_p,   SIGNAL( editingFinished () ),
             this,      SLOT(   updateParameter() ) );

    connect( m_qpb_p,   SIGNAL( clicked () ),
             this,      SLOT(   openDialogBox() ) );
}

CFilePathParameterEditor::~CFilePathParameterEditor ()
{}

void
CFilePathParameterEditor::updateParameter()
{
    if ( m_parameter_p -> getValue() != m_qle_p -> text().toStdString() )
    {
        m_parameter_p -> setValue( m_qle_p -> text().toStdString() );
    }
}

bool
CFilePathParameterEditor::updateWidget()
{
    if ( m_parameter_p -> getValue() != m_qle_p -> text().toStdString() )
        m_qle_p -> setText( QString( m_parameter_p -> getValue().c_str() ) );
                             
    return true;
}

void
CFilePathParameterEditor::openDialogBox()
{
    QString fileName_str;
    
    if (m_fileType_e == FT_FILE)
        fileName_str = QFileDialog::getOpenFileName( this,
                                                     tr("Open File"), 
                                                     m_qle_p->text() );
    else
        fileName_str = QFileDialog::getExistingDirectory( this,
                                                          tr("Open File"), 
                                                          m_qle_p->text() );

    
    if ( fileName_str != "" &&
         m_qle_p -> text() != fileName_str )
    {
        m_qle_p -> setText(fileName_str);
        updateParameter();    
    }
}
