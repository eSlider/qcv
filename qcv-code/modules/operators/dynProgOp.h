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

#ifndef __DYNPROGRAMMINGOP_H
#define __DYNPROGRAMMINGOP_H

/**
*******************************************************************************
*
* @file   dynProOp.h
*
* \class  CDynamicProgrammingOp
* \author Hernan Badino (hbadino@nrec.ri.cmu.edu)
*
* \brief  Class for computing dynamic programming on a images. The
* dynamic programming finds the optimal path from the left to right.
*
*******************************************************************************
*****             (C) Hernan Badino 2010 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>


/* CONSTANTS */
#define DP4F_MAX_MEDFILT_KERNEL_SIZE 51

/* PROTOTYPES */

namespace QCV
{    
    class CDynamicProgrammingOp
    {
    public:
        /******************************/
        /*  DATA TYPES and CONSTANTS  */
        /******************************/

        struct Node
        {
            /// Value of the node.
            float     m_nodCost_f;

            /// Accumulated cost of the path.
            float     m_accCost_f;

            /// Best parent node.
            short int m_parNode_i;
        };

    public:
    
        /******************************/
        /*  Constructors/Destructor   */
        /******************************/

        CDynamicProgrammingOp ( );

        CDynamicProgrammingOp ( const int f_width_i,
                                const int f_height_i );

        virtual ~CDynamicProgrammingOp ( );


        /******************************/
        /*    COMPUTE METHODS         */
        /******************************/

        virtual bool compute ( const cv::Mat          & f_costImg,
                               std::vector<int>       & fr_vecRes_v,
                               const std::vector<int> & f_followPath    = std::vector<int>(0),
                               const std::vector<int> & f_pathTolVector = std::vector<int>(0) );

        /******************************/
        /*          DISPLAY           */
        /******************************/
    
        //virtual void showCostImageOvl ( const CDrawingList & f_drawList );
    
        /******************************/
        /*    SET GET METHODS         */
        /******************************/

        virtual CParameterSet *
              getParameterSet (  ) { return m_paramSet_p; }

        bool  setDistanceCost ( const float f_distCost_f ) { m_distCost_f = f_distCost_f; return true; }
        float getDistanceCost ( ) const { return m_distCost_f; }

        bool  setDistanceTh ( const float f_distTh_f ) { m_distTh_f = f_distTh_f; return true;}
        float getDistanceTh ( ) const { return m_distTh_f; }

        bool  setPredictionCost ( const float f_predCost_f ) { m_predCost_f = f_predCost_f; return true;}
        float getPredictionCost ( ) const { return m_predCost_f; }

        bool  setPredictionTh ( const float f_predTh_f ) { m_predTh_f = f_predTh_f; return true;}
        float getPredictionTh ( ) const { return m_predTh_f; }
 
        bool  setInitialCost ( const float f_initialCost_f ) { m_initialCost_f = f_initialCost_f; return true;}
        float getInitialCost ( ) const { return m_initialCost_f; }

        bool  setMinCostValue ( const float f_minCost_f ) { m_minCostValue_f = f_minCost_f; return true;}
        float getMinCostValue ( ) const { return m_minCostValue_f; }

        bool  setMaxCostValue ( const float f_maxCost_f ) { m_maxCostValue_f = f_maxCost_f;return true; }
        float getMaxCostValue ( ) const { return m_maxCostValue_f; }

        bool  setApplyMedianFilter ( const bool f_apply_b ) { m_applyMedianFilter_b = f_apply_b; return true;}
        bool  getApplyMedianFilter ( ) const { return m_applyMedianFilter_b; }

        bool  setMedFiltKernelSize ( const int f_size_i ) { m_medFiltHKSize_i = f_size_i<=3?1:f_size_i/2; return true;}
        int   getMedFiltKernelSize ( ) const { return m_medFiltHKSize_i * 2 + 1; }

        bool  setFollowPathTolerance ( const int f_tol_i ) { m_pathTol_i = f_tol_i<1?1:f_tol_i; return true;}
        int   getFollowPathTolerance ( ) const { return m_pathTol_i; }
    
        bool  setCostImageSize( const int f_width_i, 
                                const int f_height_i );
 
        int   getWidth()  const { return m_width_i; }
        int   getHeight() const { return m_height_i; }
    
        bool  setExpectedGradient ( const float * const f_vec_p );
        const float * getExpectedGradient ( ) const { return m_expectedGradient_p; }
        
        bool  setDistCostVector ( const std::vector <float> * const f_vec_p );
        const std::vector <float> * getDistCostVector ( ) const { return m_distCostVector_p; }


        cv::Mat  getGraphImage() const { return m_nodesImg; }

        /******************************/
        /*    PROTECTED METHODS       */
        /******************************/
    protected:

        void  reinitialize();
        void  createParamSet();


        /******************************/
        /*    PROTECTED MEMBERS       */
        /******************************/
    protected:

        /// Matrix for the nodes.
        cv::Mat                           m_nodesImg;

        /// Width of the depth input cost image.
        int                               m_width_i;

        /// Height of the depth input cost image.
        int                               m_height_i;

        /// Distance cost.
        float                             m_distCost_f;

        /// Prediction cost.
        float                             m_predCost_f;

        /// Threshold for saturating the smoothness cost.
        float                             m_distTh_f;

        /// Threshold for saturating the prediction cost.
        float                             m_predTh_f;

        /// Initial cost.
        float                             m_initialCost_f;

        /// Minimum Cost to consider in the freespace computation.
        float                             m_minCostValue_f;

        /// Maximum Cost to consider in the freespace computation.
        float                             m_maxCostValue_f;

        /// Auxiliar internal vector.
        std::vector<int>                  m_auxVector;

        /// Expected Gradient vector.
        const float *                     m_expectedGradient_p;

        /// Distance Cost Vector.
        const std::vector<float> *        m_distCostVector_p;

        /// Apply Median Filter to result?
        bool                              m_applyMedianFilter_b;

        /// Size of the median filter kernel
        int                               m_medFiltHKSize_i;

        /// Tolerance in pixels of the region to consider 
        /// around the path to follow. For pyramidal implementation.
        int                               m_pathTol_i;

        /// Parameter set.
        CParameterSet *                   m_paramSet_p;

    };
}

#endif // __DYNPROGRAMMINGOP_H

