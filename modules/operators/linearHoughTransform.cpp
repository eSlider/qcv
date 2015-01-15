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

/**
 *******************************************************************************
 *
 * @file linearHoughTransform.cpp
 *
 * \class CLInearHoughTransform
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Hough transform for finding linear segments.
 *
 * Hough transform for finding linear segments.
 *
 *******************************************************************************/

/* INCLUDES */
#include "parameterSet.h"
#include "dbl2DParam.h"
#include "paramBaseConnector.h"
#include "linearHoughTransform.h"
 
#include <math.h>

#define NORMALIZE_ANGLE(d) (d)+((d)<0?(((int)(1-(d)/(M_PI)))*(M_PI)):(d)>M_PI?-(((int)((d)/(M_PI)))*(M_PI)):0)

using namespace QCV;

CLinearHoughTransform::CLinearHoughTransform() 
        : m_accumImg (                         ),
          m_auxImg (                           ),
          m_theta (                   0., M_PI ),
          m_range (                  -400, 400 ),
          m_scale (              M_PI/180., 1. ),
          m_paramSet_p (                  NULL )
{
    allocateAccumulator();
}


CLinearHoughTransform::CLinearHoughTransform( double f_minTheta_d, 
                                              double f_maxTheta_d, 
                                              double f_minRange_d, 
                                              double f_maxRange_d, 
                                              double f_thetaScale_d, 
                                              double f_rangeScale_d )
        : m_accumImg (                                    ),
          m_auxImg (                                      ),
          m_theta (            f_minTheta_d, f_maxTheta_d ),
          m_range (            f_minRange_d, f_maxRange_d ),
          m_scale (        f_thetaScale_d, f_rangeScale_d ),
          m_paramSet_p (                             NULL )
          
{
    allocateAccumulator();

}

CLinearHoughTransform::CLinearHoughTransform( S2D<double> f_theta,
                                              S2D<double> f_range,
                                              S2D<double> f_scale )
        : m_accumImg (                         ),
          m_auxImg (                           ),
          m_theta (                    f_theta ),
          m_range (                    f_range ),
          m_scale (                    f_scale ),
          m_paramSet_p (                  NULL )
{
    allocateAccumulator();
}


CLinearHoughTransform::~CLinearHoughTransform()
{
    freeAccumulator();
}


bool
CLinearHoughTransform::setThetaAndRange ( S2D<double> f_theta,
                                          S2D<double> f_range ) 
{
    if (m_theta != f_theta ||
        m_range != f_range )
    {
        freeAccumulator(); 
        m_theta = f_theta;
        m_range = f_range;
        allocateAccumulator();
    }

    return true;
}


S2D<double>
CLinearHoughTransform::getTheta ( ) const
{
    return m_theta;
}

bool
CLinearHoughTransform::setTheta ( S2D<double> f_theta )
{
    //f_theta.min = std::min(std::max(f_theta.min, 0.), M_PI );
    //f_theta.max = std::min(std::max(f_theta.max, 0.), M_PI );

    if ( m_theta != f_theta )
    {
        freeAccumulator(); 
        m_theta = f_theta;
        allocateAccumulator();
    }

    return true;
}

bool
CLinearHoughTransform::setMinTheta ( double f_minTheta_d )
{
    //f_minTheta_d = std::min(std::max(f_minTheta_d, 0.), M_PI );

    if ( m_theta.min != f_minTheta_d )
    {
        freeAccumulator(); 
        m_theta.min = f_minTheta_d;
        allocateAccumulator();
    }

    return true;
}

double
CLinearHoughTransform::getMinTheta ( ) const
{
    return m_theta.min;
}

bool
CLinearHoughTransform::setMaxTheta ( double f_maxTheta_d )
{
    //f_maxTheta_d = std::min(std::max(f_maxTheta_d, 0.), M_PI );

    if ( m_theta.max != f_maxTheta_d )
    {
        freeAccumulator(); 
        m_theta.max = f_maxTheta_d;
        allocateAccumulator();
    }

    return true;

}

double
CLinearHoughTransform::getMaxTheta ( ) const
{
    return m_theta.max;
}

bool
CLinearHoughTransform::setRange ( S2D<double> f_range )
{
    if ( m_range != f_range )
    {
        freeAccumulator(); 
        m_range = f_range;
        allocateAccumulator();
    }

    return true;
}

S2D<double>
CLinearHoughTransform::getRange ( ) const
{
    return m_range;
}

bool
CLinearHoughTransform::setMinRange ( double f_minRange_d )
{
    if ( m_range.min != f_minRange_d )
    {
        freeAccumulator(); 
        m_range.min = f_minRange_d;
        allocateAccumulator();
    }

    return true;
}

double
CLinearHoughTransform::getMinRange ( ) const
{
    return m_range.min;
}

bool
CLinearHoughTransform::setMaxRange ( double f_maxRange_d )
{
    if ( m_range.max != f_maxRange_d )
    {
        freeAccumulator(); 
        m_range.max = f_maxRange_d;
        allocateAccumulator();
    }

    return true;
}

double
CLinearHoughTransform::getMaxRange ( ) const
{
    return m_range.max;
}

        
bool
CLinearHoughTransform::setScale ( S2D<double> f_scale )
{
    if ( m_scale != f_scale )
    {
        freeAccumulator(); 
        m_scale = f_scale;
        allocateAccumulator();
    }

    return true;

}

S2D<double>
CLinearHoughTransform::getScale ( ) const
{
    return m_scale;
}

bool
CLinearHoughTransform::setThetaScale ( double f_thetaScale_d )
{
    if ( m_scale.x != f_thetaScale_d )
    {
        freeAccumulator(); 
        m_scale.x = f_thetaScale_d;
        allocateAccumulator();
    }

    return true;
}

double
CLinearHoughTransform::getThetaScale ( ) const
{
    return m_scale.x;
}

        
bool
CLinearHoughTransform::setRangeScale ( double f_rangeScale_d )
{
    if ( m_scale.y != f_rangeScale_d )
    {
        freeAccumulator(); 
        m_scale.y = f_rangeScale_d;
        allocateAccumulator();
    }

    return true;

}

double
CLinearHoughTransform::getRangeScale ( ) const
{
    return m_scale.y;
}

/// Free acumulator
void
CLinearHoughTransform::freeAccumulator()
{
    m_cosLUV.resize(0);
    m_sinLUV.resize(0);
}

/// Allocate acumulator
void
CLinearHoughTransform::allocateAccumulator()
{
    m_accumImg = cv::Mat( (m_range.max-m_range.min)/m_scale.y + .5,
                          (m_theta.max-m_theta.min)/m_scale.x + .5,
                          CV_32FC1);                          

    m_auxImg = cv::Mat::zeros( m_accumImg.size(),
                               CV_32FC1);

    m_cosLUV.resize(m_accumImg.size().width);
    m_sinLUV.resize(m_accumImg.size().width);

    for (int j = 0; j < m_accumImg.size().width; ++j)
    {
        double theta_d =  m_theta.min + j * m_scale.x;
        m_cosLUV[j] = cos( theta_d );
        m_sinLUV[j] = sin( theta_d );
    }
}

cv::Mat
CLinearHoughTransform::getAccumulatorImage() const
{
    return m_accumImg;
}

cv::Mat &
CLinearHoughTransform::getAccumulatorImageReference() 
{
    return m_accumImg;
}

void
CLinearHoughTransform::clear()
{
    m_auxImg   = cv::Mat::zeros(m_auxImg.size(),   CV_32FC1);
    m_accumImg = cv::Mat::zeros(m_accumImg.size(), CV_32FC1);
}

bool
CLinearHoughTransform::compute ( )
{
    cv::GaussianBlur( m_accumImg, m_auxImg, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    
    bool onlyApplyGauss_b = true;
    
    if (onlyApplyGauss_b)
       m_auxImg.copyTo(m_accumImg);
    else
    {
        int w_i = m_accumImg.size().width;
        int h_i = m_accumImg.size().height;
        
        m_accumImg = cv::Mat::zeros(m_accumImg.size(), CV_32FC1);
        
        float threshold_f = 0.5;
        for (int i = 1; i < h_i - 1; ++i)
        {
            float *  sl0_p = &m_auxImg.at<float>(i-1,+1);
            float *  sl1_p = &m_auxImg.at<float>(i  ,+1);
            float *  sl2_p = &m_auxImg.at<float>(i+1,+1);
            
            float *  slD_p = &m_accumImg.at<float>( i, 0 );
            
            for (int j = 1; j < w_i - 1; ++j, ++sl0_p, ++sl1_p, ++sl2_p, ++slD_p)
            {
                if ( *sl1_p > threshold_f &&
                     *sl1_p > sl0_p[-1] && sl0_p[-1] > 0 && 
                     *sl1_p > sl0_p[ 0] && sl0_p[ 0] > 0 && 
                     *sl1_p > sl0_p[+1] && sl0_p[+1] > 0 && 
                     *sl1_p > sl1_p[-1] && sl1_p[-1] > 0 && 
                     *sl1_p > sl1_p[ 1] && sl1_p[ 1] > 0 && 
                     *sl1_p > sl2_p[-1] && sl2_p[-1] > 0 && 
                     *sl1_p > sl2_p[ 0] && sl2_p[ 0] > 0 && 
                     *sl1_p > sl2_p[ 1] && sl2_p[ 1] > 0 )
                    *slD_p = *sl1_p ;
            }
        }
    }

    return true;
}

bool
CLinearHoughTransform::addPoint ( double f_x_d, 
                                  double f_y_d,
                                  double f_weight_d )
{
    cv::Size size = m_accumImg.size();

    S2D<float> p1, p2;
    for (int j = 0; j < size.width; ++j)
    {
        float value_f = f_weight_d;

        double range_d = f_x_d * m_cosLUV[j] + f_y_d * m_sinLUV[j];

        float i_f = (range_d - m_range.min) / m_scale.y;
        p1 = S2D<float>(j, i_f);

        //printf("p1.x = %f p1.y = %f (p2.x = %f p2.y = %f) i_f = %f range_d = %f m_range.min = %f, m_scale.y = %f\n",
        //       p1.x, p1.y, p2.x, p2.y, i_f, range_d, m_range.min, m_scale.y );

        if (j != 0 && i_f >= 0 && i_f < size.height )
        {   
            int dist_i = (int)fabs(p1.y-p2.y)+1;
            S2D<float> d ((p2.x-p1.x)/dist_i, 
                          (p2.y-p1.y)/dist_i);

            for (int i = 0; i < dist_i; ++i)
            {                
                float pointx = p1.x + (i) * d.x;
                float pointy = p1.y + (i) * d.y;
                
                int intPx = (int)(pointx);
                int intPy = (int)(pointy);
                
                float f1  = (pointx-(int)intPx);
                float f2  = (pointy-(int)intPy);

                float w11 = (1.-f1) * (1.-f2);
                float w12 = (f1) * (1.-f2);
                float w21 = (1.-f1) * (f2);
                float w22 = (f1) * (f2);                

              if ( intPy < (int) size.height-1 && intPy >= 0 && 
                   intPx < (int) size.width-1 && intPx >= 0 )
                {
                    if (i == 0)
                    {
                        m_accumImg.at<float>(intPy,intPx  ) += value_f * w11;
                        m_accumImg.at<float>(intPy,intPx+1) += value_f * w12;
                    }
                    m_accumImg.at<float>(intPy+1, intPx  ) += value_f * w21;
                    m_accumImg.at<float>(intPy+1, intPx+1) += value_f * w22;
                }
            }
        }
        p2=p1;
    }
    
    return true;
}

bool
CLinearHoughTransform::addPoint ( double f_x_d, 
                                  double f_y_d,
                                  double f_weight_d,
                                  double f_expTheta_d,
                                  double f_deltaTheta_d )
{
    cv::Size size = m_accumImg.size();

    S2D<float> p1, p2;

    //f_expTheta_d = NORMALIZE_ANGLE( f_expTheta_d );

    /// Determine the limits of the image to update. Determine also
    /// if two cycles are required because of a the circular sense of the 
    /// accumulator.
    double t1_d = f_expTheta_d - f_deltaTheta_d;
    double t2_d = f_expTheta_d + f_deltaTheta_d;

    //printf("x: %f y: %f f_expTheta_d = %f t1_d = %f t2_d = %f (violation t1: %i t2: %i)\n",
    //       f_x_d, f_y_d, f_expTheta_d, t1_d, t2_d, (int)(m_theta.min > t1_d), (int)(m_theta.max < t2_d) );    

    int minJ[2];
    int maxJ[2];
    int cycles_i = 1;
    
    minJ[0] = (std::max(t1_d, m_theta.min) - m_theta.min) / m_scale.x;
    maxJ[0] = (std::min(t2_d, m_theta.max) - m_theta.min) / m_scale.x + .5;
    
    //printf("\tminJ[0] = %i maxJ[0] = %i\n", minJ[0], maxJ[0] );    

    if ( (m_theta.min > t1_d && m_theta.max >= t2_d) ||
         (m_theta.min <= t1_d && m_theta.max < t2_d) )
    {
        double nt1_d = NORMALIZE_ANGLE(t1_d);
        double nt2_d = NORMALIZE_ANGLE(t2_d);
        
        //printf("\tnt1_d = %f nt2_d = %f\n", nt1_d, nt2_d );
        
        if ( m_theta.min >  t1_d  &&
             m_theta.max >= t2_d  &&
             m_theta.max >= nt1_d &&
             m_theta.min <= nt1_d )
        {
            cycles_i = 2;
            minJ[1] = (nt1_d - m_theta.min) / m_scale.x;
            if (minJ[1] < 0 )
                minJ[1] = 0;
            maxJ[1] = size.width-1;

            //printf("\tcase 1 minJ[1] = %i maxJ[1] = %i\n", minJ[1], maxJ[1] );    
        }
        else if ( m_theta.min <= t1_d  &&
                  m_theta.max <  t2_d  &&
                  m_theta.max >= nt2_d &&
                  m_theta.min <= nt2_d )
        {
            cycles_i = 2;
            minJ[1] = 0;
            maxJ[1] = (nt2_d - m_theta.min) / m_scale.x + .5;
            
            if (maxJ[1] >= (signed) size.width )
                maxJ[1] = size.width-1;
            //printf("\tcase 2 minJ[1] = %i maxJ[1] = %i\n", minJ[1], maxJ[1] );    
        }
    }
    

    if (minJ[0] < 0 )
        minJ[0] = 0;

    if (maxJ[0] >= (signed) size.width )
        maxJ[0] = size.width-1;

    //printf("\tminJ[0] %i maxJ[0] %i diff = %i\n", minJ[0], maxJ[0], maxJ[0]-minJ[0]  );

    
    
    for (int l = 0; l < cycles_i; ++l)
    {
        for (int j = minJ[l]; j <= maxJ[l]; ++j)
        {
            //printf("l= %i j=%i\n",
            //       l, j);
            double range_d = f_x_d * m_cosLUV[j] + f_y_d * m_sinLUV[j];
            
            float i_f = (range_d - m_range.min) / m_scale.y;
            p1 = S2D<float>(j, i_f);

            if (j != minJ[l] && i_f >= 0 && i_f < size.height )
            {   
                int dist_i = (int)fabs(p1.y-p2.y)+1;
                S2D<float> d ((p2.x-p1.x)/dist_i, 
                              (p2.y-p1.y)/dist_i);


                float value_f = f_weight_d;
        
                for (int i = 0; i < dist_i; ++i)
                {
                    float pointx = p1.x + (i) * d.x;
                    float pointy = p1.y + (i) * d.y;

                    int intPx = (int)(pointx);
                    int intPy = (int)(pointy);

                    float f1  = (pointx-(int)intPx);
                    float f2  = (pointy-(int)intPy);

                    float w11 = (1.-f1) * (1.-f2);
                    float w12 = (f1) * (1.-f2);
                    float w21 = (1.-f1) * (f2);
                    float w22 = (f1) * (f2);
                
                    if ( intPy < (int) size.height-1 && intPy >= 0 && 
                         intPx < (int) size.width-1  && intPx >= 0 )
                    {
                        if (i == 0)
                        {
                            //printf("settting (%i, %i) and (%i, %i)\n",
                            //       intPy, intPx, intPy, intPx+1 );
                            
                            m_accumImg.at<float>(intPy,  intPx  ) += value_f * w11;
                            m_accumImg.at<float>(intPy,  intPx+1) += value_f * w12;
                        }
                        //printf("settting (%i, %i) and (%i, %i)\n",
                        //           intPy+1, intPx, intPy+1, intPx+1 );

                        m_accumImg.at<float>(intPy+1,  intPx  ) += value_f * w21;
                        m_accumImg.at<float>(intPy+1,  intPx+1) += value_f * w22;
                    }
                }
            
            }
            p2=p1;
        }
    }
    
    return true;
}

CParameterSet *   
CLinearHoughTransform::getParameterSet ( std::string f_name_str )
{
    if ( !m_paramSet_p )
    {
        m_paramSet_p = new CParameterSet ( NULL );
        m_paramSet_p -> setName ( f_name_str );

        m_paramSet_p -> addParameter (
                new CDbl2DParameter ( "Theta",
                                      "Min and max theta parameters for the accumulator "
                                      "image [deg].",
                                      getTheta(),
                                      "Min.", "Max.",
                                     new CParameterConnector< CLinearHoughTransform, S2D<double>, CDbl2DParameter>
                                     ( this,
                                       &CLinearHoughTransform::getTheta,
                                       &CLinearHoughTransform::setTheta ) ) );
        
        m_paramSet_p -> addParameter (
                new CDbl2DParameter ( "Range",
                                      "Min and max range parameters for the accumulator "
                                      "image [px].",
                                      getRange(),
                                      "Min.", "Max.",
                                     new CParameterConnector< CLinearHoughTransform, S2D<double>, CDbl2DParameter>
                                     ( this,
                                       &CLinearHoughTransform::getRange,
                                       &CLinearHoughTransform::setRange ) ) );
        
        m_paramSet_p -> addParameter (
                new CDbl2DParameter ( "Scale",
                                      "Scale for theta and range.",
                                      getScale(),
                                      "Theta", "Range",
                                      new CParameterConnector< CLinearHoughTransform, S2D<double>, CDbl2DParameter>
                                      ( this,
                                        &CLinearHoughTransform::getScale,
                                        &CLinearHoughTransform::setScale ) ) );
    }

    return m_paramSet_p;
}

/// Hough to Image space
bool
CLinearHoughTransform::hough2Image ( double f_theta_d,
                                     double f_range_d,
                                     double &fr_u_d,
                                     double &fr_v_d ) const
{
    fr_u_d = theta2Column ( f_theta_d );
    fr_v_d = range2Row    ( f_range_d );
    return true;
}


/// Image to Hough space
bool
CLinearHoughTransform::image2Hough ( double f_u_d,
                                     double f_v_d,
                                     double &fr_theta_d,
                                     double &fr_range_d ) const
{
    fr_theta_d = column2Theta ( f_u_d );
    fr_range_d = row2Range    ( f_v_d );
    return true;
}

/// Image to Hough space
double
CLinearHoughTransform::theta2Column ( double f_theta_d ) const
{
    //f_theta_d  = NORMALIZE_ANGLE(f_theta_d);    

    return (f_theta_d - m_theta.min) / m_scale.x;
    
}

/// Image to Hough space
double
CLinearHoughTransform::column2Theta ( double f_u_d ) const
{
    double theta_d = f_u_d * m_scale.x + m_theta.min;

    //theta_d = NORMALIZE_ANGLE(theta_d);    

    return theta_d;
}       

/// Image to Hough space
double
CLinearHoughTransform::range2Row ( double f_range_d ) const
{
    return (f_range_d - m_range.min) / m_scale.y;
}

/// Image to Hough space
double
CLinearHoughTransform::row2Range ( double f_v_d ) const
{
    return f_v_d * m_scale.y + m_range.min;
}

/// Copy from other object.
bool
CLinearHoughTransform::copyFrom ( const CLinearHoughTransform & other )
{
    freeAccumulator();
    m_theta = other.m_theta;
    m_range = other.m_range;
    m_scale = other.m_scale;    
    allocateAccumulator();

    other.m_accumImg.copyTo(m_accumImg);
    other.m_auxImg.copyTo(m_auxImg);

    return true;
}

/// Copy from other object.
CLinearHoughTransform & 
CLinearHoughTransform::operator = ( const CLinearHoughTransform & other )
{
    copyFrom ( other );
    
    return *this;
}

/// Copy from other object.
bool
CLinearHoughTransform::hasSameFormat ( const CLinearHoughTransform & other ) const
{
    const double e = 1.e-7;
    
    return ( fabs(m_theta.min - other.m_theta.min) < e && 
             fabs(m_theta.max - other.m_theta.max) < e && 
             fabs(m_range.min - other.m_theta.min) < e && 
             fabs(m_range.max - other.m_theta.max) < e && 
             fabs(m_scale.min - other.m_theta.min) < e && 
             fabs(m_scale.max - other.m_theta.max) < e );
}

        
