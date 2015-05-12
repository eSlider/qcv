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
#include <unistd.h>
#include <QApplication>
#include <QTimer>

#include "surfOp.h"
#include "mainWindow.h"
#include "seqDevVideoCapture.h"
#include "seqDevHDImg.h"
#include "paramIOXmlFile.h"

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
    CSurfOp *rootOp_p = new CSurfOp( );

    /// Load parameters from parameters.xml file 
    CParamIOXmlFile pio ( "params_surf.xml" );
    rootOp_p->getParameterSet() -> load ( pio );

    CSeqDeviceControl * device_p;

    /// Create hard disk device
    if (deviceFile_str.substr(deviceFile_str.length() - 4) == ".xml")
        device_p = new CSeqDevHDImg (deviceFile_str);
    else
        /// Create video capture device
        device_p = new CSeqDevVideoCapture (deviceFile_str);
    
    /// Create the main window passing the connector. 2x2 default screen count.
    CMainWindow *mwind_p = new CMainWindow ( device_p, 
                                             rootOp_p,
                                             1, 1 );
    
    /// Show main window
    mwind_p->show();

    /// Execute Qt app.
    int retval_i = app.exec();

    // while (1)
    // {
    //     usleep(1000);
    //     app.processEvents();
    // };

    /// Save parameters
    rootOp_p->getParameterSet() -> save ( pio );
    pio.save ("params_surf.xml");

    delete mwind_p;
    delete device_p;

    return retval_i;
}

