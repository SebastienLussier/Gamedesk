
#include "LWOImpExp.h"
#include "MemoryFile.h"


MemoryFile::MemoryFile() : 
    mEOF(false),
    mFailed(false),
    mFileCursor(0),
    mFileSize(0),
    mFileBytes(NULL)
{
}

MemoryFile::MemoryFile( const String& pFilename ) :
    mEOF(false),
    mFailed(false),
    mFileCursor(0),
    mFileSize(0),
    mFileBytes(NULL)
{
    open( pFilename );
}

MemoryFile::~MemoryFile()
{
    close();
}

void MemoryFile::open( const String& pFilename )
{
    std::ifstream file;
    file.open( pFilename.c_str(), std::ios_base::binary );

    file.seekg( 0, std::ios_base::end );
    mFileSize = file.tellg();
    file.seekg( 0, std::ios_base::beg );

    mFileBytes = GD_NEW_ARRAY(Byte, mFileSize, this, "MemoryFile Data");
    file.read( (Char*)mFileBytes, mFileSize );
}

void MemoryFile::close()
{
    if( mFileBytes )
    {
        GD_DELETE_ARRAY(mFileBytes);
        mFileBytes = NULL;
    }
    
    mFileSize = 0;
    mFailed = true;
    mEOF = true;
}

MemoryFile& MemoryFile::operator>> ( Bool& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Bool*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( Char& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Char*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( Byte& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Byte*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( Int16& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Int16*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( UInt16& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(UInt16*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( Int32& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Int32*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( UInt32& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(UInt32*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( Float& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Float*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::operator>> ( Double& val )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+sizeof(val) > mFileSize;

    if( !mFailed )
    {
        val = *(Double*)(mFileBytes + mFileCursor);
        mFileCursor += sizeof(val);
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

Bool MemoryFile::fail() const
{
    return mFailed;
}

MemoryFile& MemoryFile::read( Char* s, std::streamsize n )
{
    if( mFailed || mEOF ) 
        return *this;
    
    mFailed = mFileCursor+n > mFileSize;

    if( !mFailed )
    {
        memcpy( s, (void*)(mFileBytes + mFileCursor), n );
        mFileCursor += n;
        
        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::seekg( std::streampos pos )
{
    if( pos < 0 || pos > (std::streamsize)mFileSize )
    {
        mFailed = true;
        return *this;
    }
    else
    {
        mFailed     = false;
        mFileCursor = pos;

        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

MemoryFile& MemoryFile::seekg( std::streamoff off, std::ios_base::seekdir dir )
{
    std::streampos newPos;

    if( dir == std::ios_base::cur )
    {
        newPos = mFileCursor + off;
    }
    else if( dir == std::ios_base::beg )
    {
        newPos = off;
    }
    else if( dir == std::ios_base::end )
    {
        newPos = mFileSize + off;
    }

    if( newPos < 0 || newPos > (std::streamsize)mFileSize )
    {
        mFailed = true;
        return *this;
    }
    else
    {
        mFileCursor = newPos;

        if( mFileCursor == mFileSize )
            mEOF = true;
    }

    return *this;
}

std::streampos MemoryFile::tellg()
{
    return mFileCursor;
}
