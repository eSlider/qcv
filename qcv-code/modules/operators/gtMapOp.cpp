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

/*@@@**************************************************************************
 * \file   gtMapOp.cpp
 * \author Hernan Badino
 * \date   April 3, 2012
 * \notes 
 ******************************************************************************/

/* INCLUDES */

#include "gtMapOp.h"
#include "drawingList.h"
#include "colorEncoding.h"

//#include "ippDefs.h"

using namespace QCV;

/// Constructors.
CGTMapOp::CGTMapOp ( COperator * const f_parent_p )
   : COperator ( f_parent_p, "Ground Truth Pose Visualizer" ),
     m_filePath_str (                           "poses.txt" ),
     m_egoMInpId_str (   "Integrated Ego-Motion Estimation" ),
     m_maxDistance_f (                                 1.e6 ),
     m_cencSections_i (                                  50 ),
     m_rotation (                                           ),
     m_translation (                                        )
{
   registerDrawingList( "Poses Overlay",
                        S2D<int> (0, 0),
                        false );

   ADD_STR_PARAMETER( "File Path", 
                      "Path to the file containing the vehicle data. "
                      "It must be relative to the image data directory.",
                      m_filePath_str,
                      this,
                      FilePath, 
                      CGTMapOp );

   ADD_STR_PARAMETER( "Ego-Motion Input String Id", 
                      "String identification of the ego-motion. Input "
                      "must be a SRigidMotion object type.",
                      m_egoMInpId_str,
                      this,
                      EgoMInputIdStr, 
                      CGTMapOp );

   ADD_FLOAT_PARAMETER( "Max Distance?", 
                        "Max distance to show poses [m]",
                        m_maxDistance_f,
                        this,
                        MaxDistance, 
                        CGTMapOp );

   ADD_INT_PARAMETER( "Color Encoding Sections", 
                      "Number of sections for color encoding",
                      m_cencSections_i,
                      this,
                      ColorEncodingSections, 
                      CGTMapOp );

   addDrawingListParameter ( "Poses Overlay" );

}

/// Virtual destructor.
CGTMapOp::~CGTMapOp ()
{
}

/// Init event.
bool CGTMapOp::initialize()
{
   m_voPoses_v.clear();
   /// Load only if not already initialized.
   if ( m_poses_v.size() == 0 )
   {
      printf("Loading GT pose data\n");
      loadFromFile();
   }
    
   double idM_p[]={1,0,0,0,1,0,0,0,1};
   m_rotation.loadIdentity();
   m_translation.clear();
    
   return COperator::initialize();
}

/// Reset event.
bool CGTMapOp::reset()
{   
   return COperator::reset();
}

bool CGTMapOp::exit()
{
   return COperator::exit();
}

/// Cycle event.
bool CGTMapOp::cycle()
{
   SRigidMotion *      egoMotion_p = getInput<SRigidMotion>( m_egoMInpId_str );
   
   if ( egoMotion_p )
   {  
      double idM_p[]={1,0,0,0,1,0,0,0,1};
      C3DMatrix rotation    = egoMotion_p->rotation;
      C3DVector translation = egoMotion_p->translation;       
      C3DMatrix invRotation = rotation.getInverse();

      m_rotation    = rotation * m_rotation;
	
      m_translation = rotation * m_translation + translation;
        
      rotation    = m_rotation;
      translation = m_translation;
        
      C3DVector currPos = rotation.multiplyTransposed( -translation );
        
      Data newPos(2);
      newPos[0] = currPos.x();
      newPos[1] = currPos.z();

      m_voPoses_v.push_back( newPos );
   }

   registerOutput ( "GT Pose2ScreenMapper", &m_mapper );

   return COperator::cycle();
}

/// Show event.
bool CGTMapOp::show()
{
   CDrawingList *list_p = getDrawingList("Poses Overlay");
   list_p->clear();
   
   if ( list_p -> isVisible() )
   {
      // if (!m_poses_v.empty() )
      // {
      //    list_p->setLineWidth ( 1 );
      //    list_p -> setLineColor ( SRgb( 255, 255, 255 ) );
         
      //    S2D<double> prevpos = m_mapper.world2Screen ( S2D<double> ( m_poses_v[0][0], 
      //                                                                m_poses_v[0][1] ) );
         
      //    for (unsigned int i = 1; i < m_poses_v.size(); ++i)
      //    {
      //       S2D<double> pos = m_mapper.world2Screen ( S2D<double> ( m_poses_v[i][0], 
      //                                                               m_poses_v[i][1] ) );
            
      //       list_p->addLine ( pos.x, pos.y, prevpos.x, prevpos.y );
      //       prevpos=pos;
      //    }
      // }
      
      if (!m_voPoses_v.empty())
      {
         CColorEncoding colorEnc ( CColorEncoding::CET_HUE,S2D<float>(0,m_voPoses_v.size()));    
         
         S2D<double> newVisRangeX;
         S2D<double> newVisRangeY;
         newVisRangeX.min = 1.e20;
         newVisRangeX.max = -1.e20;
         newVisRangeY.min = 1.e20;
         newVisRangeY.max = -1.e20;
         
         for (unsigned int i = 0; i < m_voPoses_v.size(); ++i)
         {
            S2D<double> pos = S2D<double> ( m_voPoses_v[i][0], 
                                            m_voPoses_v[i][1] );
            
            newVisRangeX.min = std::min(newVisRangeX.min, m_voPoses_v[i][0]);
            newVisRangeX.max = std::max(newVisRangeX.max, m_voPoses_v[i][0]);
            newVisRangeY.min = std::min(newVisRangeY.min, m_voPoses_v[i][1]);
            newVisRangeY.max = std::max(newVisRangeY.max, m_voPoses_v[i][1]);
         }
      
         m_mapper.visRangeX.min = newVisRangeX.min;
         m_mapper.visRangeX.max = newVisRangeX.max;
         m_mapper.visRangeY.min = newVisRangeY.min;
         m_mapper.visRangeY.max = newVisRangeY.max;
         
         m_mapper.visScale_d = 0.9 * std::min(getScreenSize().width /(m_mapper.visRangeX.max-m_mapper.visRangeX.min),
                                              getScreenSize().height/(m_mapper.visRangeY.max-m_mapper.visRangeY.min) );

         m_mapper.offset = S2D<double>(
            (getScreenSize().width -m_mapper.visScale_d * (m_mapper.visRangeX.max-m_mapper.visRangeX.min))/2, 
            (getScreenSize().height-m_mapper.visScale_d * (m_mapper.visRangeY.max-m_mapper.visRangeY.min))/2 );

         S2D<double> pos = m_mapper.world2Screen ( S2D<double> ( m_voPoses_v.back()[0], 
                                                                 m_voPoses_v.back()[1] ) );
         
         list_p->setLineWidth ( 2 );
         list_p -> setLineColor ( SRgb( 30, 255, 30 ) );
         list_p->addLine ( pos.x-20, pos.y-20, pos.x+20, pos.y+20 );
         list_p->addLine ( pos.x+20, pos.y-20, pos.x-20, pos.y+20 );
      
         for (unsigned int i = 0; i < m_voPoses_v.size(); ++i)
         {
            SRgb color;
            colorEnc.colorFromValue ( i, color);
            list_p->setLineColor ( color );

            S2D<double> pos = m_mapper.world2Screen ( S2D<double> ( m_voPoses_v[i][0], 
                                                                    m_voPoses_v[i][1] ) );
           
            list_p->addSquare ( pos.x, pos.y, 2 );
         }
      
      }
             

      //m_mapper.offset = S2D<double>(
      //   (getScreenSize().width  - m_mapper.visScale_d * (m_mapper.visRangeX.max-m_mapper.visRangeX.min))/2, 
      //   (getScreenSize().height - m_mapper.visScale_d * (m_mapper.visRangeY.max-m_mapper.visRangeY.min))/2 );

   }

   return COperator::show();
}

void 
CGTMapOp::keyPressed ( CKeyEvent * f_event_p )
{
   return COperator::keyPressed ( f_event_p );    
}

bool
CGTMapOp::loadFromFile ( )
{        
   std::string f_filePath_str = getInput<std::string> ("Image 0 Path", "");
    
   int pos_i = f_filePath_str.find_last_of ("/\\");
    
   if ( pos_i != -1 )
      f_filePath_str.erase( pos_i );
   else
      f_filePath_str = std::string("./"); 
    
   f_filePath_str += m_filePath_str;
    
   printf("Path to pose file is %s\n", f_filePath_str.c_str());

   FILE *file_p = fopen ( f_filePath_str.c_str(), "r" );
    
   m_mapper.visRangeX.min = 1.e20;
   m_mapper.visRangeX.max = -1.e20;
   m_mapper.visRangeY.min = 1.e20;
   m_mapper.visRangeY.max = -1.e20;

   if (!file_p)
   {
      printf("%s:%i file %s could not be opened\n", __FILE__, __LINE__, f_filePath_str.c_str());
      return false;
   }
    
   size_t fieldsRead_i;
    
   size_t strDataSize_i = 4192;
   char *strData_p = (char *)malloc(strDataSize_i);
    
   m_poses_v.clear();
    
   while (!feof(file_p))
   {
      fieldsRead_i = getline ( &strData_p, &strDataSize_i, file_p );

      if (fieldsRead_i <= 0)
      {
         printf("Data could not be read from file"); 
      }

      Data data(2);

      fieldsRead_i = sscanf ( strData_p,
                              "%lf %lf\n", 
                              &data[0],
                              &data[1] );

      if ( fieldsRead_i == 2 )
      {
         m_poses_v.push_back(data);
         m_mapper.visRangeX.min = std::min(m_mapper.visRangeX.min,data[0]);
         m_mapper.visRangeX.max = std::max(m_mapper.visRangeX.max,data[0]);
         m_mapper.visRangeY.min = std::min(m_mapper.visRangeY.min,data[1]);
         m_mapper.visRangeY.max = std::max(m_mapper.visRangeY.max,data[1]);
      }
   }

   m_mapper.visScale_d = std::min(getScreenSize().width /(m_mapper.visRangeX.max-m_mapper.visRangeX.min),
                                  getScreenSize().height/(m_mapper.visRangeY.max-m_mapper.visRangeY.min) );

   m_mapper.offset = S2D<double>(
      (getScreenSize().width  - m_mapper.visScale_d * (m_mapper.visRangeX.max-m_mapper.visRangeX.min))/2, 
      (getScreenSize().height - m_mapper.visScale_d * (m_mapper.visRangeY.max-m_mapper.visRangeY.min))/2 );
    

   printf("%i Ground truth poses matrices read\n", (int)m_poses_v.size());
    
   free(strData_p);

   fclose ( file_p );

   return true;
}

