/*
 * Copyright (C) 2015 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of StereoSFM
 *
 * StereoSFM is under the terms of the GNU General Public License v2.
 * See the GNU GPL version 2.0 for details.
 * StereoSFM uses dual licensing. Contact the author to get information
 * for a commercial/proprietary license.
 *
 * StereoSFM is distributed "AS IS" without ANY WARRANTY, without even 
 * the implied warranty of merchantability or fitness for a particular
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

/*

This project is an implementation of the method presented in the paper:

Hernan Badino and Takeo Kanade. A Head-Wearable Short-Baseline Stereo System for the Simultaneous Estimation of Structure and Motion. In IAPR Conference on Machine Vision Applications (MVA), Nara, Japan, June 2011.

*/

#include <QApplication>
#include <QTimer>

#include "stereoSFMOp.h"
#include "mainWindow.h"
#include "seqDevVideoCapture.h"
#include "seqDevHDImg.h"
#include "paramIOXmlFile.h"

using namespace QCV;

int main(int f_argc_i, char *f_argv_p[])
{
   std::string deviceFile_str = "";
   std::string paramFile = "params_stereoSFM.xml";
   bool nowindows_b = false;

   if (f_argc_i >= 2 )
   {
     deviceFile_str = f_argv_p[1];

     if (f_argc_i > 2 )
     {
       std::string arg(f_argv_p[2]); 
       if ( arg.substr(0,2) != "--")
	 paramFile = arg;
       
       for (int i = 2; i < f_argc_i; ++i)
	 nowindows_b |= ( std::string(f_argv_p[i]) == "--nowindows" );
     }
   }
   else
   {
     printf("\n\nUsage: %s [file [paramFile]] [--autoplay] [--nowindows], where file can be a camera device or a xml with stereo sequence\n", f_argv_p[0]);
     return 1;
   }

   /// Create app
   QApplication app (f_argc_i, f_argv_p);

   /// Create root operator
   CStereoSFMOp *rootOp_p = new CStereoSFMOp( );

   /// Load parameters from parameters.xml file 
   CParamIOXmlFile pio ( paramFile );
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
                                            3, 3 );
    
   /// Show main window
    if (! nowindows_b )
	   mwind_p->show();

   /// Execute Qt app.
   int retval_i = app.exec();

   /// Save parameters
   rootOp_p->getParameterSet() -> save ( pio );
   pio.save (paramFile);

   delete mwind_p;
   delete device_p;

   return retval_i;
}

