/**
 *  @file       InputDevice.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       05/01/04.
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
#ifndef     _INPUT_DEVICE_H_
#define     _INPUT_DEVICE_H_


namespace Gamedesk {


/**
 *  Base class of all input devices (keyboard, mouse, joystick, etc...)
 */
class ENGINE_API InputDevice : public Object
{
    DECLARE_ABSTRACT_CLASS(InputDevice,Object);
    friend class InputSubsystem;

public:
    InputDevice();
    virtual ~InputDevice();
};


} // namespace Gamedesk


#endif  //  _INPUT_DEVICE_H_
