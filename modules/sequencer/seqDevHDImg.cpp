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
* \file  seqDevHDImg.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QApplication>
#include <QDir>
#include <QTimer>
#include <opencv/highgui.h>

#include "seqDevHDImg.h"
#include "paramIOXmlFile.h"

using namespace QCV;


CSeqDevHDImg::CSeqDevHDImg(const std::string &f_confFilePath_str)
        : m_dialog_p (                          NULL ),
          m_qtPlay_p (                          NULL ),
          m_currentFrame_i (                      -1 ),
          m_framesCount_i (                        0 ),
          m_backward_b (                       false ),
          m_imagesPerFrame_uc (                    0 ),
          m_seqName_str (                         "" ),
          m_printDebug_b (                      true ),
          m_fskip_i (                              0 ),
          m_loopMode_b (                       false ),
          m_exitOnLastFrame_b (                false )
{
    m_qtPlay_p = new QTimer ( this );
    connect(m_qtPlay_p, SIGNAL(timeout()), this, SLOT(timeOut()));

    //connect(m_dialog_p, SIGNAL(reset()), this, SLOT(stopAndEmitReset()));

    m_currentState_e = S_PAUSED;

    if ( f_confFilePath_str != "" )
        loadNewSequence ( f_confFilePath_str );
}

/// Destructor
CSeqDevHDImg::~CSeqDevHDImg()
{
}

void
CSeqDevHDImg::timeOut()
{
    if (m_backward_b)
        prevFrame();
    else
        nextFrame();

    emit cycle( );
}

/// Initialize Device.
bool 
CSeqDevHDImg::initialize()
{
    m_currentFrame_i = 0;

    return loadCurrentFrame();
}

/// Load next frame
bool CSeqDevHDImg::nextFrame()
{
    ++m_currentFrame_i;
    m_currentFrame_i += m_fskip_i;

    if (m_currentFrame_i >= m_framesCount_i)
    {
        if ( !m_loopMode_b )
            m_currentFrame_i  =  m_framesCount_i-1;
        else
        {
            m_currentFrame_i  =  0;
        }
    }
    
    // Autoexit
    if (0 && m_currentFrame_i == m_framesCount_i)
        exit(0);

    if (m_currentFrame_i == m_framesCount_i-1)
    {
        if ( !m_loopMode_b )
            pause();

        if ( m_exitOnLastFrame_b )
            QApplication::exit(1);
    }
    
    return loadCurrentFrame();
}

/// Load next frame
bool CSeqDevHDImg::reloadFrame()
{
    return loadCurrentFrame();
}

/// Load previous frame
bool CSeqDevHDImg::loadCurrentFrame()
{
    bool res_b = true;
    
    if ( m_imageData_v.size() != m_imagesPerFrame_uc )
        m_imageData_v.resize( m_imagesPerFrame_uc );

    for (int i = 0 ; i < m_imagesPerFrame_uc; ++i)
    {
        if ( (unsigned int) m_currentFrame_i < m_fileName_p[i].size()  )
        {
            std::string fullPathFile_str = m_directoryPath_p[i];
            fullPathFile_str += "/";
            fullPathFile_str += m_fileName_p[i][m_currentFrame_i];

            m_filePaths_p[i] = (std::string) fullPathFile_str;

            if (!loadImageFile ( fullPathFile_str, m_imageData_v[i].image ))
            {
                res_b = false;
                printf("File \"%s\" could not be read", fullPathFile_str.c_str() );
            }

            m_timeStamps_p[i] = getTimeStampFromFilename(m_fileName_p[i][m_currentFrame_i]);

            m_imageData_v[i].path_str = fullPathFile_str;
            m_imageData_v[i].timeStamp_d = m_timeStamps_p[i];
            
        }
    }

    return res_b;
}

double CSeqDevHDImg::getTimeStampFromFilename( std::string f_fileName_p )
{
#if 0
    double tt_d = 0;
    int fieldsRead_i =  sscanf( f_fileName_p.c_str()+13, "%lfus", 
                                &tt_d );

    if (fieldsRead_i == 1)
        return tt_d/1000000.0;    
#else

    double tt_d = 0;
    
    int offset_i = f_fileName_p.find_first_of ( '_' );

    int fieldsRead_i =  sscanf( f_fileName_p.c_str() + offset_i + 10, "%lfus", 
                                &tt_d );

    if (fieldsRead_i == 1)
        return tt_d/1000000.0;    
#endif
    return -1.0;
}

inline bool 
CSeqDevHDImg::loadImageFile( std::string f_filePath_str, 
                             cv::Mat &   fr_image )
{
    fr_image = cv::imread ( f_filePath_str, -1 );

    return ( fr_image.size().width  > 0 && 
             fr_image.size().height > 0 );
}

/// Load previous frame
bool CSeqDevHDImg::prevFrame()
{
    --m_currentFrame_i;
    m_currentFrame_i -= m_fskip_i;

    if (m_currentFrame_i < 0 )
    {
        if ( !m_loopMode_b )
            m_currentFrame_i  =  0;
        else
            m_currentFrame_i  =  m_framesCount_i-1;
    }
    
    // Autoexit
    if (0 && m_currentFrame_i == m_framesCount_i)
        exit(0);

    if (m_currentFrame_i == 0)
    {
        if ( !m_loopMode_b )
            pause();
    }
    
    return loadCurrentFrame();
}

/// Load next frame
bool CSeqDevHDImg::goToFrame( int f_frameNumber_i )
{
    if ( f_frameNumber_i > 0 && f_frameNumber_i <= m_framesCount_i )
    {
        m_currentFrame_i = f_frameNumber_i - 1;
        return loadCurrentFrame();
    }
    
    return false;
}

/// Stop/Stand
bool CSeqDevHDImg::stop()
{
    m_currentFrame_i = 0;
    m_currentState_e = S_PAUSED;
    // Stop timer.
    m_qtPlay_p -> stop();    
    return true;
}

/// Play
bool CSeqDevHDImg::startPlaying()
{
    m_currentState_e = S_PLAYING;
    m_backward_b = false;

    if ( not m_qtPlay_p -> isActive() )
        m_qtPlay_p -> start(1);

    return true;
}

/// Play Backwards
bool CSeqDevHDImg::startPlayingBackward()
{
    m_currentState_e = S_PLAYING_BACKWARD;
    m_backward_b = true;

    if ( not m_qtPlay_p -> isActive() )
        m_qtPlay_p -> start(1);

    return true;
}

/// Pause
bool CSeqDevHDImg::pause()
{
    m_currentState_e = S_PAUSED;
    m_qtPlay_p -> stop();
    return true;
}

/*
void CSeqDevHDImg::reset()
{

}
*/

/// Get number of frames in this sequence.
int CSeqDevHDImg::getNumberOfFrames() const
{
    return m_framesCount_i;
}
 
/// Get current frame in the sequence.
int CSeqDevHDImg::getCurrentFrame() const
{
    return m_currentFrame_i+1;
}

/// Is a forward/backward device?
bool CSeqDevHDImg::isBidirectional() const
{
    return true;
}

/// Get the dialogs of this device.
std::vector<QWidget *> CSeqDevHDImg::getDialogs ( ) const
{
    return std::vector<QWidget *>();
}

bool 
CSeqDevHDImg::loadNewSequence ( const std::string &f_confFilePath_str )
{
    CParamIOXmlFile  paraReader;
    bool ok_b;

    ok_b = paraReader.load ( f_confFilePath_str );

    /// Get directory path from full path.
    std::string f_pathToConfFile_str = f_confFilePath_str;

    int pos_i = f_pathToConfFile_str.find_last_of ("/\\");

    if ( m_printDebug_b )
        printf("loading new sequence %i\n", pos_i);
    
    if ( pos_i != -1 )
        f_pathToConfFile_str.erase(pos_i);
    else
        f_pathToConfFile_str = std::string(".");
    
    if ( m_printDebug_b )
        printf("Path to configuration file is = %s\n", f_pathToConfFile_str.c_str());

    ok_b = paraReader.setCurrentCategory ( "Image Sequence" );

    if (!ok_b)
    {
        //logger::warn ( "Category \"Sequence\" was not found in %s.", 
        //               f_confFilePath_str );
        printf ( "Category \"Image Sequence\" was not found in %s.\n", 
                 f_confFilePath_str.c_str() );
        m_imagesPerFrame_uc = 0;
    }
    else
    {
        int i;
        char paramName_str[256];
        
        /// Lets load the image paths.
        for ( i = 0 ; i < m_maxImgsPerFrame_uc ; ++i )
        {
            sprintf ( paramName_str, "Relative Directory %i", i );
            std::string dir_str;
            ok_b = paraReader.get ( paramName_str, dir_str );
            if (not ok_b) break;
            
            sprintf ( paramName_str, "Filter Image %i", i );
            std::string filter_str;
            ok_b = paraReader.get ( paramName_str, filter_str );
            if (not ok_b) break;
            
            //m_directoryPath_p[i] = f_pathToConfFile_str;
            //m_directoryPath_p[i] += "/";
            m_directoryPath_p[i] = dir_str;
            //printf("  directory %s\n", m_directoryPath_p[i].c_str());
        
            findFiles ( m_directoryPath_p[i], filter_str, m_fileName_p[i] );
        }
        
        if (i == 0)
        {
            //logger::warn ( "No entries for image sequences have been found in %s.", 
            //               f_confFilePath_str );
            if ( m_printDebug_b )
                printf ( "No entries for image sequences have been found in %s.", 
                         f_confFilePath_str.c_str() );
        }
        else
        {
            if ( m_printDebug_b )
                printf("Sequence read from configuration file:\n");

            /// update the number of frames, give corresponding warnings if number
            /// of images is not equal, etc...
            m_framesCount_i = 0;
            
            for (int j = 0 ; j < i; ++j)
            {
                m_framesCount_i = std::max(m_framesCount_i, 
                                           (int)m_fileName_p[j].size() );

                if ( m_printDebug_b )
                    printf("\tDirectory: %s - number of files %i\n", 
                           m_directoryPath_p[j].c_str(),
                           (int)m_fileName_p[j].size());
            }
        }
        
        m_imagesPerFrame_uc = i;

        emit start();
    }
    
    return m_imagesPerFrame_uc != 0;
}

void 
CSeqDevHDImg::findFiles ( std::string    f_fullPath_str, 
                          std::string    filter_str, 
                          std::vector<std::string>
                                        &fr_fileNames ) const
{
    QDir        dir (f_fullPath_str.c_str());
    QString     qsFilter (filter_str.c_str());
    QStringList qslFilters;
    QStringList qslImageFiles;

    qslFilters << qsFilter;

    qslImageFiles = dir.entryList(qslFilters, QDir::Files | QDir::Readable);

    /// Transform to standard vector.
    for (int i = 0; i < qslImageFiles.count(); ++i)
    {
        fr_fileNames.push_back ( qslImageFiles[i].toStdString() );
    }    
}

bool
CSeqDevHDImg::registerOutputs ( 
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

        sprintf(txt, "Image %i Timestamp", i);
        fr_map[txt] = new CIO<double>(&m_imageData_v[i].timeStamp_d);

        sprintf(txt, "Image %i Path", i);
        fr_map[txt] = new CIO<std::string>(&m_imageData_v[i].path_str);

    }
    
    fr_map[ "Frame Number" ] = new CIO<int>(&m_currentFrame_i);
    fr_map[ "Frame Count" ]  = new CIO<int>(&m_framesCount_i);

    return true;
}
