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


#ifndef __PARAMETERGROUP_H
#define __PARAMETERGROUP_H

/**
 *******************************************************************************
 *
 * @file paramGroup.h
 *
 * \class CParameterGroup
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Virtual parameter for defining the beginning of a display group.
 *
 ******************************************************************************/

/* INCLUDES */
#include "parameter.h"
#include "colors.h"

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
    class CParameterGroup: public CParameter
    {
    /// Constructors/Destructor
    public:
        CParameterGroup (  std::string   f_name_str,
                           bool          f_applyColor_b = false,
                           SRgb          f_color = CColor::white );

        
        virtual ~CParameterGroup ( );
        
    public:
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        void setColor ( SRgb f_newColor ) { m_color = f_newColor;  }
        SRgb getColor ( ) const { return m_color; }
        
        bool applyColor (  ) const {  return m_applyColor_b; }
        void setApplyColor ( bool f_apply_b ) { m_applyColor_b = f_apply_b; }

        /// Can/should this parameter be saved?
        bool shouldBeSaved() { return false; }

    /// Protected help methods.
    protected:

        /// Use background color?
        bool    m_applyColor_b;

        /// Background color;
        SRgb    m_color;
   };
}


#endif // __PARAMETERGROUP_H
