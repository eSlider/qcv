/*
 * Copyright (C) 2015 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of StereoSFM
 *
 * StereoSFM is under the terms of the GNU General Public License v2.
 * See the GNU GPL version 2.0 for details.
 * StereoSFM uses dual licensing. Contact the author to get information
 * for a commercial/proprietary license.
 *
 * StereoSFM is distributed "AS IS" without ANY WARRANTY, without even 
 * the implied warranty of merchantability or fitness for a particular
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

/*

This project is an implementation of the method presented in the paper:

Hernan Badino and Takeo Kanade. A Head-Wearable Short-Baseline Stereo System for the Simultaneous Estimation of Structure and Motion. In IAPR Conference on Machine Vision Applications (MVA), Nara, Japan, June 2011.

*/

#ifndef __FEATUREKFDISPLAYOP_H
#define __FEATUREKFDISPLAYOP_H

/**
*******************************************************************************
*
* @file featKFOp.cpp
*
* \class CFeatureKFDisplayOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for tracking features with Kalman Filter
* 
* 
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include "operator.h"
#include "feature.h"
#include "colorEncoding.h"
#include "3DPointVector.h"

#include "kf3DStereoPointCommon.h"
#include "kf3DStereoPoint.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
   class CFeatureKFDisplayOp: public QCV::COperator
   {

   /// Public data types
   public:
      typedef enum
      {
         CEM_DISTANCE = 0, 
         CEM_SPEED,
         CEM_XSPEED,
         CEM_YSPEED,
         CEM_ZSPEED,
         CEM_ABSXSPEED, 
         CEM_ABSYSPEED,
         CEM_ABSZSPEED,
         CEM_POSVARIANCE,
         CEM_XPOSVARIANCE,
         CEM_YPOSVARIANCE,
         CEM_ZPOSVARIANCE,
         CEM_VELVARIANCE,
         CEM_XVELVARIANCE,
         CEM_YVELVARIANCE,
         CEM_ZVELVARIANCE,
         CEM_ABSDISPRATE,
         CEM_NIS 
      } EColorEncodingMode_t;
        
      typedef enum
      {
         CEM3D_ORIGINAL = 0, 
         CEM3D_COUNT,
      } EColorEncodingMode3D_t;
        
   /// Constructor, Desctructors
   public:    
        
      /// Constructors.
      CFeatureKFDisplayOp ( COperator * const f_parent_p = NULL );
        
      /// Virtual destructor.
      virtual ~CFeatureKFDisplayOp ();

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
      virtual void mousePressed (   CMouseEvent * f_event_p );

   /// Gets and sets
   public:

   /// Gets and sets (Parameters)
   public:

      ADD_PARAM_ACCESS (bool,        m_show_b,              Show );
      ADD_PARAM_ACCESS (std::string, m_egoMInpId_str,       EgoMInputIdStr );
      ADD_PARAM_ACCESS (int,         m_minAge_i,            MinAge );
        
      ADD_PARAM_ACCESS (S2D<double>, m_velXRange,           VelXRange );
      ADD_PARAM_ACCESS (S2D<double>, m_velYRange,           VelYRange );
      ADD_PARAM_ACCESS (S2D<double>, m_velZRange,           VelZRange );

      ADD_PARAM_ACCESS (S2D<double>, m_vectorLengthRange,   VectorLengthRange );

      ADD_PARAM_ACCESS (double,      m_predTime_d,          PredTime );
        
      ADD_PARAM_ACCESS (EColorEncodingMode_t,
                        m_colorEncMode_e,      ColorEncodingMode );


      ADD_PARAM_ACCESS (double,      m_arrowWidth_d,        ArrowWidth );
      ADD_PARAM_ACCESS (double,      m_arrowLength_d,       ArrowLength );
      ADD_PARAM_ACCESS (bool,        m_intensifyWithNIS_b,  IntensifyWithNIS );
      ADD_PARAM_ACCESS (float,       m_lineWidth_f,         LineWidth );


      ADD_PARAM_ACCESS (bool,        m_3dVisShow_b,         3DVisShow );
      ADD_PARAM_ACCESS (int,         m_3dVisMaxPoints_i,    3DMaxPoints );
      ADD_PARAM_ACCESS (float,       m_3dVisMaxDist_f,      3DVisMaxDistance );
      ADD_PARAM_ACCESS (float,       m_3dVisMaxVel_f,       3DVisMaxVelocity );
      ADD_PARAM_ACCESS (int,         m_3dVisMinAge_i,       3DVisMinAge );
      ADD_PARAM_ACCESS (float,       m_3dVisMinFeatSpeed_f, 3DVisMinFeatSpeed );
      ADD_PARAM_ACCESS (EColorEncodingMode3D_t,
                        m_3dColorEncMode_e,    3DColorEncodingMode );

      ADD_PARAM_ACCESS (double,      m_maxNis_d,            MaxNIS );

      ADD_PARAM_ACCESS (double,      m_minVel4Vector_d,     MinVel4Vector );

      ADD_PARAM_ACCESS (bool,        m_show3dAnaglyph_b,    Show3dAnaglyph );

      ADD_PARAM_ACCESS ( C3DVector,  m_3dRotVec,            3dRotVector );
      ADD_PARAM_ACCESS ( C3DVector,  m_3dTransVec,          3dTransVector );

      ADD_PARAM_ACCESS ( SRgb,       m_3dBGColor,           3dBackgroundColor );

      ADD_PARAM_ACCESS (float,       m_3dLineWidth_f,       3dLineWidth );
      ADD_PARAM_ACCESS (float,       m_3dPointSize_f,       3dPointSize );
      ADD_PARAM_ACCESS (float,       m_3dEgoPosPointSize_f, 3dEgoPosPointSize );

      ADD_PARAM_ACCESS (float,       m_3dGrayNorm_f,        3dGrayNorm );

      ADD_PARAM_ACCESS (float,       m_sqSize_f,            SquareSize );

      ADD_PARAM_ACCESS (float,       m_displayScale_f,      DisplayScale );


   /// Protected Data Types
   protected:

   protected:

      void registerDrawingLists();
      void registerParameters(); 
      void show3D();
      void printTrackedPoint ( const int f_idx_i, const CKF3DStereoPoint & f_point ) const;
       
   private:
        
      /// Compute?
      bool                           m_show_b;

      /// Scale of the display?
      float                         m_displayScale_f;

      /// Input String
      std::string                    m_egoMInpId_str;

      /// Min age 4 display.
      int                            m_minAge_i;

      /// Time for prediction in seconds.
      double                         m_predTime_d;

      /// X Velocity range
      S2D<double>                    m_velXRange;

      /// Y Velocity range
      S2D<double>                    m_velYRange;

      /// Z Velocity range
      S2D<double>                    m_velZRange;

      /// Vector length range
      S2D<double>                    m_vectorLengthRange;

      /// Max normalized innovation square.
      double                         m_maxNis_d;

      /// Color encoding mode.
      EColorEncodingMode_t           m_colorEncMode_e;

      /// Color encoding mode.
      CColorEncoding                 m_ceDistance;

      /// Color encoding mode.
      CColorEncoding                 m_ceSpeed;

      /// Color encoding mode.
      CColorEncoding                 m_ceVariance;

      /// Color encoding mode.
      CColorEncoding                 m_ceNis;

      /// Color encoding mode.
      CColorEncoding                 m_ceDispRate;

      /// Width of the arrows.
      double                         m_arrowWidth_d;

      /// Length of the arrows.
      double                         m_arrowLength_d;
        
      /// Intensify (transparency) with nis?
      bool                           m_intensifyWithNIS_b;

      /// Line Width [px].
      float                          m_lineWidth_f;
        
      /// Show 3D points.
      bool                           m_3dVisShow_b;

      /// previous feature vector.
      C3DPointDataVector             m_3dVisDataVector_v;
        
      /// previous feature vector.
      std::vector<SRgb>              m_3dVisColorVector_v;
        
      /// Index for points.
      int                            m_3dVisCurrIdx_i;

      /// Max points to visualize.
      int                            m_3dVisMaxPoints_i;

      /// Total points.
      int                            m_3dVisTotalPoints_i;

      /// Min speed of feature to visualize
      float                          m_3dVisMinFeatSpeed_f;

      /// Min Age of feature to visualize.
      int                            m_3dVisMinAge_i;

      /// Max distance for 3D points.
      float                          m_3dVisMaxDist_f;

      /// Max velocity for 3D points.
      float                          m_3dVisMaxVel_f;

      /// Last feature count.
      int                            m_3dVisLastCount_i;

      /// Mapping between feature track number and corresponding 3D point.
      std::vector<int>               m_3dVisFeatMap_v;

      /// Color encoding mode.
      EColorEncodingMode3D_t         m_3dColorEncMode_e;

      /// Color encoding mode.
      CColorEncoding                 m_3dCeCount;

      /// Noramlization factor for gray scale values.
      float                          m_3dGrayNorm_f;

      /// Color encoding mode.
      C3DVector                      m_3dRotVec;

      /// Color encoding mode.
      C3DVector                      m_3dTransVec;

      /// Background color
      SRgb                           m_3dBGColor;

      /// Background color
      float                          m_3dLineWidth_f;

      /// Background color
      float                          m_3dPointSize_f;

      /// Background color
      float                          m_3dEgoPosPointSize_f;

      /// Minimum velocity that a point must have to show it with a vector.
      double                         m_minVel4Vector_d;

      /// Show Anaglyph?
      bool                           m_show3dAnaglyph_b;

      /// Ego-position.
      C3DPointDataVector             m_egoPositions;

      /// Max egopositions
      int                            m_maxEgoPositions_i;

      /// Selected feature for printout 
      ssize_t                        m_selIdx_i;
       
      // Display square size
      float                          m_sqSize_f;
   };
}
#endif // __FEATUREKFDISPLAYOP_H
