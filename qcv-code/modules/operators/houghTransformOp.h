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

#ifndef __HOUGHTRANSFORMOP_H
#define __HOUGHTRANSFORMOP_H


/*@@@**************************************************************************
 ** \file  houghTransformOp
 * \author Hernan Badino
 * \notes  
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>


#include "operator.h"
#include "linearHoughTransform.h"
#include "colorEncoding.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    struct SHoughLine
    {
        float x_f;
        float y_f;
        float val_f;

        SHoughLine () { x_f = y_f = val_f = 0.f; }
            
        SHoughLine (float f_x_f, float f_y_f, float f_val_f )
            : x_f (     f_x_f ),
              y_f (     f_y_f ),
              val_f ( f_val_f ) {}

        bool operator < ( const SHoughLine & f_other ) const
        {
            return val_f > f_other.val_f;
        }
    };

    typedef std::vector<SHoughLine> THoughLineVector;

    class CHoughTransformOp: public QCV::COperator
    {
    /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CHoughTransformOp ( COperator * const f_parent_p = NULL,
                            std::string       f_name_str = "Hough Transform" );
        
        /// Virtual destructor.
        virtual ~CHoughTransformOp ();

        /// Cycle event.
        virtual bool cycle();
    
        /// Show event.
        virtual bool show();
    
        /// Init event.
        virtual bool initialize();
    
        /// Reset event.
        virtual bool reset();
    
        /// Exit event.
        virtual bool exit();

    /// User Operation Events
    public:
        /// Key pressed in display.
        virtual void keyPressed (     CKeyEvent * f_event_p );

        /// Mouse moved.
        virtual void mouseMoved (     CMouseEvent * f_event_p );

    /// Gets and sets.
    public:

        ADD_PARAM_ACCESS(std::string, m_inputId_str,       InputId  );
        ADD_PARAM_ACCESS(std::string, m_lineVectorId_str,  OutputLineVectorId );
        ADD_PARAM_ACCESS(std::string, m_accumImgId_str,    OutputAccumImgId );
     
        ADD_PARAM_ACCESS(float,       m_gradThreshold_f,   GradThreshold  );
        ADD_PARAM_ACCESS(double,      m_deltaTheta_d,      DeltaTheta     );
        ADD_PARAM_ACCESS(bool,        m_compute_b,         Compute        );
        ADD_PARAM_ACCESS(double,      m_magnitudeNorm_d,   MagnitudeNorm  );

        ADD_PARAM_ACCESS(float,       m_minHoughVal_f,     MinHoughValue  );

        ADD_PARAM_ACCESS(S2D<int>,    m_roiTopLeft,        RoiTopLeft );
        ADD_PARAM_ACCESS(S2D<int>,    m_roiBottomRight,    RoiBottomRight );
        ADD_PARAM_ACCESS(int,         m_showMaxLines_i,    ShowMaxLines );

        ADD_PARAM_ACCESS(S2D<float>,  m_minHoughDistance,  MinHoughDistance );
    public:

        CLinearHoughTransform *  getLinearHoughTransformOp();
        cv::Mat                  getAccumulatorImage();
        cv::Mat                  getGradientXImage();
        cv::Mat                  getGradientYImage();
        cv::Mat                  getBinaryImage() const;

    /// Data types
    public:

    protected:
        /// Set the input of this operator
        bool getInput  ( );

        void registerParameters();
        void registerDrawingLists();
        bool computeBinaryImage( const cv::Mat & f_gradX,
                                 const cv::Mat & f_gradY,
                                 cv::Mat       & fr_binImg );
    private:
        
        /// Input id
        std::string                m_inputId_str;

        /// Output id
        std::string                m_lineVectorId_str;

        /// Output id
        std::string                m_accumImgId_str;

        /// Accumulator image.
        cv::Mat                    m_srcImg;        

        /// Hough Transform
        CLinearHoughTransform      m_houghTransOp;

        /// Result of prewitt
        cv::Mat                    m_gradX;

        /// Result of prewitt
        cv::Mat                    m_gradY;

        /// Gradient magnitude.
        cv::Mat                    m_gradient;

        /// Binary image.
        cv::Mat                    m_binImg;

        /// Binary image.
        cv::Mat                    m_binImg2;

        /// Auxiliar Binary image.
        cv::Mat                    m_auxBinImg;

        /// Gradient threshold
        float                      m_gradThreshold_f;

        /// Interval of theta registration in the accumulator [deg].
        double                     m_deltaTheta_d;

        /// Interval of theta registration in the accumulator [deg].
        double                     m_magnitudeNorm_d;

        /// Compute  hough transform?
        bool                       m_compute_b;

        /// Hough accumulator gradient 
        cv::Mat                    m_gradHX;

        /// Hough accumulator gradient 
        cv::Mat                    m_gradHY;

        /// Color encoding for hough accumulator images.
        CColorEncoding             m_colorEncHoughImg;

        /// Min value for extracting lines
        float                      m_minHoughVal_f;

        /// Vector with extracted lines
        THoughLineVector           m_lines;
      
        /// Show only top X lines.
        int                        m_showMaxLines_i;
        
        /// ROI
        S2D<int>                   m_roiTopLeft;

        /// ROI
        S2D<int>                   m_roiBottomRight;

        /// ROI
        S2D<float>                 m_minHoughDistance;
     };
}

#endif // __HOUGHTRANSFORMOP_H
