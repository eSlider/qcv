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

#ifndef __INPUTIMGFROMFILE_H
#define __INPUTIMGFROMFILE_H

/**
 *******************************************************************************
 *
 * @file imageFromFile.h
 *
 * \class SInpImgFromFile
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Simple struct to load images from file and pass them to the operators
 *
 *******************************************************************************/

/* INCLUDES */

/* CONSTANTS */

/* INCLUDES */
#include <vector>
#include <string>

#include <opencv/cv.h>

namespace QCV
{
    struct SInpImgFromFile
    {
        SInpImgFromFile():
            image (          ), 
            timeStamp_d ( 0. ),
            path_str   (  "" )
        {
        }

        cv::Mat     image;
        double      timeStamp_d;
        std::string path_str;
    };
      
    class CInpImgFromFileVector: public std::vector<SInpImgFromFile>
    {
    public:
        CInpImgFromFileVector(): std::vector<SInpImgFromFile> ( ) { }
        CInpImgFromFileVector(int f_n_i): std::vector<SInpImgFromFile> ( f_n_i ) { }
        CInpImgFromFileVector(int f_n_i, SInpImgFromFile f_defval): std::vector<SInpImgFromFile> ( f_n_i, f_defval ) { }
        CInpImgFromFileVector(cv::Mat f_elem): std::vector<SInpImgFromFile> ( ) 
        {
            SInpImgFromFile f;
            
            f.image = f_elem;
            std::vector<SInpImgFromFile>::push_back ( f );
        }

        /// Cast operator
        operator cv::Mat ()
        {
            static cv::Mat def;
            if (std::vector<SInpImgFromFile>::size() > 0)
                return operator [](0).image;
            else
                return def;
        }        
    };
}

#endif //__INPUTIMGFROMFILE_H
