#include "EditorLib.h"
#include "AVIEncoder.h"
//Added by qt3to4:
#include <QPixmap>


AVIEncoder::AVIEncoder( QWidget* pWidget )
    : mWidget(pWidget)
    , mBuffer(NULL)
    , mNumberOfAvi(0)
    , mFile(NULL)
    , mStream(NULL)
    , mCompressedStream(NULL)
    , mRecording(false)
{
}

AVIEncoder::~AVIEncoder()
{
    if( mRecording )
        Stop();        
}

Bool AVIEncoder::IsRecording() const
{
    return mRecording;
}

void AVIEncoder::Start( const String& pFileName, UInt32 pFPS )
{
    HRESULT hr;

    if( HIWORD(VideoForWindowsVersion()) < 0x010a )
    {
        Core::MsgBox( "Movie Encoder", "VideoForWindows version must be greater than 1.1 !" );
        return;
    }

    
    AVIFileInit();

    hr = AVIFileOpen( &mFile, pFileName.c_str(), OF_WRITE | OF_CREATE, NULL );
    if( hr != AVIERR_OK )
    {
        Core::MsgBox( "Movie Encoder", "Unable to create AVI file \"%s\" (Error=0x%08x).", pFileName.c_str(), hr );       
        Stop();
        return;
    }    

    mHeight = mWidget->height() - 2;
    mWidth  = mWidget->width() - 2;

    mNumBytesPerLine = mWidth * 4;
    mBufferSize = mHeight * mNumBytesPerLine;
    mBuffer = GD_NEW_ARRAY(Byte, mBufferSize, this, "EditorLib::Viewer::AVIEncoder");

    memset( &mStreamInfo, 0, sizeof(mStreamInfo) );
	mStreamInfo.fccType                = streamtypeVIDEO;// stream type
	mStreamInfo.fccHandler             = 0;
	mStreamInfo.dwScale                = 1;
	mStreamInfo.dwRate                 = pFPS;		    // 15 fps
	mStreamInfo.dwSuggestedBufferSize  = mBufferSize;
	SetRect(&mStreamInfo.rcFrame, 1, 1,		    // rectangle for stream
		(int) mWidth+1,
		(int) mHeight+1);

    strcpy( mStreamInfo.szName, "Gamedesk Editor Video" );

    hr = AVIFileCreateStream( mFile, &mStream, &mStreamInfo );
    if( hr != AVIERR_OK )
    {
        Core::MsgBox( "Movie Encoder", "Unable to create AVI stream (Error=0x%08x).", hr );
        Stop();
        return;
    }

   // Display a dialog to allow the user to select compression options
    AVICOMPRESSOPTIONS options;
    AVICOMPRESSOPTIONS* arrOptions[1] = { &options };
    memset( &options, 0, sizeof(options) );

    if( !AVISaveOptions( mWidget->winId(), 0, 1, &mStream, (LPAVICOMPRESSOPTIONS*) &arrOptions ) )
    {
        Stop();
        return;
    }

    hr = AVIMakeCompressedStream(&mCompressedStream, mStream, &options, NULL);
    if( hr != AVIERR_OK )
    {
        Core::MsgBox( "Movie Encoder", "Unable to create AVI compressed stream (Error=0x%08x).", hr );
        Stop();
        return;
    }

    mBitmapHeader.biSize            = sizeof(mBitmapHeader);
    mBitmapHeader.biWidth           = mWidth;
    mBitmapHeader.biHeight          = mHeight;
    mBitmapHeader.biPlanes          = 1;
    mBitmapHeader.biBitCount        = 32;
    mBitmapHeader.biCompression     = BI_RGB;
    mBitmapHeader.biSizeImage       = 0;
    mBitmapHeader.biXPelsPerMeter   = 1000;
    mBitmapHeader.biYPelsPerMeter   = 1000;
    mBitmapHeader.biClrUsed         = 0;
    mBitmapHeader.biClrImportant    = 0;

    hr = AVIStreamSetFormat( mCompressedStream, 0, &mBitmapHeader, sizeof(mBitmapHeader) );
    if( hr != AVIERR_OK )
    {
        Core::MsgBox( "Movie Encoder", "Unable to create AVI compressed stream (Error=0x%08x).", hr );
        Stop();
        return;
    }

    mPosition = 0;
    mRecording = true;
}

void AVIEncoder::Stop()
{
    mRecording = false;

    if( mStream )
    {
        AVIStreamRelease(mStream);
        mStream = NULL;
    }
    
    if( mCompressedStream )
    {
        AVIStreamRelease(mCompressedStream);
        mCompressedStream = NULL;
    }

    if( mFile )
    {
        AVIFileRelease(mFile);
        mFile = NULL;
    }

    if( mBuffer )
    {
        GD_DELETE(mBuffer);
        mBuffer = NULL;
    }

    AVIFileExit();
}

void AVIEncoder::TakeShot()
{
    QImage  img( QPixmap::grabWindow( mWidget->winId(), 1, 1, mWidth, mHeight ).toImage().mirrored() );
    HRESULT hr = AVIStreamWrite( mCompressedStream, mPosition, 1, img.bits(), img.numBytes(), 0, NULL, NULL );
    if( hr != AVIERR_OK )
    {
        Core::MsgBox( "Movie Encoder", "Unable to write AVI frame %d (Error=0x%08x).", mPosition, hr );
        Stop();
        return;
    }
    
    mPosition++;
}