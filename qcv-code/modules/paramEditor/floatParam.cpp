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
 * \file  floatParam
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>
#include "floatParam.h"
#include "stringOp.h"
#include "floatParamEditor.h"

#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <limits>

using namespace QCV;

CFloatParameter::CFloatParameter ( std::string               f_name_str, 
                                   std::string               f_comment_str,
                                   float                     f_value_f,
                                   CParameterBaseConnector * f_connector_p )
        : CParameter ( f_name_str, f_comment_str, f_connector_p ),
          m_value_f (                  f_value_f )
{
    /// update string value.
    CParameter::updateInitialValue();
    update();
}

CFloatParameter::~CFloatParameter()
{}

std::string
CFloatParameter::getStringFromValue ( ) const
{
    return CStringOp::numToString<float> ( m_value_f, 0, std::numeric_limits<float>::digits );
}

bool
CFloatParameter::setValueFromString ( std::string f_val_str )
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
        m_value_f = (float)val_d;
        updateInitialValue();
        return update();
    }
    
    /// Else do not change nothing and return false.
    return false;
}

float CFloatParameter::getValue ( ) const
{
    return m_value_f;
}

bool CFloatParameter::setValue ( float f_value_f,
                                 bool  f_shouldUpdate_b )
{
    if ( f_value_f == m_value_f )
        return true;

    m_value_f = f_value_f;

    if ( f_shouldUpdate_b )
        return update();

    return true;
}

::QWidget *
CFloatParameter::createEditor ( )
{
    return  (m_qtEditor_p = new CFloatParameterEditor ( this ));
}
