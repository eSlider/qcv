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


   
namespace QCV
{
   class CDisplayWidget;
   class CDrawingListHandler;
   class CNode;
   class CEventHandler;
   class CSimpleWindow;
   class CClockTreeDlg;
   class CClockHandler;
   class CGLViewer;

   
   CDisplayWidget *      g_disp_p         = 0;
   CDrawingListHandler * g_drawHandler_p  = 0;
   CNode *               g_rootNode_p     = 0;
   CEventHandler *       g_evHandler_p    = 0;
   CSimpleWindow *       g_mainWindow_p   = 0;
   CClockTreeDlg *       g_clockTree_p    = 0;
   CClockHandler *       g_clockHandler_p = 0;
#if defined HAVE_QGLVIEWER
   CGLViewer *           g_3dViewer_p     = 0;
#endif
}

#include "cinterface.h"
