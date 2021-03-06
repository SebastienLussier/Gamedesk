/**
 *  @file       SelectionListener.h
 *  @brief	    A listener class to receive events about selection changes.
 *  @author     Marco Arsenault.
 *  @date       02/02/04.
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
#ifndef     _SELECTION_LISTENER_H_
#define     _SELECTION_LISTENER_H_


class Entity;


/**
 *  SelectionListener base class.  This listener will receive event from the sender
 *  concerning selection changes.
 *  @brief  SelectionListener base class.
 *  @author Marco Arsenault.
 *  @date   02/02/04.
 */
class EDITORLIB_API SelectionListener
{
    CLASS_DISABLE_COPY(SelectionListener);

public:
    //! Default constructor.
	SelectionListener();

    //! Destructor.
    virtual ~SelectionListener();

    //! Called when a selection is changed.
    virtual void OnSelectionChanged(const std::list<Entity*>& pSelection) = 0;
};


#endif  //  _SELECTION_LISTENER_H_
