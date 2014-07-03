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

#ifndef __SEQCONTROLLER_H
#define __SEQCONTROLLER_H

/**
 *******************************************************************************
 *
 * @file seqController.h
 *
 * \class CSeqController
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Interface class between the sequence control dialog and a device.
 *
 * Class acting as an interface between the sequence control dialog and
 * the device being used providing sequence information.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QtGui/QWidget>

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
/* PROTOTYPES */
    class CSeqControlDlg;
    class CSeqDeviceControl;    

    class CSeqController: public QObject
    {
    /// Do I need to declare Q_OBJECT here?
        Q_OBJECT

    /// Constructors, Destructors
    public:
        /// Constructor
        CSeqController( CSeqDeviceControl *f_device_p );

        /// Destructor
        virtual ~CSeqController();

        
    /// Get Methods.
    public:

    /// Sequence Handling.
    public slots:

        void start();

        void stopClicked();
        void playClicked();
        void playBackwardClicked();
        void pauseClicked();
        void previousClicked();
        void nextClicked();
        void beginClicked();
        void endClicked();
        void reloadClicked();

        void deviceReset();
        void goTo ( int f_frameNumber_i );

        void playCycle();

        void loopModeChanged( bool f_val_b );
        void skipFramesChanged( int f_skip_i );
        //void randomJump();
        void exitOnLastFrameChanged( bool f_val_b );
        //void startController();

    /// Signals
    signals:
        void cycle();
        void stop();
        void initialize();
        void exit();
        void reset();

    /// Get/Set Methods.
    public:
        QWidget * getDialog() const;
        
    /// Private slots
    protected slots:

    /// Private methods.
    private:
        void refreshControlDlg();
        
    /// Protected members
    protected:

        /// Dialog control.
        CSeqControlDlg *        m_controlDlg_p;

        /// Dialog control.
        CSeqDeviceControl * m_deviceCtrl_p;
    };
}


#endif // __SEQCONTROLLER_H
