/*@@@**************************************************************************
 * \file  operator
 * \author Hernan Badino
 * \date  Thu Feb 19 12:58:34 Local time zone must be set--see zic manual page 2009
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "operatorBase.h"
#include "drawingList.h"
#include "clock.h"
#include "displayStateParam.h"

using namespace QCV;

CDrawingListHandler    COperatorBase::m_drawingListHandler;
CClockHandler          COperatorBase::m_clockHandler;

COperatorBase::COperatorBase (  COperatorBase * const f_parent_p /* = NULL */, 
                                const std::string f_name_str /* = "Unnamed OperatorBase" */ )
    : CNode (      f_parent_p, f_name_str ),
      m_paramSet_p (                 NULL )
{
    m_paramSet_p = new CParameterSet(NULL);
    m_paramSet_p -> setName ( f_name_str );

    if ( m_parent_p )
        getParentOp() -> getParameterSet ()->addSubset ( m_paramSet_p );
}

COperatorBase::~COperatorBase ()
{
    /// Delete parameter set only if this is the root node.
    if ( m_parent_p == NULL )
        delete m_paramSet_p;

    deleteChildren ( );
}

bool 
COperatorBase::cycle()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);

        if ( child_p )
        {
            child_p -> startClock ("Cycle");
            bool res_b = child_p -> cycle();
            child_p -> stopClock ("Cycle");
            result_b &= res_b;
        }
    }

    return result_b;
}

bool 
COperatorBase::show()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);

        if ( child_p )
        {
            child_p -> startClock ("Show");
            bool res_b = child_p ->  show();
            child_p -> stopClock ("Show");
            result_b &= res_b;
        }
    }

    return result_b;
}

bool 
COperatorBase::initialize()
{    
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);

        if ( child_p )
        {
            child_p -> startClock ("Initialize");
            bool res_b = child_p ->  initialize();
            child_p -> stopClock ("Initialize");
            result_b &= res_b;
        }
    }

    return result_b;
}

bool 
COperatorBase::reset()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);

        if ( child_p )
        {
            child_p -> startClock ("Reset");
            bool res_b = child_p ->  reset();
            child_p -> stopClock ("Reset");
            result_b &= res_b;
        }
    }

    return result_b;
}

bool 
COperatorBase::exit()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);

        if ( child_p )
        {
            child_p -> startClock ("Exit");
            bool res_b = child_p ->  exit();
            child_p -> stopClock ("Exit");
            result_b &= res_b;
        }
    }

    return result_b;
}

void 
COperatorBase::mousePressed ( CMouseEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  mousePressed ( f_event_p );
        }
    }
    
}

void 
COperatorBase::mouseReleased ( CMouseEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
        child_p ->  mouseReleased ( f_event_p );
        }
    }
}

void 
COperatorBase::mouseMoved ( CMouseEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  mouseMoved ( f_event_p );
        }
    }
}

void 
COperatorBase::wheelTurned ( CWheelEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  wheelTurned ( f_event_p );
        }
    }
}

void 
COperatorBase::regionSelected ( CRegionSelectedEvent * const 
                                f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  regionSelected ( f_event_p );
        }
    }
}

void 
COperatorBase::keyPressed ( CKeyEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  keyPressed ( f_event_p );
        }
    }
}


std::vector<QWidget*> 
COperatorBase::getWidgets( ) const
{
    // Return empty vector.
    std::vector<QWidget *> v;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperatorBase *child_p = static_cast<COperatorBase *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            std::vector<QWidget *> childWidgets = child_p ->  getWidgets ( );        
            v.insert ( v.end(), childWidgets.begin(), childWidgets.end() );
        }
    }

    return v;
}

void 
COperatorBase::registerDrawingList ( std::string f_id_str,
                                     S2D<int>    f_position, 
                                     bool        f_visibile_b,
                                     int         f_overlayLevel_i )
{
    CDrawingList *  list_p = m_drawingListHandler.getDrawingList ( f_id_str, this );
    list_p -> setPosition ( f_position );
    list_p -> setVisibility ( f_visibile_b );
    list_p -> setOverlayLevel ( f_overlayLevel_i );
}

CDrawingList * 
COperatorBase::getDrawingList ( std::string f_id_str )
{
    return m_drawingListHandler.getDrawingList ( f_id_str, this );
}

/// Inform the drawing list handler to update the display.
void 
COperatorBase::updateDisplay ( )
{
    m_drawingListHandler.setDisplayUpdateFlag ( true );
}

/// Register a clock so that is visible and available from the 
/// beginning.
void
COperatorBase::registerClock ( std::string f_name_str )
{
    getClock ( f_name_str );
}

/// Get a clock to measure computation time.
CClock *
COperatorBase::getClock ( std::string f_id_str )
{
    printf("getting clock %s\n", f_id_str.c_str());
    
    return m_clockHandler.getClock ( f_id_str, this );
}

/// Start clock.
void
COperatorBase::startClock ( std::string f_name_str )
{
    CClock *  clock_p = getClock ( f_name_str );
    if ( clock_p ) clock_p -> start();
}

/// Start clock.
void
COperatorBase::stopClock ( std::string f_name_str )
{
    CClock *  clock_p = getClock ( f_name_str );
    if ( clock_p ) clock_p -> stop();
}

CParameterSet *
COperatorBase::getParameterSet() const
{
    return m_paramSet_p;
}

void 
COperatorBase::addDrawingListParameter ( std::string f_id_str,
                                         std::string f_comment_str )
{
    CDrawingList *list_p;
    list_p = getDrawingList(f_id_str);

    if (f_comment_str.empty())
        f_comment_str = list_p -> getName();

    m_paramSet_p -> addParameter (
        new CDisplayStateParameter ( list_p -> getName(), 
                                     f_comment_str, 
                                     list_p -> getDisplayState(),
                                     new CParameterConnector< CDrawingList, SDisplayState, CDisplayStateParameter >
                                     ( list_p, 
                                       &CDrawingList::getDisplayState, 
                                       &CDrawingList::setDisplayState ) ) );
}

bool
COperatorBase::setScreenSize ( S2D<unsigned int> f_size )
{
    m_drawingListHandler.setScreenSize(f_size);
    return true;
}

S2D<unsigned int> COperatorBase::getScreenSize (  ) const
{
    return m_drawingListHandler.getScreenSize();
}
