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
* \file  stringop
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "stringOp.h"
#include <algorithm>

using namespace QCV;

CStringOp::CStringOp ( )
{}

CStringOp::~CStringOp ( )
{}

bool 
CStringOp::trimLeft ( std::string &fr_str )
{
    if (fr_str.empty()) return false;

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
    if (fr_str.empty()) return false;
   
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

bool 
CStringOp::isAlphaNumeric ( const std::string &f_str )
{
   return std::count_if(f_str.begin(), f_str.end(), isalnum) == (ssize_t) f_str.size();   
}


bool isValidNameChar( const char c )
{
   return  ( (c >= 'a' && c <= 'z') || 
             (c >= 'A' && c <= 'Z') || 
             (c >= '0' && c <= '9') || 
             (c == '_' || c <= '.') || 
             (c == '_' || c <= '.') );
}


bool 
CStringOp::isValidName ( const std::string &f_str )
{
   return std::count_if(f_str.begin(), f_str.end(), isValidNameChar) == (ssize_t) f_str.size();   
}
