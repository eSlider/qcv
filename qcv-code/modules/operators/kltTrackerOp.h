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

#ifndef __KLTTRACKER_H
#define __KLTTRACKER_H

/**
*******************************************************************************
*
* @file gfttFreakOp.h
*
* \class CGfttFreakOp
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief Class for computing and trackign features.
*
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#if CV_MAJOR_VERSION > 2 || ( CV_MAJOR_VERSION == 2 && CV_MINOR_VERSION >= 4 )
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#endif

#include "operator.h"
#include "colorEncoding.h"

#include "feature.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    class CKltTrackerOp: public COperator
    {
    public:       
          
    public:        
        
        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CKltTrackerOp ( COperator * const f_parent_p = NULL,
                       const std::string f_name_str = "KLT Tracker" );
        
        /// Virtual destructor.
        virtual ~CKltTrackerOp ();

        /// Cycle event.
        virtual bool cycle( );
    
        /// Show event.
        virtual bool show();
    
        /// Init event.
        virtual bool initialize();
    
        /// Reset event.
        virtual bool reset();
    
        /// Exit event.
        virtual bool exit();

        /// Mouse moved.
        virtual void mouseMoved (  CMouseEvent * f_event_p );

        /// Key pressed.
        virtual void keyPressed (  CKeyEvent *   f_event_p );

    /// Parameters
    public:

        ADD_PARAM_ACCESS (std::string,  m_inpImageId_str,          InputImageIdStr );
        ADD_PARAM_ACCESS (std::string,  m_featPointVector_str,     FeaturePointVectorId );
        ADD_PARAM_ACCESS (bool,         m_compute_b,               Compute );
        ADD_PARAM_ACCESS (int,          m_numFeatures_i,           NumFeatures );
	ADD_PARAM_ACCESS (int,          m_subPixBlockSize_i,       SubPixBlockSize );
        ADD_PARAM_ACCESS (bool,         m_checkCollisions_b,       CheckCollisions );
        ADD_PARAM_ACCESS (bool,         m_useSubPix_b,             UseSubPix );
	ADD_PARAM_ACCESS (int,          m_subPixIterNum_i,         SubPixIterNum );
	ADD_PARAM_ACCESS (float,        m_subPixEPS_f,             SubPixEPS );
	ADD_PARAM_ACCESS (int,          m_minDistance_i,           MinDistance );
	ADD_PARAM_ACCESS (bool,         m_adaptiveDistance_b,      AdaptiveDistance );
	ADD_PARAM_ACCESS (int,          m_kernelSize_i,            KernelSize );
        ADD_PARAM_ACCESS (float,        m_pyrLKEpsilon_f,          PyrLKEpsilon );      
        ADD_PARAM_ACCESS (int,          m_pyrLKMaxCount_i,         PyrLKMaxCount );
        ADD_PARAM_ACCESS (bool,         m_usePrediction_b,         UsePrediction );
        ADD_PARAM_ACCESS (float,        m_maxSqDist4Collision_f,   MaxSqDist4Collision );
        ADD_PARAM_ACCESS (bool,         m_preFilter_b,             PreFilter);
        ADD_PARAM_ACCESS (int,          m_pFMaskSize_i,            PreFilterMaskSize);
        ADD_PARAM_ACCESS (float,        m_pFClampScale_f,          PreFilterClampScale);
        ADD_PARAM_ACCESS (float,        m_minEigenvalue_f,         MinEigenvalue );
        ADD_PARAM_ACCESS (int,          m_pyrLevels_i,             PyramidLevels);
        ADD_PARAM_ACCESS (bool,         m_detectGFTT_b,            DetectGFTT );      
        ADD_PARAM_ACCESS (double,       m_harrisK_d,               HarrisK );      
        ADD_PARAM_ACCESS (double,       m_minHarrisResponse_d,     MinHarrisResponse);
       
    /// Parameters
    public:

       CFeatureVector * getFeatureVector ()
       {
           return &m_featureVector;
       }

       cv::Mat &getCurrentImage() 
       {
          return m_currImg;
       }

       cv::Mat &getPreviousImage() 
       {
          return m_prevImg;
       }
       
    /// Help methods
    protected:

        void registerDrawingLists(  );

        void registerParameters(  );

        void selectGoodFeatures();
       
    /// Protected data types
    protected:
        struct SFeatureData
        {
            std::vector<cv::KeyPoint>                keypoints_v;
            cv::Mat                                  descriptors;
            /// Map from keypoints_v to feature vector
            std::vector<int>                         idx_feature_v;
            /// Map from feature vector to keypoints_v
            std::vector<int>                         idx_feature_rev_v;
            std::vector< std::vector< cv::DMatch > > radius_matches_v;

           void clear()
           {
              keypoints_v.clear();
              descriptors=cv::Mat();
              idx_feature_rev_v.clear();
              radius_matches_v.clear();
           }
           
        };

    private:
       struct SEigenvalue
        {
           SEigenvalue () {}
           SEigenvalue ( int f_x_f, int f_y_f, float f_eigenvalue_f )
              : x ( f_x_f ), y ( f_y_f), eigenvalue ( f_eigenvalue_f ) { }
           
           bool operator < ( const SEigenvalue & f_other ) const
           {
              return eigenvalue < f_other.eigenvalue;
           }            
           
           int x, y;
           float eigenvalue;

        };

    private:
        
        /// Input image Id.
        std::string                         m_inpImageId_str;
   
        /// Feature Vector Input String
        std::string                         m_featPointVector_str;

        /// Compute
        bool                                m_compute_b;                                            

        /// Input current image                     
        cv::Mat                             m_currImg;

        /// Input previous image                     
        cv::Mat                             m_prevImg;

        /// Eigenvalue image.
        cv::Mat                             m_eigenImg;
       
        /// Feature Mask to ensure min distance between features
        cv::Mat                             m_featureMask;
       
        /// Response color encoding
        CColorEncoding                      m_respCE;

        // Pre-filter input image?
        bool                                m_preFilter_b;
       
        // Pre-filter normalization mask size [px]
        int                                 m_pFMaskSize_i;
       
        // Pre-filter clamp scale
        float                               m_pFClampScale_f;

        /// Number of features to track
        int                                 m_numFeatures_i;

        /// Number of features to track
        int                                 m_pyrLevels_i;

        /// Min distance between features
	int                                 m_minDistance_i;

        /// Adaptive distance
	bool                                m_adaptiveDistance_b;

        /// Check feature collisions
        bool                                m_checkCollisions_b;

        /// Max squared distance to check collisions
        float                               m_maxSqDist4Collision_f;

        /// Kernel size for tracking
	int                                 m_kernelSize_i;
        
        /// Epsilon for pyramidal LK algorithm
        float                               m_pyrLKEpsilon_f;

        /// Max iterations for KL algorithm
        int                                 m_pyrLKMaxCount_i; 

        /// Use prediction to track features
        bool                                m_usePrediction_b;

       /// Min eigenvalue to consider for detection
        float                               m_minEigenvalue_f;

        /// Apply subpixel estimation to new detections
        bool                                m_useSubPix_b;       

        /// Block size for subpixel estimation
        int                                 m_subPixBlockSize_i; 

        /// Number of iterations for subpixel estimation
        int                                 m_subPixIterNum_i;   

        /// Epsilon for subpixel estimation
        float                               m_subPixEPS_f;       

        /// Previous set of tracked features
        CFeatureVector                      m_prevFeatureVector;

        /// Current set of tracked features
        CFeatureVector                      m_featureVector;

        /// Vector of selected eigenvalues.
        std::vector< SEigenvalue >           m_eigenvalueVector;

       /// Search good fatures to track (thresholding min eigenvalue) or harris detector
       bool                                  m_detectGFTT_b;

       /// Harris K value
       double                                m_harrisK_d;

       /// Min harris response to sort values
       double                                m_minHarrisResponse_d;

       /// For display purposes
       int                                   m_selectedIdx_i;
    };
}
#endif // __KLTTRACKER_H
 
