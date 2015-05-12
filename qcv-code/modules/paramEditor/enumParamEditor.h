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


#ifndef __ENUMPARAMEDITOR_H
#define __ENUMPARAMEDITOR_H

/**
 *******************************************************************************
 *
 * @file enumParamEditor.h
 *
 * \class CEnumParameterEditor
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements an editor for an object of type CEnumParameter
 *
 * The class implements a simple Qt editor with a combo box where the user
 * can select one of the multiple enumeration options.
 *
 *******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "enumParamBase.h"

/* PROTOTYPES */
class QComboBox;

namespace QCV
{
    /* PROTOTYPES */

    class CEnumParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
        public:
        
        /// Constructors/Destructor
        CEnumParameterEditor ( CEnumParameterBase *  f_parameter_p,
                               QWidget *             f_parent_p = NULL );

        virtual ~CEnumParameterEditor ();

        virtual bool updateWidget();
                      
        virtual void updateContent ();
       
    public slots:
        void updateParameter();

    /// Protected static members
    protected:

    /// Protected members
    protected:

        /// Stored value.
        QComboBox *           m_qcb_p;

        /// Associated Enum parameter
        CEnumParameterBase *  m_parameter_p;

	/// Updating widget
	bool                  m_updatingWidget_b;
    };
}

#endif // __ENUMPARAMEDITOR_H
