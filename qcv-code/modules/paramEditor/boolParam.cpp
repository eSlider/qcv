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


/*@@@**************************************************************************
* \file  boolParam
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "boolParam.h"
#include "stringOp.h"
#include "boolParamEditor.h"

#include <math.h>
#include <errno.h>
#include <stdlib.h>

using namespace QCV;

CBoolParameter::CBoolParameter ( std::string               f_name_str, 
                                 std::string               f_comment_str,
                                 bool                      f_value_b,
                                 CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_val_b (                    f_value_b )
{
    /// update string value.
    CParameter::updateInitialValue();
    
    update();
}

CBoolParameter::~CBoolParameter()
{}

std::string
CBoolParameter::getStringFromValue ( ) const
{
    if ( m_val_b ) return std::string("true");
    
    return std::string("false");
}


// Return true if the first word after elimintaing spaces is 
// is true or false or if the string contains a valid number.
// In any other case return false and no value is assigned.

bool
CBoolParameter::setValueFromString ( std::string f_val_str )
{
    if (f_val_str.length() == 0)
        return false;
    
    CStringOp::trimLeft ( f_val_str );
    unsigned char val_uc = 2;

    if ( f_val_str.substr(0, 4) == "true" &&
         (  f_val_str.length() == 4 || isspace(f_val_str[4])))
        val_uc = 1; // true
    else if ( f_val_str.substr(0, 5) == "false" &&
              ( f_val_str.length() == 5 || isspace(f_val_str[5]) ) )
        val_uc = 0; // false;
    else
    {
        const char* begin_p = f_val_str.c_str();
        char* end_p   = NULL;
        errno = 0;
        double val_d = strtod ( begin_p, &end_p );
        
        if ( end_p != begin_p )
        {
            if ( isfinite(val_d) )
            {
                /// True value is not only assign if val_d has non-zero value but also
                /// if it has non-zero value and errno indicates over/under-flow
                /// (errno==ERANGE).
                val_uc = (val_d || errno!=0)?1:0;
            }
        }
    }

    if (val_uc != 2)
    {
        m_val_b = (bool) val_uc;
        /// update string value.
        updateInitialValue();
        return update();
    }
    
    /// Else do not change nothing and return false.
    return false;
}

bool CBoolParameter::getValue ( ) const
{
    return m_val_b;
}

bool CBoolParameter::setValue ( bool f_value_b,
                                bool f_shouldUpdate_b )
{
    if ( f_value_b == m_val_b )
        return true;

    m_val_b = f_value_b;

    if ( f_shouldUpdate_b )
        return update();    
    return true;    
}

::QWidget *
CBoolParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CBoolParameterEditor ( this ));
}
