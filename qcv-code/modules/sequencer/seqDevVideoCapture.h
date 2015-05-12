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

#ifndef __SEQDEVVIDEOCAPTURE_H
#define __SEQDEVVIDEOCAPTURE_H

/**
 *******************************************************************************
 *
 * @file seqDevVideoCapture.h
 *
 * \class CSeqDevVideoCapture
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Device control class for grabbing images from a video or camera.
 *
 * This class is derived from CSeqDeviceControl and implements the virtual
 * functions in the parent class. It registers as output the images read from a 
 * camera.
 *
 *******************************************************************************/

/* INCLUDES */

/* CONSTANTS */

/* INCLUDES */
#include "seqDeviceControl.h"
#include "imageFromFile.h"
#include "matVector.h"
#include "io.h"

#include <opencv/highgui.h>

#include <vector>
#include <map>
#include <QtCore/QObject>

/* PROTOTYPES */
class QTimer;
class QWidget;

namespace QCV
{   
    /* PROTOTYPES */
    class CSeqDevVideoCapture: public CSeqDeviceControl
    {
        Q_OBJECT

    /// Constructors, Destructors
    public:
        /// Constructor
        CSeqDevVideoCapture( std::string f_file_str = "" );

        /// Destructor
        virtual ~CSeqDevVideoCapture();

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
        virtual bool     setFrameSkip(int /*f_skip_i*/ ) { return false; };

        /// Set loop mode.
        virtual bool     setLoopMode( bool /*f_val_b*/ ) { return false; };

        virtual bool     setExitOnLastFrame( bool /*f_val_b*/ )  { return false; };

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

        /// Get current frame in the sequence.
        virtual void timeOut();

    /// Virtual signals
    signals:
        void start();
        void cycle();
        void reset();

    /// Private methods.
    private:
        
        
    /// Private constants.
    private:

    /// Protected members
    private:
        /// Timer for handling play actions.
        QTimer *                      m_qtPlay_p;

        /// Current frame
        int                           m_currentFrame_i;

        /// Number of frames of the sequence.
        int                           m_framesCount_i;

        /// Fetch flag
	bool                          m_fetch_b;

        /// Acknowledge flag
	bool                          m_acknowledge_b;
        
        /// Vector containing current image data, paths and timestamps
        CInpImgFromFileVector         m_imageData_v;

        /// Vector containing only current image data
        CMatVector                    m_imageVector_v;

        /// Video capture
        cv::VideoCapture *            m_capture_p;

    };
}

#endif // __SEQDEVVIDEOCAPTURE_H
