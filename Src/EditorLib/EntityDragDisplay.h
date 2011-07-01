/**
 *  @file       EntityDragDisplay.h
 *  @brief	    An EntityDragDisplay base class to display the dragged objects.
 *  @author     Marco Arsenault.
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
#ifndef     _ENTITY_DRAG_DISPLAY_H_
#define     _ENTITY_DRAG_DISPLAY_H_


/**
 *  EntityDragDisplay base class.  This class should be derived for every object that
 *  needs to be displayed when dragged.
 *  @brief  EntityDragDisplay base class.
 *  @author Marco Arsenault.
 *  @date   10/02/04.
 */
class EDITORLIB_API EntityDragDisplay
{
    CLASS_DISABLE_COPY(EntityDragDisplay);

public:
    //! Default constructor.
	EntityDragDisplay();

    //! Destructor.
    virtual ~EntityDragDisplay();

    //! Render the dragged entity. Must be implemented in derived classes.
    virtual void Render() const = 0;
};


#endif  //  _ENTITY_DRAG_DISPLAY_H_
