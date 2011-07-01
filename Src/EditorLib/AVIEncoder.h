/**
 *  @file       AVIEncoder.h
 *  @brief	    
 *  @author     Sébastien Lussier.
 *  @date       30/08/04.
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
#ifndef     _AVI_ENCODER_H_
#define     _AVI_ENCODER_H_


#pragma comment(lib, "vfw32.lib")


class AVIEncoder
{
public:
    AVIEncoder( QWidget* pWidget );
    ~AVIEncoder();

    Bool IsRecording() const;

    void Start( const String& pFileName, UInt32 pFPS );
    void Stop();
    void TakeShot();

private:
    Bool                mRecording;
    Byte*               mBuffer;
    
    UInt32              mNumberOfAvi;
    UInt32              mPosition;
    UInt32              mBufferSize;
    UInt32              mTotalFrames;
    
    UInt32              mWidth;
    UInt32              mHeight;
    UInt32              mNumBytesPerLine;

    // Windows structures
    PAVIFILE            mFile;
    AVISTREAMINFO       mStreamInfo;
    PAVISTREAM          mStream;
    PAVISTREAM          mCompressedStream;
    BITMAPINFOHEADER    mBitmapHeader;

    QWidget*            mWidget;
};


#endif  //  _AVI_ENCODER_H_