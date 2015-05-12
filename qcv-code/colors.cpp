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
* \file  colors
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "colors.h"

using namespace QCV;

const SRgb QCV::CColor::red     = SRgb(255,0,0);
const SRgb QCV::CColor::blue    = SRgb(0,0,255);
const SRgb QCV::CColor::green   = SRgb(0,255,0);
const SRgb QCV::CColor::white   = SRgb(255,255,255);
const SRgb QCV::CColor::black   = SRgb(0,0,0);
const SRgb QCV::CColor::cyan    = SRgb(0,255,255);
const SRgb QCV::CColor::magenta = SRgb(255,0,255);
const SRgb QCV::CColor::yellow  = SRgb(255,255,0);


SHsl CColor::getHslFromRgb ( const SRgb & f_color )
{
    SHsl newColor;
    
    float max_f;
    float min_f = std::min(std::min(f_color.r, f_color.g), f_color.b);
    
    float scale_f, offset_f;
    
    if (f_color.r >= f_color.g && f_color.r >= f_color.b)
    {
        scale_f  = f_color.g - f_color.b;
        offset_f = 360.f;
        max_f    = f_color.r;
    }
    else if (f_color.g >= f_color.r && f_color.g >= f_color.b)
    {
        scale_f  = f_color.b - f_color.r;
        offset_f = 120.f;
        max_f    = f_color.g;
    }
    else
    {
        scale_f  = f_color.r - f_color.g;
        offset_f = 240.f;
        max_f    = f_color.b;
    }
    
    float max_plus_min_f = min_f + max_f;

    newColor.l = max_plus_min_f / (2.f * 255.f);

    float max_minus_min_f = max_f - min_f;

    if (max_minus_min_f == 0)
    {
        newColor.h = 0.f;
        newColor.s = 0.f;
    }
    else
    {
        newColor.h = 60.f * scale_f / max_minus_min_f + offset_f;
        newColor.h -= ((int)(newColor.h/360.f)) * 360.f;
        
        if ( newColor.l <= .5f )
            newColor.s = max_minus_min_f / max_plus_min_f;
        else
            newColor.s = max_minus_min_f / ((2.f*255.f)-max_plus_min_f);
    }

    return newColor; 
}

SHsv CColor::getHsvFromRgb ( const SRgb & f_color )
{
    SHsv newColor;
    
    float max_f;
    float min_f = std::min(std::min(f_color.r, f_color.g), f_color.b);
    
    float scale_f, offset_f;
    
    if (f_color.r >= f_color.g && f_color.r >= f_color.b)
    {
        scale_f  = f_color.g - f_color.b;
        offset_f = 360.f;
        max_f    = f_color.r;
    }
    else if (f_color.g >= f_color.r && f_color.g >= f_color.b)
    {
        scale_f  = f_color.b - f_color.r;
        offset_f = 120.f;
        max_f    = f_color.g;
    }
    else
    {
        scale_f  = f_color.r - f_color.g;
        offset_f = 240.f;
        max_f    = f_color.b;
    }
    
    newColor.v = max_f/255.f;

    float max_minus_min_f = max_f - min_f;

    if (max_f == 0 || max_f == min_f)
    {
        newColor.h = 0;
        newColor.s = 0;
    }
    else
    {
        newColor.h = 60.f * scale_f / max_minus_min_f + offset_f;
        newColor.h -= ((int)(newColor.h/360.f)) * 360.f;
        
        newColor.s = max_minus_min_f / max_f;
    }

    return newColor; 
}

SRgb CColor::getRgbFromHsl ( const SHsl & f_color )
{
    if (f_color.s == 0)
    {
        int val_i = f_color.l * 255.f;
        return SRgb( val_i, val_i, val_i );
    }
    
    float q_f, p_f, h_f;
    
    if ( f_color.l < .5f )
        q_f = f_color.l * ( 1.f + f_color.s );
    else
        q_f = f_color.l + f_color.s - ( f_color.l * f_color.s );        
    
    p_f = 2.f * f_color.l - q_f;
    
    h_f = f_color.h / 360.f;
    
    float t_p[3], c_p[3];
    
    t_p[0] = h_f + 1.f/3.;
    t_p[1] = h_f;
    t_p[2] = h_f - 1.f/3.;

    for (int i = 0; i < 3; ++i)
    {
        if (t_p[i] < 0) t_p[i] += 1.f;
        if (t_p[i] > 1) t_p[i] -= 1.f;

        if ( t_p[i] < 1.f/6.f )
            c_p[i] = p_f + ((q_f - p_f) * 6.f * t_p[i]);
        else if ( t_p[i] < .5f )
            c_p[i] = q_f;
        else if ( t_p[i] < 2.f/3.f )
            c_p[i] = p_f + ((q_f - p_f) * 6.f * (2.f/3.f * t_p[i]));
        else
            c_p[i] = p_f;
    }

    return SRgb ( (int) (c_p[0] * 255.f),
                  (int) (c_p[1] * 255.f),
                  (int) (c_p[2] * 255.f) );
}

SRgb CColor::getRgbFromHsv ( const SHsv & f_color )
{
    float h_f = f_color.h / 60.f;
    int   h_i = ((int)h_f)%6;
    float f_f = h_f - h_i;

    float p_f = f_color.v * (1.f - f_color.s);
    float q_f = f_color.v * (1.f - f_f * f_color.s);
    float t_f = f_color.v * (1.f - (1.f - f_f) * f_color.s);
    
    float r_f, g_f, b_f;
    
    switch(h_i)
    {
        case 0:
        {
            r_f = f_color.v;
            g_f = t_f;
            b_f = p_f;
        }
        break;

        case 1:
        {
            r_f = q_f;
            g_f = f_color.v;
            b_f = p_f;
        }
        break;

        case 2:
        {
            r_f = p_f;
            g_f = f_color.v;
            b_f = t_f;
        }
        break;

        case 3:
        {
            r_f = p_f;
            g_f = q_f;
            b_f = f_color.v;
        }
        break;

        case 4:
        {
            r_f = t_f;
            g_f = p_f;
            b_f = f_color.v;
        }
        break;

        case 5:
        default:
        {
            r_f = f_color.v;
            g_f = p_f;
            b_f = q_f;
        }
        break;
    }

    return SRgb ( (int) (r_f * 255.f),
                  (int) (g_f * 255.f),
                  (int) (b_f * 255.f) );
}
