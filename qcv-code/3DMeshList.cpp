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
 * \file  3DMeshList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */

#include "3DMeshList.h"
#include "glheader.h"

using namespace QCV;

C3DMeshList::C3DMeshList( int /* f_bufferSize_i */ )
{}

/// Destructor.
C3DMeshList::~C3DMeshList()
{}

// Add 3DMeshs from other list.
bool
C3DMeshList::add ( const C3DMeshList &f_otherList )
{
    m_mesh_v.insert( m_mesh_v.begin(), 
                     f_otherList.m_mesh_v.begin(),
                     f_otherList.m_mesh_v.end() );
    
    return true;
}

// Draw all 3DMeshs.
bool
C3DMeshList::add (  cv::Mat     f_vectorImg,
                    cv::Mat     f_dispTexture,
                    const float f_maxDist_f,
                    const float f_maxInvDist_f )
{
    if ( f_vectorImg.cols == 0 || f_vectorImg.cols == 0 )
    {
        printf("%s:%i Invalid size of input 3D point image\n", __FILE__, __LINE__);
        return false;
    }
    
    if ( f_vectorImg.type() != CV_64FC3 ) 
    {
        printf("%s:%i The input type of the 3D point image must be CV_64FC3\n", __FILE__, __LINE__);
        return false;
    }
    
    if ( f_vectorImg.size() != f_dispTexture.size() )
    {
        printf("%s:%i The size of the input image and the disparity texture are different\n", __FILE__, __LINE__);
        return false;
    }
    
    if ( f_dispTexture.type() != CV_8U && f_dispTexture.type() != CV_8UC3 )
    {
        printf("%s:%i The texture image must be of type CV_8U or  CV_8UC3.", __FILE__, __LINE__);
        return false;
    }

    m_mesh_v.push_back( SMeshData() );

    SMeshData & new3DMesh = m_mesh_v.back();
    
    new3DMesh.vectorImg      = f_vectorImg;
    new3DMesh.dispTexture    = f_dispTexture;
    new3DMesh.maxDist_f      = f_maxDist_f;
    new3DMesh.maxInvDist_f   = f_maxInvDist_f;
    new3DMesh.created_b      = false;
    new3DMesh.displayList_ui = 0;

    return true;
}

// Clear all 3DMeshs.
bool
C3DMeshList::clear ()
{
    std::vector< SMeshData >::const_iterator last = m_mesh_v.end();
    for (std::vector< SMeshData >::const_iterator m = m_mesh_v.begin(); 
         m != last; ++m )
        if (m->created_b)
            glDeleteLists( m->displayList_ui, 1 );
    
    m_mesh_v.clear();
    return m_mesh_v.empty();
}

// Draw all Meshes.
bool
C3DMeshList::show () 
{
    std::vector< SMeshData >::iterator last = m_mesh_v.end();

    for (std::vector< SMeshData >::iterator m = m_mesh_v.begin(); 
         m != last; ++m )
    {   
        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();

        if ( m->created_b == true && 
             m->displayList_ui != 0 )
            glCallList( m->displayList_ui );
        else
        {
            m->displayList_ui  = glGenLists(1);
            m->created_b       = true;

            const C3DVector *vimg_p = &m->vectorImg.at<C3DVector>(0,0);

            const int rows_i = (int) m->vectorImg.rows;
            const int cols_i = (int) m->vectorImg.cols;

            glNewList( m->displayList_ui, GL_COMPILE );

            glBegin(GL_TRIANGLES);
            
            int s_i = 1;
            
            bool      isColorImage_b = m->dispTexture.type() == CV_8UC3;
            SRgb *    cImg_p         = &m->dispTexture.at<SRgb>(0,0);
            uint8_t * gImg_p         = &m->dispTexture.at<unsigned char>(0,0);

            float maxSqDist_f  = m->maxDist_f * m->maxDist_f;
            float maxSqInvDist_f = 1./m->maxInvDist_f * 1./m->maxInvDist_f;
            for (int i = 0 ; i < rows_i-s_i-1; i+=s_i)
            {
                for (int j = 0 ; j < cols_i-s_i-1; j+=s_i)
                {
                    int idx[4] = { (i    ) * cols_i + j,
                                   (i+s_i) * cols_i + j,
                                   (i    ) * cols_i + j+s_i,
                                   (i+s_i) * cols_i + j+s_i };

                    C3DVector vecs[4] = { vimg_p[idx[0]],
                                          vimg_p[idx[1]],
                                          vimg_p[idx[2]],
                                          vimg_p[idx[3]] };

                    SRgb colors[4];

                    int t;
                    for (t = 0; t < 4; ++t)
                    {
                        if ( !vecs[t] ) break;

                        if (t < 3)
                        {
                            float sqDist_f = (vecs[t] - vecs[t+1]).sumOfSquares();
                            if (  sqDist_f > maxSqDist_f ||
                                  (sqDist_f && 1./sqDist_f < maxSqInvDist_f) )
                                break;
                        }

                        if (!isColorImage_b)
                        {
                            unsigned short int val = gImg_p[idx[t]];
                            colors[t].set( val, val, val );
                        }
                        else
                        {
                            colors[t] = cImg_p[idx[t]];
                        }                            
                    }

                    if (t<4) continue;
                
                    for (t = 0; t < 2; ++t)
                    {
                        SRgb color;
                        //glBegin(GL_LINE_LOOP);                  

                        for (int k = 0; k < 3; ++k)
                        {
                            glColor4f( colors[t+k].r/255., 
                                       colors[t+k].g/255., 
                                       colors[t+k].b/255.,
                                       1.0 );
                        
                            glVertex3f( vecs[t+k].x(), 
                                        vecs[t+k].y(), 
                                        vecs[t+k].z() );
                        }

                        //glEnd();
                    }
                }
            }

            glEnd();

            glEndList();

            glCallList( m->displayList_ui );
        }
        glPopMatrix();
    }

    /// Todo: Check GL status and return value.
    return true;
}

/// Return number of elements.
int
C3DMeshList::getSize () const
{
    return m_mesh_v.size();    
}
