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
* \file  remapper
* \author Hernan Badino
* \date  Mon Aug 30 16:14:28 EDT 2010
* \notes 
*******************************************************************************
*****          (C) COPYRIGHT Hernan Badino - All Rights Reserved          *****
******************************************************************************/

/* INCLUDES */
#include <stdio.h>
#include "imgRemapper.h"
#include <errno.h>
#include <string.h>

using namespace QCV;


CImgRemapper::CImgRemapper()
        :  m_outWidth_i (              0 ),
           m_outHeight_i (             0 ),
           m_offsetU_i (               0 ),
           m_offsetV_i (               0 ),
           m_nnInterpolation_b (   false )
{
}

CImgRemapper::~CImgRemapper()
{}

bool
CImgRemapper::load ( std::string f_path_str )
{
    FILE *file_p = fopen(f_path_str.c_str(), "r");

    if (!file_p)
    {
        printf("File %s could not be opened\n",
               f_path_str.c_str());
        printf("%s\n",
               strerror(errno));
        
        return false;
    }

    int n_i = fscanf( file_p, "%i %i\n%i %i\n", 
                      &m_outWidth_i, 
                      &m_outHeight_i,
                      &m_offsetU_i,
                      &m_offsetV_i );

    if (n_i!=4)
    {
        printf("In file %s, image size could not be read\n",
               f_path_str.c_str());
        fclose(file_p);
        m_outWidth_i = 0;
        m_outHeight_i = 0;
        m_offsetU_i = 0;
        m_offsetV_i = 0;
        return false;        
    }    

    m_lutU = cv::Mat(m_outHeight_i, m_outWidth_i, CV_32FC1, cv::Scalar(-1) );
    m_lutV = cv::Mat(m_outHeight_i, m_outWidth_i, CV_32FC1, cv::Scalar(-1) );

    n_i = fread ( m_lutU.data, m_outWidth_i*m_outHeight_i*sizeof(float), 1, file_p);

    if (n_i != 1)
    {
        printf("Could not read LUT from file %s\n",
               f_path_str.c_str());        
        fclose(file_p);
        m_outWidth_i = 0;
        m_outHeight_i = 0;         
        
        return false;
    }

    n_i = fread ( m_lutV.data, m_outWidth_i*m_outHeight_i*sizeof(float), 1, file_p);
    
    if (n_i!=1)
    {
        printf("Could not read LUT from file %s\n",
               f_path_str.c_str());
        fclose(file_p);
        m_outWidth_i = 0;
        m_outHeight_i = 0;         
        return false;
    }

    return true;
}

bool
CImgRemapper::setOutputSize(S2D<unsigned int> f_size)
{
    if ( m_outWidth_i  != (int)f_size.width ||
         m_outHeight_i != (int)f_size.height )
    {
        m_outWidth_i  = f_size.width;
        m_outHeight_i = f_size.height;

        clearLuts();
    }    

    return true;
}

void
CImgRemapper::clearLuts()
{
   m_lutU = cv::Mat(m_outHeight_i, m_outWidth_i, CV_32FC1, cv::Scalar(-1) );
   m_lutV = cv::Mat(m_outHeight_i, m_outWidth_i, CV_32FC1, cv::Scalar(-1) );
}

bool
CImgRemapper::remap ( const cv::Mat &f_input,
                      cv::Mat       &fr_output )
{
    if ( fr_output.size() != m_lutU.size() )
    {
        printf("CImgRemapper::remap output image has wrong size.\n");
        printf("Expected image size is %i %i\n", 
               m_lutU.size().width, m_lutU.size().height );
        printf("Received image size is %i %i.\n", 
               fr_output.size().width, 
               fr_output.size().height );
        return false;
    }

    const int interpolation_i = m_nnInterpolation_b?cv::INTER_NEAREST:cv::INTER_LINEAR;
        
    cv::remap( f_input, fr_output, m_lutU, m_lutV, interpolation_i, cv::BORDER_TRANSPARENT, 0);

    return true;
}


bool 
CImgRemapper::setNNInterpolation( bool f_val_b )
{
    m_nnInterpolation_b = f_val_b;
    return true;
}

bool 
CImgRemapper::getNNInterpolation( ) const
{
    return m_nnInterpolation_b;
}



