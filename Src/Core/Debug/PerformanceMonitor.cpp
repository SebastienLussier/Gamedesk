/**
 *  @file       PerformanceMonitor.cpp
 *  @brief      Small profiler that lets you collect info at runtime.
 *  @desc       Posted on Flipcode COTD by Chris Brodie
 *              - Is reasonably lightweight. 
 *              - Easy to use. 
 *              - Real-time data access, you don't need to wait until application exits to get your data. 
 *              - Frame aware, i.e. it can be used to collect data on a frame by frame basis or perhaps per second for averaged display. 
 *              - Collects hierarchical data to aid in pinpointing bottlenecks 
 *  @author     Sébastien Lussier.
 *  @date       10/02/04.
 */
/*
 *  Copyright (C) 2004 Gamedesk
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *  Gamedesk
 *  http://gamedesk.type-cast.com
 *
 */
#include "Core.h"
#include "PerformanceMonitor.h"


namespace Gamedesk {
	
	
#if GD_CFG_USE_PERF_MONITOR == GD_ENABLED

tree<PerformanceStats>						PerformanceMonitor::mStats;
tree<PerformanceStats>::sibling_iterator	PerformanceMonitor::mParent = mStats.begin();

#endif


} // namespace Gamedesk
