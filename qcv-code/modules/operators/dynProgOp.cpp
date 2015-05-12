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

/*@@@**************************************************************************
 * \file   dynProgOp.cpp
 * \author Hernan Badino
 * \date   Wed March 7 16:51 CET 2009
 * \notes 
 *******************************************************************************
 *****            (C) Hernan Badino 2010 - All Rights Reserved             *****
 ******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "paramMacros.h" 
#include "dynProgOp.h"
#include "medf_inline.h"

using namespace QCV;

/* *************************** METHOD ************************************** */
/**
 * Standardconstructor.
 *
 *
 * \brief          Standardconstructor.
 * \author         Hernan Badino
 * \date           07.03.2009
 *
 * \note           Set the initial variables and allocate the object.
 * \remarks        -
 * \sa             -
 *
 * \param           (IN)   - .
 * \param          - (OUT)
 * \param          - (INOUT)
 * \return         -
 * \exception      none none
 *
 *************************************************************************** */
CDynamicProgrammingOp::CDynamicProgrammingOp( const int f_width_i,
                                              const int f_height_i )
        : m_nodesImg (                                              ),
          m_width_i (                                     f_width_i ),
          m_height_i (                                   f_height_i ),
          m_distCost_f (                                        5.f ),
          m_predCost_f (                                       0.5f ),
          m_distTh_f (                                         10.f ),
          m_predTh_f (                                         10.f ),
          m_initialCost_f (                                     0.f ),
          m_minCostValue_f (                                   0.0f ),
          m_maxCostValue_f (                                  1.e9f ),
          m_expectedGradient_p (                               NULL ), 
          m_distCostVector_p (                                 NULL ),
          m_applyMedianFilter_b  (                             true ),
          m_medFiltHKSize_i (                                     9 ),
          m_pathTol_i (                                           5 ),
          m_paramSet_p (                                       NULL )
{
    createParamSet();
    reinitialize();
}

CDynamicProgrammingOp::CDynamicProgrammingOp( )
        : m_nodesImg (                                              ),
          m_width_i (                                             8 ),
          m_height_i (                                            8 ),
          m_distCost_f (                                        5.f ),
          m_predCost_f (                                       0.5f ),
          m_distTh_f (                                         10.f ),
          m_predTh_f (                                         10.f ),
          m_initialCost_f (                                     0.f ),
          m_minCostValue_f (                                   0.0f ),
          m_maxCostValue_f (                                  1.e9f ),
          m_expectedGradient_p (                               NULL ), 
          m_distCostVector_p (                                 NULL ),
          m_applyMedianFilter_b  (                             true ),
          m_medFiltHKSize_i (                                     9 ),
          m_pathTol_i (                                           5 ),
          m_paramSet_p (                                       NULL )
{
    createParamSet();
    reinitialize();
}

void
CDynamicProgrammingOp::createParamSet()
{
    m_paramSet_p = new CParameterSet ( NULL );
    m_paramSet_p->setName ( "Dynamic Programming" );
    
    ADD_FLOAT_PARAMETER( "Distance Cost", 
                         "Proportional costs for jumping more than 1px.",
                         getDistanceCost(),
                         this,
                         DistanceCost,
                         CDynamicProgrammingOp );

    ADD_FLOAT_PARAMETER( "Distance Th", 
                         "Distance Threshold [px].",
                         getDistanceTh(),
                         this,
                         DistanceTh,
                         CDynamicProgrammingOp );


    ADD_FLOAT_PARAMETER( "Prediction Cost", 
                         "Prediction cost [cost/px].",
                         getPredictionCost(),
                         this,
                         PredictionCost,
                         CDynamicProgrammingOp );

    ADD_FLOAT_PARAMETER( "Prediction Th", 
                         "Prediction cost threshold [px].",
                         getPredictionTh(),
                         this,
                         PredictionTh,
                         CDynamicProgrammingOp );

    ADD_FLOAT_PARAMETER( "Initial Cost", 
                         "Offset cost for the first column.",
                         getInitialCost(),
                         this,
                         InitialCost,
                         CDynamicProgrammingOp );
    

    ADD_FLOAT_PARAMETER( "Min Cost Value", 
                         "Valid minimum possible value.",
                         getMinCostValue(),
                         this,
                         MinCostValue,
                         CDynamicProgrammingOp );

    ADD_FLOAT_PARAMETER( "Max Cost Value", 
                         "Valid maximum possible value.",
                         getMaxCostValue(),
                         this,
                         MaxCostValue,
                         CDynamicProgrammingOp );


    ADD_BOOL_PARAMETER( "Apply MF", 
                        "Apply median filter to the result?",
                        getApplyMedianFilter(),
                        this,
                        ApplyMedianFilter,
                        CDynamicProgrammingOp );

    ADD_INT_PARAMETER( "MF Kernel Size", 
                       "Kernel size of the MF [px]",
                       getMedFiltKernelSize(),
                       this,
                       MedFiltKernelSize,
                       CDynamicProgrammingOp );

    ADD_INT_PARAMETER( "Follow Path Tolerance", 
                       "Kernel size of the MF [px]",
                       getFollowPathTolerance(),
                       this,
                       FollowPathTolerance,
                       CDynamicProgrammingOp );
    
}

/* *************************** METHOD ************************************** */
/**
 * (Re)initialize the node structure.
 *
 *
 * \brief          (Re)initialize the node structure.
 * \author         Hernan Badino
 * \date           07.03.2009
 *
 * \remarks        -
 * \sa             -
 *
 * \param          - (IN)
 * \param          - (OUT)
 * \param          - (INOUT)
 * \return         -
 * \exception      none none
 *
 *************************************************************************** */
void CDynamicProgrammingOp::reinitialize()
{
    m_nodesImg = cv::Mat::zeros( m_height_i, m_width_i * sizeof ( Node ), CV_8UC1 );
}


/* *************************** METHOD ************************************** */
/**
 * Destructor.
 *
 *
 * \brief          Destructor.
 * \author         Hernan Badino
 * \date           07.03.2009
 *
 * \note           -
 * \remarks        -
 * \sa             -
 *
 *************************************************************************** */
CDynamicProgrammingOp::~CDynamicProgrammingOp( )
{
    if ( !m_paramSet_p -> getParent() )
        delete m_paramSet_p;
}

/******************************/
/*    SET GET METHODS         */
/*****************************/
/* *************************** METHOD ************************************** */
/**
 * Sets the cost image size.
 *
 *
 * \brief          Sets the cost image size.
 * \author         Hernan Badino
 * \date           08.03.2009
 *
 * \note           - if current size is equal to new one no change 
 *                   is performed.
 * \sa             -
 *
 * \param          - f_width_i (IN) : new width of image.
 * \param          - f_height_i (IN): new height of image.
 * \param          - (OUT)
 * \param          - (INOUT)
 * \return         -
 * \exception      none none
 *
 *************************************************************************** */
bool CDynamicProgrammingOp::setCostImageSize( const int f_width_i, 
                                              const int f_height_i )
{
    if ( f_width_i  != m_width_i || 
         f_height_i != m_height_i )
    {
        m_width_i  = f_width_i;
        m_height_i = f_height_i;
        reinitialize();
    }

    return true;
}



/* *************************** METHOD ************************************** */
/**
 * Sets the expected gradient vector.
 *
 * \brief          Sets the expected gradient vector.
 * \author         Hernan Badino
 * \date           11.05.2009
 *
 * \note           This vector allows to define a gradient of the expected 
 *                 solution.
 * \sa             -
 *
 * \param[in]      -  new vector.
 * \return         -
 * \exception      none none
 *
 *************************************************************************** */
bool CDynamicProgrammingOp::setExpectedGradient ( const float * const f_vec_p )
{ 
    m_expectedGradient_p = f_vec_p; 
    return true;
}


/* *************************** METHOD ************************************** */
/**
 * Sets the distance cost vector.
 *
 * \brief          Sets the distance cost vector.
 * \author         Hernan Badino
 * \date           08.03.2009
 *
 * \note           This vector allows to define a different distance cost for 
 *                 every row. The element store in the position i specifies the 
 *                 cost of a jump from the row i to row i+1.
 * \sa             -
 *
 * \param[in]      -  new vector.
 * \return         -
 * \exception      none none
 *
 *************************************************************************** */
bool CDynamicProgrammingOp::setDistCostVector ( const std::vector <float> * const f_vec_p )
{ 
    if ((int)f_vec_p->size() < m_width_i)
    {
        printf("Input vector length is too small: %i must be at least %i.",
               (int)f_vec_p -> size(),
               m_width_i );
        
        return false;
    }
    
    
    m_distCostVector_p = f_vec_p; 
    return true;
}

/******************************/
/*    COMPUTE METHODS         */
/******************************/

/* *************************** METHOD ************************************** */
/**
 * Computes dynammic programming with the input image and return 
 * as output the optimal path from left to right.
 *
 *
 * \brief          Computes dynammic programming with the input cost image.
 *
 * \author         Hernan Badino
 * \date           08.03.2009
 *
 * \note           -
 * \remarks        -
 * \sa             -
 *
 * \param          f_costImg_p (IN)    - Cost image.
 * \param          f_vecRes   (IN/OUT) - Prediction and output path.
 * \return         bool                - Success of computation.
 *
 *************************************************************************** */

bool 
CDynamicProgrammingOp::compute (  const cv::Mat          & f_costImg,
                                  std::vector<int>       & fr_vecRes,
                                  const std::vector<int> & f_followPath,
                                  const std::vector<int> & f_pathTolVector )
{
    if ( (int) fr_vecRes.size()      < m_height_i ||
         (int) f_costImg.size().width  < m_width_i || 
         (int) f_costImg.size().height < m_height_i )
    {
        printf("Size of input image or vector is too small.\n");
        return false;
    }

    if ( ( f_followPath.size() != 0 &&
           (int) f_followPath.size() < m_height_i ) ||
         ( f_pathTolVector.size() != 0 && 
           f_followPath.size() == 0 ) )
    {
        printf("Size of follow path vector is too small.\n");
        return false;
    }

    if ( f_pathTolVector.size() != 0 &&
         (int) f_pathTolVector.size() < m_height_i ) 
    {
        printf("Size of follow path tolerance vector is too small.\n");
        return false;
    }

    if ( m_auxVector.size() < fr_vecRes.size() )
    {
        m_auxVector.resize( fr_vecRes.size() );
    }

    int      i, j, k;
    float    newCost_f;
    float    node2nodeDist_f;
    int      startCol_i = 0, endCol_i = m_width_i - 1;
    int      startColPrevRow_i = 0, endColPrevRow_i = m_width_i - 1;
    int      startColThisRow_i, endColThisRow_i;
    int      tolerance_i = m_pathTol_i;

    /// Set the current expected gradient of the solution to 0.
    float    currGrad_f = 0.;

    /// Set the jump cost equal to the default distance cost.
    float    jumpCost_f = m_distCost_f;
    const std::vector<float> jumpCostVec = m_distCostVector_p?*m_distCostVector_p:std::vector<float>(0);

    // Lets start with the first row.

    // Let set a flag indicating that the first row has not stil been found.
    startColThisRow_i= -1;
    endColThisRow_i  = -1;

    //m_nodesImg.clear();

    for (i = 0;  i < m_height_i && startColThisRow_i == -1; ++i)
    {
        // Set first limits to startCol and endCol if required.
        if ( f_followPath.size() )
        {
            if (f_pathTolVector.size())
                tolerance_i = f_pathTolVector[i];
            
            startCol_i = f_followPath[i] - tolerance_i;
            if (startCol_i < 0 || startCol_i >= m_width_i ) startCol_i = 0;
            
            endCol_i = f_followPath[i] + tolerance_i;
            if (endCol_i < 0 || endCol_i >= m_width_i) endCol_i = m_width_i - 1;
            
        }
        
        //printf("Starting in col %i ending in col %i (init)\n", 
        //           startCol_i, endCol_i );
        for (j = startCol_i; j <= endCol_i; ++j)
        {
            // Check first if this cost can be considered as valid.
            if ( f_costImg.at<float>(i,j) > m_minCostValue_f && 
                 f_costImg.at<float>(i,j) <= m_maxCostValue_f )
            {            
                m_nodesImg.at<Node>(i,j).m_nodCost_f = f_costImg.at<float>(i,j);
                
                // Up to now the last row is this row.
                endColThisRow_i = j;
                
                if ( startColThisRow_i < 0 ) 
                {
                    // For the first column we reduce some cost in order to be robust against noise
                    // in the same row.
                    m_nodesImg.at<Node>(i,j).m_nodCost_f += m_initialCost_f;
                    
                    startColThisRow_i = j;
                }        
                
                // Add prediction cost with saturation.
                if ( fr_vecRes[i] >= 0 && m_predCost_f )
                {
                    node2nodeDist_f = fabs(fr_vecRes[i] - j);
                    
                    m_nodesImg.at<Node>(i,j).m_nodCost_f += m_predCost_f * std::min(node2nodeDist_f, m_predTh_f);
                }
                
                // Update accumulated cost = current cost.
                m_nodesImg.at<Node>(i,j).m_accCost_f = m_nodesImg.at<Node>(i,j).m_nodCost_f;
                
                // Parent node is the base node.
                m_nodesImg.at<Node>(i,j).m_parNode_i = -1;   
            }
            else
            {                
                m_nodesImg.at<Node>(i,j).m_nodCost_f = m_maxCostValue_f;
                m_nodesImg.at<Node>(i,j).m_parNode_i = -1;
            }
        }
    }

    int firstValidRow_i = i;
    //printf("First valid row is %i\n",
    //       firstValidRow_i);
    
    
    for (++i; i < m_height_i; ++i)
    {
        if (startColThisRow_i == -1)
        {
            m_nodesImg.at<Node>(i,startCol_i).m_parNode_i = -1;
            m_nodesImg.at<Node>(i,startCol_i).m_nodCost_f = 0;
            m_nodesImg.at<Node>(i,startCol_i).m_accCost_f = 0;
            
            //startColThisRow_i = endColThisRow_i = startCol_i;
            printf("Aborting DP optimization because there is no valid node on row %i\n", i-1);            
            break;
        }

        // Store the first valid node of the previous row in an auxiliar variable.
        // This will be use to set the default parent node in the current row.
        startColPrevRow_i = startColThisRow_i;
        endColPrevRow_i   = endColThisRow_i;

        if (m_expectedGradient_p)
            currGrad_f = m_expectedGradient_p[i];

        
        // Set firstlimits to startCol and endCol if required.
        if ( f_followPath.size() )
        {
            if (f_pathTolVector.size())
                tolerance_i = f_pathTolVector[i];

            startCol_i = f_followPath[i] - tolerance_i;
            if (startCol_i < 0 || startCol_i >= m_width_i ) startCol_i = 0;
            
            endCol_i = f_followPath[i] + tolerance_i;
            if (endCol_i < 0 || endCol_i >= m_width_i) endCol_i = m_width_i - 1;
        }
        
        //printf("Starting in col %i ending in col %i (prev start: %i prev end: %i)\n", 
        //       startCol_i, endCol_i, startColThisRow_i, endColThisRow_i );

        // Let set a flag indicating that the first row has not stil been found.
        startColThisRow_i = -1;
        endColThisRow_i   = -1;

        for (j = startCol_i; j <= endCol_i; ++j)
        {
            // Check first if this cost can be considered as valid.
            //if ( f_costImg.at<float>(i,j) < m_minCostValue_f ||
            //     f_costImg.at<float>(i,j) > m_maxCostValue_f )
            //    continue;

            // Check first if this cost can be considered as valid.
            if ( f_costImg.at<float>(i,j) > m_minCostValue_f &&
                 f_costImg.at<float>(i,j) <= m_maxCostValue_f )
            {
                m_nodesImg.at<Node>(i,j).m_nodCost_f = f_costImg.at<float>(i,j);

                // Up to now the last row is this row.
                endColThisRow_i = j;
                    
                if ( startColThisRow_i < 0 ) 
                {
                    // For the first column we reduce some cost in order to be robust against noise
                    // in the same row.
                    m_nodesImg.at<Node>(i,j).m_nodCost_f += m_initialCost_f;
                        
                    startColThisRow_i = j;
                }
                    
                // Add prediction cost with saturation.
                if ( fr_vecRes[i] >= 0 && m_predCost_f )
                {
                    node2nodeDist_f = fabs(fr_vecRes[i] - j);
                    m_nodesImg.at<Node>(i,j).m_nodCost_f += m_predCost_f * std::min(node2nodeDist_f, m_predTh_f);
                }                    

                // Per default the parent node is the first one, i.e.
                // the node corresponding to first valid cell in the 
                // previous row.
                m_nodesImg.at<Node>(i,j).m_parNode_i  = startColPrevRow_i;
                    
                // Initial cost (of the current parent node).

                if (jumpCostVec.size())
                    jumpCost_f = jumpCostVec[i-1];
                    
                node2nodeDist_f = fabs(j - startColPrevRow_i - currGrad_f);

                newCost_f = ( // Accumulated Cost.
                    m_nodesImg.at<Node>(i-1,startColPrevRow_i).m_accCost_f + 
                        // Local Cost.
                    m_nodesImg.at<Node>(i  ,j).m_nodCost_f +
                        // Smoothness cost with saturation.
                        jumpCost_f * std::min(node2nodeDist_f, m_distTh_f) ); 
                    
                m_nodesImg.at<Node>(i,j).m_accCost_f = newCost_f;
                    
                for (k = startColPrevRow_i+1; k <= endColPrevRow_i; ++k)
                {
                    if (jumpCostVec.size())
                        jumpCost_f = jumpCostVec[i-1];
                    //printf("%i, %i %i jumpCost_f = %f (size vec: %i)\n", i,j,k, jumpCost_f, jumpCostVec.size());
                        
                    // then compute new cost...
                    node2nodeDist_f = fabs(j - k - currGrad_f);
                    newCost_f = ( 
                            // Accumulated Cost. 
                        m_nodesImg.at<Node>(i-1,k).m_accCost_f +
                            // Local Cost.
                        m_nodesImg.at<Node>(i  ,j).m_nodCost_f +
                            // Smoothness cost with saturation.
                            jumpCost_f * std::min(node2nodeDist_f, m_distTh_f) );
                        
                    // and check if it is better than the current one.
                    if ( newCost_f < m_nodesImg.at<Node>(i,j).m_accCost_f )
                    {
                        m_nodesImg.at<Node>(i,j).m_accCost_f = newCost_f;
                        m_nodesImg.at<Node>(i,j).m_parNode_i = k;                            
                    }                
                }
            }
            else
                m_nodesImg.at<Node>(i,j).m_nodCost_f = m_maxCostValue_f;
        }
    }

    if (firstValidRow_i == m_height_i)
    {
        printf("there is no valid data in input cost image\n");
        return false;
    }

    if (startColThisRow_i == -1)
    {
        printf("Warning:\n");
        --i;
        startColThisRow_i = startColPrevRow_i;
        endColThisRow_i   = endColPrevRow_i;
//         m_nodesImg.at<Node>(i,startCol_i).m_parNode_i = startColPrevRow_i;
//         m_nodesImg.at<Node>(i,startCol_i).m_nodCost_f = m_nodesImg.at<Node>(i-1,startColPrevRow_i).m_nodCost_f;
//         m_nodesImg.at<Node>(i,startCol_i).m_accCost_f = m_nodesImg.at<Node>(i-1,startColPrevRow_i).m_nodCost_f;
        
//         startColThisRow_i = endColThisRow_i = startCol_i;
        //return false;
    }   

    // Lets search now the best path (lowest cost).

    --i; // it is not the same as i = m_height_i - 1, because optimiziation could have been interrupted.

    // We can reuse the startColThisRow and endColThisRow, since they are still valid.
    int      f_bestNode_i = -1;
    bool     f_uninit_b = true;
    float    minAccCost_f = 0;
    
    for (j = startColThisRow_i; j <= endColThisRow_i; ++j)
    {
        if ( m_nodesImg.at<Node>(i,j).m_accCost_f < minAccCost_f || 
             f_uninit_b )
        {
            f_uninit_b = false;
            minAccCost_f  = m_nodesImg.at<Node>(i,j).m_accCost_f;
            f_bestNode_i = j;
        }        
    }

    // This means no valid cell in the last row.
    if (f_bestNode_i == -1)
    {
        printf("This shouldn't happen! Debug %s at line %i\n", __FILE__, __LINE__);
        return false;
    }
    
    // Get the best path from bottom to top.

    for (; i >= firstValidRow_i; --i)
    {        
        m_auxVector[i] = f_bestNode_i;
        f_bestNode_i = m_nodesImg.at<Node>(i,f_bestNode_i).m_parNode_i;

        if (i > firstValidRow_i && (f_bestNode_i < 0 || f_bestNode_i >= m_width_i))
        {
            printf("This shouldn't happen. Debug here %s line %i\n", __FILE__, __LINE__);
            printf("error i = %i f_bestNode_i = %i, width = %i\n", i,
                   f_bestNode_i, m_width_i );
            return false;
        }
    }

    if (m_applyMedianFilter_b)
    {
        int f_array_a[DP4F_MAX_MEDFILT_KERNEL_SIZE];
        int f_filtSize_i = m_medFiltHKSize_i * 2 + 1;
        int f_lastElem_i = m_height_i-m_medFiltHKSize_i;
        int f_min_i, f_max_i;
        f_min_i = 0;
        f_max_i = f_filtSize_i-1;

        // Copy left part of vector.
        for (i = 0; i < m_medFiltHKSize_i; ++i)
            fr_vecRes[i] = m_auxVector[i];
       
        while (i < f_lastElem_i)
        {   
            for (j = f_min_i; j <= f_max_i; ++j)
                f_array_a[j-f_min_i] = m_auxVector[j];

            fr_vecRes[i] = median_filter(f_array_a, f_filtSize_i);
            
            ++i;
            ++f_min_i;
            ++f_max_i;
        }

        // Copy right part of vector.
        for (; i < m_height_i; ++i)
            fr_vecRes[i] = m_auxVector[i];
    }
    else
    {
        for (unsigned int i = 0; i < m_auxVector.size(); ++i)
        {
            fr_vecRes[i] = m_auxVector[i];
        }
    }

    return true;
}

