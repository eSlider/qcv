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
#include "deviceOpBinder.h"
#include "seqDevHDImg.h"

using namespace QCV;

int main(int f_argc_i, char *f_argv_p[])
{
    QApplication app (f_argc_i, f_argv_p);

    /// Create operator
    CStereoOp *rootOp_p = new CStereoOp();

    CSeqDevHDImg device;
    device.loadNewSequence ( "sequence.xml" );
    
    CDeviceOpBinder<TInpImgFromFileVector, cv::Mat> binder ( rootOp_p, &device );

    CMainWindow *mwind = new CMainWindow ( &binder );
    
    mwind->show();

    int retval_i = app.exec();

    return retval_i;
}

