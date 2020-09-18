#include "StdAfx.h"
#include "KWsr98DataOperator.h"

#include "KWsr98Data.h"

namespace Wsr98d
{

	KWsr98DataOperator::KWsr98DataOperator(void)
	{
		m_pOwner = 0;
	}

	KWsr98DataOperator::~KWsr98DataOperator(void)
	{
	}

	// 颜色值转换到物理量值
	double KWsr98DataOperator::Color2Value(COLORREF clr)
	{
		double dValue = RPG_DATA_INVALID_VALUE;

		KWsr98Data* pWsr98Data = (KWsr98Data*)GetOwner();

		Legend* pLegend = pWsr98Data->GetLegend();
		for(int i=0;i<16;i++)
		{
			if(clr == pLegend[i].dwColor)
			{
				dValue = pLegend[i].dValue;
				break;
			}
		}

		return dValue;
	}

	// 要素值转换为颜色
	COLORREF KWsr98DataOperator::Value2Color(double value)
	{
		COLORREF clr = 0;

		KWsr98Data* pWsr98Data = (KWsr98Data*)GetOwner();

		Legend* pLegend = pWsr98Data->GetLegend();
		for(int i=0;i<16;i++)
		{
			if(fabs(value-pLegend[i].dValue)<EPS)
			{
				clr = pLegend[i].dwColor;
				break;
			}
		}

		return clr;
	}


	// 要素值转换为颜色索引
	int KWsr98DataOperator::Value2ColorIndex(double value)
	{
		int iColorIndex = 0;
		//----------------------------
		//if(value>=-5)	iColorIndex++;	//1
		//if(value>=0)	iColorIndex++;	//2
		//if(value>=5)	iColorIndex++;	//3
		//if(value>=10)	iColorIndex++;	//4
		//if(value>=15)	iColorIndex++;	//5
		//if(value>=20)	iColorIndex++;	//6
		//if(value>=25)	iColorIndex++;	//7
		//if(value>=30)	iColorIndex++;	//8
		//if(value>=35)	iColorIndex++;	//9
		//if(value>=40)	iColorIndex++;	//10
		//if(value>=45)	iColorIndex++;	//11
		//if(value>=50)	iColorIndex++;	//12
		//if(value>=55)	iColorIndex++;	//13
		//if(value>=60)	iColorIndex++;	//14
		//if(value>=65)	iColorIndex++;	//15
		KWsr98Data* pWsr98Data = (KWsr98Data*)GetOwner();

		Legend* pLegend = pWsr98Data->GetLegend();
		for(int i=0;i<16;i++)
		{
			if(fabs(value-pLegend[i].dValue)<EPS)
			{
				iColorIndex = i;
				break;
			}
		}

		return iColorIndex;
	}

	///========================================================
	///结构字节转换 - beg
	///===============
	// Position
	void KWsr98DataOperator::ReverseBytes(Position* position)
	{
		KBytesOrder::ReverseBytes(&position->I);
		KBytesOrder::ReverseBytes(&position->J);
	}

	// MHB
	void KWsr98DataOperator::ReverseBytes(MHB* mhb)
	{
		KBytesOrder::ReverseBytes(&mhb->iMsgCode);
		KBytesOrder::ReverseBytes(&mhb->iMsgDate);
		KBytesOrder::ReverseBytes(&mhb->lMsgTime);
		KBytesOrder::ReverseBytes(&mhb->lMsgLength);
		KBytesOrder::ReverseBytes(&mhb->iSID);
		KBytesOrder::ReverseBytes(&mhb->iDID);
		KBytesOrder::ReverseBytes(&mhb->iBlockNumber);
	}
	// PDB
	void KWsr98DataOperator::ReverseBytes(PDB* pdb)
	{
		KBytesOrder::ReverseBytes(&pdb->iBlockDivider);
		KBytesOrder::ReverseBytes(&pdb->lLat);
		KBytesOrder::ReverseBytes(&pdb->lLon);
		KBytesOrder::ReverseBytes(&pdb->iAlt);
		KBytesOrder::ReverseBytes(&pdb->iProductCode);

		KBytesOrder::ReverseBytes(&pdb->iOPMode);
		KBytesOrder::ReverseBytes(&pdb->iVCP);
		KBytesOrder::ReverseBytes(&pdb->iSequenceNumber);
		KBytesOrder::ReverseBytes(&pdb->iVScanNumber);
		KBytesOrder::ReverseBytes(&pdb->iVScanDate);

		KBytesOrder::ReverseBytes(&pdb->lVScanStartTime);
		KBytesOrder::ReverseBytes(&pdb->iPGDate);
		KBytesOrder::ReverseBytes(&pdb->lPGTime);
		KBytesOrder::ReverseBytes(&pdb->iPD01);
		KBytesOrder::ReverseBytes(&pdb->iPD02);

		KBytesOrder::ReverseBytes(&pdb->iElevationNumber);

		KBytesOrder::ReverseBytes(&pdb->iPD03);

		for(int i=0;i<16;i++)
		{
			KBytesOrder::ReverseBytes(&pdb->iDataLevelThreshold[i]);
		}

		KBytesOrder::ReverseBytes(&pdb->iPD04);
		KBytesOrder::ReverseBytes(&pdb->iPD05);
		KBytesOrder::ReverseBytes(&pdb->iPD06);
		KBytesOrder::ReverseBytes(&pdb->iPD07);
		KBytesOrder::ReverseBytes(&pdb->iPD08);
		KBytesOrder::ReverseBytes(&pdb->iPD09);
		KBytesOrder::ReverseBytes(&pdb->iPD10);

		KBytesOrder::ReverseBytes(&pdb->iMapNumber);
		//--------------------------------------------
		//因87 88 110 113号产品是中尾字节编码
		if(    pdb->iProductCode==87
			|| pdb->iProductCode==88
			|| pdb->iProductCode==110
			|| pdb->iProductCode==113)
		{
			//KBytesOrder::ReverseBytes(&pdb->iOffsetSymbology[0]);
			//KBytesOrder::ReverseBytes(&pdb->iOffsetSymbology[1]);
			KBytesOrder::ReverseBytes(&pdb->lOffsetSymbology,TRUE);
		}
		else
		{
			KBytesOrder::ReverseBytes(&pdb->lOffsetSymbology);
		}
		//--------------------------------------------
		KBytesOrder::ReverseBytes(&pdb->lOffsetGraphic);
		KBytesOrder::ReverseBytes(&pdb->lOffsetTabular);

	}
	// PSB
	void KWsr98DataOperator::ReverseBytes(PSB* psb)
	{
		KBytesOrder::ReverseBytes(&psb->iBlockDivider);
		KBytesOrder::ReverseBytes(&psb->iBlockID);
		KBytesOrder::ReverseBytes(&psb->lBlockLength);
		KBytesOrder::ReverseBytes(&psb->iTotalLayers);
	}
	// GAB
	void KWsr98DataOperator::ReverseBytes(GAB* gab)
	{
		KBytesOrder::ReverseBytes(&gab->iBlockDivider);
		KBytesOrder::ReverseBytes(&gab->iBlockID);
		KBytesOrder::ReverseBytes(&gab->lBlockLength);
		KBytesOrder::ReverseBytes(&gab->iNumberOfPages);
	}
	// TAB
	void KWsr98DataOperator::ReverseBytes(TAB* tab)
	{
		KBytesOrder::ReverseBytes(&tab->iBlockDivider);
		KBytesOrder::ReverseBytes(&tab->iBlockID);
		KBytesOrder::ReverseBytes(&tab->lBlockLength);
		this->ReverseBytes(&tab->tMsgHeader);
		this->ReverseBytes(&tab->tProductDescription);
		KBytesOrder::ReverseBytes(&tab->iBlockDivider2);
		KBytesOrder::ReverseBytes(&tab->iNumberOfPages);
	}

	// TDataPacketHeader
	void KWsr98DataOperator::ReverseBytes(DataPacketHeader* pDataPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pDataPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pDataPacketHeader->iLengthOfDataBlock);
	}
	// DataLayerHeader
	void KWsr98DataOperator::ReverseBytes(DataLayerHeader* pDataLayerHeader)
	{
		KBytesOrder::ReverseBytes(&pDataLayerHeader->iLayerDivider);
		KBytesOrder::ReverseBytes(&pDataLayerHeader->lLengthOfDataLayer);
	}
	// PageHeader
	void KWsr98DataOperator::ReverseBytes(PageHeader* pPageHeader)
	{
		KBytesOrder::ReverseBytes(&pPageHeader->iCurrentPageNumber);
		KBytesOrder::ReverseBytes(&pPageHeader->iLengthOfPage);
	}
	// EachRadialHeader
	void KWsr98DataOperator::ReverseBytes(EachRadialHeader* pEachRadialHeader)
	{
		KBytesOrder::ReverseBytes(&pEachRadialHeader->iNumbersOfRLECodeInRadial);
		KBytesOrder::ReverseBytes(&pEachRadialHeader->iRadialStartAngle);
		KBytesOrder::ReverseBytes(&pEachRadialHeader->iDeltaAngle);
	}
	// EachRowHeader
	void KWsr98DataOperator::ReverseBytes(EachRowHeader* pEachRowHeader)
	{
		KBytesOrder::ReverseBytes(&pEachRowHeader->iNumberOfBytesInThisRow);
	}
	// DigitalRadialDataArrayPacketHeader
	void KWsr98DataOperator::ReverseBytes(DigitalRadialDataArrayPacketHeader* pDigitalRadialDataArrayPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iIndexOfFirstRangeBin);
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iNumberOfRangeBinsPerRadial);
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iICenterOfSweep);
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iJCenterOfSweep);
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iScaleFactor);
		KBytesOrder::ReverseBytes(&pDigitalRadialDataArrayPacketHeader->iNumberOfRadials);
	}
	// DigitalPrecipitationDataArrayPacketHeader
	void KWsr98DataOperator::ReverseBytes(DigitalPrecipitationDataArrayPacketHeader* pDigitalPrecipitationDataArrayPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pDigitalPrecipitationDataArrayPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pDigitalPrecipitationDataArrayPacketHeader->Spare[0]);
		KBytesOrder::ReverseBytes(&pDigitalPrecipitationDataArrayPacketHeader->Spare[1]);
		KBytesOrder::ReverseBytes(&pDigitalPrecipitationDataArrayPacketHeader->iNumberOfLFMBoxsInRow);
		KBytesOrder::ReverseBytes(&pDigitalPrecipitationDataArrayPacketHeader->iNumberOfRows);
	}
	// PrecipitationRateDataArrayPacketHeader
	void KWsr98DataOperator::ReverseBytes(PrecipitationRateDataArrayPacketHeader* pPrecipitationRateDataArrayPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pPrecipitationRateDataArrayPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pPrecipitationRateDataArrayPacketHeader->Spare[0]);
		KBytesOrder::ReverseBytes(&pPrecipitationRateDataArrayPacketHeader->Spare[1]);
		KBytesOrder::ReverseBytes(&pPrecipitationRateDataArrayPacketHeader->iNumberOfLFMBoxsInRow);
		KBytesOrder::ReverseBytes(&pPrecipitationRateDataArrayPacketHeader->iNumberOfRows);
	}
	// CellTrendDataPacketHeader
	void KWsr98DataOperator::ReverseBytes(CellTrendDataPacketHeader* pCellTrendDataPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pCellTrendDataPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pCellTrendDataPacketHeader->iLengthOfDataBlock);
		this->ReverseBytes(&pCellTrendDataPacketHeader->oPosition);
	}
	// SetColorLevelPacketHeader
	void KWsr98DataOperator::ReverseBytes(SetColorLevelPacketHeader* pSetColorLevelPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pSetColorLevelPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pSetColorLevelPacketHeader->iColorValueIndicator);
	}
	// LinkedContourVectorPacketHeader
	void KWsr98DataOperator::ReverseBytes(LinkedContourVectorPacketHeader* pLinkedContourVectorPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pLinkedContourVectorPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pLinkedContourVectorPacketHeader->iInitialPointIndicator);
	}
	// UnlinkedContourVectorPacketHeader
	void KWsr98DataOperator::ReverseBytes(UnlinkedContourVectorPacketHeader* pUnlinkedContourVectorPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pUnlinkedContourVectorPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pUnlinkedContourVectorPacketHeader->iLengthOfVector);
	}
	// RadialDataPacketHeader
	void KWsr98DataOperator::ReverseBytes(RadialDataPacketHeader* pRadialDataPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iPacketCode);
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iIndexOfFirstRangeBin);
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iNumberOfRangeBinsPerRadial);
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iICenterOfSweep);
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iJCenterOfSweep);
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iScaleFactor);
		KBytesOrder::ReverseBytes(&pRadialDataPacketHeader->iNumberOfRadials);
	}
	// RasterDataPacketHeader
	void KWsr98DataOperator::ReverseBytes(RasterDataPacketHeader* pRasterDataPacketHeader)
	{
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iPacketCode[0]);
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iPacketCode[1]);
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iPacketCode[2]);

		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iICoordinateStart);
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iJCoordinateStart);

		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iXScaleINT);
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iXScaleFractional);

		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iYScaleINT);
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iYScaleFractional);

		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iNumberOfRows);
		KBytesOrder::ReverseBytes(&pRasterDataPacketHeader->iPackDescriptor);
	}

	// WriteText
	void KWsr98DataOperator::ReverseBytes(WriteText* pWriteText)
	{
		KBytesOrder::ReverseBytes(&pWriteText->iColorLevel);
		this->ReverseBytes(&pWriteText->oPosition);
	}
	// Mesocyclone
	void KWsr98DataOperator::ReverseBytes(Mesocyclone* pMesocyclone)
	{
		this->ReverseBytes(&pMesocyclone->oPosition);
		KBytesOrder::ReverseBytes(&pMesocyclone->iRadiusOfMesocyclone);
	}
	// WindBarb
	void KWsr98DataOperator::ReverseBytes(WindBarb* pWindBarb)
	{
		KBytesOrder::ReverseBytes(&pWindBarb->iColorLevelOfWindBarb);
		KBytesOrder::ReverseBytes(&pWindBarb->iXCoordinate);
		KBytesOrder::ReverseBytes(&pWindBarb->iYCoordinate);
		KBytesOrder::ReverseBytes(&pWindBarb->iWindDirection);
		KBytesOrder::ReverseBytes(&pWindBarb->iWindSpeed);
	}
	// VectorArrow
	void KWsr98DataOperator::ReverseBytes(VectorArrow* pVectorArrow)
	{
		KBytesOrder::ReverseBytes(&pVectorArrow->iICoordinate);
		KBytesOrder::ReverseBytes(&pVectorArrow->iJCoordinate);
		KBytesOrder::ReverseBytes(&pVectorArrow->iDirectionOfArrow);
		KBytesOrder::ReverseBytes(&pVectorArrow->iLengthOfArrow);
		KBytesOrder::ReverseBytes(&pVectorArrow->iLengthOfArrowHead);
	}
	// LinkedVector
	void KWsr98DataOperator::ReverseBytes(LinkedVector* pLinkedVector)
	{
		KBytesOrder::ReverseBytes(&pLinkedVector->iColorLevel);
	}
	// UnlinkedVector
	void KWsr98DataOperator::ReverseBytes(UnlinkedVector* pUnlinkedVector)
	{
		KBytesOrder::ReverseBytes(&pUnlinkedVector->iColorLevel);
	}
	// HdaHail
	void KWsr98DataOperator::ReverseBytes(HdaHail* pHdaHail)
	{
		this->ReverseBytes(&pHdaHail->oPosition);
		KBytesOrder::ReverseBytes(&pHdaHail->iProbabilityOfHail);
		KBytesOrder::ReverseBytes(&pHdaHail->iProbabilityOfSevereHail);
		KBytesOrder::ReverseBytes(&pHdaHail->iMaxHailSize);
	}
	// StiCircle
	void KWsr98DataOperator::ReverseBytes(StiCircle* pStiCircle)
	{
		this->ReverseBytes(&pStiCircle->oPosition);
		KBytesOrder::ReverseBytes(&pStiCircle->iRadiusOfCircle);
	}
	///===============
	///字节转换 - end
	///========================================================

//---------------------------------------------------------
}// namespace Wsr98d - end
//---------------------------------------------------------
