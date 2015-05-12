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


#ifndef __UINTPARAMEDITOR_H
#define __UINTPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file uintParamEditor.h
 *
 * \class CUIntParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for  an object of type CUIntParameter.
 *
 * The class implements a simple Qt editor with an input box where
 * the user enters an unsigned int value.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"
#include "uintParam.h"

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CUIntEditor;

    class CUIntParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
    public:
        
        /// Constructors/Destructor
        CUIntParameterEditor ( CUIntParameter *  f_parameter_p,
                              QWidget        *  f_parent_p = NULL );

        virtual ~CUIntParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

    /// Protected members
    protected:

        /// Stored value.
        CUIntEditor *           m_editor_p;

        /// Associated UInt parameter
        CUIntParameter *        m_parameter_p;
    };
}

#endif // __UINTPARAMEDITOR_H
