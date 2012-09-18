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

#endif
