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

#ifndef __3DMESHLIST_H
#define __3DMESHLIST_H

/**
 *******************************************************************************
 *
 * @file 3DMeshList.h
 *
 * \class C3DMeshList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of 3DMeshs for displaying in QGLViewer
 *
 * The class is derived from C3DDrawingElementList implementing a list of 3DMeshs.
 * 3DMeshs are composed of: 
 *  - 3D mesh
 *  - 3D normal
 *  - a color, and
 *  - a mesh size.
 *
 *******************************************************************************/

/* INCLUDES */
#include "3DDrawingElementList.h"
#include "colors.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class C3DMeshList: public C3DDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        C3DMeshList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~C3DMeshList();

    /// Operations.
    public:

        // Add drawing 3DMeshs from other list.
        virtual bool add (  const C3DMeshList & f_otherList );

        // Draw all 3DMeshs.
        virtual bool add (  cv::Mat     f_vectorImg,
                            cv::Mat     f_dispTexture,
                            const float f_maxDist_f,
                            const float f_maxInvDist_f );

        // Clear all 3DMeshs.
        virtual bool clear ();

        // Draw all 3DMeshs.
        virtual bool show ();

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "3DMeshes"; };

    protected:

        struct SMeshData
        {
            /// Image of 3D points
            cv::Mat       vectorImg;

            /// Texture image
            cv::Mat       dispTexture;

            /// Max distance to form triangles.
            float         maxDist_f;

            /// Max inverse distance to form triangles
            float         maxInvDist_f;

            /// Flag indicating that the mesh already was created.
            bool          created_b;

            /// Id of mesh
            unsigned int  displayList_ui;
        };

    /// Private Members
    private:
        std::vector<SMeshData>    m_mesh_v;
    };
} // Namespace QCV


#endif // __3DMESHLIST_H
