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
* \file   seqControlDlg.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "seqControlDlg.h"
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QFileInfo>
#include <stdio.h>

using namespace QCV;

extern std::string g_QCVAppId_str;

CSeqControlDlg::CSeqControlDlg(QWidget *f_parent_p /* = 0 */ )
        : QWidget (                       f_parent_p ),
          m_numFrames_i (                         -1 ),
          m_currentFrame_i (                       1 ),
          m_prevFrame_i (                          0 ),
          m_sliderPressed_b (                   true ),
          m_positionMode_e (         IPM_FIRST_FRAME ),
          m_actionMode_e (               AM_DISABLED ),
          m_deviceType_e (          DT_BIDIRECTIONAL )
{
    setupUi(this); // this sets up GUI

    setWindowTitle( tr("Sequence Control") );
    setObjectName ( windowTitle() );

    m_qlStatus_p -> setText("");

    m_numFrames_i = 100;
    
    m_qsPosition_p -> setValue ( 1 );
    m_qsPosition_p -> setMinimum ( 1 );
    m_qsPosition_p -> setMaximum ( 100 );

    /// So that valueChange events are not emited while the user drags the
    /// slider to different positions. Only when the user releases the slider
    /// the event valueChanged is emitted.
    m_qsPosition_p -> setTracking ( true );
    
    /// Allowing signals to be emited in on_m_qsPosition_p_valueChanged ().
    m_sliderPressed_b = false;

    QString appName_str = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();

    QSettings qSettings;
    bool exitOnLastFrame_b = qSettings.value ( appName_str + 
                                               QString("/ExitOnLastFrame"), "0").toBool();
    
    m_cbExit_p -> setCheckState (  exitOnLastFrame_b?Qt::Checked:Qt::Unchecked);

    //emit exitOnLastFrameChanged ( f_state_i == Qt::Checked );

    updateMode ();
}

void
CSeqControlDlg::closeEvent ( QCloseEvent * /*f_event_p*/ )
{
}

CSeqControlDlg::~CSeqControlDlg( )
{    
    QString appName_str = QFileInfo( QCoreApplication::applicationFilePath() ).fileName();

    QSettings qSettings;
    qSettings.setValue ( appName_str + 
                         QString("/ExitOnLastFrame"), (m_cbExit_p->checkState()==Qt::Checked)?1:0);

}

void CSeqControlDlg::setCurrentFrame( const int f_currentFrame_i )
{
    m_currentFrame_i = f_currentFrame_i;
    
    if ( m_numFrames_i > 0 )
        m_qsPosition_p -> setValue ( f_currentFrame_i );

    updateModeFromSlider();
    updateLabel();

    if (f_currentFrame_i == m_sbPauseAt_p -> value())
        m_qpbPause_p->click();
    
    m_prevFrame_i = m_currentFrame_i;
}

void CSeqControlDlg::setNumberOfFrames ( const int f_numFrames_i )
{
    if ( f_numFrames_i > 0 )
        m_qsPosition_p -> setMaximum ( f_numFrames_i );
    else
        m_qsPosition_p -> setMaximum ( 1 );

    m_numFrames_i = f_numFrames_i;

    updateLabel();
}

void CSeqControlDlg::on_m_qsPosition_p_valueChanged ( int /*f_value_i */)
{
    /*
    m_currentFrame_i = f_value_i;    

    if (!m_sliderPressed_b)
    {
        emit goTo(f_value_i);
        updateModeFromSlider();
    }

    updateLabel();
    */
}

void CSeqControlDlg::on_m_sbSkip_p_valueChanged (int f_value_i )
{
    emit skipFramesChanged ( f_value_i );
}

void CSeqControlDlg::on_m_cbLoopMode_p_stateChanged ( int f_state_i )
{
    emit loopModeChanged ( f_state_i == Qt::Checked );
}
void CSeqControlDlg::on_m_cbExit_p_stateChanged ( int f_state_i )
{
    emit exitOnLastFrameChanged ( f_state_i == Qt::Checked );
}


/// In this slot we only handle when the user press on the bar changing the 
/// slide to a new position (page change).
void CSeqControlDlg::on_m_qsPosition_p_actionTriggered ( int f_action_i )
{
    if (f_action_i == QAbstractSlider::SliderMove)
    {
        QString qsText;    
        
        if ( m_numFrames_i > 0 )
        {
            qsText =  ( QString::number(m_qsPosition_p -> sliderPosition()) + "/" + 
                        QString::number(m_numFrames_i) );
        }
        else
        {
            qsText = QString::number(m_currentFrame_i);
        }
        
        m_qlStatus_p -> setText( qsText );

        return;
    }

    m_currentFrame_i = m_qsPosition_p -> sliderPosition();
    
    if (!m_sliderPressed_b)
    {
        emit goTo(m_currentFrame_i);
        updateModeFromSlider();
    }

    updateLabel();
}


void CSeqControlDlg::updateModeFromSlider()
{
    if ( m_currentFrame_i < m_numFrames_i && 
         m_currentFrame_i > 1 )
        setPositionMode ( IPM_MIDDLE_FRAME );
    else if ( m_currentFrame_i == 1 )
         setPositionMode ( IPM_FIRST_FRAME );
    else 
        setPositionMode ( IPM_LAST_FRAME );
}

void CSeqControlDlg::updateLabel()
{
    QString qsText;    

    if ( m_numFrames_i > 0 )
    {
        qsText =  ( QString::number(m_currentFrame_i) + "/" + 
                    QString::number(m_numFrames_i) );
    }
    else
    {
        qsText = QString::number(m_currentFrame_i);
    }

    m_qlStatus_p -> setText( qsText );
}

void CSeqControlDlg::on_m_qsPosition_p_sliderReleased ( ) 
{
    m_sliderPressed_b = false;
    m_currentFrame_i = m_qsPosition_p -> sliderPosition();

    emit( goTo( m_currentFrame_i ) ) ;
    updateModeFromSlider();
}

void CSeqControlDlg::updateMode ()
{
    // If disabled, then disable all.
    if ( m_actionMode_e == AM_DISABLED )
    {
        m_qpbStop_p     -> setEnabled ( false );
        m_qpbPause_p    -> setEnabled ( false );
        m_qpbPlay_p     -> setEnabled ( false );
        m_qpbPlayBack_p -> setEnabled ( false );
        m_qpbNext_p     -> setEnabled ( false );
        m_qpbPrev_p     -> setEnabled ( false );
        m_qpbBegin_p    -> setEnabled ( false );
        m_qpbEnd_p      -> setEnabled ( false );
        m_qpbReload_p   -> setEnabled ( false );

        m_qsPosition_p  -> setValue ( 1 );
        m_qsPosition_p  -> setEnabled ( false );
    }
    else if ( m_positionMode_e == IPM_FIRST_FRAME )
    {
        if (m_actionMode_e == AM_PLAY_BACKWARD && !m_cbLoopMode_p -> isChecked())
            m_actionMode_e = AM_PAUSE;

        m_qpbStop_p     -> setEnabled ( true );
        m_qpbPause_p    -> setEnabled ( m_actionMode_e == AM_PLAY ||
                                        m_actionMode_e == AM_PLAY_BACKWARD );

        m_qpbBegin_p    -> setEnabled ( false );
        m_qpbPrev_p     -> setEnabled ( false );
        m_qpbPlayBack_p -> setEnabled ( false ); 

        m_qpbPlay_p     -> setEnabled ( m_numFrames_i != 1 && 
                                        m_actionMode_e != AM_PLAY );
        
        m_qpbNext_p     -> setEnabled ( m_numFrames_i != 1 && 
                                        !( m_actionMode_e == AM_PLAY ||
                                           m_actionMode_e == AM_PLAY_BACKWARD ) );
        m_qpbEnd_p      -> setEnabled ( m_numFrames_i != 1 );

        m_qpbReload_p   -> setEnabled (  !( m_actionMode_e == AM_PLAY ||
                                            m_actionMode_e == AM_PLAY_BACKWARD ) );

        if ( m_deviceType_e == DT_BIDIRECTIONAL && m_numFrames_i != 1 )
        {
            m_qsPosition_p -> setValue ( 1 );
            m_currentFrame_i = 1;
            updateLabel();
        }
        m_qsPosition_p  -> setEnabled ( true );
    }
    else if ( m_positionMode_e == IPM_LAST_FRAME )
    {
        if (m_actionMode_e == AM_PLAY && !m_cbLoopMode_p -> isChecked())
            m_actionMode_e = AM_PAUSE;

        m_qpbStop_p     -> setEnabled ( true );
        m_qpbPause_p    -> setEnabled ( m_actionMode_e == AM_PLAY ||
                                        m_actionMode_e == AM_PLAY_BACKWARD );

        m_qpbPlay_p     -> setEnabled ( m_actionMode_e == AM_PLAY );
        m_qpbNext_p     -> setEnabled ( false );
        m_qpbEnd_p      -> setEnabled ( false );

        const bool enableBack_b = ( m_deviceType_e == DT_BIDIRECTIONAL && 
                                    m_numFrames_i != 1 );

        m_qpbBegin_p    -> setEnabled ( enableBack_b );
        m_qpbPrev_p     -> setEnabled ( enableBack_b && 
                                        !( m_actionMode_e == AM_PLAY ||
                                           m_actionMode_e == AM_PLAY_BACKWARD ) );
        m_qpbPlayBack_p -> setEnabled ( enableBack_b && 
                                        m_actionMode_e != AM_PLAY_BACKWARD ); 

        m_qpbReload_p   -> setEnabled (  !( m_actionMode_e == AM_PLAY ||
                                            m_actionMode_e == AM_PLAY_BACKWARD ) );

        if ( enableBack_b )
        {
            m_qsPosition_p -> setValue ( m_numFrames_i );
            m_currentFrame_i = m_numFrames_i;
            updateLabel();
        }
        m_qsPosition_p  -> setEnabled ( true );
    }
    else if ( m_positionMode_e == IPM_MIDDLE_FRAME )
    {
        m_qpbStop_p     -> setEnabled ( true );
        m_qpbPause_p    -> setEnabled ( m_actionMode_e == AM_PLAY ||
                                        m_actionMode_e == AM_PLAY_BACKWARD );
        
        m_qpbPlay_p     -> setEnabled ( m_numFrames_i  != 1 &&
                                        m_actionMode_e != AM_PLAY );

        m_qpbNext_p     -> setEnabled ( m_numFrames_i != 1 && 
                                        !( m_actionMode_e == AM_PLAY ||
                                           m_actionMode_e == AM_PLAY_BACKWARD ));

        m_qpbEnd_p      -> setEnabled ( m_numFrames_i != 1 );

        const bool enableBack_b = ( m_deviceType_e == DT_BIDIRECTIONAL && 
                                    m_numFrames_i != 1 );

        m_qpbPrev_p     -> setEnabled ( enableBack_b  && 
                                        !( m_actionMode_e == AM_PLAY ||
                                           m_actionMode_e == AM_PLAY_BACKWARD ));
        m_qpbPlayBack_p -> setEnabled ( enableBack_b && 
                                        m_actionMode_e != AM_PLAY_BACKWARD );
        m_qpbBegin_p    -> setEnabled ( enableBack_b );

        m_qpbReload_p   -> setEnabled (  !( m_actionMode_e == AM_PLAY ||
                                            m_actionMode_e == AM_PLAY_BACKWARD ) );
 
        m_qsPosition_p  -> setEnabled ( true );
    }
}

void CSeqControlDlg::setPositionMode ( const EFramePositionMode_t f_newMode_e )
{
    //if (f_newMode_e == m_positionMode_e)
    //    return;

    m_positionMode_e = f_newMode_e;
    
    updateMode();
}

void CSeqControlDlg::setActionMode ( const EActionMode_t f_newMode_e )
{
    if ( f_newMode_e == m_actionMode_e )
        return;

    m_actionMode_e  = f_newMode_e;
    
    updateMode();
}


void CSeqControlDlg::setDeviceType ( const EDeviceType_t f_newType_e )
{
    if ( f_newType_e == m_deviceType_e )
        return;

    m_deviceType_e = f_newType_e;

    updateMode();

}
