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

#include <QApplication>
#include <QTimer>

#include "stereoOp.h"
#include "mainWindow.h"
#include "seqDevHDImg.h"
#include "paramIOXmlFile.h"

using namespace QCV;

int main(int f_argc_i, char *f_argv_p[])
{
    std::string seqFile_str = "sequence.xml";

    /// Create app
    QApplication app (f_argc_i, f_argv_p);

    if (f_argc_i > 1)
       seqFile_str = f_argv_p[1];
    
    /// Create root operator
    CStereoOp *rootOp_p = new CStereoOp( );

    /// Load parameters from parameters.xml file 
    CParamIOXmlFile pio ( "params_stereo.xml" );
    rootOp_p->getParameterSet() -> load ( pio );

    /// Create hard disk device
    CSeqDevHDImg device;
    device.loadNewSequence ( seqFile_str );

    /// Create the main window passing the connector. 2x2 default screen count.
    CMainWindow *mwind = new CMainWindow ( &device, 
                                           rootOp_p,
                                           2, 2 );
    
    /// Show main window
    mwind->show();

    /// Execute Qt app.
    int retval_i = app.exec();

    /// Save parameters
    rootOp_p->getParameterSet() -> save ( pio );
    pio.save ("params_stereo.xml");

    delete mwind;
    
    return retval_i;
}

