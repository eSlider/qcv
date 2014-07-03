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

/* INCLUDES */

#include <math.h>
#include "imagePyramid.h"

using namespace QCV;

CImagePyramid::CImagePyramid ( )
   : m_images_v (                     1 )
{
}

CImagePyramid::CImagePyramid ( unsigned int f_levels_ui )
   : m_images_v (                f_levels_ui, cv::Mat() )
{
}

CImagePyramid::~CImagePyramid ( )
{
}

cv::Mat &
CImagePyramid::getLevelImage ( unsigned int f_level_ui )
{
   static cv::Mat s_img;
   if (f_level_ui < m_images_v.size())
      return m_images_v[f_level_ui];
    
   return s_img;
}

cv::Mat 
CImagePyramid::getLevelImage ( unsigned int f_level_ui ) const
{
   if (f_level_ui < m_images_v.size())
      return m_images_v[f_level_ui];
    
   return cv::Mat();
}

bool
CImagePyramid::setLevels ( unsigned int f_levels_ui )
{
   if ( f_levels_ui != m_images_v.size() )
   {  
      m_images_v.resize(f_levels_ui, cv::Mat());
   }

   return true;
}

unsigned int
CImagePyramid::getLevels ( ) const
{
   return m_images_v.size();
}

unsigned int
CImagePyramid::getWidth ( ) const
{
   if (!m_images_v.empty())
      return m_images_v[0].cols;
   
   return 0;
}

unsigned int
CImagePyramid::getHeight ( ) const
{
   if (!m_images_v.empty())
      return m_images_v[0].rows;

   return 0;
}

void
CImagePyramid::clear ()
{
   for (unsigned int i = 0; i < m_images_v.size(); ++i)
   {
      m_images_v[i] = cv::Mat::zeros ( m_images_v[i].size(), m_images_v[i].type() );
   }    
}

bool 
CImagePyramid::compute ( const cv::Mat &f_image )
{
    /// Set first the first level with the original image.
    m_images_v[0] = f_image;
    
    for (unsigned int i = 1 ; i < m_images_v.size(); ++i)
       pyrDown( m_images_v[i-1], m_images_v[i], cv::Size( m_images_v[i-1].cols/2, m_images_v[i-1].rows/2 ) );
    
    return true;
}

