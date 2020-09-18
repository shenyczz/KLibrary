/******************************************************************************
 ** KWsr98DataOperator.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KWsr98DataOperator - WSR98D 数据操作类
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

#include "KRadarDataOperator.h"

namespace Wsr98d
{

	class KIND_EXT_CLASS KWsr98DataOperator : public KRadarDataOperator
	{
	public:
		KWsr98DataOperator(void);
		virtual ~KWsr98DataOperator(void);

	public:
		// 颜色转换为要素值
		double Color2Value(COLORREF clr);
		// 要素值转换为颜色
		COLORREF Value2Color(double value);

		// 要素值转换为颜色索引
		int Value2ColorIndex(double value);

	protected:
		///--- 结构字节转换 beg ---------------------------------------
		// Position
		virtual void ReverseBytes(Position* position);

		// MHB
		virtual void ReverseBytes(MHB* mhb);
		// PDB
		virtual void ReverseBytes(PDB* pdb);
		// PSB
		virtual void ReverseBytes(PSB* psb);
		// GAB
		virtual void ReverseBytes(GAB* gab);
		// TAB
		virtual void ReverseBytes(TAB* tab);

		// DataPacketHeader
		virtual void ReverseBytes(DataPacketHeader* pDataPacketHeader);
		// DataLayerHeader
		virtual void ReverseBytes(DataLayerHeader* pDataLayerHeader);
		// PageHeader
		virtual void ReverseBytes(PageHeader* pPageHeader);
		// EachRadialHeader
		virtual void ReverseBytes(EachRadialHeader* pEachRadialHeader);
		// EachRowHeader
		virtual void ReverseBytes(EachRowHeader* pEachRowHeader);
		// DigitalRadialDataArrayPacketHeader
		virtual void ReverseBytes(DigitalRadialDataArrayPacketHeader* pDigitalRadialDataArrayPacketHeader);
		// DigitalPrecipitationDataArrayPacketHeader
		virtual void ReverseBytes(DigitalPrecipitationDataArrayPacketHeader* pDigitalPrecipitationDataArrayPacketHeader);
		// PrecipitationRateDataArrayPacketHeader
		virtual void ReverseBytes(PrecipitationRateDataArrayPacketHeader* pPrecipitationRateDataArrayPacketHeader);
		// CellTrendDataPacketHeader
		virtual void ReverseBytes(CellTrendDataPacketHeader* pCellTrendDataPacketHeader);
		// SetColorLevelPacketHeader
		virtual void ReverseBytes(SetColorLevelPacketHeader* pSetColorLevelPacketHeader);
		// LinkedContourVectorPacketHeader
		virtual void ReverseBytes(LinkedContourVectorPacketHeader* pLinkedContourVectorPacketHeader);
		// UnlinkedContourVectorPacketHeader
		virtual void ReverseBytes(UnlinkedContourVectorPacketHeader* pUnlinkedContourVectorPacketHeader);
		// RadialDataPacketHeader
		virtual void ReverseBytes(RadialDataPacketHeader* pRadialDataPacketHeader);
		// RasterDataPacketHeader
		virtual void ReverseBytes(RasterDataPacketHeader* pRasterDataPacketHeader);

		// WriteText
		virtual void ReverseBytes(WriteText* pWriteText);
		// Mesocyclone
		virtual void ReverseBytes(Mesocyclone* pMesocyclone);
		// WindBarb
		virtual void ReverseBytes(WindBarb* pWindBarb);
		// VectorArrow
		virtual void ReverseBytes(VectorArrow* pVectorArrow);
		// LinkedVector
		virtual void ReverseBytes(LinkedVector* pLinkedVector);
		// UnlinkedVector
		virtual void ReverseBytes(UnlinkedVector* pUnlinkedVector);
		// HdaHail
		virtual void ReverseBytes(HdaHail* pHdaHail);
		// StiCircle
		virtual void ReverseBytes(StiCircle* pStiCircle);
		///--- 结构字节转换 end ---------------------------------------
	};

//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------
