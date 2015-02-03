#include "ImageSourceFileStbImage.h"

#include "cinder/Utilities.h"

namespace cinder {

//  stb IO implemented with IStream
static int _Read(void *user, char *data, int size)
{
	IStream* stream = static_cast<IStream*>(user);
	return stream->readDataAvailable(data, size);
}

static void _Skip(void *user, int n)
{
	IStream* stream = static_cast<IStream*>(user);
    stream->seekRelative(n);
}

static int _Eof(void *user)
{
	IStream* stream = static_cast<IStream*>(user);
    return stream->isEof();
}

// ----------------------------------------------------------

ImageSourceFileStbImageRef ImageSourceFileStbImage::createFileFreeImageRef( DataSourceRef dataSourceRef, ImageSource::Options options )
{
	return ImageSourceFileStbImageRef( new ImageSourceFileStbImage( dataSourceRef, options ) );
}

ImageSourceFileStbImage::ImageSourceFileStbImage( DataSourceRef dataSourceRef, ImageSource::Options options )
	: ImageSource(), mData(0)
{
    stbi_io_callbacks io;
    io.read = _Read;
    io.skip = _Skip;
    io.eof  = _Eof;

	IStreamRef stream = dataSourceRef->createStream();

    mData = stbi_load_from_callbacks(&io, stream.get(), &mWidth, &mHeight, &mComponents, 0);
    setDataType(ImageIo::UINT8);
    switch (mComponents)
    {
        case 1:
            setColorModel( ImageIo::CM_GRAY );
            setChannelOrder( ImageIo::Y );
            break;
        case 2:
            setColorModel( ImageIo::CM_GRAY );
            setChannelOrder( ImageIo::YA );
            break;
        case 3:
            setColorModel( ImageIo::CM_RGB );
            setChannelOrder( ImageIo::RGB );
            break;
        case 4:
            setColorModel( ImageIo::CM_RGB );
            setChannelOrder( ImageIo::RGBA );
            break;
        default:
            break;
    }
}

void ImageSourceFileStbImage::load( ImageTargetRef target )
{
	// get a pointer to the ImageSource function appropriate for handling our data configuration
	ImageSource::RowFunc func = setupRowFunc( target );
	
	for( int32_t row = 0; row < mHeight; ++row ) {
        const uint8_t *data = mData + (row * mWidth * mComponents);
		((*this).*func)( target, row, data );
	}
}

void ImageSourceFileStbImage::registerSelf()
{
    ImageIoRegistrar::registerSourceType("png", ImageSourceFileStbImage::createRef, 1);
    ImageIoRegistrar::registerSourceType("jpg", ImageSourceFileStbImage::createRef, 1);
    ImageIoRegistrar::registerSourceType("jpeg", ImageSourceFileStbImage::createRef, 1);
}

ImageSourceFileStbImage::~ImageSourceFileStbImage()
{
	if (mData) {
        stbi_image_free(mData);
        mData = 0;
	}
}

} // namespace ci
