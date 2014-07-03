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
* @file stereoTrackerOp.cpp
*
* \class CStereoTrackerOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "stereoTrackerOp.h"

#include "paramMacros.h"
#include "drawingList.h"
#include "featureStereoOp.h"
#include "gfttFreakOp.h"

#include "paramIOXmlFile.h"

using namespace QCV;

/// Constructors.
CStereoTrackerOp::CStereoTrackerOp ( COperator * const f_parent_p,
                     const std::string f_name_str )
    : COperator (             f_parent_p, f_name_str ),
      m_gftt_p (                                NULL ),
      m_compute_b (                             true ),
      m_origCamera(                                  ),
      m_camera (                                     ),
      m_centralPointOffset (                0.f, 0.f )
{
    m_gftt_p = new CGfttFreakOp     ( this, "Harris-Freak Tracker" );
   
    addChild ( m_gftt_p );
    addChild ( new CFeatureStereoOp ( this, "Feature Stereo" ) );

    registerDrawingLists(  );
    registerParameters (  );
}

void
CStereoTrackerOp::registerDrawingLists(  )
{
}

void
CStereoTrackerOp::registerParameters(  )
{

    BEGIN_PARAMETER_GROUP("Computation", false, SRgb(220,0,0));
    ADD_BOOL_PARAMETER ( "Compute",
                         "Perform the scaling?.",
                         m_compute_b,
                         this,
                         Compute,
                         CStereoTrackerOp );

    ADD_FLT2D_PARAMETER ( "Central point offset",
                          "Camera central point offset [px]",
                          m_centralPointOffset,
                          "u", "v",
                          this,
                          CentralPointOffset,
                          CStereoTrackerOp );
    END_PARAMETER_GROUP;
}

/// Virtual destructor.
CStereoTrackerOp::~CStereoTrackerOp ()
{
}

/// Cycle event.
bool
CStereoTrackerOp::cycle()
{
    if ( m_compute_b )
    {
       m_camera = m_origCamera;
       
       m_camera.setU0( m_camera.getU0() + m_centralPointOffset.x );
       m_camera.setV0( m_camera.getV0() + m_centralPointOffset.y );
       
       registerOutput<CStereoCamera> ( "Rectified Camera", &m_camera );

       COperator::cycle();

       if ( getParentOp() )
       {
          getParentOp() -> registerOutput <CFeatureVector> ( m_gftt_p->getFeaturePointVectorId(), 
                                                             m_gftt_p->getFeatureVector() );
       }
    }
    
    return true;
}
    
/// Show event.
bool CStereoTrackerOp::show()
{
   cv::Mat img =  getInput<cv::Mat>("Image 0", cv::Mat() );
   
   /// Set the screen size if this is the parent operator.
   if ( img.size().width > 0 &&
        !getParentOp() )
   {
      setScreenSize ( img.size() );
   }

   if (m_compute_b )
      return COperator::show();

   return true;
}

/// Init event.
bool CStereoTrackerOp::initialize()
{
    std::string  filePath0_str = getInput<std::string> ("Image 0 Path", "");

    printf("File path to Image 0 is %s\n",
           filePath0_str.c_str());
    
    int pos_i = filePath0_str.find_last_of ("/\\");

    if ( pos_i != -1 )
        filePath0_str.erase(pos_i);
    else
        filePath0_str = std::string("."); 

    filePath0_str += "/camera.xml";

    bool fileFound_b = false;

    CParamIOXmlFile paramFile;

    printf("Trying to open parameter file from \"%s\"\n",
           filePath0_str.c_str() );

    /// Try to load camera file.
    if (!paramFile.load( filePath0_str ))
    {    
       printf("NOT Found. NOW trying from local directory\n" );
       /// If does not exist, try to open camera.xml in the current directory.
        if (paramFile.load ("./camera.xml"))
            fileFound_b = true;
    }       
    else
        fileFound_b = true;
    
    if (fileFound_b)
    {
        paramFile.setCurrentCategory ( "Rectified Camera" );
        m_origCamera.load ( paramFile );
        printf("Camera parameters successfuly loaded\n");
    }
    else
    {
       printf("%s:%i camera.xml file could not be found\n", __FILE__, __LINE__ );
    }

    registerOutput<CStereoCamera> ( "Rectified Camera", &m_origCamera );

    return COperator::initialize();
}

/// Reset event.
bool CStereoTrackerOp::reset()
{
    return COperator::reset();
}

bool CStereoTrackerOp::exit()
{
    return COperator::exit();
}


