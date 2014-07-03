#ifndef __FEATURESTEREOOP_H
#define __FEATURESTEREOOP_H

/**
 *******************************************************************************
 *
 * @file featureStereoOp.cpp
 *
 * \class CFeatureStereoOp
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Framework operator class for computing sparse stereo.
 *
 * Framework operator class for computing sparse stereo.
 *
 *******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include "operator.h"
#include "imagePyramid.h"
#include "colorEncoding.h"
#include "3DPointVector.h"
#include "feature.h"

/* PROTOTYPES */

/* CONSTANTS */
#define FSO_MAX_CORES 8
#define FSO_MAX_WIDTH 4096

namespace QCV
{
    class CStereoFeaturePointVector;


    class CFeatureStereoOp: public COperator
    {
    /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CFeatureStereoOp ( COperator * const f_parent_p = NULL, 
                           const std::string f_name_str = "Feature Stereo" );
        
        /// Virtual destructor.
        virtual ~CFeatureStereoOp ();

        /// Cycle event.
        virtual bool cycle();
    
        /// Show event.
        virtual bool show();
    
        /// Init event.
        virtual bool initialize();
    
        /// Reset event.
        virtual bool reset();
    
        /// Exit event.
        virtual bool exit();

    /// User Operation Events
    public:
        /// Key pressed in display.
        virtual void keyPressed (     CKeyEvent * f_event_p );

    /// Gets and sets.
    public:

        bool              setLevels ( unsigned int f_levels_ui );
        unsigned int      getLevels ( ) const;

        ADD_PARAM_ACCESS (std::string, m_idLeftImage_str,     IdLeftImageStr);
        ADD_PARAM_ACCESS (std::string, m_idRightImage_str,    IdRightImageStr);
        ADD_PARAM_ACCESS (std::string, m_featPointVector_str, FeaturePointVectorId );

        ADD_PARAM_ACCESS (bool,        m_compute_b,           Compute );
        ADD_PARAM_ACCESS (S2D<int>,    m_dispRange,           DisparityRange );
        ADD_PARAM_ACCESS (S2D<int>,    m_maskSize,            MaskSize );

        ADD_PARAM_ACCESS (bool,        m_checkVars_b,         CheckVariances );
        ADD_PARAM_ACCESS (float,       m_minVar_f,            MinVariance );
        ADD_PARAM_ACCESS (float,       m_maxZssd_f,           MaxZSSD );
        ADD_PARAM_ACCESS (float,       m_maxSsd_f,            MaxSSD );

        ADD_PARAM_ACCESS_BOUNDED(int,  m_deltaDisp_i,         DeltaDisp, 1, 65535 );

        ADD_PARAM_ACCESS (float,       m_dispOffset_f,        DispOffset );

        ADD_PARAM_ACCESS (bool,        m_show3DPoints_b,      Show3DPoints );
    /// Protected Data Types
    protected:

    protected:

        void registerDrawingLists();
        void registerParameters();

        void show3D();

        bool setPyramidParams ( unsigned int f_levels_ui );

        bool transferLevel( int f_level_i );
        bool computeFirstCorrelation( int m_fromLevel_i = -1 );

        void generateDispImage( const CFeatureVector *f_vec );

       //void generate3DPointList( const CStereoFeaturePointVector *f_vec );

    private:

        /// Compute?
        bool                        m_compute_b;

        /// Left image Id
        std::string                 m_idLeftImage_str;
        
        /// Right image Id
        std::string                 m_idRightImage_str;
        
        /// Feature Vector Input String
        std::string                 m_featPointVector_str;

        /// Disparity range.
        S2D<int>                    m_dispRange;

        /// Set the correlation mask size.
        S2D<int>                    m_maskSize;

        /// Check the variances of the patches?
        bool                        m_checkVars_b;

        /// Min allowed variance for left and right windows [px^2].
        float                       m_minVar_f;

        /// Max allowed zero-mean sum of square difference [px^2].
        float                       m_maxZssd_f;

        /// Max allowed zero-mean sum of square difference [px^2].
        float                       m_maxSsd_f;        
        
        /// Disparity offset.
        float                       m_dispOffset_f;

        /// Left image pyramid.
        CImagePyramid               m_pyrLeft;

        /// Right image pyramid.
        CImagePyramid               m_pyrRight;
        
        /// Disparity image pyramid.
        CImagePyramid               m_pyrDisp;

        /// Levels
        unsigned int                m_levels_ui;

        /// Correction
        int                         m_deltaDisp_i;

        /// Float disparity image
         cv::Mat                    m_dispImg;

        /// Color encoding mode.
        CColorEncoding              m_ceDistance;

	/// List of 3D points
	C3DPointDataVector          m_3dPoints;

        /// Show 3D points?
	bool                        m_show3DPoints_b;
    private:
        /// Data structure to store scores.
        float m_scoresACTUAL_p[FSO_MAX_CORES][2*FSO_MAX_WIDTH];
    };
}
#endif // __FEATURESTEREOOP_H
