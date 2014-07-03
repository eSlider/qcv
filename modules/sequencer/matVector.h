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


#ifndef __MATVECTOR_H
#define __MATVECTOR_H

/**
*******************************************************************************
*
* @file matVector.cpp
*
* \class CMatVector
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Subclass of CVector with instantiation of cv::Mat as data
* type. It adds the constructor for obtaining data from a
* CInpImgFromFileVector object.
*
*******************************************************************************/

#include "qcvVector.h"
#include "imageFromFile.h"
#include <opencv/cv.h>

namespace QCV
{
    //typedef CVector<cv::Mat> CMatVector;
    
    class CMatVector: public CVector<cv::Mat>
    {
    public:
        CMatVector(): CVector<cv::Mat> ( ) { }
        CMatVector(int f_n_i): CVector<cv::Mat> ( f_n_i ) { }
        CMatVector(int f_n_i, cv::Mat f_defval): CVector<cv::Mat> ( f_n_i, f_defval ) { }
        CMatVector(cv::Mat f_elem): CVector<cv::Mat> ( f_elem ) { }
        CMatVector(const CMatVector &f_vec): CVector<cv::Mat> ( f_vec ) { }

        CMatVector ( const CInpImgFromFileVector & f_inp_v )
        {
            for (unsigned int i = 0; i < f_inp_v.size(); ++i)
            {
                std::vector<cv::Mat>::push_back ( f_inp_v[i].image );
            }
        }
    };
};

    

#endif
