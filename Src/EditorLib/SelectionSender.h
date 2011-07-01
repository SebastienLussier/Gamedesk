/**
 *  @file       SelectionSender.h
 *  @brief	    A sender class to notify the listeners about selection changes.
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
#ifndef     _SELECTION_SENDER_H_
#define     _SELECTION_SENDER_H_


#include "Containers/Containers.h"


class SelectionListener;
class Entity;


/**
 *  SelectionSender base class.  This class can notify the listeners about
 *  selection changes.
 *  @brief  SelectionSender base class.
 *  @author Marco Arsenault.
 *  @date   02/02/04.
 */
class EDITORLIB_API SelectionSender
{
    CLASS_DISABLE_COPY(SelectionSender);

public:
    //! Default constructor.
	SelectionSender();

    //! Destructor.
    virtual ~SelectionSender();

    //! Add a selection listener.
    void AddSelectionListener(SelectionListener* pListener);

    //! Remove a selection listener.
    void RemoveSelectionListener(SelectionListener* pListener);
    
protected:
    //! Notify the listeners that the selection has changed.
    void FireSelectionChanged(const std::list<Entity*>& pSelection);

private:
    std::list<SelectionListener*>  mSelectionListeners;
};


#endif  //  _SELECTION_SENDER_H_