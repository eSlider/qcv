/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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
 * @file seqDevHDImg.h
 *
 * \class seqDevHDImgSeq
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Device control class for reading sequence of image from the hard disk.
 *
 * This class is derived from CSeqDeviceControl and implements the virtual
 * functions in the parent class. It registers as output the images read from a 
 * directory specified throught parameters obtained from a xml file (with a call to
 * loadNewSequence ( const std::string )).
 *
 *******************************************************************************/

/* INCLUDES */

/* CONSTANTS */

/* INCLUDES */
#include <vector>
#include <string>

#include <cv.h>

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
      
    typedef std::vector<SInpImgFromFile> TInpImgFromFileVector;
}

#endif //__INPUTIMGFROMFILE_H
