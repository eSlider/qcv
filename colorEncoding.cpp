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
* \file  colorEncoding.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

#include "colorEncoding.h"
#include <math.h>
#include <stdint.h>

using namespace QCV;

CColorEncoding::CColorEncoding( const EColorEncodingType_t f_type_e,
                                S2D<float>                 f_range )
        : m_encodingType_e( f_type_e ),
          m_range (          f_range ),
          m_logarithmic (      false )
{
    recomputeLut();
}

CColorEncoding::~CColorEncoding()
{
}

bool
CColorEncoding::colorFromValue ( const float      f_value_f,
                                 SRgb            &fr_color ) const
{
    const bool useLut_b = false;
    
    if ( !useLut_b )
        return colorFromValue ( f_value_f, 
                                m_range.min, 
                                m_range.max, 
                                fr_color );
    else
        return colorFromValueLUT ( f_value_f, 
                                   fr_color );
}

bool
CColorEncoding::colorFromValue ( const float      f_value_f,
                                 const S2D<float> f_range,
                                 SRgb            &fr_color ) const
{
    return colorFromValue ( f_value_f, 
                            f_range.min, 
                            f_range.max, 
                            fr_color );
}

bool
CColorEncoding::colorFromValue ( float  f_value_f,
                                 float  f_min_f,
                                 float  f_max_f,
                                 SRgb  &fr_color ) const
{
    if (m_logarithmic)
    {
        f_value_f = log10f( f_value_f );
        f_min_f   = log10f( f_min_f );
        f_max_f   = log10f( f_max_f );
    }
    
    switch ( m_encodingType_e )
    {
        case CET_BLACK2WHITE :
            encodeBlackToWhite ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_WHITE2BLACK :
            encodeBlackToWhite ( f_value_f, f_min_f, f_max_f, fr_color, true );
            break;

        case CET_GREEN2RED :
            encodeGreen2Red ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_RED2GREEN:
            encodeGreen2Red ( f_value_f, f_max_f, f_min_f, fr_color, true );
            break;

        case CET_BLUE2RED  :
            encodeBlue2Red ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_RED2BLUE:
            encodeBlue2Red ( f_value_f, f_min_f, f_max_f, fr_color, true );
            break;

        case CET_BLUE2GREEN2RED:
            encodeBlue2Green2Red( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_RED2GREEN2BLUE:
            encodeBlue2Green2Red( f_value_f, f_min_f, f_max_f, fr_color, true );
            break;
        
        case CET_RED2DARKGREEN:
            encodeRed2DarkGreen ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_GREEN2DARKRED:
            encodeRed2DarkGreen ( f_value_f, f_min_f, f_max_f, fr_color, true );
            break;
            
        case CET_BLACK2CYAN:
            encodeBlack2Cyan ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_CYAN2BLACK:
            encodeBlack2Cyan ( f_value_f, f_min_f, f_max_f, fr_color, true );
            break;

        case CET_BLACK2RED2YELLOW:
            encodeBlack2Red2Yellow ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_YELLOW2RED2BLACK:
            encodeBlack2Red2Yellow ( f_value_f, f_min_f, f_max_f, fr_color, true );
            break;
            
        case CET_HUE:
            encodeHue ( f_value_f, f_min_f, f_max_f, fr_color, false );
            break;

        case CET_INVALID:
        default:
            return false;
    }

    return true;
}

inline
void
CColorEncoding::encodeBlue2Red ( const float   f_value_f,
                                 const float   f_min_f,
                                 const float   f_max_f,
                                 SRgb          &fr_color,
                                 bool           f_inverse_b )
{
    float dX = (f_value_f - f_min_f) / (f_max_f - f_min_f);

    if ( f_inverse_b )
        dX = 1-dX;
    
    int r, g, b;
    
    if (dX < 0.0)
    {
        r = 0;
        g = 0;
        b = 255;
    }
    else if (dX >= (.99 * .99)) // 
    {
        r = 255;
        g = 0;
        b = 0;
    }
    else
    {
        dX = sqrt( dX );

        r  = int(255.0 * sin(dX * M_PI/ 1.8));
        g  = int(255.0 * sin(dX * M_PI / 0.9));
        b  = int(255.0 * (1.0 - sin(dX * M_PI / 1.8)));
    }

    fr_color.set ( r, g, b );
}


inline
void
CColorEncoding::encodeBlue2Green2Red  ( const float   f_value_f,
                                        const float   f_min_f,
                                        const float   f_max_f,
                                        SRgb          &fr_color,
                                        bool           f_inverse_b )
{
    float dX     = (f_value_f - f_min_f) / (f_max_f - f_min_f);

    if ( f_inverse_b )
        dX = 1-dX;

    int r, g, b;

    if (dX < 0.0)
    {
        r = 0;
        g = 0;
        b = 255;         // Blue
    }
    else if (dX >= (.99 * .99))
    {
        r = 255;
        g = 0;
        b = 0;           // Red
    }
    else
    {
        r = int(255.0 * cos( dX * M_PI + M_PI ));
        g = int(255.0 * sin( 2.0 * dX * M_PI - (M_PI/2.0) ));
        b = int(255.0 * cos( dX * M_PI ));
    }

    fr_color.set ( r, g, b );
}

inline
void
CColorEncoding::encodeGreen2Red  ( const float   f_value_f,
                                   const float   f_min_f,
                                   const float   f_max_f,
                                   SRgb          &fr_color,
                                   bool           f_inverse_b )
{
    float dX     = (f_value_f - f_min_f) / (f_max_f - f_min_f);
    
    if ( f_inverse_b )
        dX = 1-dX;

    int r, g, b;

    b  = 0;    // Blue component is always 0
    
    if (dX < 0.0)
    {
        r = 0;
        g = 255;         // Green
    }
    else if (dX >= 1.0)
    {
        r = 255;
        g = 0;           // Red
    }
    else
    {
        r   = int(510.0 * dX);
        g   = int((-510.0 * dX) + 510.0);
    }

    fr_color.set ( r, g, b );
}

inline
void
CColorEncoding::encodeRed2DarkGreen ( const float   f_value_f,
                                      const float   f_min_f,
                                      const float   f_max_f,
                                      SRgb          &fr_color,
                                      bool          f_inverse_b )
{
    float dX     = (f_value_f - f_min_f) / (f_max_f - f_min_f);
    
    if ( f_inverse_b )
        dX = 1-dX;

    int r, g, b = 0;

    if (dX > 1.0) // ( f_value_f > f_max_f )
    {   
        g = (int)( 382.5 - dX * 127.5);
        
        if ( g > 255) 
            g = 255;
        if ( g < 63) 
            g = 63;    // Dark Green Limit
        
        r = 0;
    }
    else if ( dX < 0.0 ) // if ( f_value_f < f_min_f )
    {
        // If below min value
        r = 255;
        g = 0;   // RED
    }
    else
    {
        g   = int(510.0 * dX);
        r = int((-510.0 * dX) + 510.0);
 
    }

    fr_color.set ( r, g, b );
}

inline
void
CColorEncoding::encodeRed2BlackGreen ( const float   f_value_f,
                                       const float   f_min_f,
                                       const float   f_max_f,
                                       SRgb          &fr_color,
                                       bool           f_inverse_b )
{
    float dX     = (f_value_f - f_min_f) / (f_max_f - f_min_f);
    
    if ( f_inverse_b )
        dX = 1-dX;

    int r, g, b = 0;

    if (dX < 0.0)
    {
        r = 255;
        g = 0;         // Red
    }
    else if (dX >= 1.0)
    {
        r = 0;
        g = 255;       // Green
    }
    else if (dX < 0.5 )
    {    
        r   = int(510.0 * (0.5 - dX) );
        g = 0;
    }
    else
    {
        g   = int(510.0 * (dX - 0.5) );
        r = 0;
    }
    
    fr_color.set ( r, g, b );
}

inline
void
CColorEncoding::encodeBlack2Cyan ( const float   f_value_f,
                                   const float   f_min_f,
                                   const float   f_max_f,
                                   SRgb          &fr_color,
                                   bool           f_inverse_b )
{
    float dX     = (f_value_f - f_min_f) / (f_max_f - f_min_f);
    
    if ( f_inverse_b )
        dX = 1-dX;

    float  b, g;

    // Encode blue.
    b  = 1 - dX;
    b *= b;
    b *= b;
    b  = (1 - b) * 255;
  
    // Encode green.
    g = dX * 255;
  
    fr_color.set ( 0, (int)g, (int)b );
}

inline
void
CColorEncoding::encodeBlack2Red2Yellow (  const float   f_value_f,
                                          const float   f_min_f,
                                          const float   f_max_f,
                                          SRgb          &fr_color,
                                          bool           f_inverse_b )
{
    float dX     = (f_value_f - f_min_f) / (f_max_f - f_min_f);

    if ( f_inverse_b )
        dX = 1-dX;
    
    float  r, g;
    

    // Encode red.
    g = 255 * dX;
    
    // Encode red.
    r = (1-dX);
    r *= r;
    r *= r;    
    r = (1-r) * 255;

    fr_color.set ( (int)r, (int)g, (int)0 );
  
}

inline
void
CColorEncoding::encodeHue ( const float   f_value_f,
                            const float   f_min_f,
                            const float   f_max_f,
                            SRgb         &fr_color,
                            bool          f_inverse_b )
{
    float range_f = f_max_f - f_min_f;

    float val_f = 360*(f_value_f - f_min_f) / range_f;
    
    if (f_inverse_b)
        val_f = 360-val_f;

    SHsv color ( val_f, .75f, .8f);
    
    fr_color = CColor::getRgbFromHsv ( color );
}

inline
void 
CColorEncoding::encodeBlackToWhite (   const float   f_value_f,
                                       const float   f_min_f,
                                       const float   f_max_f,
                                       SRgb         &fr_color,
                                       bool          f_inverse_b )
{
    float delta_f = (f_max_f - f_min_f);
    
    int grey_i = (int)(255.f * (f_value_f - f_min_f) / delta_f);
    
    if ( grey_i < 0   ) grey_i = 0;
    if ( grey_i > 255 ) grey_i = 255;
    
    if (f_inverse_b) grey_i = 255-grey_i;
    
    fr_color.set(grey_i, grey_i, grey_i);
}

// Set the min and max range.
bool
CColorEncoding::setMinMaxRange ( S2D<float> f_range )
{
    if ( m_range != f_range )
        recomputeLut();

    m_range = f_range;
    return true;    
}


// Set the min and max range.
S2D<float>
CColorEncoding::getMinMaxRange ( ) const
{
    return m_range;
}


// Set the min and max range.
bool
CColorEncoding::setMinimum ( float f_min_f )
{
    if (m_range.min != f_min_f )
        recomputeLut();

    m_range.min = f_min_f;
    return true;    
}

// Set the min and max range.
float
CColorEncoding::getMinimum ( ) const
{
    return m_range.min;
}

// Set the min and max range.
bool
CColorEncoding::setMaximum ( float f_max_f )
{
    if (m_range.max != f_max_f )
        recomputeLut();

    m_range.max = f_max_f;
    return true;    
}

// Set the min and max range.
float
CColorEncoding::getMaximum ( ) const
{
    return m_range.max;
}

// Set logarithmic encoding?
bool
CColorEncoding::setLogarithmic ( bool f_log_b )
{
    m_logarithmic = f_log_b;
    return true;
}

// Set the min and max range.
bool 
CColorEncoding::getLogarithmic ( ) const
{
    return m_logarithmic;
}

bool
CColorEncoding::setColorEncodingType( EColorEncodingType_t f_newType_e )
{
    if ( m_encodingType_e !=  f_newType_e )
        recomputeLut();

    m_encodingType_e =  f_newType_e;
    return true;
}

void
CColorEncoding::recomputeLut()
{
    m_lut.clear();
    
    float val_f = m_range.min;
    m_dx_f = (m_range.max-m_range.min)/512.f;
    
    SRgb color;

    for (int i = 0; i < 512; ++i, val_f += m_dx_f)
    {
        colorFromValue ( val_f,
                         m_range.min,
                         m_range.max,
                         color );
        m_lut.push_back ( color );
    }    

    m_lut.push_back ( color );
}


