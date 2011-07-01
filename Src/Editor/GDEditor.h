/**
 *  @file       GDEditor.h
 *  @brief      Gamedesk editor.
 *  @author     Sébastien Lussier.
 *  @date       27/10/03.
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
#ifndef     _GD_EDITOR_H_
#define     _GD_EDITOR_H_


#include "Editor.h"
#include "EditorBase.h"


class EDITOR_API GDEditor : public EditorBase
{

public:
	GDEditor();

private:
    // Disable copy of this object.
    GDEditor(const GDEditor&);
    const GDEditor& operator=(const GDEditor&);
};


#endif  //  _GD_EDITOR_H_
