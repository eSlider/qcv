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

#ifndef __CLIPLINE_H
#define __CLIPLINE_H

#include <math.h>

/*@@@**************************************************************************
 ** \file  clipline
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */

/* CONSTANTS */

/* *************************** METHOD ************************************** */
/**
 * Clipps a line.
 *
 * \brief          Clipps a line.
 * \author         Hernan Badino
 *
 * \remarks        - 
 * \sa             -
 *
 * \param          - fr_u1_f     start u coord line position
 * \param          - fr_v1_f     start v coord line position
 * \param          - fr_u2_f     end u coord line position
 * \param          - fr_v2_f     end v coord line position
 * \param          - f_lef_f     left coordinate of clipping region.
 * \param          - f_top_f     top coordinate of clipping region.
 * \param          - f_rig_f     right coord. of clipping region.
 * \param          - f_bot_f     bottom coord. of clipping region.
 * \return         sucess of clipping.
 *
 *************************************************************************** */

inline
bool clipLine( float &fr_u1_f, float &fr_v1_f,
               float &fr_u2_f, float &fr_v2_f,
               float f_lef_f,  float f_top_f,
               float f_rig_f,  float f_bot_f )
{
    
    if ( ( fr_u1_f < f_lef_f && 
           fr_u2_f < f_lef_f ) ||
         ( fr_u1_f > f_rig_f && 
           fr_u2_f > f_rig_f ) ||
         ( fr_v1_f < f_top_f && 
           fr_v2_f < f_top_f ) ||
         ( fr_v1_f > f_bot_f && 
           fr_v2_f > f_bot_f ) )
        return false;

    float f_du_f = fr_u2_f - fr_u1_f;
    float f_dv_f = fr_v2_f - fr_v1_f;

    if (fabs(f_du_f) < 1.e-6 && fabs(f_dv_f) < 1.e-6)
        return false;

    if (fabs(f_du_f) < 1.e-6)
    {
        fr_v1_f = std::max(std::min(fr_v1_f, f_bot_f), f_top_f);
        fr_v2_f = std::max(std::min(fr_v2_f, f_bot_f), f_top_f);
    }
    else if (fabs(f_dv_f) < 1.e-6)
    {
        fr_u1_f = std::max(std::min(fr_u1_f, f_rig_f), f_lef_f);
        fr_u2_f = std::max(std::min(fr_u2_f, f_rig_f), f_lef_f);
    }
    else
    {
        float f_limit_f;
        float f_a_f;

        f_a_f = f_dv_f / f_du_f;        
        
        f_limit_f = f_lef_f;
        if (fr_u1_f < f_limit_f)
        {
            if (f_du_f)
                fr_v1_f += (f_limit_f-fr_u1_f) * f_a_f;
            fr_u1_f = f_limit_f;
        }
        
        if (fr_u2_f < f_limit_f)
        {
            if (f_du_f)
                fr_v2_f += (f_limit_f-fr_u2_f) * f_a_f;
            fr_u2_f = f_limit_f;
        }
        
        f_limit_f = f_rig_f;
        if (fr_u1_f > f_limit_f)
        {
            if (f_du_f)
                fr_v1_f += (f_limit_f-fr_u1_f) * f_a_f;
            fr_u1_f = f_limit_f;
        }
        
        if (fr_u2_f > f_limit_f)
        {
            if (f_du_f)
                fr_v2_f += (f_limit_f-fr_u2_f) * f_a_f;
            fr_u2_f = f_limit_f;
        }

        //////
        if (f_dv_f)
            f_a_f = f_du_f / f_dv_f;
        
        f_limit_f = f_top_f;
        if (fr_v1_f < f_limit_f)
        {
            if (f_dv_f)
                fr_u1_f += (f_limit_f-fr_v1_f) * f_a_f;
            fr_v1_f = f_limit_f;
        }
        
        if (fr_v2_f < f_limit_f)
        {
            if (f_dv_f)
                fr_u2_f += (f_limit_f-fr_v2_f) * f_a_f;
            fr_v2_f = f_limit_f;
        }
        
        
        f_limit_f = f_bot_f;
        if (fr_v1_f > f_limit_f)
        {
            if (f_dv_f)
                fr_u1_f += (f_limit_f-fr_v1_f) * f_a_f;
            fr_v1_f = f_limit_f;
        }
        
        f_limit_f = f_bot_f;
        if (fr_v2_f > f_limit_f)
        {
            if (f_dv_f)
                fr_u2_f += (f_limit_f-fr_v2_f) * f_a_f;
            fr_v2_f = f_limit_f;
        }
    }
    
    return true;
}

#endif // __CLIPLINE_H
