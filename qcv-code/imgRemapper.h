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

#ifndef __REMAPPER_H
#define __REMAPPER_H

/**
*******************************************************************************
*
* @file   imgRemapper.h
*
* \class  CImgRemapper
* \author Hernan Badino (hernan.badino@gmail.com)
*
* Remaps images by using the provided LUT.
*
*******************************************************************************
*****             (C) Hernan Badino 2011 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <opencv2/imgproc/imgproc.hpp>
#include "s2d.h"

/* CONSTANTS */

namespace QCV
{
    class CImgRemapper
    {
    /// Some public enumerators.
    public:
        CImgRemapper();
        ~CImgRemapper();

    public:
        bool load    ( std::string f_file_str  );
        
        bool remap ( const cv::Mat  &f_input,
                     cv::Mat        &fr_output );

       void clearLuts ( );

    public:

        bool setNNInterpolation( bool f_val_b );
        bool getNNInterpolation( ) const;

    public:

        bool   setOutputSize(S2D<unsigned int> f_size);
        S2D<unsigned int>
               getOutputSize() const { return S2D<unsigned int>(getOutputWidth(), getOutputHeight()); }
        int    getOutputWidth() const { return m_outWidth_i; }
        int    getOutputHeight() const { return m_outHeight_i; }

        
        bool   setOffset(S2D<unsigned int> f_offset) { m_offsetU_i=f_offset.x; m_offsetV_i=f_offset.y; return true;}
        S2D<unsigned int>
               getOffset() const { return S2D<unsigned int>(m_offsetU_i, m_offsetV_i); }
        int    getOffsetU() const { return m_offsetU_i; }
        int    getOffsetV() const { return m_offsetV_i; }

        cv::Mat
               getLutU() const {return m_lutU;}

        cv::Mat
               getLutV() const {return m_lutV;}

        cv::Mat &
               getLutUReference() {return m_lutU;}

        cv::Mat &
               getLutVReference() {return m_lutV;}

    private:
        
    private:

        /// LUT for U
        cv::Mat               m_lutU;

        /// LUT for V
        cv::Mat               m_lutV;
        
        /// Output width
        int                   m_outWidth_i;

        /// Output height
        int                   m_outHeight_i;

        /// Output width
        int                   m_offsetU_i;

        /// Output height
        int                   m_offsetV_i;
        
        /// NN interpolation?
        bool                  m_nnInterpolation_b;
    };
}



#endif // __REMAPPER_H
