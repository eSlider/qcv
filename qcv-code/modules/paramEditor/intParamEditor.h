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


#ifndef __INTPARAMEDITOR_H
#define __INTPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file intParamEditor.h
 *
 * \class CIntParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for  an object of type CIntParameter.
 *
 * The class implements a simple Qt editor with an input box where
 * the user enters an signed int value.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"
#include "intParam.h"

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CIntEditor;

    class CIntParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
    public:
        
        /// Constructors/Destructor
        CIntParameterEditor ( CIntParameter *  f_parameter_p,
                              QWidget        *  f_parent_p = NULL );

        virtual ~CIntParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

    /// Protected members
    protected:

        /// Stored value.
        CIntEditor *           m_qsb_p;

        /// Associated Int parameter
        CIntParameter *        m_parameter_p;
    };
}

#endif // __INTPARAMEDITOR_H
