/**
 *  @file       RenderTarget.h
 *  @brief      .
 *  @author     S�bastien Lussier.
 *  @date       20/01/03.
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
#ifndef     _RENDER_TARGET_H_
#define     _RENDER_TARGET_H_


namespace Gamedesk {


class ENGINE_API RenderTarget : public Object
{
    DECLARE_ABSTRACT_CLASS(RenderTarget,Object);

public:
    class Format
    {
    public:
        Bool            mAlpha;
        Bool            mDepth;
        Bool            mStencil;
        Bool            mStereo;
        Bool            mDoubleBuffer;
    };
    
    virtual void MakeCurrent() = 0;
    virtual void Resize( UInt32 pWidth, UInt32 pHeight );
    
    UInt32  GetWidth() const;
    UInt32  GetHeight() const;    

protected:
    RenderTarget()
        : mWidth(0)
        , mHeight(0)
    {
    }

    UInt32      mWidth;
    UInt32      mHeight;
};


} // namespace Gamedesk


#endif  //  _RENDER_TARGET_H_
