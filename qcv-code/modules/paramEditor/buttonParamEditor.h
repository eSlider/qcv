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


#ifndef __BUTTONPARAMEDITOR_H
#define __BUTTONPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file buttonParamEditor.h
 *
 * \class CButtonParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for  an object of type CButtonParameter.
 *
 * The class implements a simple Qt editor with an button.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"
#include "buttonParam.h"

/* PROTOTYPES */

class QPushButton;

namespace QCV
{
    /* PROTOTYPES */
    class CButtonParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
    public:
        
        /// Constructors/Destructor
        CButtonParameterEditor ( CButtonParameter *  f_parameter_p,
                                 QWidget          *  f_parent_p = NULL );

        virtual ~CButtonParameterEditor ();

    public slots:
        void clicked();

    /// Protected members
    protected:

        /// Stored value.
        QPushButton *             m_qpb_p;

        /// Associated Button parameter
        CButtonParameter *        m_parameter_p;
    };
}

#endif // __BUTTONPARAMEDITOR_H
