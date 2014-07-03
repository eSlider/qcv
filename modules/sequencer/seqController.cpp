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
 * \file   seqController.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QtCore/QObject>

#include "seqController.h"
#include "seqControlDlg.h"
#include "seqDeviceControl.h"

using namespace QCV;

/// Constructor
CSeqController::CSeqController( CSeqDeviceControl * const f_device_p )
        : m_controlDlg_p (                   NULL ),
          m_deviceCtrl_p (             f_device_p )
{
    m_controlDlg_p = new CSeqControlDlg ( NULL );
    
    /// Make it invisible. The window manager will show it if necessary.
    m_controlDlg_p -> setVisible ( false );
    m_controlDlg_p -> setObjectName ( tr("Control Dialog") );

    /// Connections between the control dialog and "this".
    connect(m_controlDlg_p, SIGNAL(stop()), this, SLOT(stopClicked()));
    connect(m_controlDlg_p, SIGNAL(play()), this, SLOT(playClicked()));
    connect(m_controlDlg_p, SIGNAL(playBackward()), this, SLOT(playBackwardClicked()));
    connect(m_controlDlg_p, SIGNAL(pause()), this, SLOT(pauseClicked()));
    connect(m_controlDlg_p, SIGNAL(previous()), this, SLOT(previousClicked()));
    connect(m_controlDlg_p, SIGNAL(next()), this, SLOT(nextClicked()));
    connect(m_controlDlg_p, SIGNAL(begin()), this, SLOT(beginClicked()));
    connect(m_controlDlg_p, SIGNAL(end()), this, SLOT(endClicked()));
    connect(m_controlDlg_p, SIGNAL(goTo(int)), this, SLOT(goTo(int)));
    connect(m_controlDlg_p, SIGNAL(reload()), this, SLOT(reloadClicked()));
    connect(m_controlDlg_p, SIGNAL(loopModeChanged( bool )), this, SLOT(loopModeChanged( bool )));
    connect(m_controlDlg_p, SIGNAL(skipFramesChanged(int )), this, SLOT(skipFramesChanged(int )));
    connect(m_controlDlg_p, SIGNAL(exitOnLastFrameChanged( bool )), this, SLOT(exitOnLastFrameChanged( bool )));

    /// Update exitOnLastFrame state.
    exitOnLastFrameChanged ( m_controlDlg_p -> exitOnLastFrame() );

    if ( m_deviceCtrl_p )
    {
        if ( m_deviceCtrl_p -> getState() == CSeqDeviceControl::S_PLAYING )
            m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PLAY );
        else if (m_deviceCtrl_p -> getState() == CSeqDeviceControl::S_PAUSED )
            m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PAUSE );
        else
            m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PLAY_BACKWARD );

    
        /// Connections between device and this.
        connect(m_deviceCtrl_p, SIGNAL(cycle()), this, SLOT(playCycle()));
        connect(m_deviceCtrl_p, SIGNAL(reset()), this, SLOT(deviceReset()));
    }
}

/// Destructor
CSeqController::~CSeqController()
{
    if (!m_controlDlg_p -> parent())
        delete m_controlDlg_p;
}

void
CSeqController::start()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> initialize();

    refreshControlDlg();

    if (result_b)
    {
        emit initialize();
    }
}

void
CSeqController::stopClicked()
{
    //bool result_b;
    
    m_deviceCtrl_p -> stop();

    m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PAUSE );

    //result_b = 
    m_deviceCtrl_p -> goToFirstFrame();

    refreshControlDlg();

    //if (result_b)
    {
        emit stop();
    }
}

void
CSeqController::playClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> startPlaying();

    if ( result_b ) 
        m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PLAY );
}

void
CSeqController::playBackwardClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> startPlayingBackward();

    if ( result_b ) 
        m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PLAY_BACKWARD );
}

void
CSeqController::playCycle()
{
    refreshControlDlg();

    emit cycle();
}

void
CSeqController::pauseClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> pause();

    if (result_b)
        m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PAUSE );
}

void
CSeqController::previousClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> prevFrame();

    refreshControlDlg();

    if (result_b)
    {
        emit cycle();
    }
}

void
CSeqController::nextClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> nextFrame();

    refreshControlDlg();

    if (result_b)
    {
        emit cycle();
    }
}

void
CSeqController::reloadClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> reloadFrame();

    refreshControlDlg();

    if (result_b)
    {
        emit cycle();
    }
}


void
CSeqController::beginClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> goToFirstFrame();

    refreshControlDlg();

    if (result_b)
    {
        emit cycle();
    }
}

void
CSeqController::endClicked()
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> goToLastFrame();

    refreshControlDlg();

    if (result_b)
    {
        emit cycle();
    }
}

void
CSeqController::goTo ( const int f_framNumber_i )
{
    bool result_b;
    
    result_b = m_deviceCtrl_p -> goToFrame (f_framNumber_i);

    refreshControlDlg();

    if (result_b)
    {
        emit cycle();
    }
}

void
CSeqController::deviceReset()
{
    m_controlDlg_p -> setActionMode ( CSeqControlDlg::AM_PAUSE );
    refreshControlDlg();

    emit reset();
}

void
CSeqController::refreshControlDlg()
{
    const int frames_i    = m_deviceCtrl_p -> getNumberOfFrames();
    const int currFrame_i = m_deviceCtrl_p -> getCurrentFrame();

    if ( m_deviceCtrl_p -> isBidirectional() )
        m_controlDlg_p -> setDeviceType ( CSeqControlDlg::DT_BIDIRECTIONAL );
    else
        m_controlDlg_p -> setDeviceType ( CSeqControlDlg::DT_FORWARD_ONLY );

    m_controlDlg_p -> setNumberOfFrames ( frames_i    );
    m_controlDlg_p -> setCurrentFrame   ( currFrame_i );

    //m_controlDlg_p -> setPositionMode
}

QWidget * 
CSeqController::getDialog() const
{ 
    return m_controlDlg_p;
}

void CSeqController::loopModeChanged( bool f_val_b )
{
    m_deviceCtrl_p -> setLoopMode ( f_val_b );
}

void CSeqController::exitOnLastFrameChanged( bool f_val_b )
{
    m_deviceCtrl_p -> setExitOnLastFrame ( f_val_b );
}

void CSeqController::skipFramesChanged( int f_skip_i )
{
    m_deviceCtrl_p -> setFrameSkip ( f_skip_i );
}
