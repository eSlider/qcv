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


#ifndef __DBL2DPARAM_H
#define __DBL2DPARAM_H

/*@@@**************************************************************************
 ** \file  Dbl2DParam
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

    class CDbl2DParameter : public CParameter
    {
    public:
        
        /// Constructors/Destructor
        CDbl2DParameter (  std::string               f_name_str = "",
                           std::string               f_comment_str = "",
                           S2D<double>               f_val = S2D<double>(0.,0.),
                           std::string               f_name1_str = "",
                           std::string               f_name2_str = "",
                           CParameterBaseConnector * f_connector_p = NULL );

        virtual ~CDbl2DParameter ();

    public:
        /// Get and set values from strings.
        virtual std::string     getStringFromValue ( ) const;
        virtual bool            setValueFromString ( std::string f_val_str );
 
        /// Get and set Dbl2D value
        virtual S2D<double>     getValue ( ) const;
        virtual bool            setValue ( S2D<double> f_value,
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
        S2D<double>  m_value;

        /// Stored value.
        std::string  m_names_p[2];
    };
}


#endif // __DBL2DPARAM_H
