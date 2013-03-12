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

#ifndef __SEQCONTROL_H
#define __SEQCONTROL_H

/**
 *******************************************************************************
 *
 * @file seqControlDlg.h
 *
 * \class seqControlDlg
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Control dialog for handling sequences.
 *
 * This class implements a dialog having the typical buttons for handling sequences
 * of images (play, stop, pause, next, previous, etc). The dialog emits signals
 * for to inform other classes about the input of the user.
 *
 *******************************************************************************/

/* INCLUDES */
#include "ui_seqControlDlg.h"

/* CONSTANTS */

namespace QCV
{
    class CSeqControlDlg: public QWidget, private Ui::CSeqControlDlgDesign
    {
        Q_OBJECT
    //// Constants
    public:
        typedef enum
        {
            IPM_FIRST_FRAME,
            IPM_LAST_FRAME,
            IPM_MIDDLE_FRAME
        } EFramePositionMode_t;

        typedef enum
        {
            AM_DISABLED,
            AM_PLAY,
            AM_PLAY_BACKWARD,
            AM_PAUSE
        } EActionMode_t;

        typedef enum
        {
            DT_BIDIRECTIONAL,
            DT_FORWARD_ONLY
        } EDeviceType_t;

    //// Constructor, Destructor
    public:

        CSeqControlDlg (QWidget *f_parent_p = 0);
        virtual ~CSeqControlDlg();

    //// Set and Gets.
    public:
        /// Set the number of frame in sequence.
        void setNumberOfFrames ( int f_numFrames_i );

        /// Get the number of frame in sequence.
        int  getFramesNumber( ) const { return m_numFrames_i; }

        /// Set current frame in sequence.
        void setCurrentFrame( int f_currentFrame_i );

        /// Exit on last frame?
        bool exitOnLastFrame () const { return m_cbExit_p->checkState() == Qt::Checked; }
        
    /// Set modes.
    public:
        EFramePositionMode_t getPositionMode( ) { return m_positionMode_e; }        
        void setPositionMode ( EFramePositionMode_t f_newMode_e);
        
        EActionMode_t getActionMode( ) { return m_actionMode_e; }        
        void setActionMode ( EActionMode_t f_newMode_e);

        EDeviceType_t getDeviceType( ) { return m_deviceType_e; }        
        void setDeviceType ( EDeviceType_t f_newMode_e);

    /// Automatic Signal/Slot connections.
    public slots:
        /// Begin Clicked.
        void on_m_qpbBegin_p_clicked() { setPositionMode ( IPM_FIRST_FRAME ); emit begin(); }

        /// Stop Clicked.
        void on_m_qpbStop_p_clicked() { setActionMode ( AM_PAUSE ); setPositionMode ( IPM_FIRST_FRAME ); emit stop();  }

        /// Previous Clicked.
        void on_m_qpbPrev_p_clicked() { emit previous(); }

        /// Play Backward Clicked.
        void on_m_qpbPlayBack_p_clicked() { setActionMode ( AM_PLAY_BACKWARD ); emit playBackward(); }

        /// Pause Clicked.
        void on_m_qpbPause_p_clicked() { setActionMode ( AM_PAUSE ); emit pause(); }

        /// Play Clicked.
        void on_m_qpbPlay_p_clicked() { setActionMode ( AM_PLAY); emit play(); }

        /// Next Clicked.
        void on_m_qpbNext_p_clicked() { emit next(); }

        /// Last Clicked.
        void on_m_qpbEnd_p_clicked() { setPositionMode ( IPM_LAST_FRAME );  emit end(); }
        
        /// Last Clicked.
        void on_m_qpbReload_p_clicked() { emit reload(); }

        /// Slider pressed.
        void on_m_qsPosition_p_valueChanged ( int f_value_i );

        /// Slider released.        
        void on_m_qsPosition_p_sliderPressed ( )
        {
            m_sliderPressed_b = true;
        }

        /// Skip frames changed
        void on_m_sbSkip_p_valueChanged (int f_value_i );

        /// Loop mode changed.
        void on_m_cbLoopMode_p_stateChanged ( int f_val_i );

        /// //Random jump changed.
        //void on_m_sbRandomEvery_p_valueChanged (int f_value_i );
        
        /// Exit on last frame changed.
        void on_m_cbExit_p_stateChanged ( int f_state_i );


        /// Slider action.
        void on_m_qsPosition_p_actionTriggered ( int f_action_i );

        /// Slider released.        
        void on_m_qsPosition_p_sliderReleased ( );
        
    /// Signals.
    signals:
        void stop();
        void previous();
        void playBackward();
        void pause();
        void next();
        void play();
        void begin();
        void end();
        void reload();
        void goTo( int f_frameNr_i );

        void loopModeChanged ( bool f_val_f);
        void skipFramesChanged ( int f_skip_i );
        //void randomJump();
        void exitOnLastFrameChanged ( bool f_val_f );        

    /// Protected member
    protected:
        
        /// Inherited from QWidget
        void closeEvent ( QCloseEvent * f_event_p );
        
    /// Private methods.
    private:
        void updateLabel ();
        void updateMode ();
        void updateModeFromSlider();

    /// Private members.
    private:

        /// Number of images in sequence.
        int                   m_numFrames_i;        

        /// Number of images in sequence.
        int                   m_currentFrame_i;

        /// Number of images in sequence.
        int                   m_prevFrame_i;

        /// Number of images in sequence.
        bool                  m_sliderPressed_b;

        /// Position mode.
        EFramePositionMode_t  m_positionMode_e;
        
        /// Action mode.
        EActionMode_t         m_actionMode_e;

        /// Device mode.
        EDeviceType_t         m_deviceType_e;

        /// Count for random jump.
        //int                   m_jumpCount_i;
    };
}

#endif // __SEQCONTROL_H

