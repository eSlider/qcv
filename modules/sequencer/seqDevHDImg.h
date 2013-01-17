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

#ifndef __SEQDEVHDIMGSEQ_H
#define __SEQDEVHDIMGSEQ_H

/**
 *******************************************************************************
 *
 * @file seqDevHDImg.h
 *
 * \class CSeqDevHDImg
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
#include "seqDeviceControl.h"
#include "imageFromFile.h"
#include "matVector.h"
#include "io.h"

#include <vector>
#include <map>
#include <QtCore/QObject>

/* PROTOTYPES */
class QTimer;
class QWidget;

namespace QCV
{   
    /// Max number of images per frame.
    const unsigned char   m_maxImgsPerFrame_uc = 255;

    /* PROTOTYPES */
    class CSeqDevHDImgDlg;
    
    class CSeqDevHDImg: public CSeqDeviceControl
    {
        Q_OBJECT

    /// Constructors, Destructors
    public:
        /// Constructor
        CSeqDevHDImg( const std::string &f_confFilePath_str = "" );

        /// Destructor
        virtual ~CSeqDevHDImg();

    /// Sequence Handling.
    public:
        /// Initialize Device.
        virtual bool initialize();

        /// Load next frame
        virtual bool nextFrame();

        /// Load previous frame
        virtual bool prevFrame();

        /// Load next frame
        virtual bool reloadFrame();

        /// Load next frame
        virtual bool goToFrame( int f_frameNumber_i );

        /// Stop/Stand
        virtual bool stop();

         /// Play
        virtual bool startPlaying();

         /// Play Backwards
        virtual bool startPlayingBackward();

         /// Pause
        virtual bool pause();

        /// Make random jump
        // virtual bool makeRandomJump();

    /// Get/Set 
    public:
        
        /// Set the number of frames to skip.
        virtual bool     setFrameSkip(int f_skip_i ) { m_fskip_i = f_skip_i; return true; };

        /// Set loop mode.
        virtual bool     setLoopMode( bool f_val_b ) { m_loopMode_b = f_val_b; return true; };

        virtual bool     setExitOnLastFrame( bool f_val_b )  { m_exitOnLastFrame_b = f_val_b; return true; };

        /// Get number of frames in this sequence.
        virtual int getNumberOfFrames() const;
 
        /// Get current frame in the sequence.
        virtual int getCurrentFrame() const;

        /// Is a forward/backward device?
        virtual bool isBidirectional() const;

        /// Get the dialogs of this device.
        virtual std::vector<QWidget *> getDialogs ( ) const;

        /// Derived from CSeqDeviceControl
        bool     isInitialized() const { return m_framesCount_i > 0; }

    /// Register outputs
    public:
        //virtual bool registerOutputs ( CInpImgFromFileVector & f_input_v );

        virtual bool registerOutputs ( 
                std::map< std::string, CIOBase* > &fr_map );

    /// Register outputs
    public slots:
        virtual bool loadNewSequence ( const std::string &f_confFilePath_str );

        /// Get current frame in the sequence.
        virtual void timeOut();

    /// Virtual signals
    signals:
        void start();
        void cycle();
        void reset();

    /// Private methods.
    private:
        void   findFiles ( std::string    f_fullPath_str, 
                           std::string    filter_str, 
                           std::vector<std::string>  
                                         &fr_fileNames ) const;
        
        bool   loadCurrentFrame();

        bool   loadImageFile( std::string f_filePath_str, cv::Mat &fr_image_p );

        double getTimeStampFromFilename( std::string f_fileName_p );
        
    /// Private constants.
    private:

    /// Protected members
    private:
        /// Dialog.
        CSeqDevHDImgDlg *             m_dialog_p;

        /// Timer for handling play actions.
        QTimer *                      m_qtPlay_p;

        /// Current frame
        int                           m_currentFrame_i;

        /// Number of frames of the sequence.
        int                           m_framesCount_i;

        /// Backward play.
        bool                          m_backward_b;

        /// Max number of images per frame.
        unsigned char                 m_imagesPerFrame_uc;

        /// Vector containing current image data, paths and timestamps
        CInpImgFromFileVector         m_imageData_v;

        /// Vector containing only current image data
        CMatVector                    m_imageVector_v;

        /// Buffer of directories where files are located.
        std::string                   m_directoryPath_p[m_maxImgsPerFrame_uc];

        // /// Buffer of output images.
        std::vector<std::string>      m_fileName_p[m_maxImgsPerFrame_uc];

        /// Name of the sequence.
        std::string                   m_filePaths_p[m_maxImgsPerFrame_uc];

        /// Output object for frame number.
        unsigned int                  m_imgNr_ui;

        /// Time stamps of the images.
        double                        m_timeStamps_p[m_maxImgsPerFrame_uc];

        /// Name of the sequence.
        std::string                   m_seqName_str;
        
        /// Print Debug information
        bool                          m_printDebug_b;

        /// Frame skip.
        int                           m_fskip_i;
        
        /// Loop mode.
        bool                          m_loopMode_b;

        /// Exit on last frame.
        bool                          m_exitOnLastFrame_b;
    };
}

#endif // __SEQDEVHDIMGSEQ_H
