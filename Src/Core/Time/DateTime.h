/**
 *  @file       DateTime.h
 *  @brief      Date and time computations.
 *  @author     Sébastien Lussier.
 *  @date       02/12/02.
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
#ifndef     _DATE_TIME_H_
#define     _DATE_TIME_H_


namespace Gamedesk {


/**
 *  Class for Date computations.
 *  @brief  Class for Date computations.
 *  @todo   Implement class
 *  @author Sebastien Lussier
 *  @date   02/03/03.
 */
class CORE_API DateTime
{
public:
    /**
     *
     */
    DateTime();

    /**
     *
     */
    DateTime( Int32 pValue );

    /**
     *
     */
    virtual ~DateTime();

private:
};


} // namespace Gamedesk


#endif  //  _DATE_TIME_H_
