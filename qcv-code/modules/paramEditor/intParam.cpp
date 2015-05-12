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
* \file  intParam
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "intParam.h"
#include "stringOp.h"
#include "intParamEditor.h"

#include <math.h>
#include <errno.h>
#include <stdlib.h>

using namespace QCV;

CIntParameter::CIntParameter ( std::string               f_name_str, 
                               std::string               f_comment_str,
                               int                       f_value_i,
                               CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_val_i (                    f_value_i )
{
    /// update string value.
    CParameter::updateInitialValue();
    
    update();
}

CIntParameter::~CIntParameter()
{}

std::string
CIntParameter::getStringFromValue ( ) const
{
    return CStringOp::numToString<int> ( m_val_i );
}


// Return true if the first word after elimintaing spaces is 
// is true or false or if the string contains a valid number.
// In any other case return false and no value is assigned.

bool
CIntParameter::setValueFromString ( std::string f_val_str )
{
    if (f_val_str.length() == 0)
        return false;
    
    CStringOp::trimLeft ( f_val_str );

    const char* begin_p = f_val_str.c_str();
    char* end_p   = NULL;
    errno = 0;
    double val_d = strtod ( begin_p, &end_p );
    
    if ( end_p != begin_p && 
         isfinite(val_d) &&
         !errno )
    {
        m_val_i = (int)val_d;

        updateInitialValue();
        return update();
    }
    
    /// Else do not change nothing and return false.
    return false;
}

int 
CIntParameter::getValue ( ) const
{
    return m_val_i;
}

bool CIntParameter::setValue ( int  f_value_i,
                               bool f_shouldUpdate_b )
{
    if ( f_value_i == m_val_i )
        return true;

    m_val_i = f_value_i;

    if ( f_shouldUpdate_b )
        return update();

    return true;    
}

::QWidget *
CIntParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CIntParameterEditor ( this ));
}
