/**
*  @file       TriangleBatch.h
*  @brief      Group of triangle (list, fan or strip).
*  @author     Sébastien Lussier.
*  @date       04/05/05.
*/
/*
*  Copyright (C) 2005 Gamedesk
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
#ifndef     _TRIANGLE_BATCH_H_
#define     _TRIANGLE_BATCH_H_


namespace Gamedesk {


class ENGINE_API TriangleBatch
{
public:
    enum BatchType
    {
        TriangleList,
        TriangleStrip,
        TriangleFan
    };

public:
    TriangleBatch();
    TriangleBatch( const TriangleBatch& pOther );
    ~TriangleBatch();

    const TriangleBatch& operator = ( const TriangleBatch& pOther );

    void            Allocate( BatchType pType, UInt32 pIndexCount );

    UInt32          GetTriangleCount() const;
    UInt32          GetIndicesCount() const;

    UInt16*         GetIndices();

    BatchType       GetBatchType() const;
    void            Stripify( TriangleBatch& pNewBatch );

private:
    void            Free();

private:
    UInt32          mIndicesCount;
    UInt16*         mIndices;
    BatchType       mType;
};


} // namespace Gamedesk


#endif // _TRIANGLE_BATCH_H_
