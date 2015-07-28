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

#ifndef __SEQDEVICECONTROL_H
#define __SEQDEVICECONTROL_H

/**
 *******************************************************************************
 *
 * @file   seqDeviceControl.h
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 *******************************************************************************/

/* INCLUDES */
#include <QtCore/QObject>
#include <map>
#include <vector>


/* CONSTANTS */

class QWidget;

namespace QCV
{
    /* PROTOTYPES */
    class CParameterSet;
    class CIOBase;
    
    class CSeqDeviceControl: public QObject
    {
        Q_OBJECT

    /// Public data types
    public:
        typedef enum 
        {
            S_PLAYING = -1,
            S_PAUSED,
            S_PLAYING_BACKWARD
        } EState_t;         

    /// Constructors, Destructors
    public:
        /// Constructor
        CSeqDeviceControl()
                : m_currentState_e (     S_PAUSED ) {  }

        /// Destructor
        virtual ~CSeqDeviceControl() {};

    /// Sequence Handling.
    public:
        /// Initialize Device.
        virtual bool     initialize() = 0;

        /// Load next frame
        virtual bool     reloadFrame() = 0;

        /// Load next frame
        virtual bool     nextFrame() = 0;

        /// Load previous frame
        virtual bool     prevFrame() = 0;

        /// Load next frame
        virtual bool     goToFrame( int f_frameNumber_i ) = 0;

        /// Stop/Stand
        virtual bool     stop() = 0;

         /// Play
        virtual bool     startPlaying() = 0;

         /// Play Backwards
        virtual bool     startPlayingBackward() = 0;

         /// Pause
        virtual bool     pause() = 0;

        /// Load next frame
        virtual bool     goToFirstFrame() { return goToFrame(1); }

        /// Set loop mode.
        virtual bool     makeRandomJump(  ) { return false; };

        /// Load next frame
        virtual bool     goToLastFrame() { return goToFrame(getNumberOfFrames()); }

        /// Set the number of frames to skip.
        virtual bool     setFrameSkip(int /*f_skip_i*/ ) { return false; };

        /// Set loop mode.
        virtual bool     setLoopMode( bool /* f_val_b */ ) { return false; };

        /// Set loop mode.
        virtual bool     setExitOnLastFrame( bool /* f_val_b */ ) { return false; };

   /// Get/Set 
    public slots:
        
        /// Get the number of frames in the sequence.
        virtual int      getNumberOfFrames() const = 0;

        /// Get current frame in the sequence.
        virtual int      getCurrentFrame() const = 0;

        /// Is a forward/backward device?
        virtual bool     isBidirectional() const = 0;

        /// Get the state of the device.
        virtual EState_t getState() const { return m_currentState_e; }

        /// Get the dialogs of this device.
        virtual std::vector<QWidget *> 
                         getDialogs ( ) const
        { 
            std::vector<QWidget *> diag_v;
            return diag_v;
        }

	virtual void     initializeDevice () {};

        /// Is initialized?
        virtual bool     isInitialized() const { return false; }
 
        /// Load next frame
        virtual CParameterSet *  
        getParameterSet( const std::string /*f_name_str*/ )  { return NULL; }

        std::string getName () const 
        {
            return "Unnamed device";
        }
        
    /// Register outputs
    public:
        virtual bool registerOutputs ( 
            std::map< std::string, CIOBase* > & /*fr_map*/ ) { return false;  }

    /// Provide feedback info to the device
    public:
        virtual void updateOutput ( 
                                   std::map< std::string, CIOBase* > /*fr_map*/ ) {  }
        

    /// Virtual signals
    signals:
        void     start();
        void     cycle();
        void     reset();
        

    /// Protected members
    protected:
        /// State
        EState_t         m_currentState_e;

    /// Private members
    private:
        void dummy() { emit cycle(); emit reset(); }
        

    };
}

#endif // __SEQDEVICECONTROL_H
