/*
 * ADAPTED FROM SAFETYDANK'S FORK OF CINDER:
 * https://github.com/safetydank/Cinder/blob/android-dev/include/cinder/ImageSourceFileStbImage.h
 *
 *
 * MODIFICATIONS:
 *
 * - USING stb_image v2.02
 */

#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "cinder/Cinder.h"
#include "cinder/ImageIo.h"
#include "cinder/Exception.h"

namespace cinder {

typedef std::shared_ptr<class ImageSourceFileStbImage>	ImageSourceFileStbImageRef;

class ImageSourceFileStbImage : public ImageSource {
  public:
	static ImageSourceRef				createRef( DataSourceRef dataSourceRef, ImageSource::Options options = ImageSource::Options() ) { return createFileFreeImageRef( dataSourceRef, options ); }
	static ImageSourceFileStbImageRef	createFileFreeImageRef( DataSourceRef dataSourceRef, ImageSource::Options options = ImageSource::Options() );

	virtual void	load( ImageTargetRef target );

	static void registerSelf();

	virtual ~ImageSourceFileStbImage();

  protected:
	ImageSourceFileStbImage( DataSourceRef dataSourceRef, ImageSource::Options options );
	
	unsigned char* mData;
	int            mComponents;
};

REGISTER_IMAGE_IO_FILE_HANDLER( ImageSourceFileStbImage )

class ImageSourceFileStbImageExceptionUnsupportedData : public ImageIoException {
};

} // namespace ci
