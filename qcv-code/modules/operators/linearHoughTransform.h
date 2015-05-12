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

#ifndef __LINEARHOUGHTRANSFORM_H
#define __LINEARHOUGHTRANSFORM_H

/**
 *******************************************************************************
 *
 * @file linearHoughTransform.cpp
 *
 * \class CLinearHoughTransform
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Hough transform for finding linear segments.
 *
 * Hough transform for finding linear segments.
 *
 *******************************************************************************/

/* INCLUDES */
#include <opencv/highgui.h>
#include "standardTypes.h"
#include "parameterSet.h"

/* CONSTANTS */

namespace QCV
{
    class CParameterSet;
    
    class CLinearHoughTransform
    {
    /// Constructors/Destructor
    public:
        CLinearHoughTransform();

        CLinearHoughTransform( S2D<double> f_theta,
                               S2D<double> f_range,
                               S2D<double> f_scales );

        CLinearHoughTransform( double f_minTheta_d, 
                               double f_maxTheta_d, 
                               double f_minRange_d, 
                               double f_maxRange_d, 
                               double f_thetaScale_d, 
                               double f_rangeScale_d );

        virtual ~CLinearHoughTransform();
        
    /// Operations
    public:
        void    clear();

        bool    copyFrom ( const CLinearHoughTransform & other );

        CLinearHoughTransform & operator = 
                         ( const CLinearHoughTransform & other );

        bool    addPoint ( double f_x_d, 
                           double f_y_d,
                           double f_weight_d = 1.);

        bool    addPoint ( double f_x_d, 
                           double f_y_d,
                           double f_weight_d,
                           double f_expTheta_d,
                           double f_deltaTheta_d );

        bool    compute ( );

    /// Coordinate transformations.
    public:
        /// Hough to Image space
        bool        hough2Image ( double f_theta_d,
                                  double f_range_d,
                                  double &fr_u_d,
                                  double &fr_v_d ) const;

        /// Image to Hough space
        bool        image2Hough ( double f_u_d,
                                  double f_v_d,
                                  double &fr_theta_d,
                                  double &fr_range_d ) const;

        /// Image to Hough space
        double      theta2Column ( double f_theta_d ) const;

        /// Image to Hough space
        double      column2Theta ( double f_u_d ) const;
        
        /// Image to Hough space
        double      range2Row    ( double f_range_d ) const;

        /// Image to Hough space
        double      row2Range    ( double f_v_d ) const;

    /// Sets and Gets
    public:
        /// Theta
        bool        hasSameFormat ( const CLinearHoughTransform & other ) const;

        bool        setThetaAndRange ( S2D<double> f_theta,
                                       S2D<double> f_range );

        bool        setTheta ( S2D<double> f_theta );
        S2D<double> getTheta ( ) const;

        bool        setMinTheta ( double f_minTheta_d );
        double      getMinTheta ( ) const;
        
        bool        setMaxTheta ( double f_maxTheta_d );
        double      getMaxTheta ( ) const;
        
        /// Range
        bool        setRange ( S2D<double> f_theta );
        S2D<double> getRange ( ) const;

        bool        setMinRange ( double f_minRange_d );
        double      getMinRange ( ) const;
        
        bool        setMaxRange ( double f_maxRange_d );
        double      getMaxRange ( ) const;
        
        /// Scale
        bool        setScale ( S2D<double> f_theta );
        S2D<double> getScale ( ) const;

        bool        setThetaScale ( double f_thetaScale_d );
        double      getThetaScale ( ) const;
        
        bool        setRangeScale ( double f_rangeScale );
        double      getRangeScale ( ) const;


        cv::Mat     getAccumulatorImage ( ) const;
        cv::Mat &   getAccumulatorImageReference ( );

        CParameterSet *   getParameterSet ( std::string f_name_str );

    /// Help functions
    protected: 
        /// Free acumulator
        void    freeAccumulator();

        /// Allocate acumulator
        void    allocateAccumulator();
        
    private:
        /// Accumulator image.
        cv::Mat              m_accumImg;
        
        /// Accumulator image.
        cv::Mat              m_auxImg;
        
        /// Look up vector for cosinus.
        std::vector<double>  m_cosLUV;

        /// Look up vector for sinus.
        std::vector<double>  m_sinLUV;
 
        /// Theta range
        S2D<double>          m_theta;
        
        /// Range "range"
        S2D<double>          m_range;

        /// Scales
        S2D<double>          m_scale;        

        /// Parameter set
        CParameterSet *     m_paramSet_p;
    };
}


#endif // __LINEARHOUGHTRANSFORM_H
