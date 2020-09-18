/******************************************************************************
 ** KImage.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KImage
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

namespace Images
{

	class KIND_EXT_CLASS KImage : public Image
	{
	public:
		enum ImageType
		{
			bmp,
			jpg,
			jpeg,
			gif,
			png,
			tiff,
		};

	public:
		KImage(void);
		virtual ~KImage(void);

	public:
		Status Save(LPCTSTR lpszFile,ImageType imgType = png);

	};

//---------------------------------------------------------
}// namespace Images - end
//---------------------------------------------------------
