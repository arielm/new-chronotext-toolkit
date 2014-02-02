/*
 * FROM SAFETYDANK'S FORK OF CINDER:
 * https://github.com/safetydank/Cinder/blob/cbd00824554251d5b5a803133d616b7f21d4b8d5/include/cinder/ImageSourceFileStbImage.h
 */

#pragma once

#include "cinder/Cinder.h"
#include "cinder/ImageIo.h"
#include "cinder/Exception.h"

#include "stb_image.h"

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
