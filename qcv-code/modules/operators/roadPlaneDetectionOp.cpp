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
* @file homoRectifier.cpp
*
* \class  CRoadPlaneDetectionOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief  Copied and adapted from previous project.
*
*******************************************************************************/

/* INCLUDES */

#include "roadPlaneDetectionOp.h"
#include "drawingList.h"
#include <opencv2/imgproc/imgproc.hpp>

#if defined ( _OPENMP )
 #undef _OPENMP
#endif

using namespace QCV;

/// Constructors.
CRoadPlaneDetectionOp::CRoadPlaneDetectionOp ( COperator * const f_parent_p )
    : COperator (     f_parent_p, "Road Plane Detector" ),
      m_dispImgInputId_str (          "Disparity Image" ),
      m_camInputId_str (                "Stereo Camera" ),
      m_compute_b (                                true ),
      m_planeParams (                                   ),
      m_camera (                                        ),
      m_apply2Camera_b (                           true ),
      m_result (                                0, 0, 0 ),
      m_imgResult (                             0, 0, 0 ),
      m_iniResult (                             0, 0, 0 ),
      m_iniImgResult (                          0, 0, 0 ),
      m_displayGrid (                                   ),
      m_minDisp_f (                               1.e-3 ),
      
      m_iniResiduum_f (                            1.e5 ),
      m_maxResiduum_f (                               5 ),
      m_acceptedResiduum_f (                        2.5 ),
      m_iniH_f(                                    -1.f ),
      m_iniTilt_f(                                  0.f ),
      m_iniRoll_f(                                  0.f ),
      m_maxDistance_f (                            10.f ),
      m_maxIters_i (                                 10 ),
      m_rowStart_i (                                240 ),
      m_showInliers_b  (                           true ),
      m_showOutliers_b (                          false ),
      
      m_gridZBack_f (                               0.f ),
      m_gridZ_i     (                                41 ),
      m_gridZStep_f (                             0.25f ),          
      m_gridX_i     (                                41 ),
      m_gridXStep_f (                             0.25f ),
      m_origin (                          -0.1, 0., 0.15),
      m_yaw_f(                                      0.f ),
      m_residuumTol_f (                          1.e-2f ),
      m_sigmaTol_f (                                3.f ),
      m_computeResidualImage_b (                  false ),
      m_residualImg (                                   ),
      m_iniPoseFromCamera_b (                     false )
{
    registerDrawingLists();
    registerParameters();
}

void
CRoadPlaneDetectionOp::registerDrawingLists()
{
    registerDrawingList ("Detected Plane", 
                         S2D<int> (0, 0),
                         false );

    registerDrawingList ("Measurements", 
                         S2D<int> (0, 0),
                         false );
}

void
CRoadPlaneDetectionOp::registerParameters()
{    
    BEGIN_PARAMETER_GROUP("Computation", false, CColor::red );

      ADD_STR_PARAMETER( "Disparity Id", 
                         "Disparity image input id",
                         m_dispImgInputId_str,
                         this,
                         DispImgInputId, 
                         CRoadPlaneDetectionOp );

      ADD_STR_PARAMETER( "Camera Id", 
                         "Stereo camera input id",
                         m_camInputId_str,
                         this,
                         CameraInputId, 
                         CRoadPlaneDetectionOp );

      ADD_BOOL_PARAMETER( "Compute", 
                          "Compute road plane detection?",
                          m_compute_b,
                          this,
                          Compute, 
                          CRoadPlaneDetectionOp );

      ADD_BOOL_PARAMETER( "Apply To Camera", 
                          "Apply result to camera?.",
                          m_apply2Camera_b,
                          this,
                          Apply2Camera, 
                          CRoadPlaneDetectionOp );

      ADD_BOOL_PARAMETER( "Compute Residual Image", 
                          "Compute residual image?",
                          m_computeResidualImage_b,
                          this,
                          ComputeResidualImage, 
                          CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Initial Residuum", 
                           "Initial residuum of difference with prediction.",
                           m_iniResiduum_f,
                           this,
                           InitialResiduum, 
                           CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Max Residuum", 
                           "Maximum residuum to identify estimation success.",
                           m_maxResiduum_f,
                           this,
                           MaxResiduum, 
                           CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Accepted Residuum", 
                           "Iteration will break if obtained residuum is smaller than this value.s",
                           m_acceptedResiduum_f,
                           this,
                           AcceptedResiduum, 
                           CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Residuum Tolerance", 
                           "Minimal difference between consecutive iterations.",
                           m_residuumTol_f,
                           this,
                           ResiduumTolerance, 
                           CRoadPlaneDetectionOp );

      ADD_BOOL_PARAMETER( "Initial Pose From Camera", 
                          "Obtain the initial pose from the camera rotation and translation?",
                          m_iniPoseFromCamera_b,
                          this,
                          InitialPoseFromCamera, 
                          CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Initial Height", 
                           "Initial height of the ground in meters.",
                           m_iniH_f,
                           this,
                           InitialHeight, 
                           CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Initial Tilt", 
                           "Initial tilt of the ground in degrees.",
                           m_iniTilt_f,
                           this,
                           InitialTilt, 
                           CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Initial Roll", 
                           "Initial roll of the ground in degrees.",
                           m_iniRoll_f,
                           this,
                           InitialRoll, 
                           CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Max Distance", 
                           "Max distance of the stereo points.",
                           m_maxDistance_f,
                           this,
                           MaxDistance, 
                           CRoadPlaneDetectionOp );

      ADD_INT_PARAMETER( "Max Iterations", 
                         "Max iterations of the robust method.",
                         m_maxIters_i,
                         this,
                         MaxIterations, 
                         CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Sigma Tolerance", 
                           "Number of standard deviations for inliers selection.",
                           m_sigmaTol_f,
                           this,
                           SigmaTolerance, 
                           CRoadPlaneDetectionOp );

      ADD_INT_PARAMETER( "Row start", 
                         "Start in given row.",
                         m_rowStart_i,
                         this,
                         RowStart, 
                         CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Yaw", 
                           "Yaw of the camera.",
                           m_yaw_f,
                           this,
                           Yaw, 
                           CRoadPlaneDetectionOp );

      ADD_3DVEC_PARAMETER( "Origin", 
                           ".",
                           m_origin,
                           "x", "y", "z",
                           this,
                           Origin, 
                           CRoadPlaneDetectionOp );

    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, CColor::red );
      addDrawingListParameter ( "Detected Plane" );
      addDrawingListParameter ( "Measurements" );
      
      ADD_BOOL_PARAMETER( "Show Inliers", 
                          "Show inliers as crosses.",
                          m_showInliers_b,
                          this,
                          ShowInliers, 
                          CRoadPlaneDetectionOp );

      ADD_BOOL_PARAMETER( "Show Outliers", 
                          "Show outliers as crosses.",
                          m_showOutliers_b,
                          this,
                          ShowOutliers, 
                          CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Grid Z Back", 
                           "Back position of overlay grid",
                           m_gridZBack_f,
                           this,
                           GridZBack, 
                           CRoadPlaneDetectionOp );

      ADD_INT_PARAMETER( "Number of Z Grids",
                         "Number of z grids.",
                         m_gridZ_i,
                         this,
                         NumberGridZ,
                         CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Step Grid Z",
                           "Step for grid z [m].",
                           m_gridZStep_f,
                           this,
                           StepGridZ, 
                           CRoadPlaneDetectionOp );

      ADD_INT_PARAMETER( "Number of X Grids",
                         "Number of x grids.",
                         m_gridX_i,
                         this,
                         NumberGridX,
                         CRoadPlaneDetectionOp );

      ADD_FLOAT_PARAMETER( "Step Grid X",
                           "Step for grid x [m].",
                           m_gridXStep_f,
                           this,
                           StepGridX, 
                           CRoadPlaneDetectionOp );
    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CRoadPlaneDetectionOp::~CRoadPlaneDetectionOp ()
{
}/// Cycle event.

bool CRoadPlaneDetectionOp::cycle()
{
    if (m_compute_b)
    {
        cv::Mat dispImg          = getInput<cv::Mat>         ( m_dispImgInputId_str, cv::Mat() );
        CStereoCamera *camera_p  = getInput<CStereoCamera>   ( m_camInputId_str );

        if ( dispImg.cols > 0 && camera_p )
        {
            m_camera = *camera_p;

            setInitialPose();

            m_result    = m_iniResult;
            m_imgResult = m_iniImgResult;

#if 0
            bool simulate_b = false;
            
            if (simulate_b)
            {
                C3DVector params(-0.016439572757232 + (random()/(double)RAND_MAX)*.4-0.2,
                                 4.192326597677202  + (random()/(double)RAND_MAX)*3-1.5,
                                 3.513846994386313);
                
                int w_i = dispImg.cols;
                int h_i = dispImg.rows;

                for (int i = 0; i < h_i; ++i)
                {
                    float * d_p = &dispImg.at<float>(i,0);
                    
                    for (int j = 0; j < w_i; ++j, ++d_p)
                    {
                        *d_p = (i - params.z() - params.x() * j)/params.y();
                    }
                }
            }
#endif
            m_minDisp_f = m_camera.getDisparityFromDistance(m_maxDistance_f);
            m_residuum_f = m_iniResiduum_f;           

            startClock ("cycle() - calculateRoadPlane");
            
            /// Calculate the road plane
            calculateRoadPlane( dispImg,
                                m_camera,
                                m_residuum_f );

            stopClock ("cycle() - calculateRoadPlane");

            bool failed_b = m_residuum_f >= m_maxResiduum_f;
            
            if ( failed_b )
            {
                printf("%s:%i Ground plane fitting failed\n", __FILE__, __LINE__ );
                m_result    = m_iniResult;
                m_imgResult = m_iniImgResult;
            }
            else
            {
                if (m_computeResidualImage_b )
                {
                    startClock ("cycle() - Compute Residual Image");
                    m_residualImg = cv::Mat::zeros(dispImg.size(), CV_16U );
                    
                    const int w_i = dispImg.cols;
                    const int h_i = dispImg.rows;

                    /// Recompute average residuum.
                    for (int i = m_rowStart_i; i < h_i; ++i)
                    {
                        const float * d_p = &dispImg.at<float>(i,0);
                        uint16_t    * r_p = &m_residualImg.at<uint16_t>(i,0);
                        
                        for (int j = 0; j < w_i; ++j, ++d_p, ++r_p)
                        {                
                            if ( *d_p > m_minDisp_f )
                            {
                                float predRow_f = (j*m_imgResult.x() + *d_p * m_imgResult.y() + m_imgResult.z());
                                /// Store deviation from plane in
                                /// pixels * 16.
                                *r_p = static_cast<uint16_t>(fabsf ( i - predRow_f ) * 16);
                            }
                        }
                    }
                }
                
                stopClock ("cycle() - Compute Residual Image");
            }

            m_gpRotation.loadIdentity();
            m_gpTranslation.clear();

            double yaw_d, roll_d, pitch_d;
            double origZ_d, origX_d;
            if ( m_iniPoseFromCamera_b  )
            {
                m_camera.getRotation().getRotationAngles ( pitch_d, yaw_d, roll_d );
                origZ_d = m_camera.getTranslation().z();
                origX_d = m_camera.getTranslation().x();
            }
            else
            {
                yaw_d = m_yaw_f/180*M_PI;
                origZ_d = m_origin.z();
                origX_d = m_origin.x();
            }
            
            /// The code above is equivalent to this one:
            C3DVector n2(-m_result.x(), 1, -m_result.y());
            n2.normalize();
            double angle_d = acos(n2.y());
            C3DVector n4 (-m_result.y(), 0, m_result.x());
            n4.normalize();

            m_gpRotation.rotateY(yaw_d);
            m_gpRotation.rotateAxis(n4, -angle_d);

            m_gpTranslation.set(0, m_result.z(), 0);
            m_gpTranslation = -m_gpRotation * m_gpTranslation;

            m_gpTranslation.setZ( origZ_d + m_origin.z() ); /// hack.
            m_gpTranslation.setX( origX_d + m_origin.x()); /// hack
            //m_gpTranslation.print();
            
            if ( m_apply2Camera_b )
            {
                m_camera.setRotation    ( m_gpRotation );
                m_camera.setTranslation ( m_gpTranslation );
            }

            if (0)
            {
		double p,y,r;
		m_gpRotation.getRotationAngles ( p, y, r );

		printf("%s:%i Optimal result rotation angles are pitch: %f yaw: %f roll: %f\n", __FILE__, __LINE__, p*180/M_PI, y*180/M_PI, r*180/M_PI);

		printf("%s:%i With translation x: %f y: %f z: %f\n", __FILE__, __LINE__, m_gpTranslation.x(),m_gpTranslation.y(),m_gpTranslation.z() );

		printf("%s:%i Result is  roll: %f pitch: %f height: %f\n", __FILE__, __LINE__, 180/M_PI*atan(m_result.x()),180/M_PI*atan(m_result.y()),m_result.z() );
            }
            
            m_planeParams.gpRotation    = m_gpRotation;
            m_planeParams.gpTranslation = m_gpTranslation;
            m_planeParams.imgResult     = m_imgResult;
            m_planeParams.result        = m_result;

            registerOutput<C3DMatrix>     ("Ground Plane Rotation",    &m_gpRotation );
            registerOutput<C3DVector>     ("Ground Plane Translation", &m_gpTranslation );
            registerOutput<C3DVector>     ("Image Plane Parameters",   &m_imgResult );
            registerOutput<C3DVector>     ("Road Plane Parameters",    &m_result );        
            registerOutput<CStereoCamera> ("Stereo Camera",            &m_camera );        
            registerOutput<cv::Mat>       ("Ground Residual Image",    &m_residualImg );
        }
    }
    else
        if ( m_apply2Camera_b )
        {
            CStereoCamera *camera_p  = getInput<CStereoCamera> ( m_camInputId_str );
            if (camera_p )
            {
                m_camera = *camera_p;

                C3DMatrix rot;
                rot.loadIdentity();
                rot.rotateY(m_yaw_f/180*M_PI);
                rot.rotateX(m_iniTilt_f/180*M_PI);
                rot.rotateZ(m_iniRoll_f/180*M_PI);
                
                m_camera.setRotation(rot);
                m_camera.setTranslation(m_origin);

                registerOutput<CStereoCamera> ("Stereo Camera", &m_camera );        
            }
        }
    

    return COperator::cycle();
}

bool
CRoadPlaneDetectionOp::calculateRoadPlane( const cv::Mat       & f_dispImg,
                                           const CStereoCamera & f_camera,
                                           float               & fr_residuum_f )
{
    
    C3DMatrix AtA;
    C3DVector Atb;

    double B_d   = f_camera.getBaseline();
    double sxy_d = f_camera.getFv() / f_camera.getFu();
    double fy_d  = f_camera.getFv();
    double v0_d  = f_camera.getV0();
    double u0_d  = f_camera.getU0();

    C3DMatrix A, AInv;

    AInv.clear();
    AInv.at(0,0) = -1/sxy_d;
    AInv.at(1,0) = -u0_d/fy_d;
    AInv.at(1,2) = -1/fy_d;
    AInv.at(2,1) = -B_d/sxy_d;

    A.clear();
    A.at(0,0) = -sxy_d;
    A.at(1,2) = -sxy_d/B_d;
    A.at(2,0) = sxy_d*u0_d;
    A.at(2,1) = -fy_d;


    C3DVector prediction;

    prediction = A * m_result;
    prediction.at(2) += v0_d;

    float residuum_f = fr_residuum_f;
    
    bool debug_b = false;
    
    startClock ("cycle() - calculateRoadPlane - Iteration");

    //debug_b=true;
    for (int it = 0; it < m_maxIters_i; ++it)
    {
        if (debug_b)
            printf("%s:%i Calculating matrices in iteration %i\n", __FILE__, __LINE__, it);
        
        startClock ("cycle() - calculateRoadPlane - calculateMatrices");
        calculateMatrices( f_dispImg, 
                           prediction, 
                           residuum_f, 
                           AtA, Atb);
        stopClock ("cycle() - calculateRoadPlane - calculateMatrices");

        int count_i = AtA.at(2,2);
        
        if ( count_i > 4 )
        {
            C3DMatrix AtAInv = AtA.getInverse();
            
            m_imgResult = AtAInv * Atb;
            //m_imgResult.print();            
            
            C3DVector xtemp = m_imgResult;
            
            xtemp.at(2) -= v0_d;
            
            m_result = AInv * xtemp;
        } 
        else
        {
            if ( debug_b )
                printf("%s:%i not enough points to compute ground normal\n", __FILE__, __LINE__);

            return false;
        }

        int w_i = f_dispImg.cols;
        int h_i = f_dispImg.rows;

        count_i  = 0;
        double sum_d    = 0.;
        
        startClock ("cycle() - calculateRoadPlane - recompute residuum");
        /// Recompute average residuum.
        for (int i = m_rowStart_i; i < h_i; ++i)
        {
            const float * d_p = &f_dispImg.at<float>(i,0);

            for (int j = 0; j < w_i; ++j, ++d_p)
            {                
                if ( *d_p > m_minDisp_f )
                {
                    float predRow_f = (j*m_imgResult.x() + *d_p * m_imgResult.y() + m_imgResult.z());
                    /// Evaluate if the point is approximatelly on the road.
                    float diff_f = fabsf ( i - predRow_f );
                    
                    if ( diff_f < residuum_f )
                    {
                        ++count_i;
                        sum_d += diff_f;
                    }
                }
            }
        }
        stopClock ("cycle() - calculateRoadPlane - recompute residuum");

        if (debug_b)
            printf("%s:%i count_i = %i avg_f = %f\n", __FILE__, __LINE__, 
                   count_i, sum_d/count_i);
        
        residuum_f = m_sigmaTol_f * sum_d/count_i;

        if (debug_b)
        {
            printf("%s:%i %i new residuum is %f\n", __FILE__, __LINE__, it, residuum_f );
            
            printf("%s:%i Result: %f %f %f\n", __FILE__, __LINE__, m_result.x(), m_result.y(), m_result.z());
        }
        
        prediction = m_imgResult;

        /// \todo: the fabs should probably be removed here.
        if (fabsf(fr_residuum_f - residuum_f) <  m_residuumTol_f)
        {
            if (debug_b)
                printf("%s:%i Breaking because fabsf(%f - %f) = %f < %f)\n", __FILE__, __LINE__,
                       fr_residuum_f, residuum_f,  fabsf(fr_residuum_f - residuum_f), m_residuumTol_f );
            
            break;
        }
        
        fr_residuum_f = residuum_f;
        
        if ( residuum_f < m_acceptedResiduum_f )
        {
            if (debug_b)
            {
                printf("%s:%i Breaking because min residuum has been achieved %f < %f\n", __FILE__, __LINE__,
                       residuum_f, m_acceptedResiduum_f );
            }

            break;
        }   
    }

    stopClock ("cycle() - calculateRoadPlane - Iteration");
    return true;
}

bool
CRoadPlaneDetectionOp::calculateMatrices( const cv::Mat       & f_dispImg,
                                          const C3DVector     & f_prediction,
                                          const float           f_residuum_f,
                                          C3DMatrix           & fr_AtA,
                                          C3DVector           & fr_Atb)
{   
    int w_i = f_dispImg.cols;
    int h_i = f_dispImg.rows;

#ifdef _OPENMP
    const int numThreads_i = (int) std::min(omp_get_max_threads(),32);

    C3DMatrix matrices_p[32];
    C3DVector bvecs_p[32];
    int       counters_p[32];

    for (int i = 0; i < 32; ++i)
    {
        matrices_p[i].clear();
        bvecs_p[i].clear();
        counters_p[i] = 0;
    }
#else
    C3DMatrix m;
    int       c_i = 0;
    C3DVector bvec;
        
    m.clear();
    bvec.clear();
#endif

#if defined ( _OPENMP )
#pragma omp parallel for num_threads(numThreads_i) schedule(static)
#endif
    for (int i = m_rowStart_i; i < h_i; ++i)
    {
#if defined ( _OPENMP )
        const unsigned int threadNum_ui = omp_get_thread_num();
        C3DMatrix &m    = matrices_p[threadNum_ui];
        int       &c_i  = counters_p[threadNum_ui];
        C3DVector &bvec = bvecs_p[threadNum_ui];
#endif
        const float * d_p = &f_dispImg.at<float>(i,0);

        for (int j = 0; j < w_i; ++j, ++d_p)
        {
            if ( *d_p > m_minDisp_f )
            {
                /// Evaluate if the point is approximatelly on the road.
                float vtest_d = ( f_prediction.x() * j + 
                                  f_prediction.y() * *d_p + 
                                  f_prediction.z() );

                if (fabsf(vtest_d - i) < f_residuum_f)
                {
                    m.at(0,0) += j*j;
                    m.at(0,1) += *d_p * j;
                    m.at(0,2) += j;
                    m.at(1,1) += *d_p * *d_p;
                    m.at(1,2) += *d_p;
                    
                    bvec.at(0) += j * i;
                    bvec.at(1) += *d_p * i;
                    bvec.at(2) += i;
                    
                    ++c_i;
                }
            }                
        }
    }

    C3DMatrix AtA;
    C3DVector Atb;
    int count_i;

#if defined ( _OPENMP )
    fr_AtA.clear();
    fr_Atb.clear();
    count_i = 0;

    for (int i = 0; i < numThreads_i; ++i)
    {
        fr_AtA  += matrices_p[i];
        count_i += counters_p[i];
        fr_Atb  += bvecs_p[i];
    }
#else
    fr_AtA  = m;
    count_i = c_i;
    fr_Atb  = bvec;
#endif

    fr_AtA.at(2,2) = count_i;
    fr_AtA.at(1,0) = fr_AtA.at(0,1);
    fr_AtA.at(2,0) = fr_AtA.at(0,2);
    fr_AtA.at(2,1) = fr_AtA.at(1,2);
    
    return true;    
}



/// Show event.
bool CRoadPlaneDetectionOp::show()
{

    const float scrWidth_f  = getScreenSize().width;
    const float scrHeight_f = getScreenSize().height;

    //float scaleFactor_f = getCastedInputObject<CIO_float, float> ("Rectification Scale Factor", 1.f );
    
    CDrawingList *list_p;    

    list_p = getDrawingList ("Detected Plane");   
    list_p -> clear();

    if ( m_compute_b && 
         list_p->isVisible() )
    {
        
        //         float gridZBack_f = 0;
        //         int   gridZ_i     = 41;
        //         float gridZStep_f = 0.25;

        //         int   gridX_i     = 41;
        //         float gridXStep_f = 0.25;

        if ((int)m_displayGrid.size() < m_gridZ_i * m_gridX_i)
            m_displayGrid.resize(m_gridZ_i * m_gridX_i, C3DVector(0,0,0));
        
        CStereoCamera *camera_p  = getInput<CStereoCamera>   ( m_camInputId_str );
        cv::Mat dispImg          = getInput<cv::Mat>         ( m_dispImgInputId_str, cv::Mat() );

        if (dispImg.cols > 0 && camera_p)
        {
            /// First build grid.
            float scaleX_f = scrWidth_f  / dispImg.cols;
            float scaleY_f = scrHeight_f / dispImg.rows;

            for (int i = 0; i < m_gridZ_i; ++i)
            {
                float z_f = m_gridZBack_f + i*m_gridZStep_f;
                
                for (int j = 0; j < m_gridX_i; ++j)
                {
                    float x_f = (j-m_gridX_i/2)*(float)m_gridXStep_f;

                    //C3DVector p(x_f,z_f,1);
                    //C3DVector imgp;

                    //float y_f = p.dotProduct(m_result);
                    
                    m_camera.world2Image ( C3DVector(x_f, 0., z_f), 
                                            m_displayGrid[i*m_gridX_i+j]);
                    
                    m_displayGrid[i*m_gridX_i+j].at(0) *= scaleX_f;
                    m_displayGrid[i*m_gridX_i+j].at(1) *= scaleY_f;
                }
            }

            /// Show now.
            {
                list_p->setLineColor(0,0,0);
                list_p->setLineWidth(2);
                
                int i, j, idx1, idx2;
                for (i = 0; i < m_gridZ_i - 1; ++i)
                {
                    for (j = 0; j < m_gridX_i - 1; ++j)
                    {
                        idx1 = (i+0)*m_gridX_i+(j+0);
                        idx2 = (i+0)*m_gridX_i+(j+1);
                        list_p->addClippedLine( m_displayGrid[idx1].x(), m_displayGrid[idx1].y(),
                                                m_displayGrid[idx2].x(), m_displayGrid[idx2].y(),
                                                0, 0, scrWidth_f, scrHeight_f);
                        
                        idx2 = (i+1)*m_gridX_i+(j+0);
                        list_p->addClippedLine( m_displayGrid[idx1].x(), m_displayGrid[idx1].y(),
                                                m_displayGrid[idx2].x(), m_displayGrid[idx2].y(),
                                                0, 0, scrWidth_f, scrHeight_f);
                    }
                    
                    idx1 = (i  )*m_gridX_i+(j);
                    idx2 = (i+1)*m_gridX_i+(j);
                    list_p->addClippedLine( m_displayGrid[idx1].x(), m_displayGrid[idx1].y(),
                                            m_displayGrid[idx2].x(), m_displayGrid[idx2].y(),
                                            0, 0, scrWidth_f, scrHeight_f);
                }

                for (j = 0; j < m_gridX_i - 1; ++j)
                {
                    idx1 = (i+0)*m_gridX_i+(j+0);
                    idx2 = (i+0)*m_gridX_i+(j+1);

                    list_p->addClippedLine( m_displayGrid[idx1].x(), m_displayGrid[idx1].y(),
                                            m_displayGrid[idx2].x(), m_displayGrid[idx2].y(),
                                            0, 0, scrWidth_f, scrHeight_f);
                }
            }
        }
    }

    list_p = getDrawingList ("Measurements");
    list_p -> clear();

    if ( m_compute_b && 
         list_p->isVisible() )
    {
        cv::Mat dispImg = getInput<cv::Mat> ( m_dispImgInputId_str, cv::Mat() );
        //CStereoCamera *  camera_p  = dynamic_cast<CStereoCamera *> (getInput ( m_camInputId_str ));
        int w_i = dispImg.cols;
        int h_i = dispImg.rows;        

        float scaleX_f = scrWidth_f  / w_i;
        float scaleY_f = scrHeight_f / h_i;

        CColorEncoding cenc(CColorEncoding::CET_GREEN2DARKRED);

        cenc.setMinimum(0);
        cenc.setMaximum(m_maxResiduum_f);
        SRgb color;

        list_p->setLineWidth(2);

        for (int i = 0; i < h_i; ++i)
        {
            const float * d_p = &dispImg.at<float>(i,0);
            
            for (int j = 0; j < w_i; ++j, ++d_p)
            {
                
                if ( *d_p > m_minDisp_f )
                {
                    /// Evaluate if the point is approximatelly on the road.
                    float val_f = fabsf ( i - ( m_imgResult.x() * j + 
                                                m_imgResult.y() * *d_p + 
                                                m_imgResult.z() ) );

                    if (val_f < m_residuum_f)
                    {
                        if ( m_showInliers_b )
                        {    
                            cenc.colorFromValue( val_f, color);
                            
                            list_p->setFillColor(SRgba(color, 100));
                            list_p->setLineColor(SRgba(color, 200));
                            
                            list_p->addFilledSquare( (j)*scaleX_f, 
                                                     (i)*scaleY_f, 
                                                     1 );
                        }                        
                    }
                    else if ( m_showOutliers_b )
                    {
                        //cenc.colorFromValue( cenc.getMaximum(), color);
                        color = SRgba(0,0,200);
        
                        list_p->setFillColor(SRgba(color, 100));
                        list_p->setLineColor(SRgba(color, 200));
                        
                        list_p->addFilledSquare ( j*scaleX_f, 
                                                  i*scaleY_f, 
                                                  1.f );
                    }                    
                }
            }
        }
    }

    return COperator::show();
}

/// Init event.
bool CRoadPlaneDetectionOp::initialize()
{
    setInitialPose();
    return COperator::initialize();
}

void
CRoadPlaneDetectionOp::setInitialPose()
{
    CStereoCamera *  camera_p  = getInput<CStereoCamera> ( m_camInputId_str );

    if ( camera_p )
    {
        float iniRoll_f = m_iniRoll_f;
        float iniTilt_f = m_iniTilt_f;
        float iniH_f    = m_iniH_f;

        if ( m_iniPoseFromCamera_b  )
        {
            C3DMatrix rot   = camera_p->getRotation();
            C3DVector trans = camera_p->getTranslation();

            double p,y,r;
            rot.getRotationAngles ( p, y, r);

            iniH_f = trans.y()/cos(p);

            iniTilt_f = p*180./M_PI;
            iniRoll_f = r*180./M_PI;

       }
    
        double B_d   = m_camera.getBaseline();
        double sxy_d = m_camera.getFv() / m_camera.getFu();
        double fy_d = m_camera.getFv();
        double v0_d  = m_camera.getV0();
        double u0_d  = m_camera.getU0();
        
        C3DMatrix A;
        
        A.clear();
        A.at(0,0) = -sxy_d;
        A.at(1,2) = -sxy_d/B_d;
        A.at(2,0) = sxy_d*u0_d;
        A.at(2,1) = -fy_d;
        
        m_iniResult = C3DVector (tan(M_PI*iniRoll_f/180.),
                                 tan(M_PI*iniTilt_f/180.),
                                 iniH_f );

        m_iniImgResult = A * m_iniResult;
        m_iniImgResult.at(2) += v0_d;

        if (0)
        {    
            printf("%s:%i ROADPLANE: iniImgResult = %f %f %f\n", __FILE__, __LINE__, 
                   m_iniImgResult.x(), 
                   m_iniImgResult.y(), 
                   m_iniImgResult.z() );
            
            printf("%s:%i ROADPLANE: iniResult = %f %f %f\n", __FILE__, __LINE__, 
                   m_iniResult.x()*180/M_PI, 
                   m_iniResult.y()*180/M_PI, 
                   m_iniResult.z() );
        }
        
    }
    else
        printf("%s:%i In initialization: no camera available\n", __FILE__, __LINE__);
    
}
/// Reset event.
bool CRoadPlaneDetectionOp::reset()
{
    return COperator::reset();
}

bool CRoadPlaneDetectionOp::exit()
{
    return COperator::exit();
}

void 
CRoadPlaneDetectionOp::keyPressed ( CKeyEvent * f_event_p )
{
    return COperator::keyPressed ( f_event_p );    
}
