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


#ifndef __BOOLPARAMEDITOR_H
#define __BOOLPARAMEDITOR_H

/*@@@**************************************************************************
 ** \file  boolParamEditor
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "baseParamEditorWidget.h"

#include "boolParam.h"

/* PROTOTYPES */
class QCheckBox;

namespace QCV
{
    /* PROTOTYPES */

    class CBoolParameterEditor : public CBaseParamEditorWidget
    {
        Q_OBJECT
    public:
        
        /// Constructors/Destructor
        CBoolParameterEditor ( CBoolParameter *  f_parameter_p,
                               QWidget        *  f_parent_p = NULL );

        virtual ~CBoolParameterEditor ();

        virtual bool updateWidget();
                             
    public slots:
        void updateParameter();

    /// Protected members
    protected:

        /// Stored value.
        QCheckBox *             m_qcb_p;

        /// Associated Bool parameter
        CBoolParameter *        m_parameter_p;
    };
}

#endif // __BOOLPARAMEDITOR_H
