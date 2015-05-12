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
 * \file  doubleParam
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "doubleParam.h"
#include "stringOp.h"
#include "doubleParamEditor.h"

#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <limits>

using namespace QCV;

CDoubleParameter::CDoubleParameter ( std::string               f_name_str, 
                                     std::string               f_comment_str,
                                     double                    f_value_d,
                                     CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_value_d (                  f_value_d )
{
    /// update string value.
    CParameter::updateInitialValue();
    update();
}

CDoubleParameter::~CDoubleParameter()
{}

std::string
CDoubleParameter::getStringFromValue ( ) const
{
    return CStringOp::numToString<double> ( m_value_d,  0, std::numeric_limits<float>::digits );
}

bool
CDoubleParameter::setValueFromString ( std::string f_val_str )
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
        m_value_d = (double)val_d;
        updateInitialValue();
        return update();
    }
    
    /// Else do not change nothing and return false.
    return false;
}

double
CDoubleParameter::getValue ( ) const
{
    return m_value_d;
}

bool 
CDoubleParameter::setValue ( double f_value_d,
                             bool   f_shouldUpdate_b  )
{
    if ( f_value_d == m_value_d )
        return true;

    m_value_d = f_value_d;

    if ( getEditor() )
        ((::QWidget*)getEditor()) -> update();

    if ( f_shouldUpdate_b )
        return update();
    return true;
}

::QWidget *
CDoubleParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CDoubleParameterEditor ( this ));
}

