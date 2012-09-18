#ifndef __SEQCONTROLER_H
#define __SEQCONTROLER_H

/**
 *******************************************************************************
 *
 * @file seqControler.h
 *
 * \class CSeqControler
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Interface class between the sequence control dialog and a device.
 *
 * Class acting as an interface between the sequence control dialog and
 * the device being used providing sequence information.
 *
 *******************************************************************************/

/* INCLUDES */
#include <QWidget>

/* CONSTANTS */

/* PROTOTYPES */

namespace QCV
{
/* PROTOTYPES */
    class CSeqControlDlg;
    class CSeqDeviceControlBase;    

    class CSeqControler: public QObject
    {
    /// Do I need to declare Q_OBJECT here?
        Q_OBJECT

    /// Constructors, Destructors
    public:
        /// Constructor
        CSeqControler( CSeqDeviceControlBase *f_device_p );

        /// Destructor
        virtual ~CSeqControler();

        
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
        //void startControler();

    /// Signals
    signals:
        void cycle();
        void stop();
        void initialize();
        void exit();
        void reset();

    /// Get/Set Methods.
    public:
        QDialog * getDialog() const;
        
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
        CSeqDeviceControlBase * m_deviceCtrl_p;
    };
}


#endif // __SEQCONTROLER_H
