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


#ifndef __COLORPARAM_H
#define __COLORPARAM_H

/*@@@**************************************************************************
 ** \file  colorParam
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "parameter.h"
#include "colors.h"
#include <stdio.h>

namespace QCV
{
    /* PROTOTYPES */

    class CColorParameter : public CParameter
    {
    public:
        
        /// Constructors/Destructor
        CColorParameter (  std::string               f_name_str = "",
                           std::string               f_comment_str = "",
                           SRgba                     f_color = CColor::black,
                           bool                      f_useAlpha_b = true,
                           CParameterBaseConnector * f_connector_p = NULL );
        
        virtual ~CColorParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set color value
        virtual SRgba           getValue ( );
        virtual bool            setValue ( SRgba f_color,
                                           bool  f_shouldUpdate_b = true );

        virtual bool            useAlpha() const { return m_useAlpha_b; }
        
        /// Get editor.
        virtual QWidget *       createEditor ( );

    /// Protected members
    protected:

        /// Stored value.
        SRgba             m_color;

        /// Alpha channel?
        bool              m_useAlpha_b;
    };
}


#endif // __COLORPARAM_H
