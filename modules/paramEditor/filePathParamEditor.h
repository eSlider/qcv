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


#ifndef __FILEPATHPARAMEDITOR_H
#define __FILEPATHPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file filePathParamEditor.h
 *
 * \class CFilePathParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CStringParameter containing
 * the path to a file.
 *
 * The class implements a simple Qt editor with an input boxes (where the user
 * the user enters a file path) and a button with which the user can open a 
 * dialog for selecting a file.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "filePathParam.h"

/* PROTOTYPES */
class QLineEdit;
class QPushButton;

namespace QCV
{
    /* PROTOTYPES */

    class CFilePathParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT

    public:
        typedef enum
        {
            FT_FILE,
            FT_DIRECTORY
        } EFileType_t;
        
    public:
        
        /// Constructors/Destructor
        CFilePathParameterEditor ( CStringParameter *    f_parameter_p,
                                   EFileType_t           f_fileType_e,
                                   QWidget *             f_parent_p = NULL );

        virtual ~CFilePathParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();
        void openDialogBox();
        

    /// Protected members
    protected:

        /// Stored value.
        EFileType_t               m_fileType_e;

        /// Stored value.
        QLineEdit *               m_qle_p;

        /// Buttom to open file dialog.
        QPushButton *             m_qpb_p;

        /// Associated FilePath parameter
        CStringParameter *        m_parameter_p;
    };
}

#endif // __FILEPATHPARAMEDITOR_H
