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
* \file  seqDevVideoCapture.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

#include "seqDevVideoCapture.h"

/* INCLUDES */
#include <QDir>
#include <QTimer>
#include <sys/time.h>

using namespace QCV;

CSeqDevVideoCapture::CSeqDevVideoCapture ( std::string f_file_str )
        : m_qtPlay_p (                          NULL ),
          m_currentFrame_i (                       0 ),
          m_framesCount_i (                        1 )          
{    
    m_qtPlay_p = new QTimer ( this );
    connect(m_qtPlay_p, SIGNAL(timeout()), this, SLOT(timeOut()));

    m_currentState_e = S_PAUSED;

    if ( f_file_str == "" ) 
        m_capture_p = new cv::VideoCapture ( 0 );
    else
        m_capture_p = new cv::VideoCapture ( f_file_str );
  
    nextFrame();
}

/// Destructor
CSeqDevVideoCapture::~CSeqDevVideoCapture()
{
}

void
CSeqDevVideoCapture::timeOut()
{
    nextFrame();

    emit cycle( );
}

/// Initialize Device.
bool 
CSeqDevVideoCapture::initialize()
{
    m_currentFrame_i = 0;

    return true;
}

/// Load next frame
bool CSeqDevVideoCapture::nextFrame()
{
    cv::Mat frame;
    
    if ( !m_capture_p->isOpened())
    {
        printf("Problem initializing video capture.\n");
        return false;
    }

    /// Get image.
    (*m_capture_p) >> frame; // get a new frame from camera

    if ( frame.size().width < 0 ) return false;

    m_imageData_v.resize(1);
    frame.copyTo(m_imageData_v[0].image);

    /// Get time stamp.
    m_imageData_v[0].timeStamp_d = m_capture_p->get ( CV_CAP_PROP_POS_MSEC );

    ++m_framesCount_i;
    ++m_currentFrame_i;

    return true;
}

/// Load next frame
bool CSeqDevVideoCapture::reloadFrame()
{
    return false;
}

/// Load previous frame
bool CSeqDevVideoCapture::prevFrame()
{
    return false;
}

/// Load next frame
bool CSeqDevVideoCapture::goToFrame( int /*f_frameNumber_i*/ )
{
    return false;
}

/// Stop/Stand
bool CSeqDevVideoCapture::stop()
{
    m_currentFrame_i = 0;
    m_currentState_e = S_PAUSED;
    // Stop timer.
    m_qtPlay_p -> stop();    
    return true;
}

/// Play
bool CSeqDevVideoCapture::startPlaying()
{
    m_currentState_e = S_PLAYING;

    if ( not m_qtPlay_p -> isActive() )
        m_qtPlay_p -> start(1);

    return true;
}

/// Play Backwards
bool CSeqDevVideoCapture::startPlayingBackward()
{

    return false;
}

/// Pause
bool CSeqDevVideoCapture::pause()
{
    m_currentState_e = S_PAUSED;
    m_qtPlay_p -> stop();
    return true;
}

/// Get number of frames in this sequence.
int CSeqDevVideoCapture::getNumberOfFrames() const
{
    return m_framesCount_i;
}
 
/// Get current frame in the sequence.
int CSeqDevVideoCapture::getCurrentFrame() const
{
    return m_currentFrame_i+1;
}

/// Is a forward/backward device?
bool CSeqDevVideoCapture::isBidirectional() const
{
    return false;
}

/// Get the dialogs of this device.
std::vector<QWidget *> CSeqDevVideoCapture::getDialogs ( ) const
{
    return std::vector<QWidget *>();
}

// bool CSeqDevVideoCapture::registerOutputs ( CInpImgFromFileVector & f_input_v )
// {
//     if ( m_imageData_v.size() == 0 ||
//          m_imageData_v[0].image.size().width == 0 )
//         return false;
    
//     f_input_v.clear();

//     f_input_v.insert ( f_input_v.begin(), 
//                        m_imageData_v.begin(), 
//                        m_imageData_v.end() );

//     return true;
// }

bool CSeqDevVideoCapture::registerOutputs ( 
        std::map< std::string, CIOBase* > &fr_map )
{
    fr_map[ "Device Images" ] = new CIO<CInpImgFromFileVector>(&m_imageData_v);

    m_imageVector_v = m_imageData_v;
    
    fr_map[ "Input Images" ] = new CIO<CMatVector>(&m_imageVector_v);

    char txt[256];
    
    for ( uint8_t i = 0 ; i < m_imageData_v.size() ; ++i)
    {
        sprintf(txt, "Image %i", i);
        fr_map[txt] = new CIO<cv::Mat>(&m_imageData_v[i].image);

        sprintf(txt, "Image %i Path", i);
        fr_map[txt] = new CIO<std::string>(NULL);
    }
    
    fr_map[ "Frame Number" ] = new CIO<int>(&m_currentFrame_i);

    return true;
}
