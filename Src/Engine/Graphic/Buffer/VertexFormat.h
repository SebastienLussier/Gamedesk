/**
 *  @file       VertexFormat.h
 *  @brief	    Define a vertex format (position, normal, color, texture coordinates, etc.).
 *  @author     Sébastien Lussier.
 *  @date       21/01/04.
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
#ifndef     _VERTEX_FORMAT_H_
#define     _VERTEX_FORMAT_H_


namespace Gamedesk {


/**
 *  Describe a vertex format by specifying which components it use.
 *  It makes it possible for you to use only the vertex components you needs,
 *  eliminating those components that aren't used.  You need to set the 
 *  currently active vertex format by using the Renderer::SetVertexFormat
 *  method.
 *  @author Sébastien Lussier.
 *  @date   21/01/04
 */
class ENGINE_API VertexFormat
{
public:
    enum Component
    {
        None            = 0x00000000,
        Position3       = 0x00000001,
        Normal3         = 0x00000002,
        Color3          = 0x00000004,
        Color4          = 0x00000008,
        TexCoord2       = 0x00000010,
        TexCoord2_2     = 0x00000020,
        DataType_MaxVal = 0xFFFFFFFF
    };

    /**
     *  Constructor.
     *  @param  pComponents     Specify wich components this vertex format is using.
     */
    VertexFormat( Component pComponents = None );

    //! Copy constructor.
    VertexFormat( const VertexFormat& pOther );

    //  Assigment operator.
    const VertexFormat& operator = ( const VertexFormat& pOther );
    
    //! Equal operator.
    Bool operator == ( const VertexFormat& pOther ) const;

    //! Not equal operator.
    Bool operator != ( const VertexFormat& pOther ) const;    

    /**
     *  Test if this vertex format use a certain type of component.
     *  @param  pComponent  The component in question.
     *  @return \bTrue if this vertex format contain the given component, \bfalse otherwise.
     */
    Bool HasComponent( Component pComponent ) const;    

private:
    Component    mComponents;   //!<    Components used by this vertex formats.
};


} // namespace Gamedesk


#endif  //  _VERTEX_FORMAT_H_
