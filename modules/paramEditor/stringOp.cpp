/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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
* \file  stringop
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "stringOp.h"

using namespace QCV;

CStringOp::CStringOp ( )
{}

CStringOp::~CStringOp ( )
{}

bool 
CStringOp::trimLeft ( std::string &fr_str )
{
    unsigned int i;
        
    for ( i = 0; 
          ( i < fr_str.length() && 
            isspace(fr_str[i]) );
          ++i) { }
    
        
    if (i)
    {
        std::string newStr = fr_str.substr( i );
        fr_str = newStr;
    }
        
    return true;
}

bool 
CStringOp::trimRight ( std::string &fr_str )
{
    int i;
    
    for ( i = fr_str.length() - 1; 
          ( i >= 0 && 
            isspace(fr_str[i]) );
          --i) {}

    if ( i < (int)fr_str.length() - 1 )
    {
        std::string newStr = fr_str.substr( 0, i + 1 );
        fr_str = newStr;
    }
    
    return true;
}

bool 
CStringOp::trim ( std::string &fr_str )
{
    bool res1_b;
    bool res2_b;
    
    res1_b = trimLeft  ( fr_str );
    res2_b = trimRight ( fr_str );

    return res1_b && res2_b;
}
