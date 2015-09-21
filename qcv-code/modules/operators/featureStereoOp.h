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

#ifndef __FEATURESTEREOOP_H
#define __FEATURESTEREOOP_H

/**
 *******************************************************************************
 *
 * @file featureStereoOp.cpp
 *
 * \class CFeatureStereoOp
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Framework operator class for computing sparse stereo.
 *
 * Framework operator class for computing sparse stereo.
 *
 *******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include "operator.h"
#include "imagePyramid.h"
#include "colorEncoding.h"
#include "3DPointVector.h"
#include "feature.h"

/* PROTOTYPES */

/* CONSTANTS */
#define FSO_MAX_CORES 8
#define FSO_MAX_WIDTH 4096

namespace QCV
{
    class CStereoFeaturePointVector;


    class CFeatureStereoOp: public COperator
    {
    /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CFeatureStereoOp ( COperator * const f_parent_p = NULL, 
                           const std::string f_name_str = "Feature Stereo" );
        
        /// Virtual destructor.
        virtual ~CFeatureStereoOp ();

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

    /// Gets and sets.
    public:

        bool              setLevels ( unsigned int f_levels_ui );
        unsigned int      getLevels ( ) const;

        ADD_PARAM_ACCESS (std::string, m_idLeftImage_str,     IdLeftImageStr);
        ADD_PARAM_ACCESS (std::string, m_idRightImage_str,    IdRightImageStr);
        ADD_PARAM_ACCESS (std::string, m_featPointVector_str, FeaturePointVectorId );

        ADD_PARAM_ACCESS (bool,        m_compute_b,           Compute );
        ADD_PARAM_ACCESS (S2D<int>,    m_dispRange,           DisparityRange );
        ADD_PARAM_ACCESS (S2D<int>,    m_maskSize,            MaskSize );

        ADD_PARAM_ACCESS (bool,        m_checkVars_b,         CheckVariances );
        ADD_PARAM_ACCESS (float,       m_minVar_f,            MinVariance );
        ADD_PARAM_ACCESS (float,       m_maxZssd_f,           MaxZSSD );
        ADD_PARAM_ACCESS (float,       m_maxSsd_f,            MaxSSD );

        ADD_PARAM_ACCESS_BOUNDED(int,  m_deltaDisp_i,         DeltaDisp, 1, 65535 );

        ADD_PARAM_ACCESS (float,       m_dispOffset_f,        DispOffset );

        ADD_PARAM_ACCESS (bool,        m_show3DPoints_b,      Show3DPoints );
        ADD_PARAM_ACCESS (bool,        m_preFilter_b,         PreFilter);

        ADD_PARAM_ACCESS (bool,        m_showLeftImage_b,     ShowLeftImage);
        ADD_PARAM_ACCESS (bool,        m_showRightImage_b,    ShowRightImage);
    /// Protected Data Types
    protected:

    protected:

        void registerDrawingLists();
        void registerParameters();

        void show3D();

        bool setPyramidParams ( unsigned int f_levels_ui );

        bool transferLevel( int f_level_i );
        bool computeFirstCorrelation( int m_fromLevel_i = -1 );

        void generateDispImage( const CFeatureVector *f_vec );

       //void generate3DPointList( const CStereoFeaturePointVector *f_vec );

    private:
       enum ERejectionCause
       {
          ERC_NONE = 0,
          ERC_LIMIT,
          ERC_ATBORDER,
          ERC_INVALIDDISP,
          ERC_LOWVARIANCE,
          ERC_LARGEZSSD,
          ERC_LARGESSD,
          ERC_OTHER
       };
       
          
    private:

        /// Left image
        cv::Mat                      m_lImg;

        /// Right image
        cv::Mat                      m_rImg;
       
        /// Compute?
        bool                         m_compute_b;

        /// Left image Id
        std::string                  m_idLeftImage_str;
        
        /// Right image Id
        std::string                  m_idRightImage_str;
        
        /// Feature Vector Input String
        std::string                  m_featPointVector_str;

        /// Disparity range.
        S2D<int>                     m_dispRange;

        /// Set the correlation mask size.
        S2D<int>                     m_maskSize;

        /// Check the variances of the patches?
        bool                         m_checkVars_b;

        /// Min allowed variance for left and right windows [px^2].
        float                        m_minVar_f;

        /// Max allowed zero-mean sum of square difference [px^2].
        float                        m_maxZssd_f;

        /// Max allowed zero-mean sum of square difference [px^2].
        float                        m_maxSsd_f;        
        
        /// Disparity offset.
        float                        m_dispOffset_f;

        /// Left image pyramid.
        CImagePyramid                m_pyrLeft;

        /// Right image pyramid.
        CImagePyramid                m_pyrRight;
        
        /// Disparity image pyramid.
        CImagePyramid                m_pyrDisp;

        /// Levels
        unsigned int                 m_levels_ui;

        /// Correction
        int                          m_deltaDisp_i;

        /// Float disparity image
         cv::Mat                     m_dispImg;

        /// Color encoding mode.
        CColorEncoding               m_ceDistance;

	/// List of 3D points
	C3DPointDataVector           m_3dPoints;

        /// Show 3D points?
	bool                         m_show3DPoints_b;

        /// Pre-filter input images?
        bool                         m_preFilter_b;

        /// Rejection cause
        std::vector<ERejectionCause> m_rejCause_v;
        /// Show left image
        bool                         m_showLeftImage_b;
       
        /// Show right image
        bool                         m_showRightImage_b;
       
    private:
        /// Data structure to store scores.
        float m_scoresACTUAL_p[FSO_MAX_CORES][2*FSO_MAX_WIDTH];
    };
}
#endif // __FEATURESTEREOOP_H
