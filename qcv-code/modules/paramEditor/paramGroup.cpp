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
* \file  parameterGroup.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "paramGroup.h"
using namespace QCV;

CParameterGroup::CParameterGroup ( std::string   f_name_str,
                                   bool          f_applyColor_b,
                                   SRgb          f_color )
        : CParameter( f_name_str, "", NULL ),
          m_applyColor_b (  f_applyColor_b ),
          m_color (                f_color )
{
}

CParameterGroup::~CParameterGroup()
{
}

std::string
CParameterGroup::getStringFromValue ( ) const
{
    return "";
}

bool
CParameterGroup::setValueFromString ( std::string /*f_val_str*/ )
{
    return false;
}
