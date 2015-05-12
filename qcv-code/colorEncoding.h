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

#ifndef __COLORENCODING_H
#define __COLORENCODING_H

/*@@@**************************************************************************
** \file  operator.h
* \author Hernan Badino
* \notes  
*******************************************************************************
*****         (C) COPYRIGHT Hernan Badino - All Rights Reserved           *****
******************************************************************************/

/* INCLUDES */
#include "colors.h"
#include "standardTypes.h"

#include <string>
#include <vector>

namespace QCV
{
    /// PROTOTYPES
    class CColorEncoding
    {
        /// Public Data Types
    public:

        /// Color encoding type
        typedef enum {
            CET_INVALID = -1,

            CET_GREEN2RED,
            CET_RED2GREEN,

            CET_BLUE2RED,
            CET_RED2BLUE,

            CET_BLUE2GREEN2RED,
            CET_RED2GREEN2BLUE,

            CET_RED2DARKGREEN,
            CET_GREEN2DARKRED,

            CET_BLACK2CYAN,
            CET_CYAN2BLACK,

            CET_BLACK2RED2YELLOW,
            CET_YELLOW2RED2BLACK,

            CET_HUE,

            CET_BLACK2WHITE,
            CET_WHITE2BLACK
            
        } EColorEncodingType_t;

    //// Constructors/Destructor
    public:

        CColorEncoding( const EColorEncodingType_t f_type_i = CET_INVALID,
                        S2D<float>                 f_range  = S2D<float>(0.f, 1.f) );
        

        virtual ~CColorEncoding();

    //// Transformations.
    public:
        // Calculate color from value in given range
        bool       colorFromValue(    float  f_value_f,
                                      float  f_min_f,
                                      float  f_max_f,
                                      SRgb  &fr_color ) const;
        
        // Calculate color from value in given range
        bool       colorFromValue(    const float      f_value_f,
                                      const S2D<float> f_range,
                                      SRgb            &fr_color ) const;

        // Calculate color from value using preset range.
        bool       colorFromValue(    const float      f_value_f,
                                      SRgb            &fr_color ) const;


    /// Parameters.
    public:
        // bool        addParameters ( CParameterSet * fr_paramSet_p,
        //                             std::string     name_str,
        //                             std::string     comment_str );
    /// Gets and Sets
    public:
        
        EColorEncodingType_t getColorEncodingType() const { return m_encodingType_e; }
        bool                 setColorEncodingType( EColorEncodingType_t f_newType_e );
        

        // Set the min and max range.
        bool                 setMinMaxRange ( S2D<float> f_range );

        // Set the min and max range.
        S2D<float>           getMinMaxRange ( ) const;
        
        // Set the min and max range.
        bool                 setMinimum ( float f_min_f );

        // Set the min and max range.
        float                getMinimum ( ) const;

        // Set the min and max range.
        bool                 setMaximum ( float f_max_f );

        // Set the min and max range.
        float                getMaximum ( ) const;

        // Set logarithmic encoding?
        bool                 setLogarithmic ( bool f_log_b );

        // Set the min and max range.
        bool                 getLogarithmic ( ) const;

    /// Help static methods
    protected:
        
        //CEnumParameter<EColorEncodingType_t> *
        //            createEnumParameter ( std::string name_str = "Color encoding",
//                                          std::string comment_str = "" );
        
        static void encodeBlackToWhite (   const float   f_value_f,
                                           const float   f_min_f,
                                           const float   f_max_f,
                                           SRgb         &fr_color,
                                           bool          f_inverse_b );

        static void encodeGreen2Red (      const float   f_value_f,
                                           const float   f_min_f,
                                           const float   f_max_f,
                                           SRgb         &fr_color,
                                           bool          f_inverse_b );
        
        static void encodeBlue2Red (       const float   f_value_f,
                                           const float   f_min_f,
                                           const float   f_max_f,
                                           SRgb         &fr_color,
                                           bool          f_inverse_b = false );

        static void encodeBlue2Green2Red ( const float   f_value_f,
                                           const float   f_min_f,
                                           const float   f_max_f,
                                           SRgb         &fr_color,
                                           bool          f_inverse_b = false );

        static void encodeRed2DarkGreen (  const float   f_value_f,
                                           const float   f_min_f,
                                           const float   f_max_f,
                                           SRgb         &fr_color,
                                           bool          f_inverse_b = false );
    
        static void encodeBlack2Cyan (     const float   f_value_f,
                                           const float   f_min_f,
                                           const float   f_max_f,
                                           SRgb         &fr_color,
                                           bool          f_inverse_b = false );

        static void encodeBlack2Red2Yellow ( const float f_value_f,
                                             const float f_min_f,
                                             const float f_max_f,
                                             SRgb       &fr_color,
                                             bool        f_inverse_b = false );
    
        static void encodeRed2BlackGreen (  const float  f_value_f,
                                            const float  f_min_f,
                                            const float  f_max_f,
                                            SRgb        &fr_color,
                                            bool          f_inverse_b = false );

        static void encodeHue            (  const float  f_value_f,
                                            const float  f_min_f,
                                            const float  f_max_f,
                                            SRgb        &fr_color,
                                            bool         f_inverse_b = false );

    /// Help methods
    protected:

        void recomputeLut();
        
        bool colorFromValueLUT ( const float   f_value_f,
                                 SRgb         &fr_color ) const;
    private:
        
        /// The color encoding scheme
        EColorEncodingType_t  m_encodingType_e;

        /// The color encoding scheme
        S2D<float>            m_range;

        /// Logarithmic?
        bool                  m_logarithmic;

        /// Lookup table
        std::vector<SRgb>     m_lut;

        /// Normalized range
        float                 m_dx_f;
        
        
    };
    
    inline bool 
    CColorEncoding::colorFromValueLUT ( const float   f_value_f,
                                        SRgb         &fr_color ) const
    {
        if (f_value_f <= m_range.min )
            fr_color = m_lut.front();
        else if (f_value_f >= m_range.max )
            fr_color = m_lut.back();
        else
        {
            int idx_i = (f_value_f - m_range.min) / m_dx_f;
            fr_color = m_lut[idx_i];
        }
        
        return true;
    }
}

#endif /* __COLORENCODING_H */
