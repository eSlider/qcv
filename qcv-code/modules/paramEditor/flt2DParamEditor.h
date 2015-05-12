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


#ifndef __FLT2DPARAMEDITOR_H
#define __FLT2DPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file flt2DParamEditor.h
 *
 * \class CFlt2DParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for  an object of type CFlt2DParameter.
 *
 * The class implements a simple Qt editor with two input boxes where
 * the user enters float values.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "flt2DParam.h"

/* PROTOTYPES */

namespace QCV
{
    /* PROTOTYPES */
    class CDoubleEditor;

    class CFlt2DParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CFlt2DParameterEditor ( CFlt2DParameter *  f_parameter_p,
                                QWidget *          f_parent_p = NULL );

        virtual ~CFlt2DParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

        /// Protected members
    protected:

        /// Stored value.
        CDoubleEditor *           m_qdsb_p[2];

        /// Associated Flt2D parameter
        CFlt2DParameter *         m_parameter_p;

        /// Amount of decimals to consider.
        const static unsigned int m_maxDecimals_cui;
    };
}

#endif // __FLT2DPARAMEDITOR_H
