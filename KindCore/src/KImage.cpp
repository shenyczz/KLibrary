#include "StdAfx.h"
#include "KImage.h"

#include "KString.h"

namespace Images
{

	KImage::KImage(void)
	{
	}

	KImage::~KImage(void)
	{
	}

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
	   UINT  num = 0;          // number of image encoders
	   UINT  size = 0;         // size of the image encoder array in bytes

	   ImageCodecInfo* pImageCodecInfo = NULL;

	   GetImageEncodersSize(&num, &size);
	   if(size == 0)
		  return -1;  // Failure

	   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	   if(pImageCodecInfo == NULL)
		  return -1;  // Failure

	   GetImageEncoders(num, size, pImageCodecInfo);

	   for(UINT j = 0; j < num; ++j)
	   {
		  if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		  {
			 *pClsid = pImageCodecInfo[j].Clsid;
			 free(pImageCodecInfo);
			 return j;  // Success
		  }    
	   }

	   free(pImageCodecInfo);
	   return -1;  // Failure
	}


	Status KImage::Save(LPCTSTR lpszFile,ImageType imgType)
	{
		WCHAR* format = NULL;

		switch(imgType)
		{
		case ImageType::bmp:
			format = L"image/bmp";
			break;

		case ImageType::jpg:
		case ImageType::jpeg:
			format = L"image/jpeg";
			break;

		case ImageType::gif:
			format = L"image/gif";
			break;

		case ImageType::png:
			format = L"image/png";
			break;

		case ImageType::tiff:
			format = L"image/tiff";
			break;

		default:
			format = L"image/bmp";
			break;
		}

		CLSID  encoderClsid;
		INT    result;

		result = GetEncoderClsid(format, &encoderClsid);
		if(result<0)
		{
			return Status::GenericError;
		}

		LPCWSTR s = KString::ToWChar(lpszFile);
		return Image::Save(s,&encoderClsid);
	}

//---------------------------------------------------------
}// namespace Images - end
//---------------------------------------------------------
