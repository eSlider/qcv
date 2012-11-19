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


#ifndef __FLOATPARAMEDITOR_H
#define __FLOATPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file floatParamEditor.h
 *
 * \class CFloatParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CFloatParameter.
 *
 * The class implements a simple Qt editor with an input box where
 * the user enters a float value.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "floatParam.h"

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CDoubleEditor;
    
    class CFloatParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CFloatParameterEditor ( CFloatParameter *  f_parameter_p,
                                 QWidget *         f_parent_p = NULL );

        virtual ~CFloatParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

    /// Protected static members
    protected:
        const static unsigned int m_maxDecimals_cui;

    /// Protected members
    protected:

        /// Stored value.
        CDoubleEditor *          m_qdsb_p;

        /// Associated Float parameter
        CFloatParameter *        m_parameter_p;
    };
}

#endif // __FLOATPARAMEDITOR_H
