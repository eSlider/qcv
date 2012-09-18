/**
*******************************************************************************
*
* @file featureStereoOp.cpp
*
* \class CStereoOp
* \date  Tue Oct 13, 2009
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Framework operator class for computing ego-motion from 3D point
*        correspondences between current and previous frame.
*
*******************************************************************************/

/* INCLUDES */
#include <limits>

#include "stereo.h"
#include "paramMacros.h"
#include "drawingList.h"

using namespace QCV;

/// Constructors.
CStereoOp::CStereoOp ( COperatorBase * const f_parent_p )
    : COperator<TInpImgFromFileVector, TOutputType>
      (               f_parent_p, "OpenCV Stereo" ),
      m_sgbm (                                    ),
      m_sbm (                                     ),
      m_leftImg (                                 ),
      m_rightImg (                                ),
      m_dispImg (                                 )
{
    registerDrawingLists();
    registerParameters();

    if ( !getParentOp() )
        addChild ( new CStereoOp ( this ) );    
}


void
CStereoOp::registerDrawingLists()
{
    registerDrawingList ("Left Image",
                         S2D<int> (0, 0),
                         true);
    registerDrawingList ("Right Image",
                         S2D<int> (1, 0),
                         true);
    registerDrawingList ("Disparity Image",
                         S2D<int> (2, 0),
                         true);
}

void
CStereoOp::registerParameters()
{

    // ADD_INT_PARAMETER ( "Min Disparity",
    //                     "",
    //                     0,
    //                     &m_sgbm,
    //                     MinDisparity,
    //                     CMyStereoSGBM );


    BEGIN_PARAMETER_GROUP("SGBM", false, SRgb(220,0,0));

      ADD_INT_PARAMETER ( "Pre Filter Cap",
                          "",
                          0, //cv::CV_STEREO_BM_NORMALIZED_RESPONSE,
                          &m_sgbm,
                          PreFilterCap,
                          CMyStereoSGBM );
      
      ADD_INT_PARAMETER ( "Window Size",
                          "",
                          15,
                          &m_sgbm,
                          SADWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Uniqueness Ratio",
                          "",
                          1,
                          &m_sgbm,
                          UniquenessRatio,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "P1",
                          "Cost parameter P1 of SGM",
                          10,
                          &m_sgbm,
                          P1,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "P2",
                          "Cost parameter P2 of SGM",
                          200,
                          &m_sgbm,
                          P2,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "SpeckeWindowSize",
                          "",
                          1,
                          &m_sgbm,
                          SpeckleWindowSize,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Speckle Range",
                          "",
                          1,
                          &m_sgbm,
                          SpeckleRange,
                          CMyStereoSGBM );

      ADD_INT_PARAMETER ( "Disp 12 Max Diff",
                          "",
                          1,
                          &m_sgbm,
                          Disp12MaxDiff,
                          CMyStereoSGBM );

      ADD_BOOL_PARAMETER ( "Full DP",
                           "",
                           false,
                           &m_sgbm,
                           FullDP,
                           CMyStereoSGBM );

    END_PARAMETER_GROUP;


    BEGIN_PARAMETER_GROUP("SBM", false, SRgb(220,0,0));

      ADD_INT_PARAMETER ( "preFilterType",
                          "",
                          0,
                          &m_sbm,
                          PreFilterType,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "preFilterSize",
                          "",
                          5,
                          &m_sbm,
                          PreFilterSize,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "preFilterCap",
                          "",
                          1,
                          &m_sbm,
                          PreFilterCap,
                          CMyStereoBM );
    
      // ADD_INT_PARAMETER ( "minDisparity",
      //                     "",
      //                     0,
      //                     &m_sbm,
      //                     MinDisparity,
      //                     CMyStereoBM );
    
      // ADD_INT_PARAMETER ( "numberOfDisparities",
      //                     "",
      //                     DEVVAL,
      //                     &m_sbm.state->numberOfDisparities,
      //                     NumberOfDisparities,
      //                     CMyStereoBM );
    
      ADD_INT_PARAMETER ( "textureThreshold",
                          "",
                          1,
                          &m_sbm,
                          TextureThreshold,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "uniquenessRatio",
                          "",
                          1,
                          &m_sbm,
                          UniquenessRatio,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "speckleWindowSize",
                          "",
                          5,
                          &m_sbm,
                          SpeckleWindowSize,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "speckleRange",
                          "",
                          4,
                          &m_sbm,
                          SpeckleRange,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "trySmallerWindows",
                          "",
                          0,
                          &m_sbm,
                          TrySmallerWindows,
                          CMyStereoBM );
    
      ADD_INT_PARAMETER ( "disp12MaxDiff",
                          "",
                          1,
                          &m_sbm,
                          Disp12MaxDiff,
                          CMyStereoBM );    
    END_PARAMETER_GROUP;

    BEGIN_PARAMETER_GROUP("Display", false, SRgb(220,0,0));

      addDrawingListParameter ( "Disparity Image" );
      addDrawingListParameter ( "Left Image" );
      addDrawingListParameter ( "Right Image" );

    END_PARAMETER_GROUP;

}

/// Virtual destructor.
CStereoOp::~CStereoOp ()
{
}

/// Cycle event.
bool
CStereoOp::cycle()
{
    if ( 0 && m_leftImg.size().width  > 0 && 
         m_leftImg.size().height > 0 && 
         m_leftImg.size() == m_rightImg.size() )
    {
        cv::Mat auxImg;

        int m_scale_i = 1;
        
        unsigned int numberOfDisparities = ceil(40 / (16.0*m_scale_i)) * 16;

        m_sgbm.numberOfDisparities = numberOfDisparities;
        m_sbm.state->numberOfDisparities = numberOfDisparities;

        cv::Size size = m_leftImg.size();
    
        if (m_scale_i > 1) 
        {
            size.width  /=  m_scale_i;
            size.height /= m_scale_i;
        
            cv::Mat tmpLeft, tmpRight;
            cv::resize(m_leftImg, tmpLeft, size);
            cv::resize(m_rightImg, tmpRight, size);

            if (0) 
            {
                m_sgbm(tmpLeft, tmpRight, auxImg);
            }
            else
            {
                m_sbm(tmpLeft, tmpRight, auxImg);
            }

            cv::resize(auxImg, m_dispImg, m_leftImg.size(), cv::INTER_NEAREST );
            m_dispImg *= 2;
        }
        else 
        {
            if (1) 
            {
                m_sgbm(m_leftImg, m_rightImg, m_dispImg);
            }
            else
                m_sbm(m_leftImg, m_rightImg, m_dispImg);
        }
    }

    return COperatorBase::cycle();
}

/// Show event.
bool CStereoOp::show()
{
    CDrawingList *list_p  = getDrawingList ( "Left Image");
    setScreenSize ( m_leftImg.size() );    

    printf("%i %i\n",     list_p->getScreenWidth(),    list_p->getScreenHeight() );
    
    list_p -> clear();    
    list_p->addImage ( m_leftImg, 0, 0 );
    list_p -> setLineColor ( SRgb ( 255, 255 ,255 ));
    list_p -> addLine ( 0, 0, 640, 480);

    list_p = getDrawingList ( "Right Image");
    list_p -> clear();    
    list_p->addImage ( m_rightImg, 0, 0 );

    list_p = getDrawingList ( "Disparity Image");
    
    list_p->addImage ( m_dispImg, 0, 0, m_dispImg.size().width, m_dispImg.size().height, 1/90. );

    return COperatorBase::show();
}

/// Init event.
bool CStereoOp::initialize()
{
    setScreenSize ( m_leftImg.size() );
    return COperatorBase::initialize();
}

/// Reset event.
bool CStereoOp::reset()
{
    return COperatorBase::reset();
}

bool CStereoOp::exit()
{
    return COperatorBase::exit();
}

void 
CStereoOp::keyPressed ( CKeyEvent * f_event_p )
{
    printf("Event received\n");
    show();
    return COperatorBase::keyPressed ( f_event_p );    
}

/// Set the input of this operator
bool
CStereoOp::setInput  ( const TInpImgFromFileVector & f_input_v )
{
    if ( f_input_v.size () < 2 )
    {
        printf("Stereo requires two images to compute\n");
        return false;
    }

    m_leftImg  = f_input_v[0].image;
    m_rightImg = f_input_v[1].image;
}

/// Gets the output of this operator
bool
CStereoOp::getOutput ( TOutputType & f_output ) const
{
    return true;
}

