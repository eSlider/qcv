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

/**
*******************************************************************************
*
* @file featKFOp.cpp
*
* \class CFeatureKFDisplayOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for for tracking features with Kalman Filter
*
*******************************************************************************/

/* INCLUDES */

#include "featVisKFOp.h"
#include "drawingList.h"
#include "stereoCamera.h"
#include "clock.h"
#include "rigidMotion.h"

#ifdef HAVE_QGLVIEWER
#include "glViewer.h"
#endif

#include "kf3DStereoPointVector.h"
#include "ceParameter.h"

using namespace QCV;

/// Constructors.
CFeatureKFDisplayOp::CFeatureKFDisplayOp ( COperator * const f_parent_p )
   : COperator (    f_parent_p, "KF Visualizer" ),
     m_show_b (                           false ),
     m_displayScale_f  (                      1 ),
     m_egoMInpId_str (  "Ego-Motion Estimation" ),
     m_minAge_i (                             3 ),
     m_predTime_d (                         0.2 ),
     m_velXRange (                       0., 5. ),
     m_velYRange (                       0., 5. ),
     m_velZRange (                       0., 5. ),
     m_vectorLengthRange (              0., 20. ),
     m_maxNis_d (                            10 ),
     m_colorEncMode_e (            CEM_DISTANCE ),
     m_ceDistance ( CColorEncoding::CET_HUE, 
                    S2D<float>(2.f,40.f) ),
     m_ceSpeed ( CColorEncoding::CET_HUE, 
                 S2D<float>(0.f,20.f) ),
     m_ceVariance ( CColorEncoding::CET_HUE, 
                    S2D<float>(0.f,1.f) ),
     m_ceNis ( CColorEncoding::CET_HUE, 
               S2D<float>(0.f,3.f) ),
     m_ceDispRate(  CColorEncoding::CET_HUE, 
                    S2D<float>(0.f,3.f) ),
     m_arrowWidth_d (                        3. ),
     m_arrowLength_d (                       8. ),
     m_intensifyWithNIS_b (                true ),
     m_lineWidth_f (                        1.f ),

     m_3dVisDataVector_v (                      ),
     m_3dVisColorVector_v (                     ),
     m_3dVisCurrIdx_i (                       0 ),
     m_3dVisMaxPoints_i (                100000 ),
     m_3dVisTotalPoints_i (                   0 ),
     m_3dVisMinFeatSpeed_f (                  0 ),
     m_3dVisMinAge_i (                        2 ),
     m_3dVisMaxDist_f (                      50 ),
     m_3dVisLastCount_i (                     0 ),
     m_3dVisFeatMap_v (                         ),
     m_3dColorEncMode_e (        CEM3D_ORIGINAL ),
     m_3dCeCount (CColorEncoding::CET_HUE,
                  S2D<float>(0.f,1000.f) ),
     m_3dGrayNorm_f (                      16.f ),
     m_3dRotVec (                     0., 0., 0.),
     m_3dTransVec (                   0., 0., 0.),
     m_3dBGColor (                   96, 96, 96 ),
     m_3dLineWidth_f (                      1.f ),
     m_3dPointSize_f (                      1.f ),
     m_3dEgoPosPointSize_f (                5.f ),

     m_minVel4Vector_d (                     0. ),
     m_show3dAnaglyph_b (                 false ),
     m_maxEgoPositions_i (                 5000 ),
     m_selIdx_i (                            -1 ),
     m_sqSize_f (                           4.f )
{
   /// Some default values.
   registerDrawingLists();
   registerParameters();
}

void
CFeatureKFDisplayOp::registerDrawingLists()
{
   registerDrawingList ("3D Velocity Vectors",
                        S2D<int> (0, 0),
                        false);

   registerDrawingList ("3D Velocity Vectors Right View",
                        S2D<int> (0, 0),
                        false);
}

void
CFeatureKFDisplayOp::registerParameters()
{
   ADD_BOOL_PARAMETER( "Show", 
                       "Show?",
                       m_show_b,
                       this,
                       Show, 
                       CFeatureKFDisplayOp );
    

   ADD_STR_PARAMETER( "Ego-Motion Input String Id", 
                      "String identification of the ego-motion. Input "
                      "must be a SRigidMotion object type.",
                      m_egoMInpId_str,
                      this,
                      EgoMInputIdStr, 
                      CFeatureKFDisplayOp );

   addDrawingListParameter ( "3D Velocity Vectors" );    

   ADD_FLOAT_PARAMETER( "Display scale",
                        "Scale display.",
                        m_displayScale_f,
                        this,
                        DisplayScale,
                        CFeatureKFDisplayOp );
   
   ADD_BOOL_PARAMETER( "Show 3D Anaglyph",
                       "Show in red/cyan anaglyph.",
                       m_show3dAnaglyph_b,
                       this,
                       Show3dAnaglyph,
                       CFeatureKFDisplayOp );


   BEGIN_PARAMETER_GROUP("Color Encoding", false, CColor::red );

   CEnumParameter<EColorEncodingMode_t> * ceMode_p = static_cast<CEnumParameter<EColorEncodingMode_t> * > (
      ADD_ENUM_PARAMETER( "Color Encoding Mode",
                          "Set the color encoding mode.",
                          EColorEncodingMode_t,
                          m_colorEncMode_e,
                          this,
                          ColorEncodingMode,
                          CFeatureKFDisplayOp ) );
    
   ceMode_p -> addDescription ( CEM_DISTANCE,      "Distance" );
   ceMode_p -> addDescription ( CEM_SPEED,         "Speed" );
   ceMode_p -> addDescription ( CEM_XSPEED,        "X Speed" );
   ceMode_p -> addDescription ( CEM_YSPEED,        "Y Speed" );
   ceMode_p -> addDescription ( CEM_ZSPEED,        "Z Speed" );

   ceMode_p -> addDescription ( CEM_ABSXSPEED,     "Absolute X Speed" );
   ceMode_p -> addDescription ( CEM_ABSYSPEED,     "Absolute Y Speed" );
   ceMode_p -> addDescription ( CEM_ABSZSPEED,     "Absolute Z Speed" );

   ceMode_p -> addDescription ( CEM_POSVARIANCE,   "Position Variance" );
   ceMode_p -> addDescription ( CEM_XPOSVARIANCE,   "X Position Variance" );
   ceMode_p -> addDescription ( CEM_YPOSVARIANCE,   "Y Position Variance" );
   ceMode_p -> addDescription ( CEM_ZPOSVARIANCE,   "Z Position Variance" );

   ceMode_p -> addDescription ( CEM_VELVARIANCE,   "Velocity Variance" );
   ceMode_p -> addDescription ( CEM_XVELVARIANCE,  "X Velocity Variance" );
   ceMode_p -> addDescription ( CEM_YVELVARIANCE,  "Y Velocity Variance" );
   ceMode_p -> addDescription ( CEM_ZVELVARIANCE,  "Z Velocity Variance" );

   ceMode_p -> addDescription ( CEM_ABSDISPRATE,   "Abs Disp Rate" );

   ceMode_p -> addDescription ( CEM_NIS,           "NIS" );


   addColorEncodingParameter ( m_ceDistance,
                               getParameterSet(),
                               "Distance Color Enc", 
                               "Color encoding for distance [m]." );

   addColorEncodingParameter ( m_ceSpeed,
                               getParameterSet(),
                               "Speed Color Enc", 
                               "Color encoding for speed [m/s]." );

   addColorEncodingParameter ( m_ceVariance,
                               getParameterSet(),
                               "Variance Color Enc", 
                               "Color encoding for variance [m^2] or [m^2/s^2]." );

   addColorEncodingParameter ( m_ceNis,
                               getParameterSet(),
                               "NIS Color Enc", 
                               "Color encoding for NIS." );

   addColorEncodingParameter ( m_ceDispRate,
                               getParameterSet(),
                               "Disp Rate Color Enc", 
                               "Color encoding for Disp Rate" );
      

   END_PARAMETER_GROUP;


   BEGIN_PARAMETER_GROUP("Filters", false, CColor::red );

   ADD_FLOAT_PARAMETER( "Min Speed",
                        "Min speed of feature to display it.",
                        m_3dVisMinFeatSpeed_f,
                        this,
                        3DVisMinFeatSpeed,
                        CFeatureKFDisplayOp );

   ADD_INT_PARAMETER( "Min Age",
                      "Min age that a feature must have "
                      "to display it.",
                      m_minAge_i,
                      this,
                      MinAge,
                      CFeatureKFDisplayOp );

   ADD_DBL2D_PARAMETER( "X Velocity",
                        "Absolute X velocity range of estimated feature.",
                        m_velXRange,
                        "min", "max",
                        this,
                        VelXRange,
                        CFeatureKFDisplayOp );
      
   ADD_DBL2D_PARAMETER( "Y Velocity",
                        "Absolute Y velocity range of estimated feature.",
                        m_velYRange,
                        "min", "max",
                        this,
                        VelYRange,
                        CFeatureKFDisplayOp );
      
   ADD_DBL2D_PARAMETER( "Z Velocity",
                        "Absolute Z velocity range of estimated feature.",
                        m_velZRange,
                        "min", "max",
                        this,
                        VelZRange,
                        CFeatureKFDisplayOp );

   ADD_DBL2D_PARAMETER( "Vector Length",
                        "Vector length range of estimated feature.",
                        m_vectorLengthRange,
                        "min", "max",
                        this,
                        VectorLengthRange,
                        CFeatureKFDisplayOp );

   ADD_DOUBLE_PARAMETER( "Max NIS",
                         "Maximum Normalized Innovation Square.",
                         m_maxNis_d,
                         this,
                         MaxNIS,
                         CFeatureKFDisplayOp );

   END_PARAMETER_GROUP;

   BEGIN_PARAMETER_GROUP("Vector Options", false, CColor::red );

   ADD_BOOL_PARAMETER( "Intensify with NIS",
                       "Intensify (transparency) with nis?",
                       m_intensifyWithNIS_b,
                       this,
                       IntensifyWithNIS,
                       CFeatureKFDisplayOp );

   ADD_FLOAT_PARAMETER( "Feature Size",
                        "Radius of the square to show features [px].",
                        m_sqSize_f,
                        this,
                        SquareSize,
                        CFeatureKFDisplayOp );

   ADD_FLOAT_PARAMETER( "Line Width",
                        "Width of the lines [px].",
                        m_lineWidth_f,
                        this,
                        LineWidth,
                        CFeatureKFDisplayOp );

   ADD_DOUBLE_PARAMETER( "Prediction Time",
                         "Time to predict future position [s].",
                         m_predTime_d,
                         this,
                         PredTime,
                         CFeatureKFDisplayOp );

   ADD_DOUBLE_PARAMETER( "Min Vel 4 Vector",
                         "Mininum velocity that a point must have to "
                         "show it with a velocity vector [m/s].",
                         m_minVel4Vector_d,
                         this,
                         MinVel4Vector,
                         CFeatureKFDisplayOp );

   ADD_DOUBLE_PARAMETER( "Arrow Width",
                         "Width of the arrow in [px].",
                         m_arrowWidth_d,
                         this,
                         ArrowWidth,
                         CFeatureKFDisplayOp );

   ADD_DOUBLE_PARAMETER( "Arrow Length",
                         "Length of the arrow in [px].",
                         m_arrowLength_d,
                         this,
                         ArrowLength,
                         CFeatureKFDisplayOp );

   END_PARAMETER_GROUP;

   BEGIN_PARAMETER_GROUP("3D Visualization", false, CColor::red );

   ADD_BOOL_PARAMETER("Show 3D",
                      "Show points in 3D viewer?",
                      m_3dVisShow_b,
                      this,
                      3DVisShow,
                      CFeatureKFDisplayOp );
    
   ADD_INT_PARAMETER("Max Points",
                     "Max number of points allowed in the viewer.",
                     m_3dVisMaxPoints_i,
                     this,
                     3DMaxPoints,
                     CFeatureKFDisplayOp );
    
   ADD_FLOAT_PARAMETER("Max Distance",
                       "Don't add points farther away than this value.",
                       m_3dVisMaxDist_f,
                       this,
                       3DVisMaxDistance,
                       CFeatureKFDisplayOp );
    
   ADD_FLOAT_PARAMETER("Max Velocity",
                       "Don't add points farther away than this value.",
                       m_3dVisMaxDist_f,
                       this,
                       3DVisMaxVelocity,
                       CFeatureKFDisplayOp );
    
   ADD_INT_PARAMETER("Min Age (3D)",
                     "Min age of points to visualize.",
                     m_3dVisMinAge_i,
                     this,
                     3DVisMinAge,
                     CFeatureKFDisplayOp );
    

   CEnumParameter<EColorEncodingMode3D_t> * ceMode3D_p = static_cast<CEnumParameter<EColorEncodingMode3D_t> * > (
      ADD_ENUM_PARAMETER( "3D Color Mode",
                          "Set the color encoding mode for 3D display.",
                          EColorEncodingMode3D_t,
                          m_3dColorEncMode_e,
                          this,
                          3DColorEncodingMode,
                          CFeatureKFDisplayOp ) );

   ceMode3D_p -> addDescription ( CEM3D_ORIGINAL,      "Original" );
   ceMode3D_p -> addDescription ( CEM3D_COUNT,         "Count" );

   addColorEncodingParameter ( m_3dCeCount,
                               getParameterSet(),
                               "Count Color Enc", 
                               "Color encoding for feature number." );

   ADD_FLOAT_PARAMETER( "Gray Scale Norm",
                        "Normalization factor for gray scale values.",
                        m_3dGrayNorm_f,
                        this,
                        3dGrayNorm,
                        CFeatureKFDisplayOp );

   ADD_3DVEC_PARAMETER( "Rotation",
                        "Rotation parameters in degrees.",
                        m_3dRotVec,
                        "x","y","z",
                        this,
                        3dRotVector,
                        CFeatureKFDisplayOp );

   ADD_3DVEC_PARAMETER( "Translation",
                        "Translation parameters in meters.",
                        m_3dTransVec,
                        "x","y","z",
                        this,
                        3dTransVector,
                        CFeatureKFDisplayOp );
      
   ADD_RGB_PARAMETER("Background color",
                     "Color for the background of the 3D display.",
                     m_3dBGColor,
                     this,
                     3dBackgroundColor,
                     CFeatureKFDisplayOp );

   ADD_FLOAT_PARAMETER( "Line Width 3D",
                        "Line width for drawings on the 3D display.",
                        m_3dLineWidth_f,
                        this,
                        3dLineWidth,
                        CFeatureKFDisplayOp );

   ADD_FLOAT_PARAMETER( "Point Size 3D",
                        "Point size for drawings on the 3D display.",
                        m_3dPointSize_f,
                        this,
                        3dPointSize,
                        CFeatureKFDisplayOp );


   ADD_FLOAT_PARAMETER( "Ego-Position Point Size",
                        "Point size for drawings on the 3D display.",
                        m_3dEgoPosPointSize_f,
                        this,
                        3dEgoPosPointSize,
                        CFeatureKFDisplayOp );

   END_PARAMETER_GROUP;



}

/// Virtual destructor.
CFeatureKFDisplayOp::~CFeatureKFDisplayOp ()
{
}

/// Cycle event.
bool CFeatureKFDisplayOp::cycle()
{
   uint32_t imgNr_ui = getInput<int> ("Frame Number", 0 );
   imgNr_ui %= m_maxEgoPositions_i;
    
   /// Accumulate and update cloud of points.
   SRigidMotion *      egoMotion_p = getInput<SRigidMotion> ( m_egoMInpId_str );

   if (egoMotion_p)
   {
      C3DVector translation = egoMotion_p->translation;       
      C3DMatrix rotation = egoMotion_p->rotation;
        
      if ( m_egoPositions.size() <= imgNr_ui )
         m_egoPositions.push_back(C3DVector(0,0,0));
      else
         m_egoPositions[imgNr_ui].clear();

      for (unsigned int i = 0; i < m_egoPositions.size(); ++i)
      {
         m_egoPositions[i] = rotation * m_egoPositions[i] + translation;
      } 
   }

   CKF3DStereoPointVector * pvector_p = getInput<CKF3DStereoPointVector>( "KF 3D Stereo Point Vector" );

   if ( pvector_p && 
        m_selIdx_i >= 0 && 
        m_selIdx_i < (signed) (*pvector_p).size() &&
        (*pvector_p)[m_selIdx_i].getAge() > 0)
      printTrackedPoint(m_selIdx_i, (*pvector_p)[m_selIdx_i]);
   else
      m_selIdx_i = -1;
    
   return COperator::cycle();
}

/// Show event.
bool CFeatureKFDisplayOp::show()
{
   if ( m_show_b )
   {
      const float scrWidth_f  = getScreenSize().width;
      const float scrHeight_f = getScreenSize().height;

      /// Obtain input data
      CKF3DStereoPointVector * pvector_p = getInput<CKF3DStereoPointVector>( "KF 3D Stereo Point Vector" );

      CStereoCamera * camera_p = getInput<CStereoCamera> ( "Rectified Camera" );

      cv::Mat * imgL_p    = getInput<cv::Mat> ( "Image 0" );
      cv::Mat * imgR_p    = getInput<cv::Mat> ( "Image 1" );
      cv::Mat * imgs_p[2] = { imgL_p, imgR_p };
        
        
      CDrawingList *list_p;    

      if ( camera_p && pvector_p && imgL_p )
      {
         CKF3DStereoPointVector &kfVector = *pvector_p;

         float scaleFactor_f = scrWidth_f/imgL_p->cols;

         S2D<int> pos;
            
         for (int view_i = 0; view_i <= m_show3dAnaglyph_b; ++view_i)
         {
            if ( view_i == 0 )
            {
               list_p = getDrawingList ("3D Velocity Vectors");
               list_p -> setScale(m_displayScale_f);
               
               if ( m_show3dAnaglyph_b ) 
               {
                  list_p -> setColorMask (1, 0, 0, 0);
               }
               else
               {
                  list_p -> setColorMask (1, 1, 1, 0);
               }

               pos = list_p -> getPosition();
            }
            else
            {
               list_p = getDrawingList ("3D Velocity Vectors Right View");
               list_p -> setColorMask (0, 1, 1, 0);
               list_p -> setScale(m_displayScale_f);
               list_p -> setPosition ( pos );
            }

            list_p -> clear();
                
            if ( list_p->isVisible() )
            {
               float minSqVelVector_f = m_minVel4Vector_d*m_minVel4Vector_d;
               float sqMinSpeed_f     = m_3dVisMinFeatSpeed_f*m_3dVisMinFeatSpeed_f;
               
               list_p -> addImage ( *imgs_p[view_i], 0, 0, scrWidth_f, scrHeight_f );
               for (unsigned int i = 0 ; i < kfVector.size(); ++i )
               {                    
                  C3DVector pos, vel, p1, p2;
                  kfVector[i].getCurrentState ( pos, vel );

                  if ( view_i == 1 ) pos.at(0) -= camera_p -> getBaseline();

                  double pspeed_d = vel.sumOfSquares();

                  double nis_d;
                  nis_d = kfVector[i].getNIS();

                  if (0)
                     printf("%i >= %i - %f >= %f - %f <= %f - %f >= %f - %f <= %f - %f >= %f - %f <= %f - %f <= %f\n",
                            kfVector[i].getAge() , m_minAge_i ,
                            fabs(vel.z()) , m_velZRange.min , 
                            fabs(vel.z()) , m_velZRange.max , 
                            fabs(vel.x()) , m_velXRange.min , 
                            fabs(vel.x()) , m_velXRange.max , 
                            fabs(vel.y()) , m_velYRange.min , 
                            fabs(vel.y()) , m_velYRange.max ,
                            nis_d , m_maxNis_d );
                        
                  if ( kfVector[i].getAge() >= m_minAge_i &&
                       fabs(vel.z()) >= m_velZRange.min && 
                       fabs(vel.z()) <= m_velZRange.max && 
                       fabs(vel.x()) >= m_velXRange.min && 
                       fabs(vel.x()) <= m_velXRange.max && 
                       fabs(vel.y()) >= m_velYRange.min && 
                       fabs(vel.y()) <= m_velYRange.max &&
                       nis_d <= m_maxNis_d &&
                       kfVector[i].getNoMeasurementCount() == 0 &&
                       pspeed_d >= sqMinSpeed_f )
                  {
                     camera_p -> local2Image ( pos, p1 );
                     camera_p -> local2Image ( pos+vel*m_predTime_d, p2 );

                     //double disp1_d = p1.at(2);
                     //double disp2_d = p2.at(2);
                     p1.at(2) = p2.at(2) = 0;
                    
                     C3DVector diffp = p2-p1;
                     double plength_d = diffp.magnitude();

                     const bool m_showFeatOnlyWithArrow_b = 1;
                     
                     if (plength_d > m_arrowLength_d && m_showFeatOnlyWithArrow_b )
                     {
                        
                        if ( plength_d >= m_vectorLengthRange.min &&
                             plength_d <= m_vectorLengthRange.max )
                        {
                           SRgb color;
                               
                           if ( m_colorEncMode_e == CEM_DISTANCE )
                           {
                              m_ceDistance.colorFromValue ( pos.z(),
                                                            color );
                           }
                           else if ( m_colorEncMode_e >= CEM_POSVARIANCE &&
                                     m_colorEncMode_e <= CEM_ZPOSVARIANCE )
                           {
                              C3DMatrix cov;
                              double val_d = 0;
                            
                              kfVector[i].getPositionCovMatrix ( cov );

                              if ( m_colorEncMode_e == CEM_POSVARIANCE )
                                 val_d = cov.trace()/3.;
                              else
                              {
                                 int id = (m_colorEncMode_e - CEM_XPOSVARIANCE);
                                 val_d = cov.at(id, id);
                              }
                              m_ceVariance.colorFromValue ( val_d,
                                                            color );
                           }
                           else if ( m_colorEncMode_e >= CEM_VELVARIANCE &&
                                     m_colorEncMode_e <= CEM_ZVELVARIANCE )
                           {
                              C3DMatrix cov;
                              double val_d = 0;
                            
                              kfVector[i].getVelocityCovMatrix ( cov );

                              if ( m_colorEncMode_e == CEM_VELVARIANCE )
                                 val_d = cov.trace()/3.;
                              else
                              {
                                 int id = (m_colorEncMode_e - CEM_XVELVARIANCE);
                                 val_d = cov.at(id, id);
                              }

                              m_ceVariance.colorFromValue ( val_d,
                                                            color );
                           }
                           else if ( m_colorEncMode_e == CEM_ABSDISPRATE )
                           {
                              float k_f = camera_p -> getBaseline() * camera_p -> getFu();
                              // original and correct one is
                              //float dispRate_f = fabs(k_f * vel.z() / (pos.z() * pos.z()));
                              float dispRate_f = fabs(k_f * vel.z() / (pos.z() * pos.z()));

                              m_ceDispRate.colorFromValue ( dispRate_f,
                                                            color );
                           }
                           else if ( m_colorEncMode_e == CEM_NIS )
                           {
                              m_ceNis.colorFromValue ( nis_d,
                                                       color );
                           }
                           else
                           {
                              float speed_f = 0;

                              if ( m_colorEncMode_e == CEM_SPEED )
                                 speed_f = sqrt(pspeed_d);
                              else if ( m_colorEncMode_e == CEM_XSPEED )
                                 speed_f = vel.x();
                              else if ( m_colorEncMode_e == CEM_YSPEED )
                                 speed_f = vel.y();
                              else if ( m_colorEncMode_e == CEM_ZSPEED )
                                 speed_f = vel.z();
                              else if ( m_colorEncMode_e == CEM_ABSXSPEED )
                                 speed_f = fabs(vel.x());
                              else if ( m_colorEncMode_e == CEM_ABSYSPEED )
                                 speed_f = fabs(vel.y());
                              else if ( m_colorEncMode_e == CEM_ABSZSPEED )
                                 speed_f = fabs(vel.z());
                                
                              m_ceSpeed.colorFromValue ( speed_f,
                                                         color );
                           }
                        
                           float alpha_f = 255;

                           if ( m_intensifyWithNIS_b && 
                                m_colorEncMode_e != CEM_NIS)
                           {
                              alpha_f = 255*(1.- nis_d / m_ceNis.getMaximum());
                              if (alpha_f < 10 )
                                 alpha_f = 10;
                           }
                        
                           p1 *= scaleFactor_f;
                           p2 *= scaleFactor_f;
                        
                           list_p -> setLineWidth ( m_lineWidth_f );
                           list_p -> setLineColor ( SRgba(color, (int)alpha_f) );
                           list_p -> setFillColor ( SRgba( color, alpha_f/3 ) );
                           list_p -> addSquare ( p1.x(), p1.y(), m_sqSize_f/2 );

                           if ( pspeed_d >  minSqVelVector_f)
                           {
                              /// Compute arrow coordinates.
                              C3DVector dp, np, p3, p4, p5;
                              np.set( -diffp.y(), diffp.x(), 0 );
                              np.normalize();
                              np *= m_arrowWidth_d;
                            
                              dp = diffp / plength_d;
                            
                              p3 = p2 - dp * m_arrowLength_d;
                              p4 = p3 + np;
                              p5 = p3 - np;
                            
                              /// Show arrow only if sufficient length is provided.
                              //printf("%f %f\n", plength_d, m_arrowLength_d );
                            
                              if ( plength_d > m_arrowLength_d )
                              {
                                 list_p -> addLine( p1.x(), p1.y(), p3.x(), p3.y() );
                              
                                 list_p -> addFilledTriangle( p2.x(), p2.y(),
                                                              p4.x(), p4.y(),
                                                              p5.x(), p5.y() );
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
         
         list_p = getDrawingList ("3D Velocity Vectors Right View");
         list_p -> setVisibility ( m_show3dAnaglyph_b );

      }
    
      //list_p = getDrawingList ("3D Velocity Vectors");
      //list_p -> write ( "test.svg", scrWidth_f, scrHeight_f );
        

      if ( m_3dVisShow_b )
         show3D();
   }
    
   return COperator::show();
}

/// Init event.
void CFeatureKFDisplayOp::show3D()
{
#ifdef HAVE_QGLVIEWER

   if ( !m_3dViewer_p) // || !leftImg_p )
      return;

   //m_3dViewer_p->clear();
   
   /// Accumulate and update cloud of points.
   SRigidMotion *egoMotion_p = getInput<SRigidMotion> ( m_egoMInpId_str );

   /// Obtain input data.
   CKF3DStereoPointVector * pvector_p =  getInput<CKF3DStereoPointVector> ( "KF 3D Stereo Point Vector" );

   if ( pvector_p && 
        egoMotion_p )
   {
      int maxPoints_i = std::max(m_3dVisMaxPoints_i, (int)pvector_p->size());

      CKF3DStereoPointVector &kfVector = *pvector_p;

      if (maxPoints_i != (int)m_3dVisDataVector_v.size())
      {
         C3DVector p(0,0,0);
         m_3dVisDataVector_v.resize(0);
         m_3dVisDataVector_v.resize(maxPoints_i, p);
         m_3dVisCurrIdx_i = 0;

         SRgb color(255,255,255);
         m_3dVisColorVector_v.resize(0);
         m_3dVisColorVector_v.resize(maxPoints_i, color);
      }
        
      if ( m_3dVisFeatMap_v.size() != kfVector.size() )
      {
         m_3dVisFeatMap_v.resize( maxPoints_i, -1 );
      }
        
      C3DMatrix rotation    = egoMotion_p->rotation;
      C3DVector translation = egoMotion_p->translation;       
      C3DMatrix invRotation = rotation.getInverse();

      int newPoints_i = 0;
      m_3dVisLastCount_i = 0;

      if ( 1 ) // Accumulate 3D points
      {       
         for (unsigned int i = 0 ; i < kfVector.size(); ++i )
         {
            C3DVector pos, vel;
            
            kfVector[i].getCurrentState(pos, vel);
            
            if ( kfVector[i].getAge() >= m_3dVisMinAge_i && 
                 pos.z() <= m_3dVisMaxDist_f )
            {
               if ( m_3dVisFeatMap_v[i] == -1 )
               {
                  ++m_3dVisCurrIdx_i;
                  m_3dVisCurrIdx_i%=maxPoints_i;
                  ++newPoints_i;

                  m_3dVisFeatMap_v[i] = m_3dVisCurrIdx_i;

                  m_3dVisDataVector_v[m_3dVisCurrIdx_i] = pos;
               }
               else
               {
                  if (i >= m_3dVisFeatMap_v.size())
                  {
                     printf("This should not happen!!!! line %i file %s\n", __LINE__, __FILE__);
                     continue;
                  }
                        
                  int idx = m_3dVisFeatMap_v[i];
                  if (idx == -1)
                  {
                     printf("Error in line %i of file %s\n",
                            __LINE__, __FILE__);
                     continue;
                  }
                    
                  m_3dVisDataVector_v[idx] = pos;
                  //m_3dVisDataVector_v[idx] = invRotation * (pos-translation);
               }
            }
            else
               m_3dVisFeatMap_v[i] = -1;
         }
         invRotation.print();
         
         translation.print();
         

         m_3dVisTotalPoints_i += newPoints_i;
         m_3dVisLastCount_i   = newPoints_i;
         //printf("lastCount = %i count_i = %i\n", m_lastCount_i, count_i );
         int iters_i = std::min(m_3dVisTotalPoints_i, maxPoints_i) - newPoints_i;

         int idx_i = (m_3dVisTotalPoints_i >= maxPoints_i)?m_3dVisCurrIdx_i+1:0;
         idx_i%=maxPoints_i;
        
         for (int i = 0 ; i < iters_i; ++i, ++idx_i, idx_i%=maxPoints_i )
         {
            m_3dVisDataVector_v[idx_i] = rotation * m_3dVisDataVector_v[idx_i] + translation;
         }
      }
      else
      {
         m_3dVisCurrIdx_i = 0;
         m_3dVisTotalPoints_i = 0;
            
         for (unsigned int i = 0 ; i < kfVector.size(); ++i )
         {
            C3DVector pos, vel;
            
            kfVector[i].getCurrentState(pos, vel);

            if ( kfVector[i].getAge() >= m_3dVisMinAge_i && 
                 pos.z() <= m_3dVisMaxDist_f &&
                 vel.z() < m_3dVisMaxVel_f )
            {
               //printf("%i t = %i\n", i, feat.t);
               ++m_3dVisCurrIdx_i;
               m_3dVisCurrIdx_i%=maxPoints_i;
               ++newPoints_i;

               m_3dVisDataVector_v[m_3dVisCurrIdx_i] = pos;
            }
         }
         m_3dVisTotalPoints_i += newPoints_i;
         m_3dVisLastCount_i   = newPoints_i;
      }
        
      /// Display now.
      /// get color from image?
      cv::Mat *        imgL_p   = getInput<cv::Mat> ( "Image 0" );
      CStereoCamera *  camera_p = getInput<CStereoCamera>("Rectified Camera" );
        
      if (!camera_p) return;

      SRgb color( 20, 255, 20 );
      m_3dViewer_p -> setLineWidth ( m_3dLineWidth_f );
      m_3dViewer_p -> setPointSize ( m_3dPointSize_f );
      m_3dViewer_p -> setBackgroundColor ( m_3dBGColor );
        
      int points_i = std::min((int) m_3dVisDataVector_v.size(), m_3dVisTotalPoints_i);
      m_3dCeCount.setMaximum ( points_i );
        
      rotation.loadIdentity();
      rotation.rotateX(m_3dRotVec.x()/180.*M_PI);
      rotation.rotateY(m_3dRotVec.y()/180.*M_PI);
      rotation.rotateZ(m_3dRotVec.z()/180.*M_PI);
      rotation.invert();

      /// Assign color to points
      for (int i = 0 ; i < points_i; ++i)
      {
         double u,v,d;
            
         if ( imgL_p && camera_p )
         {
            camera_p -> local2Image ( m_3dVisDataVector_v[i], u, v, d );
            int u_i = u+.5;
            int v_i = v+.5;
            int val_i = 127;
                
            if ( ( m_3dVisCurrIdx_i - m_3dVisLastCount_i >= 0 && 
                   i >= m_3dVisCurrIdx_i - m_3dVisLastCount_i && 
                   i < m_3dVisCurrIdx_i ) ||
                 ( m_3dVisCurrIdx_i - m_3dVisLastCount_i <= 0 &&
                   ( i <= m_3dVisCurrIdx_i ||
                     i > (m_3dVisCurrIdx_i - m_3dVisLastCount_i + maxPoints_i)%maxPoints_i ) ) )
            {
               if ( u_i > 0 && u_i < (int)imgL_p->cols && 
                    v_i > 0 && v_i < (int)imgL_p->rows )
               {
                  val_i = (&imgL_p->at<unsigned char>(v_i))[u_i]/m_3dGrayNorm_f;
                  if (val_i < 0 ) val_i = 0;
                  if (val_i > 255 ) val_i = 255;
                  m_3dVisColorVector_v[i].set( val_i, val_i, val_i );
               }
               else
                  m_3dVisColorVector_v[i].set( 255, 0, 0 );
            }
         }
            
         if (m_3dColorEncMode_e == CEM3D_COUNT )
            m_3dCeCount.colorFromValue ( i,
                                         color );
         else
            color = m_3dVisColorVector_v[i];            

         //SRgb color(*limg_p/256, *limg_p/256, *limg_p/256 );
         if (m_3dVisDataVector_v[i])
         {
            C3DVector p = rotation.multiplyTransposed(m_3dVisDataVector_v[i] - translation);

            m_3dViewer_p -> addPoint ( p,
                                       color, 
                                       m_3dPointSize_f,
                                       C3DVector(0,0,0) );
         }
      }
        
      m_3dCeCount.setMaximum ( m_egoPositions.size() );
      m_3dViewer_p -> setPointSize ( m_3dEgoPosPointSize_f );

      for (unsigned int i = 0; i < m_egoPositions.size(); ++i)
      {
         m_3dCeCount.colorFromValue ( i,
                                      color );
            
         m_3dViewer_p -> addPoint ( rotation.multiplyTransposed( (m_egoPositions[i] - translation) ),
                                    color,
                                    m_3dEgoPosPointSize_f,
                                    C3DVector(0,0,0) );
      }
   }
#endif
}

/// Init event.
bool
CFeatureKFDisplayOp::initialize()
{
   C3DVector p(0,0,0);
   SRgb color ( 255, 255, 255 );
    
   m_3dVisDataVector_v.resize(0);
   m_3dVisDataVector_v.resize(m_3dVisMaxPoints_i, p);
   m_3dVisColorVector_v.resize(0);
   m_3dVisColorVector_v.resize(m_3dVisMaxPoints_i, color);
   m_egoPositions.resize(0);

   m_3dVisCurrIdx_i = 0;
   m_3dVisTotalPoints_i = 0;
   return COperator::initialize();
}

/// Reset event.
bool
CFeatureKFDisplayOp::reset()
{
   return COperator::reset();
}

bool
CFeatureKFDisplayOp::exit()
{
   return COperator::exit();
}

void 
CFeatureKFDisplayOp::keyPressed ( CKeyEvent * f_event_p )
{
   return COperator::keyPressed ( f_event_p );    
}

void 
CFeatureKFDisplayOp::mousePressed ( CMouseEvent * f_event_p )
{
   if (f_event_p -> qtMouseEvent_p -> buttons() & Qt::LeftButton )
   {
 
      C3DVector mPos ( f_event_p -> posInScreen.x,
                       f_event_p -> posInScreen.y, 0.);

      CDrawingList *list_p;    

      list_p = getDrawingList ("3D Velocity Vectors");

      if ( list_p-> getPosition() == f_event_p -> displayScreen )
      {
         /// Obtain input data.
         CKF3DStereoPointVector * pvector_p =  getInput<CKF3DStereoPointVector>("KF 3D Stereo Point Vector");
    
         CStereoCamera *     camera_p = getInput<CStereoCamera>("Rectified Camera" );

         cv::Mat * imgL_p = getInput<cv::Mat>("Image 0" );
    
         if ( camera_p && pvector_p )
         {
            float scaleX_f = getScreenSize().width / (float)imgL_p->cols;
            float scaleY_f = getScreenSize().height / (float)imgL_p->rows;

            mPos.at(0) /= scaleX_f;
            mPos.at(1) /= scaleY_f;

            printf("Mouse pressed at %f %f\n",
                   mPos.x(), mPos.y());
        
            CKF3DStereoPointVector &kfVector = *pvector_p;
        
            //const float scrWidth_f  = getScreenWidth();
            //const float scrHeight_f = getScreenHeight();
        
            //float scaleFactor_f =  scrWidth_f/imgL_p->cols;
        
            printf("\n\n");
            for (unsigned int i = 0 ; i < kfVector.size(); ++i )
            {
               C3DVector pos, vel, p1;
               kfVector[i].getCurrentState ( pos, vel );
            
               camera_p -> local2Image ( pos, p1 );
                
               C3DVector diff = mPos - p1;
               diff.at(2) = 0;
            
               if ( diff.magnitude() < 3 )
               {
                  m_selIdx_i = i;
                  printTrackedPoint(m_selIdx_i, kfVector[m_selIdx_i]);
                  break;
               }
            }
         }
      }
   }
    
   return COperator::mouseMoved ( f_event_p );    
}

void CFeatureKFDisplayOp::printTrackedPoint( const int f_idx_i,
                                             const CKF3DStereoPoint & f_point ) const
{
   double cov_p[36];
   C3DVector pos, vel;
   f_point.getCurrentState ( pos, vel );

   f_point.getCovarianceMatrix (cov_p);

   printf("------------------------------------\n"
          "Tracked Point %i\n"
          "  Age: %i\n"
          "  3D Position: %f %f %f\n"
          "  3D Velocity: %f %f %f\n"
          "  Covariances: %f %f %f %f %f %f\n"
          "  No Meas Count: %i\n"
          "  Status: %i\n"
          "  NIS: %f\n",
          f_idx_i,
          f_point.getAge(),
          pos.x(), pos.y(), pos.z(), 
          vel.x(), vel.y(), vel.z(),
          cov_p[0], cov_p[7], cov_p[14], cov_p[21], cov_p[28], cov_p[35],
          f_point.getNoMeasurementCount(),
          (int)f_point.getStatus (),
          f_point.getNIS() );
   
   for (int j = 0; j < 6; ++j)
      printf("|  %10f %10f %10f %10f %10f %10f |\n",
             cov_p[j*6+0], 
             cov_p[j*6+1], 
             cov_p[j*6+2], 
             cov_p[j*6+3], 
             cov_p[j*6+4], 
             cov_p[j*6+5] );
}
