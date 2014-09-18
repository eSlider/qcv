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
 * \file  operator
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "operator.h"
#include "drawingList.h"
#include "clock.h"
#include "displayStateParam.h"

using namespace QCV;

CDrawingListHandler    COperator::m_drawingListHandler;
CClockHandler          COperator::m_clockHandler;
CGLViewer *            COperator::m_3dViewer_p = NULL;

COperator::COperator (  COperator * const f_parent_p /* = NULL */, 
                                const std::string f_name_str /* = "Unnamed Operator" */ )
    : CNode (      f_parent_p, f_name_str ),
      m_paramSet_p (                 NULL )
{
    m_paramSet_p = new CParameterSet(NULL);
    m_paramSet_p -> setName ( f_name_str );

    /// Register the ensure existance of root when the first CClockHandler::getClock
    /// is called.
    registerClock ( "Cycle");

    if ( m_parent_p )
        getParentOp() -> getParameterSet ()->addSubset ( m_paramSet_p );
}

COperator::~COperator ()
{
    /// Delete parameter set only if this is the root node.
    if ( m_parent_p == NULL )
        delete m_paramSet_p;

    deleteChildren ( );
}

bool 
COperator::cycle()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);

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
COperator::cycle( COperator *f_child_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);

        if ( child_p == f_child_p && child_p )
        {
            child_p -> startClock ("Cycle");
            bool res_b = child_p -> cycle();
            child_p -> stopClock ("Cycle");
            return res_b;
        }
    }
    return false;
}

bool 
COperator::show()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);

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
COperator::initialize()
{    
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);

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
COperator::reset()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);

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
COperator::exit()
{
    bool result_b = true;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);

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
COperator::mousePressed ( CMouseEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  mousePressed ( f_event_p );
        }
    }
    
}

void 
COperator::mouseReleased ( CMouseEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
        child_p ->  mouseReleased ( f_event_p );
        }
    }
}

void 
COperator::mouseMoved ( CMouseEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  mouseMoved ( f_event_p );
        }
    }
}

void 
COperator::wheelTurned ( CWheelEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  wheelTurned ( f_event_p );
        }
    }
}

void 
COperator::regionSelected ( CRegionSelectedEvent * const 
                                f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  regionSelected ( f_event_p );
        }
    }
}

void 
COperator::keyPressed ( CKeyEvent * const f_event_p )
{
    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            child_p ->  keyPressed ( f_event_p );
        }
    }
}


std::vector<QWidget*> 
COperator::getWidgets( ) const
{
    // Return empty vector.
    std::vector<QWidget *> v;

    for (uint32_t i = 0; i < m_children_v.size(); ++i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        
        if ( child_p )
        {
            std::vector<QWidget *> childWidgets = child_p ->  getWidgets ( );        
            v.insert ( v.end(), childWidgets.begin(), childWidgets.end() );
        }
    }

    return v;
}

void 
COperator::registerDrawingList ( std::string f_id_str,
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
COperator::getDrawingList ( std::string f_id_str )
{
    return m_drawingListHandler.getDrawingList ( f_id_str, this );
}

/// Inform the drawing list handler to update the display.
void 
COperator::updateDisplay ( )
{
    m_drawingListHandler.setDisplayUpdateFlag ( true );
}

/// Register a clock so that is visible and available from the 
/// beginning.
void
COperator::registerClock ( std::string f_name_str )
{
    getClock ( f_name_str );
}

/// Get a clock to measure computation time.
CClock *
COperator::getClock ( std::string f_id_str )
{
    return m_clockHandler.getClock ( f_id_str, this );
}

/// Start clock.
void
COperator::startClock ( std::string f_name_str )
{
    CClock *  clock_p = getClock ( f_name_str );
    if ( clock_p ) clock_p -> start();
}

/// Start clock.
void
COperator::stopClock ( std::string f_name_str )
{
    CClock *  clock_p = getClock ( f_name_str );
    if ( clock_p ) clock_p -> stop();
}

CParameterSet *
COperator::getParameterSet() const
{
    return m_paramSet_p;
}

void 
COperator::addDrawingListParameter ( std::string f_id_str,
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
COperator::setScreenSize ( S2D<unsigned int> f_size )
{
    /// We want only the root to be able to set the size of the screen.
    if ( ! getParentOp() )
        m_drawingListHandler.setScreenSize(f_size);
    return true;
}

S2D<unsigned int> COperator::getScreenSize (  ) const
{
    return m_drawingListHandler.getScreenSize();
}


        
/// Clear all I/O registers.
void
COperator::clearIOMap ( )
{
    for (int i = m_children_v.size()-1; i >=0 ; --i)
    {
        COperator *child_p = static_cast<COperator *>(m_children_v[i].ptr_p);
        child_p->clearIOMap ( );
    }

    // Clear elements
    std::map<std::string, CIOBase*>::iterator 
        it = m_ios.begin ();

    while (it != m_ios.end() )
    {
        /// Delete CIO object
        delete it->second;
        ++it;
    }

    m_ios.erase( m_ios.begin(), m_ios.end() );
}



/// Set the output of this operator.
void
COperator::registerOutputs ( const std::map< std::string, CIOBase * > &f_elements )
{
    m_ios.insert( f_elements.begin(), f_elements.end() ) ;   
    
    if (getParentOp())
        getParentOp() -> m_ios.insert( f_elements.begin(), f_elements.end() ) ;
}
  
/*      
/// Register the outputs of this operator to a second operator.
void
COperator::registerOutputsTo ( COperator *f_other_p )
{
    f_other_p -> m_ios.insert( m_ios.begin(), m_ios.end() ) ;
}
*/

/// Get output of this operator.
void
COperator::getOutputMap ( std::map< std::string, CIOBase* > &fr_elements ) const
{
    fr_elements.clear();
    fr_elements.insert( m_ios.begin(), m_ios.end() );
}

/// Set the 3D viewer
void
COperator::set3DViewer ( CGLViewer * f_viewer_p )
{
    m_3dViewer_p = f_viewer_p;
}

