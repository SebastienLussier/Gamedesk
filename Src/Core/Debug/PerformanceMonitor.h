/**
 *  @file       PerformanceMonitor.h
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
#ifndef     _PERFORMANCEMONITOR_H_
#define     _PERFORMANCEMONITOR_H_


#if GD_CFG_USE_PERF_MONITOR == GD_ENABLED


#include "Containers/Tree.h"
#include "SystemInfo/SystemInfo.h"
#include "Maths/Maths.h"


namespace Gamedesk {


class CORE_API PerformanceStats
{
public:
    PerformanceStats()
        : mName("")
		, mMin(0)
		, mMax(0)
		, mTotalTime(0)
		, mSamples(0)
    {
    }


    PerformanceStats( const String& pName )
		: mName(pName)
		, mMin(0)
		, mMax(0)
		, mTotalTime(0)
		, mSamples(0)
	{
	}

	Bool operator == ( const PerformanceStats& pOther ) const
	{
		return mName == pOther.mName;
	}

public:
	String	mName;
	UInt64  mMin;
	UInt64  mMax;
	UInt64	mTotalTime;
	UInt32	mSamples;
};


class CORE_API PerformanceMonitor
{
public:
	PerformanceMonitor( const String& pName )
	{
        // The tree is empty, create a root node
		if( mStats.size() == 0 )
			mParent = mStats.insert( mStats.begin(), PerformanceStats("Gamedesk") );

		// Check to see if we are resampling an existing profile subject.
		PerformanceStats NewStats( pName );
		tree<PerformanceStats>::sibling_iterator itr = std::find( mStats.begin(mParent), mStats.end(mParent), NewStats );
			
		if( itr != mStats.end(mParent) )
        {
            mParent = itr;
            mThisNode = itr;
        }
	    else
		{ 	
			// Profile subject did not exist, create a new one.
			mParent = mStats.append_child( mParent, NewStats );
			mThisNode = mParent;
		}
		
		mStart = SystemInfo::Instance()->GetMicroSec64();
	}

	~PerformanceMonitor()
	{
		UInt64 Elapsed = SystemInfo::Instance()->GetMicroSec64() - mStart;

		// Record statistics for this sampling
		mThisNode->mSamples++;
		mThisNode->mTotalTime += Elapsed;
		
		if( mThisNode->mSamples == 1 )
		{
			mThisNode->mMax = Elapsed;
			mThisNode->mMin = Elapsed;
	    }
		else
		{
			mThisNode->mMax = Maths::Max<Double>(Elapsed, mThisNode->mMax);
			mThisNode->mMin = Maths::Min<Double>(Elapsed, mThisNode->mMin);
		}
		
		// Reset parent pointer
	    mParent = mStats.parent(mThisNode);
	}

	static void Reset(void)
	{
		mStats.clear();
	}

public:
	static tree<PerformanceStats>					mStats;

private:
	UInt64											mStart;
	tree<PerformanceStats>::iterator				mThisNode;
	static tree<PerformanceStats>::sibling_iterator	mParent;
};


} // namespace Gamedesk


#endif  // #if GD_CFG_USE_PERF_MONITOR


#if GD_CFG_USE_PERF_MONITOR == GD_ENABLED
    #define Profile(Name) Gamedesk::PerformanceMonitor 	monitor(Name)
#else
    #define Profile(Name)
#endif


#endif	//	_PERFORMANCEMONITOR_H_
