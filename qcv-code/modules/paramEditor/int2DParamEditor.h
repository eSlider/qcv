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


#ifndef __INT2DPARAMEDITOR_H
#define __INT2DPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file int2DParamEditor.h
 *
 * \class CInt2DParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CInt2DParameter.
 *
 * The class implements a simple Qt editor with two input boxes where
 * the user enters signed integer values.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "int2DParam.h"

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CIntEditor;

    class CInt2DParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CInt2DParameterEditor ( CInt2DParameter *  f_parameter_p,
                                QWidget *          f_parent_p = NULL );

        virtual ~CInt2DParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

        /// Protected members
    protected:

        /// Stored value.
        CIntEditor *      m_qsb_p[2];

        /// Associated Int2D parameter
        CInt2DParameter * m_parameter_p;
    };
}

#endif // __INT2DPARAMEDITOR_H
