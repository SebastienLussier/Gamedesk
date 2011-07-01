/**
 *  @file       Event.h
 *  @brief      Event synchronisation primitive.
 *  @author     S�bastien Lussier.
 *  @date       2008/10/17.
 */
/*
 *  Copyright (C) 2008 Gamedesk
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
#ifndef     _EVENT_H_
#define     _EVENT_H_


namespace Gamedesk {


class CORE_API Event
{
public:
    Event(Bool pAutoReset = true, Bool pInitiallyLocked = false);
    ~Event();

    void SetDone();
    void Wait();
    Bool TryWait(UInt32 pTimeoutMS);
    Bool IsDone() const;
    void Reset();

private:
    HANDLE mEvent;
};


} // namespace Gamedesk


#endif
