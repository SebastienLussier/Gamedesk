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
#include "EditorLib.h"
#include "SelectionSender.h"
#include "SelectionListener.h"
#include "World/Entity.h"


SelectionSender::SelectionSender()
{
}

SelectionSender::~SelectionSender()
{
}

void SelectionSender::AddSelectionListener(SelectionListener* pListener)
{
    mSelectionListeners.push_back(pListener);
}

void SelectionSender::RemoveSelectionListener(SelectionListener* pListener)
{
    mSelectionListeners.remove(pListener);
}

void SelectionSender::FireSelectionChanged(const std::list<Entity*>& pSelection)
{
    std::list<SelectionListener*>::iterator iter;
    for(iter = mSelectionListeners.begin(); iter != mSelectionListeners.end(); iter++)
    {
        (*iter)->OnSelectionChanged(pSelection);
    }   
}
