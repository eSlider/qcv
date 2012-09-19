/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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

#include <QApplication>
#include <QTimer>

#include "stereo.h"
#include "mainWindow.h"
#include "deviceOpConnector.h"
#include "seqDevHDImg.h"
#include "paramIOFile.h"

using namespace QCV;

int main(int f_argc_i, char *f_argv_p[])
{
    /// Create app
    QApplication app (f_argc_i, f_argv_p);

    /// Create root operator
    CStereoOp *rootOp_p = new CStereoOp( );

    /// Load parameters from parameters.xml file 
    CParamIOFile pio ( "parameters.xml" );
    rootOp_p->getParameterSet() -> load ( pio );

    /// Create hard disk device
    CSeqDevHDImg device;
    device.loadNewSequence ( "sequence.xml" );

    /// Let's connect the device to the root operator
    CDeviceOpConnector<TInpImgFromFileVector, cv::Mat> connector ( rootOp_p, &device );

    /// Create the main window passing the connector. 2x2 default screen count.
    CMainWindow *mwind = new CMainWindow ( &connector, 2, 2 );
    
    /// Show main window
    mwind->show();

    /// Execute Qt app.
    int retval_i = app.exec();

    /// Save parameters
    rootOp_p->getParameterSet() -> save ( pio );
    pio.save ("parameters.xml");

    delete mwind;
    
    return retval_i;
}

