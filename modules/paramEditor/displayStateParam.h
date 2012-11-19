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


#ifndef __DISPLAYSTATEPARAM_H
#define __DISPLAYSTATEPARAM_H

/*@@@**************************************************************************
 ** \file  displayStateParam
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "parameter.h"
#include "standardTypes.h"

namespace QCV
{
    /* PROTOTYPES */

    class CDisplayStateParameter : public CParameter
    {
    public:
        
        /// Constructors/Destructor
        CDisplayStateParameter (  std::string               f_name_str = "",
                                  std::string               f_comment_str = "",
                                  SDisplayState             f_displayState = SDisplayState(S2D<int>(0,0), false),
                                  CParameterBaseConnector * f_connector_p = NULL );
        
        virtual ~CDisplayStateParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set displayState value
        virtual SDisplayState getValue ( ) const;
        virtual bool          setValue ( SDisplayState f_displayState,
                                         bool          f_shouldUpdate_b = true );
        
        /// Get editor.
        virtual QWidget *       createEditor ( );

    /// Protected members
    protected:

        /// Stored value.
        SDisplayState  m_displayState;
    };
}


#endif // __DISPLAYSTATEPARAM_H
