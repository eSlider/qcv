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


#ifndef __3DVECTORPARAM_H
#define __3DVECTORPARAM_H

/*@@@**************************************************************************
 ** \file  3DVectorParam.h
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****     (C) COPYRIGHT Hernan Badino - All Rights Reserved               *****
******************************************************************************/

/* INCLUDES */
#include "parameter.h"
#include <opencv/cv.h>

namespace QCV
{
    /* PROTOTYPES */

    class C3DVectorParameter : public CParameter
    {
    public:
        
        /// Constructors/Destructor
        C3DVectorParameter (  std::string               f_name_str = "",
                              std::string               f_comment_str = "",
                              cv::Vec3d                 f_val = cv::Vec3d(0.,0.,0.),
                              std::string               f_name1_str = "x",
                              std::string               f_name2_str = "y",
                              std::string               f_name3_str = "z",
                              CParameterBaseConnector * f_connector_p = NULL );

        virtual ~C3DVectorParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set Dbl2D value
        virtual cv::Vec3d       getValue ( ) const;
        virtual bool            setValue ( cv::Vec3d   f_value,
                                           bool        f_shouldUpdate_b = true );
        
        const std::string *     getNames ( ) const 
        {
            return m_names_p;
        }

        /// Get editor.
        virtual QWidget *       createEditor ( );

    /// Protected members
    protected:

        /// Stored value.
        cv::Vec3d    m_value;

        /// Stored value.
        std::string  m_names_p[3];
    };
}


#endif // __3DVECTORPARAM_H
