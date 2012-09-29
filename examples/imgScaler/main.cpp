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

#include "imgScalerOp.h"
#include "mainWindow.h"
#include "deviceOpConnector.h"
#include "seqDevVideoCapture.h"
#include "seqDevHDImg.h"
#include "paramIOFile.h"

using namespace QCV;

int main(int f_argc_i, char *f_argv_p[])
{
    std::string deviceFile_str = "";

    if (f_argc_i != 1 )
    {
        deviceFile_str = f_argv_p[1];
    }
    else
    {
        printf("\n\nUsage: %s [file], where file can be a video, a camera device, or a xml with sequence\n", f_argv_p[0]);
        return 1;
    }    

    /// Create app
    QApplication app (f_argc_i, f_argv_p);

    /// Create root operator
    CImageScalerOp *rootOp_p = new CImageScalerOp( );

    /// Load parameters from parameters.xml file 
    CParamIOFile pio ( "params_imgScaler.xml" );
    rootOp_p->getParameterSet() -> load ( pio );

    CSeqDeviceControl<CInpImgFromFileVector> * device_p;

    /// Create hard disk device
    if (deviceFile_str.substr(deviceFile_str.length() - 4) == ".xml")
        device_p = new CSeqDevHDImg (deviceFile_str);
    else
        device_p = new CSeqDevVideoCapture (deviceFile_str);

    /// Let's connect the device to the root operator
    CDeviceOpConnector<CMatVector, CMatVector, CInpImgFromFileVector> connector ( rootOp_p, device_p );

    /// Create the main window passing the connector. 2x2 default screen count.
    CMainWindow *mwind_p = new CMainWindow ( &connector, 2, 1 );
    
    /// Show main window
    mwind_p->show();

    /// Execute Qt app.
    int retval_i = app.exec();

    /// Save parameters
    rootOp_p->getParameterSet() -> save ( pio );
    pio.save ("params_imgScaler.xml");

    delete mwind_p;
    delete device_p;

    return retval_i;
}

