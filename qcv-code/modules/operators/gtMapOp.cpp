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
#include "linePlotter.h"

//#include "ippDefs.h"

using namespace QCV;

/// Constructors.
CGTMapOp::CGTMapOp ( COperator * const f_parent_p,
		     const std::string f_name_str )
   : COperator                     ( f_parent_p, f_name_str ),
     m_filePath_str (                           "poses.txt" ),
     m_egoMInpId_str (   "Integrated Ego-Motion Estimation" ),
      m_absolutePose_b (                               false ),
     m_maxDistance_f (                                 1.e6 ),
     m_cencSections_i (                                  50 ),
     m_x_v (                                                ),
     m_y_v (                                                ),
     m_z_v (                                                ),
     m_rx_v (                                               ),
     m_ry_v (                                               ),
     m_rz_v (                                               ),
     m_pitch_v (                                            ),
     m_yaw_v (                                              ),
    m_roll_v (                                             ),
    m_maxPoses_i (                                    4096 )
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
    ADD_BOOL_PARAMETER( "Absolute pose?", 
                        "Ego-Motion input is a step or a total motion estimate?",
                        m_absolutePose_b,
                        this,
                        AbsolutePose, 
                        CGTMapOp );

    ADD_UINT_PARAMETER( "Max number of poses",
                        "Max number of poses to show",
                        m_maxPoses_i,
                        this,
                        MaxPoses,
                        CGTMapOp);

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


   std::vector<std::string> names_v;
   names_v.push_back("Translation X");
   names_v.push_back("Translation Y");
   names_v.push_back("Translation Z");
   names_v.push_back("Rotation Axis X");
   names_v.push_back("Rotation Axis Y");
   names_v.push_back("Rotation Axis Z");
   names_v.push_back("Pitch");
   names_v.push_back("Yaw");
   names_v.push_back("Roll");
   
   for (int i = 0; i < 9; ++i)
   {
       registerDrawingList( names_v[i],
                            S2D<int> (0, 0),
                            false );
       addDrawingListParameter ( names_v[i] );
   }
}

/// Virtual destructor.
CGTMapOp::~CGTMapOp ()
{
}

/// Init event.
bool CGTMapOp::initialize()
{
    m_idx_i = 0;
   m_voPoses_v.clear();
   /// Load only if not already initialized.
   if ( m_poses_v.size() == 0 )
   {
      printf("Loading GT pose data\n");
      loadFromFile();
   }
    
   double idM_p[]={1,0,0,0,1,0,0,0,1};
   m_totalMotion.clear();
    
   m_x_v.clear();
   m_y_v.clear();
   m_z_v.clear();
   m_rx_v.clear();
   m_ry_v.clear();
   m_rz_v.clear();
   m_pitch_v.clear();
   m_yaw_v.clear();
   m_roll_v.clear();

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

        if ( !m_absolutePose_b )
        {
      m_totalMotion.rotation    = rotation * m_totalMotion.rotation;	
      m_totalMotion.translation =  rotation * m_totalMotion.translation + translation;
        }
        else
        {
            m_totalMotion.rotation    = rotation;
            m_totalMotion.translation = translation;
        }
        
      rotation    = m_totalMotion.rotation;
      translation = m_totalMotion.translation;

      C3DVector currPos = rotation.multiplyTransposed( -translation );
        
      Data newPos(2);
      newPos[0] = currPos.x();
      newPos[1] = currPos.z();

        if (m_voPoses_v.size() >= m_maxPoses_i )
        {
            size_t newSize_i = m_voPoses_v.size()/2;
            for (size_t i = 1 ; i < newSize_i; ++i)
                m_voPoses_v[i] = m_voPoses_v[i*2];

            m_voPoses_v.resize(newSize_i);
        }
        else
      m_voPoses_v.push_back( newPos );

        //if (m_x_v.size() != m_maxPoses_i);
        {
            int n_i = m_idx_i+1;
            m_x_v    .resize(n_i, 0.);
            m_y_v    .resize(n_i, 0.);
            m_z_v    .resize(n_i, 0.);
            m_rx_v   .resize(n_i, 0.);
            m_ry_v   .resize(n_i, 0.);
            m_rz_v   .resize(n_i, 0.);
            m_pitch_v.resize(n_i, 0.);
            m_yaw_v  .resize(n_i, 0.);
            m_roll_v .resize(n_i, 0.);
        }

        if (m_idx_i == m_maxPoses_i-1)
        {
            std::copy(m_x_v.begin()+1, m_x_v.end(), m_x_v.begin());
            std::copy(m_y_v.begin()+1, m_y_v.end(), m_y_v.begin());
            std::copy(m_z_v.begin()+1, m_z_v.end(), m_z_v.begin());
            std::copy(m_rx_v.begin()+1, m_rx_v.end(), m_rx_v.begin());
            std::copy(m_ry_v.begin()+1, m_ry_v.end(), m_ry_v.begin());
            std::copy(m_rz_v.begin()+1, m_rz_v.end(), m_rz_v.begin());
            std::copy(m_rx_v.begin()+1, m_rx_v.end(), m_rx_v.begin());
            std::copy(m_ry_v.begin()+1, m_ry_v.end(), m_ry_v.begin());
            std::copy(m_rz_v.begin()+1, m_rz_v.end(), m_rz_v.begin());
        }

        m_x_v[m_idx_i] = m_totalMotion.translation.x();
        m_y_v[m_idx_i] = m_totalMotion.translation.y();
        m_z_v[m_idx_i] = m_totalMotion.translation.z();

      C3DVector rotAxis;
      m_totalMotion.rotation.getRotationAxis(rotAxis);
        m_rx_v[m_idx_i] = rotAxis.x();
        m_ry_v[m_idx_i] = rotAxis.y();
        m_rz_v[m_idx_i] = rotAxis.z();

      double p_d,y_d,r_d;
      m_totalMotion.rotation.getRotationAngles(p_d,y_d,r_d);
        m_pitch_v[m_idx_i] = p_d;
        m_yaw_v  [m_idx_i] = y_d;
        m_roll_v [m_idx_i] = r_d;

        if (m_idx_i < m_maxPoses_i-1)
            ++m_idx_i;

        m_idx_i%=m_maxPoses_i;

      registerOutput ( "Total " + m_egoMInpId_str, &m_totalMotion );
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
      if (!m_voPoses_v.empty())
      {
         CColorEncoding colorEnc ( CColorEncoding::CET_HUE,S2D<float>(0,m_voPoses_v.size()));    
         
            S2D<float> newVisRangeX;
            S2D<float> newVisRangeY;
         newVisRangeX.min = 1.e20;
         newVisRangeX.max = -1.e20;
         newVisRangeY.min = 1.e20;
         newVisRangeY.max = -1.e20;
         
         for (unsigned int i = 0; i < m_voPoses_v.size(); ++i)
         {
                S2D<float> pos = S2D<float> ( m_voPoses_v[i][0], 
                                            m_voPoses_v[i][1] );
            
            newVisRangeX.min = std::min(newVisRangeX.min, m_voPoses_v[i][0]);
            newVisRangeX.max = std::max(newVisRangeX.max, m_voPoses_v[i][0]);
            newVisRangeY.min = std::min(newVisRangeY.min, m_voPoses_v[i][1]);
            newVisRangeY.max = std::max(newVisRangeY.max, m_voPoses_v[i][1]);
         }
      
            m_mapper.visRangeX.min = std::min(m_mapper.visRangeX.min, (double)newVisRangeX.min);
            m_mapper.visRangeX.max = std::max(m_mapper.visRangeX.max, (double)newVisRangeX.max);
         
            m_mapper.visRangeY.min = std::min(m_mapper.visRangeY.min, (double)newVisRangeY.min);
            m_mapper.visRangeY.max = std::max(m_mapper.visRangeY.max, (double)newVisRangeY.max);
         
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
         
         S2D<double> base = m_mapper.world2Screen ( S2D<double> ( m_voPoses_v.back()[0], 
                                                                  m_voPoses_v.back()[1] ) );
         
  
         C3DMatrix m = m_totalMotion.rotation.getInverse();
            const float hfov_d = 100/180. * M_PI/2;
         for (int i = 0; i < 3; ++i)
         {
             C3DVector dir;
             if (i == 0)  
                 dir = m * C3DVector(0,0,1); 
             else if (i == 1)
                 dir = m * C3DVector(sin(hfov_d),0,cos(hfov_d));
             else
                 dir = m * C3DVector(-sin(hfov_d),0,cos(hfov_d));

             C3DVector v  = m_totalMotion.translation + dir*100;

             S2D<double> pos = m_mapper.world2Screen ( S2D<double> ( v.x(), v.z() ) );
             list_p->setLineWidth(1);
             list_p->setLineColor((i==0)*255,(i==1)*255,(i==2)*255);         
             list_p->addClippedLine( base.x, base.y, pos.x, pos.y, 0, 0, getScreenSize().width, getScreenSize().height );
         }
      }
      
      if (!m_poses_v.empty() )
      {
         list_p->setLineWidth ( 1 );
         list_p -> setLineColor ( SRgb( 255, 255, 255 ) );
         
         S2D<double> prevpos = m_mapper.world2Screen ( S2D<double> ( m_poses_v[0][0], 
                                                                     m_poses_v[0][1] ) );
         
         for (unsigned int i = 1; i < m_poses_v.size(); ++i)
         {
            S2D<double> pos = m_mapper.world2Screen ( S2D<double> ( m_poses_v[i][0], 
                                                                    m_poses_v[i][1] ) );
            
            list_p->addLine ( pos.x, pos.y, prevpos.x, prevpos.y );
            prevpos=pos;
         }
      }

      std::vector<std::string> names_v;
      names_v.push_back("Translation X");
      names_v.push_back("Translation Y");
      names_v.push_back("Translation Z");
      names_v.push_back("Rotation Axis X");
      names_v.push_back("Rotation Axis Y");
      names_v.push_back("Rotation Axis Z");
      names_v.push_back("Pitch");
      names_v.push_back("Yaw");
      names_v.push_back("Roll");

      std::vector<float> * vectors[] = {&m_x_v, &m_y_v, &m_z_v, 
                                        &m_rx_v, &m_ry_v, &m_rz_v, 
                                        &m_pitch_v, &m_yaw_v, &m_roll_v};
      
      for (int i = 0; i < 9; ++i)
      {
          list_p = getDrawingList(names_v[i]);
          list_p->clear();
          if (list_p->isVisible())
          {
              CLinePlotter<float> plotter;
              plotter.setData( vectors[i] );
              
              if (i >= 3)
              {
                  plotter.setYRange(S2D<float>(-M_PI, M_PI));
              }
              
              list_p->setLineColor(255,255,255);
              plotter.plot(list_p, getScreenSize().width,getScreenSize().height );
              
              list_p->addText(names_v[i], 20, 420, 20, false);
              
          }
      }
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
                              "%f %f\n", 
                              &data[0],
                              &data[1] );

      if ( fieldsRead_i == 2 )
      {
         m_poses_v.push_back(data);
            m_mapper.visRangeX.min = std::min(m_mapper.visRangeX.min,(double)data[0]);
            m_mapper.visRangeX.max = std::max(m_mapper.visRangeX.max,(double)data[0]);
            m_mapper.visRangeY.min = std::min(m_mapper.visRangeY.min,(double)data[1]);
            m_mapper.visRangeY.max = std::max(m_mapper.visRangeY.max,(double)data[1]);
      }
   }

   m_mapper.visScale_d = std::min(getScreenSize().width /(m_mapper.visRangeX.max-m_mapper.visRangeX.min),
                                  getScreenSize().height/(m_mapper.visRangeY.max-m_mapper.visRangeY.min) );

   m_mapper.offset = S2D<double>(
      (getScreenSize().width  - m_mapper.visScale_d * (m_mapper.visRangeX.max-m_mapper.visRangeX.min))/2, 
      (getScreenSize().height - m_mapper.visScale_d * (m_mapper.visRangeY.max-m_mapper.visRangeY.min))/2 );
    

   printf("%i Ground truth poses read\n", (int)m_poses_v.size());
    
   free(strData_p);

   fclose ( file_p );

   return true;
}

