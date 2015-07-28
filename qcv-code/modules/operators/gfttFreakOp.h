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

#ifndef __GFTTFREAKOP_H
#define __GFTTFREAKOP_H

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
    class CGfttFreakOp: public COperator
    {
    public:       
          
    public:        
        
        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CGfttFreakOp ( COperator * const f_parent_p = NULL,
                       const std::string f_name_str = "GFFT+Freak Operator" );
        
        /// Virtual destructor.
        virtual ~CGfttFreakOp ();

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
        virtual void mouseMoved (     CMouseEvent * f_event_p );

    /// Parameters
    public:

        ADD_PARAM_ACCESS (std::string,  m_inpImageId_str,          InputImageIdStr );
        ADD_PARAM_ACCESS (std::string,  m_featPointVector_str,     FeaturePointVectorId );
        ADD_PARAM_ACCESS (bool,         m_compute_b,               Compute );
        ADD_PARAM_ACCESS (int,          m_numFeatures_i,           NumFeatures );
	ADD_PARAM_ACCESS (int,          m_subPixBlockSize_i,       SubPixBlockSize );
        ADD_PARAM_ACCESS (bool,         m_useSubPix_b,             UseSubPix );
	ADD_PARAM_ACCESS (int,          m_subPixIterNum_i,         SubPixIterNum );
	ADD_PARAM_ACCESS (float,        m_subPixEPS_f,             SubPixEPS );
        ADD_PARAM_ACCESS (float,        m_maxDistance_f,           MaxDist );
        ADD_PARAM_ACCESS (float,        m_maxDistanceForPred_f,    MaxDistForPrediction );
	ADD_PARAM_ACCESS (double,       m_qualityLevel_d,          QualityLevel );
	ADD_PARAM_ACCESS (double,       m_qualityFactor_d,         QualityFactor );
	ADD_PARAM_ACCESS (double,       m_minDistance_d,           MinDistance );
	ADD_PARAM_ACCESS (int,          m_blockSize_i,             BlockSize );
        ADD_PARAM_ACCESS (bool,         m_useHarris_b,             UseHarris );
        ADD_PARAM_ACCESS (float,        m_maxDescriptorDistance_f, MaxDescriptorDistance );
	ADD_PARAM_ACCESS (int,          m_maxFeatPerTile_i,        MaxFeaturesPerTile );

        ADD_PARAM_ACCESS (bool,         m_correlation_b,           Correlation );
        ADD_PARAM_ACCESS (bool,         m_normalizedCorr_b,        NormalizedCorr );

    /// Parameters
    public:

       CFeatureVector * getFeatureVector ()
       {
           return &m_featureVector;
       }
       
    /// Help methods
    protected:

        void registerDrawingLists(  );

        void registerParameters(  );

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
        
        /// Input image Id.
        std::string                         m_inpImageId_str;
   
        /// Feature Vector Input String
        std::string                         m_featPointVector_str;

        /// Compute
        bool                                m_compute_b;                                            

        /// Number of features to track
        int                                 m_numFeatures_i;

        /// Input image                     
        cv::Mat                             m_img;

        /// Input previous image                     
        cv::Mat                             m_prevImg;

        /// Response color encoding
        CColorEncoding                      m_respCE;

        /// Good features to track
	cv::GoodFeaturesToTrackDetector *   m_gftt_p;

        /// Freak extractor
        cv::FREAK *                         m_freak_p;

        /// Matcher
 	cv::BFMatcher *                     m_matcher_p;

        /// Feature data
        SFeatureData                        m_featData[2];
       
        /// Counter
        int                                 m_cnt_i;

        // /todo: add descriptions
	// GFTT parameters
        int                                 m_subPixBlockSize_i; 
        bool                                m_useSubPix_b;       
        int                                 m_subPixIterNum_i;   
        float                               m_subPixEPS_f;       
        float                               m_maxDistance_f;     
        float                               m_maxDistanceForPred_f;     

	double                              m_qualityLevel_d;
	double                              m_qualityFactor_d;
	double                              m_minDistance_d;
	int                                 m_blockSize_i;
	bool                                m_useHarris_b;

        CFeatureVector                      m_prevFeatureVector;
        CFeatureVector                      m_featureVector;

       float                                m_maxDescriptorDistance_f;

       
       bool m_correlation_b;
       bool m_normalizedCorr_b;

       int m_maxFeatPerTile_i;
       
    };
}
#endif // __GFTTFREAKOP_H
 
