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

#ifndef __COLORENCODINGPARAM_H
#define __COLORENCODINGPARAM_H

#include "parameterSet.h"
#include "colorEncoding.h"

namespace QCV
{

    static
    CEnumParameter<CColorEncoding::EColorEncodingType_t> *
    createEnumParameter ( CColorEncoding        &f_ce,
                          std::string            f_name_str,
                          std::string            f_comment_str )
    {
        CEnumParameter<CColorEncoding::EColorEncodingType_t> * param_p = 
            new CEnumParameter<CColorEncoding::EColorEncodingType_t> 
            ( f_name_str, f_comment_str, f_ce.getColorEncodingType(), "None",
              new CParameterConnector< CColorEncoding, CColorEncoding::EColorEncodingType_t, CEnumParameter<CColorEncoding::EColorEncodingType_t> >
              ( &f_ce,
                &CColorEncoding::getColorEncodingType,
                &CColorEncoding::setColorEncodingType ) );

        //param_p -> addDescription ( CColorEncoding::CET_INVALID, "Invalid");
        param_p -> addDescription ( CColorEncoding::CET_GREEN2RED, "Green to Red" );
        param_p -> addDescription ( CColorEncoding::CET_RED2GREEN, "Red to Green" );
        param_p -> addDescription ( CColorEncoding::CET_BLUE2RED, "Blue to Red" );
        param_p -> addDescription ( CColorEncoding::CET_RED2BLUE, "Red to Blue" );
        param_p -> addDescription ( CColorEncoding::CET_BLUE2GREEN2RED, "Blue to Green to Red" );
        param_p -> addDescription ( CColorEncoding::CET_RED2GREEN2BLUE, "Red to Green to Blue" );
        param_p -> addDescription ( CColorEncoding::CET_RED2DARKGREEN, "Red to Dark Green" );
        param_p -> addDescription ( CColorEncoding::CET_GREEN2DARKRED, "Green to Dark Red" );
        param_p -> addDescription ( CColorEncoding::CET_BLACK2CYAN, "Black to Cyan" );
        param_p -> addDescription ( CColorEncoding::CET_CYAN2BLACK, "Cyan to Black" );
        param_p -> addDescription ( CColorEncoding::CET_BLACK2RED2YELLOW, "Black to Red Yellow" );
        param_p -> addDescription ( CColorEncoding::CET_YELLOW2RED2BLACK, "Yellow to Red Black" );
        param_p -> addDescription ( CColorEncoding::CET_HUE, "Hue" );
        param_p -> addDescription ( CColorEncoding::CET_WHITE2BLACK, "White to Black" );
        param_p -> addDescription ( CColorEncoding::CET_BLACK2WHITE, "Black to White" );

        return param_p;
    } 

    inline bool
    addColorEncodingParameter ( CColorEncoding        &f_ce,
                                CParameterSet *        fr_paramSet_p,
                                std::string            f_name_str,
                                std::string            f_comment_str )
    {
        fr_paramSet_p -> addParameter ( createEnumParameter ( f_ce, f_name_str + " Color Mode", f_comment_str ) );

        std::string str = f_name_str + std::string(" Range");
    
        fr_paramSet_p -> addParameter (
            new CFlt2DParameter ( str, f_comment_str, f_ce.getMinMaxRange(), "Min", "Max",
                                  new CParameterConnector< CColorEncoding, S2D<float>, CFlt2DParameter >
                                  ( &f_ce,
                                    &CColorEncoding::getMinMaxRange,
                                    &CColorEncoding::setMinMaxRange ) ) );
    
        return true;
    }

} // Namespace QCV
#endif // __COLORENCODINGPARAM_H
