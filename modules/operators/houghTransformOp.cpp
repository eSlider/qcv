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
 * @file houghTransformOp.cpp
 *
 * \class CHoughTransformOp
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Framework operator class for computing hough transform.
 *
 *******************************************************************************/

/* INCLUDES */
#include "houghTransformOp.h"
#include "drawingList.h"
#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CHoughTransformOp::CHoughTransformOp ( COperator * const f_parent_p,
                                       std::string           f_name_str )
    : COperator (          f_parent_p, f_name_str  ),
      m_inputId_str (                    "Image 0" ),
      m_srcImg (                                   ),
      m_houghTransOp (                             ),
      m_gradX (                                    ),
      m_gradY (                                    ),
      m_binImg (                                   ),
      m_gradThreshold_f (                     0.1f ),
      m_deltaTheta_d (                         10. ),
      m_magnitudeNorm_d (                      10. ),
      m_compute_b (                           true ),
      m_gradHX (                                   ),
      m_gradHY (                                   ),
      m_colorEncHoughImg ( CColorEncoding::CET_HUE, 
                                S2D<float>(0.,20.) ),
      m_minHoughVal_f (                        2.f ),
      m_lines (                                    ),
      m_showMaxLines_i (                        50 ),
      m_roiTopLeft (                        -1, -1 ),
      m_roiBottomRight (                    -1, -1 ),
      m_minHoughDistance (              25.f, 10.f )
{
    registerDrawingLists();
    registerParameters();
}

void
CHoughTransformOp::registerDrawingLists()
{
    /// If no parent, this is the root. Let's let the draing list to
    /// be shown per default

    bool showDef_b = (getParentOp() == NULL);
    
    registerDrawingList ("Input Image", 
                         S2D<int> (2, 0),
                         showDef_b );

    registerDrawingList ("Accumulator", 
                         S2D<int> (0, 0),
                         showDef_b );

    registerDrawingList ("Binary Image", 
                         S2D<int> (1, 0),
                         showDef_b );

    registerDrawingList ("Hough Lines", 
                         S2D<int> (2, 0),
                         showDef_b );

    registerDrawingList ("Maximum in Accumulator",
                         S2D<int> (0, 0),
                         showDef_b );  

    registerDrawingList ("Overlay Hough Line", 
                         S2D<int> (2, 0),
                         showDef_b );

    registerDrawingList ("X Gradient",
                         S2D<int> (0, 1),
                         false );

    registerDrawingList ("Y Gradient",
                         S2D<int> (1, 1),
                         false );

    registerDrawingList ("Overlay Hough Accum Image",
                         S2D<int> (0, 0),
                         true );
}

void
CHoughTransformOp::registerParameters()
{
    CParameterSet * sriSubset_p = m_houghTransOp.getParameterSet("Accumulator");
    m_paramSet_p -> addSubset ( sriSubset_p );

    ADD_BOOL_PARAMETER ( "Compute",
                         "Compute hough transform?",
                         m_compute_b,
                         this,
                         Compute,
                         CHoughTransformOp );

    ADD_INT2D_PARAMETER ( "Roi Top-Left",
                          "Top left corner of roi. If negative, (0,0) is used.",
                          m_roiTopLeft,
                          "x", "y",
                          this,
                          RoiTopLeft,
                          CHoughTransformOp );

    ADD_INT2D_PARAMETER ( "Roi Bottom-Right",
                          "Bottom right corner of roi. If negative, (width-1,height-1) is used.",
                          m_roiBottomRight,
                          "x", "y",
                          this,
                          RoiBottomRight,
                          CHoughTransformOp );

    ADD_FLOAT_PARAMETER( "Gradient Threshold", 
                         "Gradient threshold for generating binary image.",
                         m_gradThreshold_f,
                         this,
                         GradThreshold, 
                         CHoughTransformOp );

    ADD_DOUBLE_PARAMETER( "Delta Theta", 
                          "Interval of theta registration in the accumulator [deg].",
                          m_deltaTheta_d,
                          this,
                          DeltaTheta, 
                          CHoughTransformOp );

    ADD_DOUBLE_PARAMETER( "Magnitude Norm", 
                          "Normalize the magnitude with this factor.",
                          m_magnitudeNorm_d,
                          this,
                          MagnitudeNorm, 
                          CHoughTransformOp );
 
    ADD_FLOAT_PARAMETER ( "Min Hough Value",
                          "Min value for exctracting lines form the accumulator.",
                          m_minHoughVal_f,
                          this,
                          MinHoughValue,
                          CHoughTransformOp );

    ADD_FLT2D_PARAMETER ( "Min Hough Value",
                          "Min distance between the found maxima in the accumulator.",
                          m_minHoughDistance,
                          "angle [deg]", "range [px]",
                          this,
                          MinHoughDistance,
                          CHoughTransformOp );

    BEGIN_PARAMETER_GROUP("Display ", false, CColor::red );

      ADD_INT_PARAMETER( "Max Lines", 
                         "Max lines to show.",
                         m_showMaxLines_i,
                         this,
                         ShowMaxLines, 
                         CHoughTransformOp );

      addColorEncodingParameter (  m_colorEncHoughImg,
                                   getParameterSet(),
                                   "Accum Color Encoding", 
                                   "Color encoding for the accumulator images." );

      addDrawingListParameter ( "Input Image", "Grayscale converted input image");

      addDrawingListParameter ( "Accumulator", "Hough accumulator");

      addDrawingListParameter ( "Binary Image", "Binary image of point measurements");

      addDrawingListParameter ( "Hough Lines", "Hough lines to show on top of input image");
      addDrawingListParameter ( "Maximum in Accumulator", "Maxima shown on accumulator" );
      
      addDrawingListParameter ( "Overlay Hough Line", 
                                "Overlay to show on top of src img for when\n"
                                "the user moves the mouse on the accumulator\n"
                                "by pressing the left mouse key.");

      addDrawingListParameter ( "X Gradient", "X gradient image" );
      addDrawingListParameter ( "Y Gradient", "Y gradient image" );

      addDrawingListParameter ( "Overlay Hough Accum Image", 
                                "Overlay to show on top of the acccumulator img\n"
                                "for when the user moves the mouse on the src img\n"
                                "by pressing the left mouse key.");

    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CHoughTransformOp::~CHoughTransformOp ()
{
}

/// Cycle event.
bool CHoughTransformOp::cycle()
{
    if ( m_compute_b && getInput() )
    {
        startClock ("Cycle: Reallocation and initialization");
        cv::Size size = m_houghTransOp.getAccumulatorImage().size();
        if ( size != m_gradHX.size() )
        {
            m_gradHX = cv::Mat(size, CV_32FC1);
            m_gradHY = cv::Mat(size, CV_32FC1);
        }
        
        m_houghTransOp.clear();
        
        /// Get input from parent.
        if ( m_srcImg.size().width == 0 )
        {
            printf("%s:%i Must provide an input image\n", __FILE__, __LINE__ );            
            return  COperator::cycle();
        }

        if ( m_srcImg.size() != m_gradX.size() )
        {
            m_gradX    = cv::Mat(m_srcImg.size(), CV_32FC1);
            m_gradY    = cv::Mat(m_srcImg.size(), CV_32FC1);
            m_binImg   = cv::Mat(m_srcImg.size(), CV_8UC1);
            m_gradient = cv::Mat(m_srcImg.size(), CV_32FC1);
        }

        stopClock ("Cycle: Reallocation and initialization");

        {
            startClock ("Cycle: Sobel");

            cv::Sobel(m_srcImg,  m_gradY, -1, 0, 1, 3, 1/6., 0, cv::BORDER_DEFAULT);
            cv::Sobel(m_srcImg,  m_gradX, -1, 1, 0, 3, 1/6., 0, cv::BORDER_DEFAULT);    

            stopClock ("Cycle: Sobel");

            startClock ("Cycle: Binary Image Computation");
            computeBinaryImage ( m_gradX, m_gradY, m_binImg);
            stopClock ("Cycle: Binary Image Computation");

            S2D<int> br = m_roiBottomRight;
            S2D<int> tl = m_roiTopLeft;

            cv::Size size = m_gradX.size();

            if (tl.x < 0)  tl.x = 0;
            if (tl.y < 0)  tl.y = 0;
            if (br.x < 0)  br.x = size.width-1;
            if (br.y < 0)  br.y = size.height-1;

            startClock ("Cycle: Accumulation");

            br.x = std::min(std::max(br.x, 1), size.width-2  );
            br.y = std::min(std::max(br.y, 1), size.height-2 );
            tl.x = std::min(std::max(tl.x, 1), size.width-2  );
            tl.y = std::min(std::max(tl.y, 1), size.height-2 );            
            
            double th_d = m_gradThreshold_f * m_gradThreshold_f;
            
            for (int i = tl.y; i <= br.y; ++i)
            {
                unsigned char *  sl_p = &m_binImg.at<unsigned char>(i, tl.x);

                float * gX_p = &m_gradX.at<float>(i,0);
                float * gY_p = &m_gradY.at<float>(i,0);

                for (int j = tl.x; j <=  br.x; ++j, ++sl_p)
                {
                    double magnitude_d = ( gX_p[j] * gX_p[j] + 
                                           gY_p[j] * gY_p[j] );

                    if ( magnitude_d > th_d )
                    {
                        double x = j-m_gradX.size().width/2.;
                        double y = i-m_gradX.size().height/2.;

                        double weight_d = sqrt(magnitude_d)/m_magnitudeNorm_d;
                        
                        if ( m_deltaTheta_d <= 0 )
                            m_houghTransOp.addPoint ( x, 
                                                      y, 
                                                      weight_d );
                        else
                        {
                            double theta = atan( gY_p[j] / gX_p[j] );
                            
                            if (theta < 0 )
                                theta+=M_PI;

                            if (theta > M_PI)
                                theta-=M_PI;
                        
                            m_houghTransOp.addPoint ( x, 
                                                      y, 
                                                      weight_d,
                                                      theta, 
                                                      m_deltaTheta_d/180. * M_PI );
                        }
                    }
                }
            }

            stopClock ("Cycle: Accumulation");

            startClock ("Cycle: Line Extraction");

            m_houghTransOp.compute();

            cv::Mat accumImg = m_houghTransOp.getAccumulatorImage();

            m_lines.clear();
            for (int i = 3; i < (signed)accumImg.size().height-3; ++i)
            {
                float *  sl0_p = &accumImg.at<float>(i-1, 3);
                float *  sl1_p = &accumImg.at<float>(i,   3);
                float *  sl2_p = &accumImg.at<float>(i+1, 3);

                for (int j = 3; j < (signed)accumImg.size().width-3; ++j, sl0_p++, sl1_p++, sl2_p++ )
                {
                    if ( *sl1_p >= m_minHoughVal_f && 
                         *sl1_p > *sl0_p && 
                         *sl1_p > *sl2_p &&
                         *sl1_p > sl0_p[-1] &&
                         *sl1_p > sl0_p[+1] &&
                         *sl1_p > sl2_p[-1] &&
                         *sl1_p > sl2_p[+1] &&
                         *sl1_p > sl1_p[-1] &&
                         *sl1_p > sl1_p[+1] )
                    {
                        m_lines.push_back(SHoughLine(j, i, *sl1_p));
                    }
                }
            }
            
            startClock ("Cycle: Line Extraction: Sort");
            if (m_lines.size() > 0 )
                sort (m_lines.begin(), m_lines.end());
            stopClock ("Cycle: Line Extraction: Sort");

            startClock ("Cycle: Line Extraction: Min distance");

            if ( m_minHoughDistance.x > 0 ||
                 m_minHoughDistance.y > 0 )
            {
                S2D<float> houghDist( std::max(m_minHoughDistance.x/180.f*(float)M_PI / m_houghTransOp.getThetaScale(), 1.),
                                      std::max(m_minHoughDistance.y / m_houghTransOp.getRangeScale(), 1.));
                
                cv::Mat accumImg = m_houghTransOp.getAccumulatorImage();
                size = accumImg.size();
                m_binImg2 = cv::Mat::zeros(accumImg.size(), m_binImg.type());
                
                for (unsigned int k = 0; k < m_lines.size(); ++k)
                {
                    if (  m_binImg2.at<uint8_t>(m_lines[k].y_f+0.5f,m_lines[k].x_f+0.5f) == 0 )
                    {
                        int top_i = std::max(0.f,             m_lines[k].y_f - houghDist.y/2.f);
                        int bot_i = std::min(size.height-1.f, m_lines[k].y_f + houghDist.y/2.f);
                        int lef_i = std::max(0.f,             m_lines[k].x_f - houghDist.x/2.f);
                        int rig_i = std::min(size.width-1.f,  m_lines[k].x_f + houghDist.x/2.f);
                        
                        for (int i = top_i; i <= bot_i; ++i)
                        {
                            uint8_t *p = &m_binImg2.at<uint8_t>(i,lef_i);
                            for (int j = lef_i; j <= rig_i; ++j, ++p)
                            {
                                *p = 255;
                            }
                        }
                    }
                    else
                    {
                        m_lines.erase(m_lines.begin()+k);
                        --k;
                    }
                }   
            }

            stopClock ("Cycle: Line Extraction: Min distance");
            stopClock ("Cycle: Line Extraction");
        }    
 
        registerOutput<THoughLineVector> ( m_lineVectorId_str, &m_lines );
        registerOutput<cv::Mat>          ( m_accumImgId_str,   &m_houghTransOp.getAccumulatorImageReference() );
        
   }
    
    return COperator::cycle();
}

bool
CHoughTransformOp::computeBinaryImage( const cv::Mat & f_gradX,
                                       const cv::Mat & f_gradY,
                                       cv::Mat       & fr_binImg )
{
    fr_binImg = cv::Mat::zeros(fr_binImg.size(), CV_8UC1);

    int t_i = (int)1;
    int b_i = ((int)f_gradX.size().height) - 2;
    int l_i = (int)1;
    int r_i = ((int)f_gradX.size().width) - 2;

    for (int i = t_i; i <= b_i; ++i)
    {
        const float *  sl0_p = &f_gradY.at<float>(i-1, l_i);
        const float *  sl1_p = &f_gradY.at<float>(i,   l_i);
        const float *  sl2_p = &f_gradY.at<float>(i+1, l_i);

        unsigned char *  bin_p = &fr_binImg.at<unsigned char>(i,  l_i);
        
        for (int j = l_i; j <= r_i; ++j, ++sl0_p, ++sl1_p, ++sl2_p, ++bin_p )
        {
            if ( *sl1_p > m_gradThreshold_f || *sl1_p < -m_gradThreshold_f )
            {
                /// Check if min in vertical direction
                if (  *sl1_p > *sl0_p && 
                      *sl1_p > *sl2_p )
                {
                    /// Check if min in diagonal direction
                    if ( ( *sl1_p > sl0_p[-1]  &&
                           *sl1_p > sl2_p[ 1]  ) || 
                         ( *sl1_p > sl2_p[-1]  &&
                           *sl1_p > sl0_p[ 1]  )  || 
                         ( *sl1_p > sl1_p[-1]  &&
                           *sl1_p > sl1_p[ 1]  ) )
                    {
                        *bin_p = 255;
                    }
                }
                else
                {
                    /// Check if min in vertical direction
                    if (  *sl1_p < *sl0_p && 
                          *sl1_p < *sl2_p )
                    {
                            /// Check if min in diagonal direction
                        if ( ( *sl1_p < sl0_p[-1]  &&
                               *sl1_p < sl2_p[ 1]  ) || 
                             ( *sl1_p < sl2_p[-1]  &&
                               *sl1_p < sl0_p[ 1]  )  || 
                             ( *sl1_p < sl1_p[-1]  &&
                               *sl1_p < sl1_p[ 1]  ) )
                        {
                            *bin_p = 255;
                        }
                    }
                }
            }
        }
    }
    
    
    for (int i = t_i; i <= b_i; ++i)
    {
        const float *  sl0_p = &f_gradX.at<float>(i-1,  l_i);
        const float *  sl1_p = &f_gradX.at<float>(i,    l_i);
        const float *  sl2_p = &f_gradX.at<float>(i+1,  l_i);
        
        unsigned char *  bin_p = &fr_binImg.at<unsigned char>(i, l_i);
        
        for (int j = l_i; j <= r_i; ++j, ++sl0_p, ++sl1_p, ++sl2_p, ++bin_p )
        {
            if ( *sl1_p > m_gradThreshold_f || *sl1_p < -m_gradThreshold_f )
            {
                /// Check if min in horizontal direction
                if (  *sl1_p > sl1_p[-1] && 
                      *sl1_p > sl1_p[ 1] )
                {
                    /// Check if min in diagonal direction
                    if ( ( *sl1_p > sl0_p[-1]  &&
                           *sl1_p > sl2_p[ 1]  ) || 
                         ( *sl1_p > sl2_p[-1]  &&
                           *sl1_p > sl0_p[ 1]  ) || 
                         ( *sl1_p > sl0_p[ 0]  &&
                           *sl1_p > sl2_p[ 0]  ) )
                    {
                        *bin_p = 255;
                    }
                }
                else
                    /// Check if min in vertical direction
                    if (  *sl1_p < sl1_p[-1] && 
                          *sl1_p < sl1_p[ 1] )
                    {
                        /// Check if min in diagonal direction
                        if ( ( *sl1_p < sl0_p[-1]  &&
                               *sl1_p < sl2_p[ 1]  ) || 
                             ( *sl1_p < sl2_p[-1]  &&
                               *sl1_p < sl0_p[ 1]  ) || 
                             ( *sl1_p < sl0_p[ 0]  &&
                               *sl1_p < sl2_p[ 0]  ) )
                        {
                            *bin_p = 255;
                        }
                    }
            }
        }
    }

    return true;
}

/// Show event.
bool CHoughTransformOp::show()
{
    getDrawingList ("Input Image" ) -> clear();
    getDrawingList ("Accumulator" ) -> clear();
    getDrawingList ("Binary Image" ) -> clear();
    getDrawingList ("Hough Lines" ) -> clear();
    getDrawingList ("Maximum in Accumulator" ) -> clear();
    getDrawingList ("Overlay Hough Line" ) -> clear();
    getDrawingList ("X Gradient" ) -> clear();
    getDrawingList ("Y Gradient" ) -> clear();
    getDrawingList ("Overlay Hough Accum Image" ) -> clear();

    if ( m_compute_b )
    {
        /// Check if we can show something.
        if ( m_srcImg.size().width == 0 || 
             m_houghTransOp.getAccumulatorImage().size() != m_gradHX.size() )
        {
            getDrawingList("Accumulator") -> clear();
            getDrawingList("Binary Image") -> clear();
            return COperator::show();
        }
    
        float w_f = getScreenSize().width;
        float h_f = getScreenSize().height;

        CDrawingList *  list_p;
    
        list_p = getDrawingList("Input Image");
        if (list_p -> isVisible() )
        {
            list_p -> addImage ( m_srcImg, 0, 0, w_f, h_f);
            list_p -> setLineColor ( 0, 255, 0 );
            list_p -> addText ( "Left click and move mouse", 5, 5, 24, false );
        }
    
        list_p = getDrawingList("Accumulator");

        if (list_p -> isVisible() )
        {
            list_p -> clear();
            if (0)
                list_p -> addImage ( m_houghTransOp.getAccumulatorImage(), 
                                     0, 0, 
                                     w_f, h_f );
            else
            {
                cv::Mat &img = m_houghTransOp.getAccumulatorImageReference();

                list_p -> addColorEncImage ( &img,
                                             m_colorEncHoughImg,
                                             0, 0, 
                                             w_f, h_f, 1., true );

                list_p -> setLineColor ( 0, 255, 0 );
                list_p -> addText ( "Left click and move mouse", 5, 5, 24, false );
            }
        }
    
        list_p = getDrawingList("Binary Image");

        if (list_p -> isVisible() )
        {
            list_p -> clear();
            list_p -> addImage ( m_binImg, 
                                 0, 0, 
                                 w_f, h_f );//, 1./100 );
        }


        S2D<int> br = m_roiBottomRight;
        S2D<int> tl = m_roiTopLeft;

        cv::Size size = m_binImg.size();

        if (tl.x < 0)  tl.x = 0;
        if (tl.y < 0)  tl.y = 0;
        if (br.x < 0)  br.x = size.width-1;
        if (br.y < 0)  br.y = size.height-1;
        
        br.x = std::min(std::max(br.x, 0), (signed)size.width-1);
        br.y = std::min(std::max(br.y, 0), (signed)size.height-1);
        tl.x = std::min(std::max(tl.x, 0), (signed)size.width-1);
        tl.y = std::min(std::max(tl.y, 0), (signed)size.height-1);

        list_p = getDrawingList("Hough Lines");
        list_p -> clear();
        CDrawingList * list2_p = getDrawingList("Maximum in Accumulator");
        list2_p -> clear();
        list2_p -> setPosition ( getDrawingList("Accumulator") -> getPosition() );        
        double accumVis_b = getDrawingList("Accumulator") -> isVisible() && list2_p -> isVisible();

        if ( list_p -> isVisible() )
        {
            cv::Size size = m_houghTransOp.getAccumulatorImage().size();
            
            const float sx_f = w_f / size.width;
            const float sy_f = h_f / size.height;

            const double dispWidth_d  = getScreenSize().width;
            const double dispHeight_d = getScreenSize().height;

            const int w_ui = m_gradX.size().width;
            const int h_ui = m_gradX.size().height;

            const float aspOX_f = dispWidth_d  /(float) w_ui;
            const float aspOY_f = dispHeight_d /(float) h_ui;
        
            const int num_i = std::min((int)m_lines.size(),m_showMaxLines_i);

            for (int i = 0; i < num_i; ++i)
            {
                double theta_d = m_houghTransOp.column2Theta ( m_lines[i].x_f );
                double range_d = m_houghTransOp.row2Range    ( m_lines[i].y_f );

                float m = -cos(theta_d) / sin(theta_d);
                float b =  range_d / sin(theta_d);
            
                S2D<float> p1 (0,    w_ui/-2.*m+b+h_ui/2.);
                S2D<float> p2 (w_ui, w_ui/ 2.*m+b+h_ui/2.);
            
                SRgb color ( (int)(255 * (random()/(float) RAND_MAX)), 
                             (int)(255 * (random()/(float) RAND_MAX)), 
                             (int)(255 * (random()/(float) RAND_MAX)) );

                list_p -> setLineColor(color);
                list_p -> setLineWidth(2);
                list_p -> addClippedLine( p1.x*aspOX_f, p1.y*aspOY_f,
                                          p2.x*aspOX_f, p2.y*aspOY_f,
                                          tl.x*aspOX_f, tl.y*aspOY_f,
                                          br.x*aspOX_f, br.y*aspOY_f );

            
                if (accumVis_b)
                {
                    list2_p -> setLineColor(color);
                    list2_p -> setLineWidth(2);
                    list2_p -> addSquare(m_lines[i].x_f * sx_f , m_lines[i].y_f * sy_f, 0.5*sx_f);
                }
            
            }
        }

        list_p = getDrawingList("X Gradient");

        float scale_f = 10;
    
        list_p -> clear();
        if (list_p -> isVisible() )
        {
            list_p -> addImage ( m_gradX,
                                 0, 0, 
                                 w_f, h_f, scale_f );
        }

        list_p = getDrawingList("Y Gradient" );
    
        list_p -> clear();
        if (list_p -> isVisible() )
        {
            list_p -> addImage ( m_gradY,
                                 0, 0, 
                                 w_f, h_f, scale_f );
        }
    }

    return COperator::show();
}

/// Init event.
bool CHoughTransformOp::initialize()
{
    if ( m_srcImg.size().width == 0 )
    {
        printf("%s:%i Must provide an input image\n", __FILE__, __LINE__);
        return  COperator::initialize();
    }

    m_gradX    = cv::Mat(m_srcImg.size(), CV_32FC1 ); 
    m_gradY    = cv::Mat(m_srcImg.size(), CV_32FC1 ); 
    m_binImg   = cv::Mat(m_srcImg.size(), CV_8UC1 );
    m_gradient = cv::Mat(m_srcImg.size(), CV_32FC1 ); 
    
    if ( m_houghTransOp.getAccumulatorImage().size() != m_gradHX.size() )
    {
        m_gradHX = cv::Mat(m_houghTransOp.getAccumulatorImage().size(), CV_32FC1 );
        m_gradHY = cv::Mat(m_houghTransOp.getAccumulatorImage().size(), CV_32FC1 );
    }
    
    return COperator::initialize();
}

/// Reset event.
bool CHoughTransformOp::reset()
{
    return COperator::reset();
}

bool CHoughTransformOp::exit()
{
    return COperator::exit();
}

void 
CHoughTransformOp::keyPressed ( CKeyEvent * f_event_p )
{
   return COperator::keyPressed ( f_event_p );    
}

void 
CHoughTransformOp::mouseMoved ( CMouseEvent * f_event_p )
{
    const double dispWidth_d  = getScreenSize().width;
    const double dispHeight_d = getScreenSize().height;
            

    const CDrawingList *  accumList_p   = getDrawingList ("Accumulator");
    const CDrawingList *  srcImgList_p  = getDrawingList ("Input Image");

    bool mouseOnAccum_b   = ( f_event_p -> displayScreen == accumList_p->getPosition() && 
                              accumList_p -> isVisible() );

    bool mouseOnSrcImg_b  = ( f_event_p -> displayScreen == srcImgList_p->getPosition() && 
                              srcImgList_p -> isVisible() );

    const float aspOX_f = dispWidth_d  /(float) m_gradX.size().width; 
    const float aspOY_f = dispHeight_d /(float) m_gradX.size().height;      
    
    cv::Mat accumImg = m_houghTransOp.getAccumulatorImage();

    const float aspAX_f = dispWidth_d  /(float) accumImg.size().width; 
    const float aspAY_f = dispHeight_d /(float) accumImg.size().height;    
    
    if ( mouseOnAccum_b )
    {
        const int w_ui = m_gradX.size().width;
        const int h_ui = m_gradX.size().height;
        
        double imgPosU_d = f_event_p -> posInScreen.x / aspAX_f;
        double imgPosV_d = f_event_p -> posInScreen.y / aspAY_f;
        
        double theta_d = m_houghTransOp.column2Theta ( imgPosU_d );
        double range_d = m_houghTransOp.row2Range    ( imgPosV_d );

        if ( (f_event_p -> qtMouseEvent_p -> buttons() & Qt::LeftButton) )
        {
            CDrawingList *  list_p =  getDrawingList ("Overlay Hough Line");
            list_p -> clear();

            float m = -cos(theta_d) / sin(theta_d);
            float b =  range_d / sin(theta_d);
            
            S2D<float> p1 (0,    w_ui/-2.*m+b+h_ui/2.);
            S2D<float> p2 (w_ui, w_ui/ 2.*m+b+h_ui/2.);
            
            /// Draw two lines and circles of different color and width
            for (int l = 0; l < 2; ++l)
            {
                list_p -> setLineWidth ( 4-l*2-1 );
                list_p -> setLineColor(SRgb(l*255,0,0));

                list_p -> addClippedLine( p1.x*aspOX_f, p1.y*aspOY_f,
                                          p2.x*aspOX_f, p2.y*aspOY_f,
                                          0, 0,
                                          dispWidth_d, dispHeight_d );
                
                list_p -> addEllipse( dispWidth_d/2., 
                                      dispHeight_d/2., 
                                      range_d*aspOX_f, 
                                      range_d*aspOY_f );
            }
        }

        updateDisplay();
    }
    else if ( mouseOnSrcImg_b )
    {
        /// Draw corresponding accumulation line on accumulator
        if ( f_event_p -> qtMouseEvent_p -> buttons() & Qt::LeftButton )
        {
            CDrawingList *  list_p =  getDrawingList ("Overlay Hough Accum Image");
            list_p -> clear();
            list_p->setPosition ( accumList_p -> getPosition() );            

            double imgPosU_d = f_event_p -> posInScreen.x / aspOX_f - m_gradX.size().width /2;
            double imgPosV_d = f_event_p -> posInScreen.y / aspOY_f - m_gradX.size().height/2;

            S2D<float> p1, p2;
            for (int i = 0; i < accumImg.size().width; ++i)
            {
                double theta_d = m_houghTransOp.column2Theta( i );
                double range_d = imgPosV_d * sin( theta_d ) + imgPosU_d * cos ( theta_d );

                p1.x = i;
                p1.y = m_houghTransOp.range2Row( range_d );

                if ( i != 0 )
                {
                    /// Draw two lines of different color and width
                    list_p -> setLineWidth(3);
                    list_p -> setLineColor(SRgb(0,0,0));
                    list_p -> addLine( p1.x*aspAX_f, p1.y*aspAY_f,
                                       p2.x*aspAX_f, p2.y*aspAY_f);

                    list_p -> setLineColor(SRgb(0,255,0));
                    list_p -> setLineWidth(1);
                    list_p -> addLine( p1.x*aspAX_f, p1.y*aspAY_f,
                                       p2.x*aspAX_f, p2.y*aspAY_f);
                }
                
                p2 = p1;
            }
        }
        
        updateDisplay();
    }

    return COperator::mouseMoved ( f_event_p );
}


CLinearHoughTransform * 
CHoughTransformOp::getLinearHoughTransformOp()
{
    return &m_houghTransOp;
}

cv::Mat 
CHoughTransformOp::getAccumulatorImage()
{
    return  m_houghTransOp.getAccumulatorImage();
}

cv::Mat
CHoughTransformOp::getBinaryImage () const
{
    return m_binImg;
}

cv::Mat 
CHoughTransformOp::getGradientXImage()
{
    return  m_gradX;
}

cv::Mat 
CHoughTransformOp::getGradientYImage()
{
    return  m_gradY;
}

/// Set the input of this operator
bool
CHoughTransformOp::getInput  (  )
{
    cv::Mat inpimg = COperator::getInput<cv::Mat>(m_inputId_str, cv::Mat());
    
    if ( inpimg.size().width == 0 ) { m_srcImg = inpimg; return false; }
    
    if (inpimg.type()!=CV_32F)
    {
        if ( inpimg.type() == CV_8UC3 )
        {
            m_srcImg      = cv::Mat(inpimg.size(), CV_32F);
            float *dst_p  = &m_srcImg.at<float>(0,0);
            const SRgb *s = &inpimg.at<SRgb>(0,0);            
            const SRgb *e = s + inpimg.size().width * inpimg.size().height;

            for (; s < e; ++s, ++dst_p) *dst_p = (s->r+s->g+s->r)/768.f;
        }
        else if ( inpimg.type() == CV_8U )
        {
            m_srcImg       = cv::Mat(inpimg.size(), CV_32F);
            float *dst_p   = &m_srcImg.at<float>(0,0);
            const uint8_t *s  = &inpimg.at<uint8_t>(0,0);            
            const uint8_t *e  = s + inpimg.size().width * inpimg.size().height;
            for (; s < e; ++s, ++dst_p) *dst_p = *s/256.f;
        }
        else
            printf("%s:%i Please provide a CV_8UC3, CV_8U or CV_32F (prefered) image.\n", __FILE__, __LINE__);
    }
    else
        m_srcImg = inpimg;

    return true;
}

