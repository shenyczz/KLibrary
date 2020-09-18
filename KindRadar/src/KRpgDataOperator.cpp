#include "StdAfx.h"
#include "KRpgDataOperator.h"

#include "KRpgData.h"

namespace Wsr98d { namespace Rpg
{

#pragma region pragma_region_XXX
#pragma endregion

	KRpgDataOperator::KRpgDataOperator(void)
	{
		m_bScitPast = FALSE;		// ��ȥ
		m_bScitForecast = FALSE;	// Ԥ�� 
	}

	KRpgDataOperator::~KRpgDataOperator(void)
	{
	}

	///========================================================
	///��ȡ���� - beg
	///===============
	// װ������
	BOOL KRpgDataOperator::LoadData(LPCTSTR lpszFile,LPVOID lpvData)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		///װ�� 5 �����ݿ�

		//1. װ�� Message Head Block
		if(!loadMHB(lpszFile))
			return FALSE;
		//2. װ�� Product Description Block
		if(!loadPDB(lpszFile))
			return FALSE;

		PDB* pdb = pRpgData->PDBPtr();

		BOOL bSymbology = FALSE;
		BOOL bGraphic = FALSE;
		BOOL bTabular = FALSE;

		//3. װ�� Product Symbology Block
		if(pdb->lOffsetSymbology > 0)
		{
			bSymbology = loadPSB(lpszFile);
		}
		//4. װ�� Graphic Alphanumeric  Block
		if(pdb->lOffsetGraphic > 0) 
		{
			bGraphic = loadGAB(lpszFile);
		}
		//5. װ�� Tabular Alphanumeric  Block
		if(pdb->lOffsetTabular > 0) 
		{
			bTabular = loadTAB(lpszFile);
		}

		// �Ƿ�������
		BOOL bLoadData = bSymbology || bGraphic || bTabular;
		if(!bLoadData)
			return FALSE;

		// ���㡢���ò�����������
		int iYear,iMonth,iDay;
		this->JulianDate2GMTDate(pRpgData->PDBPtr()->iVScanDate,&iYear,&iMonth,&iDay);			// ת����������

		int iHour,iMinute,iSecond;
		this->JulianTime2GMTTime(pRpgData->PDBPtr()->lVScanStartTime,&iHour,&iMinute,&iSecond);	// ת������ʱ��

		// �������ݵ�����ʱ��
		pRpgData->SetDateTime(iYear,iMonth,iDay,iHour,iMinute,iSecond);

		// ����״���Ϣ
		pRpgData->FillRadarInfo(pRpgData->GetRadarInfo());

		// ���������Ϣ
		pRpgData->FillDataInfo(pRpgData->GetDataInfo());

		// ������Ϣͷ��MHB�Ͳ�Ʒ������PDB����ͼ��
		pRpgData->MakeLegendInfo(pRpgData->GetProductCode());

		// ���ɵ�ɫ��
		this->GeneratePalette();

		return TRUE;
	}

#pragma region װ��5�����ݿ� - load_XXB
	///--------------------------------------------------------
	///װ��5�����ݿ� - beg
	// 1. װ�� Message Head Block
	BOOL KRpgDataOperator::loadMHB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		MHB* pMhb = pRpgData->MHBPtr();

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		fseek(fp,0,SEEK_SET);
		fread(pMhb,sizeof(MHB),1,fp);

		fclose(fp);

		this->ReverseBytes(pMhb);	// �ֽ�ת��

		return (pMhb->iMsgCode >= RPG_DATA_PRODUCT_CODE_MIN && pMhb->iMsgCode <= RPG_DATA_PRODUCT_CODE_MAX);
	}
	// 2. װ�� Product Description  Block
	BOOL KRpgDataOperator::loadPDB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		PDB* pPdb = pRpgData->PDBPtr();

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		fseek(fp,sizeof(MHB),SEEK_SET);
		fread(pPdb,sizeof(PDB),1,fp);

		fclose(fp);

		this->ReverseBytes(pPdb);	// �ֽ�ת��

		return (pPdb->iBlockDivider == RPG_DATA_BLOCK_DIVIDER);
	}
	// 3. װ�� Product Symbology Block  
	BOOL KRpgDataOperator::loadPSB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		long lOffset = pRpgData->PSBOffset();	// ȡ�ñ����ݿ�ƫ�Ƶ�ַ
		if(lOffset<=0) return FALSE;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// �Ҳ��� Symbology ����ʼ��ַ(ע�� *2)
		if(fseek(fp,lOffset*2,SEEK_SET) != 0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ȡ PSB
		PSB* psb = pRpgData->PSBPtr();
		if(fread(psb,sizeof(PSB),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		this->ReverseBytes(psb);	// �ֽ�ת��

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		// 62,82�ŵ�PSB�Ƚ�����
		if(iProductCode == 62 || iProductCode == 82)
		{
			// �ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(PSB),SEEK_CUR);
		}
		// �Ƿ� PSB
		else if(  psb->iBlockDivider != RPG_DATA_BLOCK_DIVIDER	// ��ָ��(-1)
			   || psb->iBlockID != RPG_DATA_PSB_BLOCK_ID)		// ��ID(1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		//�����ļ�ָ�뵱ǰƫ��
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		//-------------------------------------------------------------------------
		// PSB�������һ����8��
		// ����ͼ���������͵�����Ӧ�Ķ�ȡ���ܺ���
		//-------------------------------------------------------------------------

		BOOL bLoadPsb = FALSE;
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());

		switch(eProductType)
		{
		case ProductType_RadialImage:					// 1
			bLoadPsb = psb_LoadRadialImageData(lpszFile,lOffsetCur);
			break;

		case ProductType_RasterImage:					// 2
			bLoadPsb = psb_LoadRasterImageData(lpszFile,lOffsetCur);
			break;

		case ProductType_LinkedContourVectorImage:	// 3
			bLoadPsb = psb_LoadLinkedContourVectorImageData(lpszFile,lOffsetCur);
			break;

		case ProductType_GeographicAlphaImage:		// 4
			bLoadPsb = psb_LoadGeographicAlphanumericImageData(lpszFile,lOffsetCur);
			break;

		case ProductType_NonGeographicAlphaImage:	// 5
			bLoadPsb = psb_LoadNonGeographicAlphanumericImageData(lpszFile,lOffsetCur);
			break;

		case ProductType_AlphanumericImage:			// 6
			bLoadPsb = psb_LoadAlphanumericImageData(lpszFile,lOffsetCur);
			break;

		case ProductType_RasterImageSection:			// 7
			bLoadPsb = psb_LoadRasterImageData_Section(lpszFile,lOffsetCur);
			break;

		case ProductType_RasterImageSlice:			// 8
			bLoadPsb = psb_LoadRasterImageData_Slice(lpszFile,lOffsetCur);
			break;
		}

		return bLoadPsb;
	}
	// 4. װ�� Graphic Alphanumeric  Block
	BOOL KRpgDataOperator::loadGAB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		long lOffset = pRpgData->GABOffset();	// ȡ�ñ����ݿ�ƫ�Ƶ�ַ
		if(lOffset<=0) return FALSE;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// �Ҳ��� Graphic ����ʼ��ַ(ע�� *2)
		if(fseek(fp,lOffset*2,SEEK_SET) != 0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ȡ GAB
		GAB* gab = pRpgData->GABPtr();
		if(fread(gab,sizeof(GAB),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		this->ReverseBytes(gab);	// �ֽ�ת��

		if(  gab->iBlockDivider != RPG_DATA_BLOCK_DIVIDER	// ��ָ��(-1)
		  || gab->iBlockID != RPG_DATA_GAB_BLOCK_ID			// ��ID(=2)
			)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		//�����ļ�ָ�뵱ǰƫ��
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		//-------------------------------------------------------------------------
		// GAB�������һ����1��
		// ����ͼ���������͵�����Ӧ�Ķ�ȡ���ܺ���
		//-------------------------------------------------------------------------
		BOOL bLoadGab = FALSE;
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());

		bLoadGab = gab_LoadWriteTextData(lpszFile,lOffsetCur);

		return bLoadGab;
	}
	// 5. װ�� Tabular Alphanumeric  Block
	BOOL KRpgDataOperator::loadTAB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		long lOffset = pRpgData->TABOffset();	// ȡ�ñ����ݿ�ƫ�Ƶ�ַ
		if(lOffset<=0) return FALSE;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// �Ҳ��� Tabular ����ʼ��ַ(ע�� *2)
		if(fseek(fp,lOffset*2,SEEK_SET) != 0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ȡ TAB
		TAB* tab = pRpgData->TABPtr();
		if(fread(tab,sizeof(TAB),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		this->ReverseBytes(tab);	// �ֽ�ת��

		if(  tab->iBlockDivider != RPG_DATA_BLOCK_DIVIDER	// ��ָ��(-1)
		  || tab->iBlockID != RPG_DATA_TAB_BLOCK_ID			// ��ID(=3)
			)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		//�����ļ�ָ�뵱ǰƫ��
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		//-------------------------------------------------------------------------
		// TAB�������һ����1��
		// ����ͼ���������͵�����Ӧ�Ķ�ȡ���ܺ���
		//-------------------------------------------------------------------------
		BOOL bLoadTab = FALSE;
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());

		bLoadTab = tab_LoadWriteTextData(lpszFile,lOffsetCur);

		return bLoadTab;
	}
	///װ��5�����ݿ� - end
	///--------------------------------------------------------
#pragma endregion

#pragma region װ�� PSB ���ݿ��8������ - psb_Load_xxxxx_Data
	///--------------------------------------------------------
	///װ�� PSB ���ݿ��8������  - beg
	//3.1 װ�� Radial Image Data
	BOOL KRpgDataOperator::psb_LoadRadialImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		//�״��Ʒ��  Radial Iamge Ŀǰֻ��һ��
		if( !(iTotalLayers==1) ) throw;

		// 2. ѭ����ȡ���в�����
		// ��������ͼ������ָ��
		KRadialList* pRadialList = pRpgData->GetRadialList();
		pRadialList->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// �����ݲ�ͷ�ṹ
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{// ��ָ������� 
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ������ͼ�����ݰ�����
			RadialDataPacketHeader* pRadialDataPacketHeader = pRpgData->GetRadialDataPacketHeader();
			fread(pRadialDataPacketHeader,sizeof(RadialDataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(RadialDataPacketHeader),SEEK_CUR);	// �ع��ļ�ָ��

			this->ReverseBytes(pRadialDataPacketHeader);

			if(pRadialDataPacketHeader->iPacketCode != const_PacketCode_Radial_Data)
			{// ���ݰ��������
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ���浱ǰָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// ����ɨ��ƽ������о������ݵ������������� pRadialList
			if(!ReadRadialImageData(pRadialList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.2 װ�� Raster Image Data
	BOOL KRpgDataOperator::psb_LoadRasterImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		// �״��Ʒ��  Raster Iamge Normal Ŀǰֻ��һ��
		if( !(iTotalLayers==1) ) throw;

		// 2. ��ȡ����
		// ��������ͼ������ָ��
		KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
		pRasterRowList->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// �����ݲ�ͷ�ṹ
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{// ��ָ������� 
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ��ȡդ��ͼ�����ݰ�ͷ
			RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();
			fread(pRasterDataPacketHeader,sizeof(RasterDataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(RasterDataPacketHeader),SEEK_CUR);

			this->ReverseBytes(pRasterDataPacketHeader);

			// ���ݰ��������
			if(   pRasterDataPacketHeader->iPacketCode[0] != const_PacketCode_Raster_Data_Normal
			   || pRasterDataPacketHeader->iPacketCode[1] != const_Raster_Data_Normal_Operate_Flag_1
			   || pRasterDataPacketHeader->iPacketCode[2] != const_Raster_Data_Normal_Operate_Flag_2
			   || pRasterDataPacketHeader->iPackDescriptor != const_Raster_Data_Pack_Descriptor)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ���浱ǰָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// ����դ��ͼ�����ݵ��������� pRasterRowList
			if(!ReadRasterImageData(pRasterRowList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.3 װ�� Linked Contour Vector Image Data
	BOOL KRpgDataOperator::psb_LoadLinkedContourVectorImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		// �״��Ʒ��  Linked Contour Vector Image Data Ŀǰֻ��һ��
		Assert(iTotalLayers==1);

		// 2. ��ȡ����
		KLinkedContourVectorList* pLinkedContourVectorList = pRpgData->GetLinkedContourVectorList();
		pLinkedContourVectorList->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// �����ݲ���Ϣͷ
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ���浱ǰָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// �����ֵ�����ݵ���������
			if(!ReadLinkedContourVectorData(pLinkedContourVectorList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.4 װ�� Geographic Alphanumeric Image Data
	BOOL KRpgDataOperator::psb_LoadGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ� 
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		// �״��Ʒ��  Geographic Alphanumeric Image Data Ŀǰֻ��һ��
		Assert(iTotalLayers==1);

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		// 2. ��ȡ����
		pRpgData->GetColorTextList()->Clear();
		pRpgData->GetSpecilaSymbolList()->Clear();
		pRpgData->GetStormIdList()->Clear();
		pRpgData->GetScitPastList()->Clear();
		pRpgData->GetScitForecastList()->Clear();
		pRpgData->GetHdaHailList()->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// �����ݲ���Ϣͷ
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ���浱ǰָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			switch(iProductCode)
			{
			case 47:		// SWP
			{
				if(!ReadColorTextData(pRpgData->GetColorTextList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			break;

			case 58:		// STI
			{
				BOOL bRead = FALSE;
				while(1)
				{
					// ��ȡ����ͷ���ع�ָ��
					DataPacketHeader tDataPacketHeadre;
					fread(&tDataPacketHeadre,sizeof(DataPacketHeader),1,fp);
					fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
					this->ReverseBytes(&tDataPacketHeadre);

					// �鿴�Ƿ� PacketCode = 2
					// ÿ��籩·����Ϣ������ SpecialSymbolPacket ��ʼ
					if(tDataPacketHeadre.iPacketCode != const_PacketCode_Special_Symbol)
					{
						// �ɹ���ȡ������
						if(bRead)
						{
							fclose(fp);
							fp = NULL;
							return TRUE;
						}
						else
						{// �ɹ���ȡ������
							fclose(fp); 
							fp = NULL;
							return FALSE; 
						}
					}

					// Code = 2 ����������� 
					BOOL b2 = ReadSpecilaSymbolData(pRpgData->GetSpecilaSymbolList());

					// Code = 15 �籩ID����
					BOOL b15 = ReadStormIdData(pRpgData->GetStormIdList());

					// Code = 23 �籩����·����Ϣ��ȥ����
					BOOL b23 = ReadScitPastData(pRpgData->GetScitPastList());

					// Code  = 24 �籩����·����ϢԤ������
					BOOL b24 = ReadScitForecastData(pRpgData->GetScitForecastList());

					// �Ƿ������ɹ�����������
					bRead = b2 || b15 || b23 || b24;
				}
			}
			break;

			case 59:		// HI
			{
				BOOL bRead = FALSE;
				while(1)
				{
					// ��ȡ����ͷ���ع�ָ��
					DataPacketHeader tDataPacketHeadre;
					fread(&tDataPacketHeadre,sizeof(DataPacketHeader),1,fp);
					fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
					this->ReverseBytes(&tDataPacketHeadre);

					if(tDataPacketHeadre.iPacketCode != const_PacketCode_Hda_Hail)
					{
						// �ɹ���ȡ������
						if(bRead)
						{
							fclose(fp);
							fp = NULL;
							return TRUE;
						}
						else
						{// �ɹ���ȡ������
							fclose(fp); 
							fp = NULL;
							return FALSE; 
						}
					}

					// Code = 19 THdaHail
					BOOL b19 = ReadHdaHailData(pRpgData->GetHdaHailList());

					// Code = 15 �籩ID����
					BOOL b15 = ReadStormIdData(pRpgData->GetStormIdList());

					// �Ƿ������ɹ�����������
					bRead = b19 || b15;
				}
			}
			break;

			case 61:		// TVS
			{
				// wait...
				fclose(fp); 
				fp = NULL;
				return FALSE; 
			}
			break;

			}// switch(iProductCode)

		}// for(i)

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.5 װ�� Non Geographic Alphanumeric Image Data
	// 48 + 84
	BOOL KRpgDataOperator::psb_LoadNonGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		// �״��Ʒ��  Geographic Alphanumeric Image Data Ŀǰֻ��һ��
		Assert(iTotalLayers==1);

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		// 2. ��ȡ����
		pRpgData->GetColorUnlinkedVectorList()->Clear();
		pRpgData->GetColorTextList()->Clear();
		pRpgData->GetWindBarbList()->Clear();
		pRpgData->GetColorLinkedVectorList()->Clear();
		pRpgData->GetColorUnlinkedVectorList()->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// �����ݲ���Ϣͷ
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// ���浱ǰָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// 2.1 ��ȡֱ�������߶�
			DataPacketHeader tDataPacketHeader;
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Color_Unlinked_Vector)
			{
				if(!ReadColorUnLinkedVectorData(pRpgData->GetColorUnlinkedVectorList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.2 ��ȡ��ע�ı�
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Color_Text)
			{
				if(!ReadColorTextData(pRpgData->GetColorTextList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.3 ��ȡͼ������
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			// 2.3.1 Wind Barb ͼ������
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Wind_Barb)
			{
				if(!ReadWindBarbData(pRpgData->GetWindBarbList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.3.2 Color UnLinked Vector ͼ������
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Color_Unlinked_Vector)
			{
				if(!ReadColorUnLinkedVectorData(pRpgData->GetColorUnlinkedVectorList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.3.3 Color Linked Vector ͼ������
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Color_Linked_Vector)
			{
				if(!ReadColorLinkedVectorData(pRpgData->GetColorLinkedVectorList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.4 ��ȡ��ע�ı�
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Color_Text)
			{
				if(!ReadColorTextData(pRpgData->GetColorTextList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.5 ������ʸ������(48��,84��)
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Color_Unlinked_Vector)
			{
				if(!ReadColorUnLinkedVectorData(pRpgData->GetColorUnlinkedVectorList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// ����
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode != const_PacketCode_Color_Unlinked_Vector)
			{
				//int xxx = 0;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.6 װ�� Alphanumeric Image Data
	// 62 + 82
	BOOL KRpgDataOperator::psb_LoadAlphanumericImageData(LPCTSTR lpszFile,long lOffset)
	{
		return ReadPSBWriteTextData(lpszFile,lOffset);
	}
	//3.7 װ�� Raster Image Data (Section)
	BOOL KRpgDataOperator::psb_LoadRasterImageData_Section(LPCTSTR lpszFile,long lOffset)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		// �״��Ʒ��  Raster Iamge Section ��2��
		Assert(iTotalLayers==2);

		//
		// 2. ��ȡ����
		//
		// 2.1 ��ȡ����ͼ���ݲ�
		DataLayerHeader tDataLayerHeader;
		fread(&tDataLayerHeader,1,sizeof(DataLayerHeader),fp);
		this->ReverseBytes(&tDataLayerHeader);

		if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ȡդ��ͼ�����ݰ�ͷ
		RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();
		fread(pRasterDataPacketHeader,sizeof(RasterDataPacketHeader),1,fp);
		fseek(fp,-1*(int)sizeof(RasterDataPacketHeader),SEEK_CUR);

		this->ReverseBytes(pRasterDataPacketHeader);

		// ���ݰ��������
		if(   pRasterDataPacketHeader->iPacketCode[0] != const_PacketCode_Raster_Data_Section
		   || pRasterDataPacketHeader->iPacketCode[1] != const_Raster_Data_Section_Operate_Flag_1
		   || pRasterDataPacketHeader->iPacketCode[2] != const_Raster_Data_Section_Operate_Flag_2
		   || pRasterDataPacketHeader->iPackDescriptor != const_Raster_Data_Pack_Descriptor)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ���浱ǰָ��ƫ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// դ��ͼ�����ݵ���������ָ��
		KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
		pRasterRowList->Clear();

		// ����դ��ͼ�����ݵ��������� pRasterRowList
		if(!ReadRasterImageData(pRasterRowList))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 2.2 ����ͱ�ע��
		DataLayerHeader tDataLayerHeader2;
		fread(&tDataLayerHeader2,1,sizeof(DataLayerHeader),fp);
		this->ReverseBytes(&tDataLayerHeader2);

		if(tDataLayerHeader2.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// 2.2.1 ������������
		//��ȡ��ɫ������ʸ�����ݰ�ͷ
		MonoUnlinkedVectorPacketHeader tMonoUnlinkedVectorPacketHeader;
		fread(&tMonoUnlinkedVectorPacketHeader,sizeof(MonoUnlinkedVectorPacketHeader),1,fp);
		fseek(fp,-1*(int)sizeof(MonoUnlinkedVectorPacketHeader),SEEK_CUR);

		this->ReverseBytes(&tMonoUnlinkedVectorPacketHeader);

		if(tMonoUnlinkedVectorPacketHeader.iPacketCode != const_PacketCode_Mono_Unlinked_Vector)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ���浱ǰָ��ƫ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// �������ݵ�����ָ��
		KMonoUnlinkedVectorList* pMonoUnlinkedVectorList = pRpgData->GetMonoUnlinkedVectorList();
		pMonoUnlinkedVectorList->Clear();

		// �����������ݵ�����
		if(!ReadMonoUnLinkedVectorData(pMonoUnlinkedVectorList))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// 2.2.2 �����ע����
		//��ȡ��ɫ�ı����ݰ�ͷ
		MonoTextPacketHeader tMonoTextPacketHeader;
		fread(&tMonoTextPacketHeader,sizeof(MonoTextPacketHeader),1,fp);
		fseek(fp,-1*(int)sizeof(MonoTextPacketHeader),SEEK_CUR);

		this->ReverseBytes(&tMonoTextPacketHeader);

		if(tMonoTextPacketHeader.iPacketCode != const_PacketCode_Mono_Text)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ���浱ǰָ��ƫ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// ��ע��������ָ��
		KMonoTextList* pMonoTextList = pRpgData->GetMonoTextList();
		pMonoTextList->Clear();

		// �����ע�������� m_MonoTextList
		if(!ReadMonoTextData(pMonoTextList))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.8 װ�� Raster Image Data (Slice)
	BOOL KRpgDataOperator::psb_LoadRasterImageData_Slice(LPCTSTR lpszFile,long lOffset)
	{
		int i,j;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		PSB* psb = pRpgData->PSBPtr();

		// ���ݲ���
		int iTotalLayers = psb->iTotalLayers;

		// �״��Ʒ��  Raster Iamge Slice �ж��
		Assert(iTotalLayers>0);

		//
		// 2. ��ȡ����
		//
		for(i=0;i<20;i++)
		{
			pRpgData->GetRasterRowList_Slice(i)->Clear();
			pRpgData->GetMonoUnLinkedVectorList_Slice(i)->Clear();
			pRpgData->GetMonoTextList_Slice(i)->Clear();
		}
		// ���ݲ���������Ƭͼ�����Ͳ�������ʸ����
		// ���һ�������ǲ�������ʸ����͸���Ƭͼ˵���ı�
		//
		// 2.1 ��ȡ�����Ƭͼ
		for(i=0;i<iTotalLayers-1;i++)
		{
			// �����ݲ�ͷ
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);

			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// �����ļ�ָ����ļ�ָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// ��ȡ��Ƭͼ
			if(!ReadRasterImageData(pRpgData->GetRasterRowList_Slice(i)))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}
		// 2.2 ���һ������
		// �����ݲ�ͷ
		DataLayerHeader tDataLayerHeader;
		fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);

		this->ReverseBytes(&tDataLayerHeader);

		if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// �����ļ�ָ����ļ�ָ��ƫ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// ��ȡ������ʸ���ͱ�ע�ı�����
		for(j=0;j<iTotalLayers-1;j++)
		{
			//2.2.1 ������������(��ɫ������ʸ��)
			if(!ReadMonoUnLinkedVectorData(pRpgData->GetMonoUnLinkedVectorList_Slice(j)))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
			//2.2.2 �����ע����(��ɫ�ı�����)
			if(!ReadMonoTextData(pRpgData->GetMonoTextList_Slice(j)))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	///װ�� PSB ���ݿ��8������  - end
	///--------------------------------------------------------
#pragma endregion

	///--------------------------------------------------------
	///װ�� GAB ���ݿ��ı����� - beg
	// 4.1 
	BOOL KRpgDataOperator::gab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		return ReadGABWriteTextData(lpszFile,lOffset);
	}
	///װ�� GAB ���ݿ��ı����� - end
	///--------------------------------------------------------

	///--------------------------------------------------------
	///װ�� TAB ���ݿ��ı����� - beg
	// 5.1 
	BOOL KRpgDataOperator::tab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		return ReadTABWriteTextData(lpszFile,lOffset);
	}
	///װ�� TAB ���ݿ��ı����� - end
	///--------------------------------------------------------

	///--------------------------------------------------------
	///�������� - Beg
	BOOL KRpgDataOperator::ReadPSBWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// ��ȡ����
		sfx_int16 iBlockID = 0;
		sfx_int16 iNumberOfPages = 0;

		fread(&iBlockID,sizeof(sfx_int16),1,fp);
		fread(&iNumberOfPages,sizeof(sfx_int16),1,fp);

		KBytesOrder::ReverseBytes(&iBlockID);
		KBytesOrder::ReverseBytes(&iNumberOfPages);

		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		pRpgData->GetPSBWriteTextList()->Clear();

		for(i=0;i<iNumberOfPages;i++)
		{
			if(!ReadTABWriteTextPage(pRpgData->GetPSBWriteTextList()))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	BOOL KRpgDataOperator::ReadGABWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 2. ��ȡ���� 
		int iNumberOfPages = pRpgData->GABPtr()->iNumberOfPages;

		pRpgData->GetGABWriteTextList()->Clear();
		pRpgData->GetGABUnlinkedVectorList()->Clear();

		//------------------------------------------------
		//���� TextPacket(1) - TextPacket(n) 
		//TextPacket �� Code 8 �� Code 10 ���
		//------------------------------------------------
		for(i=0;i<iNumberOfPages;i++)
		{
			//����ÿҳҳͷ
			PageHeader tPageHeader;
			fread(&tPageHeader,sizeof(PageHeader),1,fp);
			this->ReverseBytes(&tPageHeader);

			int iCurPageNumber		= tPageHeader.iCurrentPageNumber;	//��ǰҳ��
			int iLengthOfPage		= tPageHeader.iLengthOfPage;		//�� TextPacket(1) - TextPacket(n) �ܵ��ֽ���

			// �����ļ�ָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			//���� TextPacket
			//Code 8
			if(!ReadColorTextData(pRpgData->GetGABWriteTextList()))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
			else
			{// ���һ����
				ColorText* pColorText = new ColorText();
				memset(pColorText,-1,sizeof(ColorText));
				pColorText->szText[0] = '\0';
				pRpgData->GetGABWriteTextList()->push_back(pColorText);
			}

			//Code 10
			// ����û�� Code 10
			if(!ReadColorUnLinkedVectorData(pRpgData->GetGABUnlinkedVectorList()))
			{
				//fclose(fp);
				//fp = NULL;
				//return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	BOOL KRpgDataOperator::ReadTABWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// ��λƫ��
		fseek(fp,lOffset,SEEK_SET);

		// �����ļ�ָ����ļ�ָ��ƫ��
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB ָ��
		TAB* tab = pRpgData->TABPtr();

		// ��ҳ��
		int iNumberOfPages = tab->iNumberOfPages;

		// �״��Ʒ��  Raster Iamge Slice �ж��
		Assert(iNumberOfPages>0);

		//
		// 2. ��ȡ����
		//
		KWriteTextList* pTABWriteTextList = pRpgData->GetTABWriteTextList();
		pTABWriteTextList->Clear();

		for(i=0;i<iNumberOfPages;i++)
		{
			if(!ReadTABWriteTextPage(pTABWriteTextList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	///�������� - End
	///--------------------------------------------------------

#pragma region װ�ظ��ָ�ʽ���� - Read_xxxxxxx_Data
	///--------------------------------------------------------
	///װ�ظ��ָ�ʽ���� - beg
	//FMT.01 ��ȡ��ɫ�ı�����
	//     Mono Text Data
	//     Packet Code = 1
	BOOL KRpgDataOperator::ReadMonoTextData(KMonoTextList* pMonoTextList)
	{
		// ��ȡ����
		BOOL bExistData = FALSE;
		while(1)
		{
			// ���������ڴ�
			MonoText* pMonoText = new MonoText;

			if(ReadMonoTextDataPacket(pMonoText))
			{
				bExistData = TRUE;
				pMonoTextList->push_back(pMonoText);
			}
			else
			{
				_delete(pMonoText);
				break;
			}
		}

		return bExistData;
	}

	//FMT.02 ��ȡ�����������
	//     Special Symbol Data
	//     Packet Code = 2
	BOOL KRpgDataOperator::ReadSpecilaSymbolData(KSpecilaSymbolList* pSpecilaSymbolList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			// ���������ڴ�
			SpecilaSymbol* pSpecilaSymbol = new SpecilaSymbol;

			if(ReadSpecilaSymbolDataPacket(pSpecilaSymbol))
			{
				bExistData = TRUE;
				pSpecilaSymbolList->push_back(pSpecilaSymbol);
			}
			else
			{
				_delete(pSpecilaSymbol);
				break;
			}
		}

		return bExistData;
	}

	//FMT.04 ��ȡ�繳����
	//     Wind Barb Data Data
	//     Packet Code = 4
	BOOL KRpgDataOperator::ReadWindBarbData(KWindBarbList* pWindBarbList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			WindBarb* pWindBarb = new WindBarb;

			if(ReadWindBarbDataPacket(pWindBarb))
			{
				bExistData = TRUE;
				pWindBarbList->push_back(pWindBarb);
			}
			else
			{
				_delete(pWindBarb);
				break;
			}
		}

		return bExistData;
	}

	//FMT.06 ��ȡ��ɫ����ʸ������
	//     Linked Vector Data
	//     Packet Code = 6
	BOOL KRpgDataOperator::ReadMonoLinkedVectorData(KMonoLinkedVectorList* pMonoLinkedVectorList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			MonoLinkedVector* pMonoLinkedVector = new MonoLinkedVector;

			if(ReadMonoLinkedVectorDataPacket(pMonoLinkedVector))
			{
				bExistData = TRUE;
				pMonoLinkedVectorList->push_back(pMonoLinkedVector);
			}
			else
			{
				_delete(pMonoLinkedVector);
				break;
			}
		}

		return bExistData;
	}

	//FMT.07 ��ȡ��ɫ������ʸ������(һ���߶�)
	//     Unlinked Vector Data
	//     Packet Code = 7
	BOOL KRpgDataOperator::ReadMonoUnLinkedVectorData(KMonoUnlinkedVectorList* pMonoUnlinkedVectorList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			MonoUnlinkedVector* pMonoUnlinkedVector = new MonoUnlinkedVector;

			if(ReadMonoUnlinkedVectorDataPacket(pMonoUnlinkedVector))
			{
				bExistData = TRUE;
				pMonoUnlinkedVectorList->push_back(pMonoUnlinkedVector);
			}
			else
			{
				_delete(pMonoUnlinkedVector);
				break;
			}
		}

		return bExistData;
	}

	//FMT.08 ��ȡ��ɫ�ı�����
	//     Color Text Data Data
	//     Packet Code = 8
	BOOL KRpgDataOperator::ReadColorTextData(KColorTextList* pColorTextList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			ColorText* pColorText = new ColorText;

			if(ReadColorTextDataPacket(pColorText))
			{
				bExistData = TRUE;
				pColorTextList->push_back(pColorText);
			}
			else
			{
				_delete(pColorText);
				break;
			}
		}

		return bExistData;
	}

	//FMT.09 ��ȡ��ɫ����ʸ������
	//     Linked Vector Data
	//     Packet Code = 9
	BOOL KRpgDataOperator::ReadColorLinkedVectorData(KColorLinkedVectorList* pColorLinkedVectorList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			ColorLinkedVector* pColorLinkedVector = new ColorLinkedVector;

			if(ReadColorLinkedVectorDataPacket(pColorLinkedVector))
			{
				bExistData = TRUE;
				pColorLinkedVectorList->push_back(pColorLinkedVector);
			}
			else
			{
				_delete(pColorLinkedVector);
				break;
			}
		}

		return bExistData;
	}

	//FMT.10 ��ȡ��ɫ������ʸ������
	//     Unlinked Vector Data
	//     Packet Code=10
	BOOL KRpgDataOperator::ReadColorUnLinkedVectorData(KColorUnlinkedVectorList* pColorUnlinkedVectorList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			ColorUnlinkedVector* pColorUnlinkedVector = new ColorUnlinkedVector;

			if(ReadColorUnlinkedVectorDataPacket(pColorUnlinkedVector))
			{
				bExistData = TRUE;
				pColorUnlinkedVectorList->push_back(pColorUnlinkedVector);
			}
			else
			{
				_delete(pColorUnlinkedVector);
				break;
			}
		}

		return bExistData;
	}

	//FMT.15 ��ȡ�籩ID����
	//     Storm Id Data
	//     Packet Code = 15
	BOOL KRpgDataOperator::ReadStormIdData(KStormIdList* pStormIdList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			StormId* pStormId = new StormId;

			if(ReadStormIdDataPacket(pStormId))
			{
				bExistData = TRUE;
				pStormIdList->push_back(pStormId);
			}
			else
			{
				_delete(pStormId);
				break;
			}
		}

		return bExistData;
	}

	//FMT.19 ��ȡ������������
	//     HDA Hail Data
	//     Packet Code = 19
	BOOL KRpgDataOperator::ReadHdaHailData(KHdaHailList* pHdaHailList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			HdaHail* pHdaHail = new HdaHail;

			if(ReadHdaHailDataPacket(pHdaHail))
			{
				bExistData = TRUE;
				pHdaHailList->push_back(pHdaHail);
			}
			else
			{
				_delete(pHdaHail);
				break;
			}
		}

		return bExistData;
	}

	//FMT.23 ��ȡ�籩����·����Ϣ��ȥ����
	//     SCIT Past Data
	//     Packet Code = 23
	BOOL KRpgDataOperator::ReadScitPastData(KScitPastList* pScitPastList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			ScitPast* pScitPast = new ScitPast;

			if(ReadScitPastDataPacket(pScitPast))
			{
				bExistData = TRUE;
				pScitPastList->push_back(pScitPast);
			}
			else
			{
				_delete(pScitPast);
				break;
			}
		}

		return bExistData;
	}

	//FMT.24 ��ȡ�籩����·����ϢԤ������
	//     SCIT Forecast Data
	//     Packet Code = 24
	BOOL KRpgDataOperator::ReadScitForecastData(KScitForecastList* pScitForecastList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			ScitForecast* pScitForecast = new ScitForecast;

			if(ReadScitForecastDataPacket(pScitForecast))
			{
				bExistData = TRUE;
				pScitForecastList->push_back(pScitForecast);
			}
			else
			{
				_delete(pScitForecast);
				break;
			}
		}

		return bExistData;
	}

	//FMT.25 ��ȡ�籩·����ϢȦ����
	//     STI Circle Data
	//     Packet Code = 25
	BOOL KRpgDataOperator::ReadStiCircleData(KStiCircleList* pStiCircleList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			StiCircle* pStiCircle = new StiCircle;

			if(ReadStiCircleDataPacket(pStiCircle))
			{
				bExistData = TRUE;
				pStiCircleList->push_back(pStiCircle);
			}
			else
			{
				_delete(pStiCircle);
				break;
			}
		}

		return bExistData;
	}

	//FMT.x0E03 ��ȡ���ӵ�ֵ��ʸ������
	//        Linked Contour Vectors Data
	//        Packet Code = 0x0E03
	BOOL KRpgDataOperator::ReadLinkedContourVectorData(KLinkedContourVectorList* pLinkedContourVectorList)
	{
		// ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);


		BOOL bExistData = FALSE;

		// ѭ����ȡ�����ֵ������
		while(1)
		{
			// ����ֵ����ɫ��(ÿһ���ֵ����һ�� TSetColorLevelPacket)
			// ��ֵ����ɫ��ͷ
			SetColorLevelPacketHeader tSetColorLevelPacketHeader;
			fread(&tSetColorLevelPacketHeader,1,sizeof(SetColorLevelPacketHeader),fp);

			this->ReverseBytes(&tSetColorLevelPacketHeader);	// �ֽ�ת��

			// �����ļ�ָ��ƫ��
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			if(tSetColorLevelPacketHeader.iPacketCode != const_PacketCode_Set_Color_Level)
			{
				// ������������Ҫ�ع��ļ�ָ��
				fseek(fp,-1*(int)sizeof(SetColorLevelPacketHeader),SEEK_CUR);
				lOffsetCur = ftell(fp);
				SetFilePtrOffset(lOffsetCur);

				return bExistData;
			}

			//��ֵ����ɫ����
			sfx_int16 iColorLevelOfContour = RPG_DATA_NO_COLOR_LEVEL;

			if(tSetColorLevelPacketHeader.iColorValueIndicator == const_Color_Level_Present_Indicator)
			{// ���д�����ɫ����
				fread(&iColorLevelOfContour,sizeof(sfx_int16),1,fp);
				KBytesOrder::ReverseBytes(&iColorLevelOfContour);	// �ֽ�ת��

				// �����ļ�ָ��ƫ��
				lOffsetCur = ftell(fp);
				SetFilePtrOffset(lOffsetCur);
			}

			// ��ȡһ���ֵ��
			while(2)
			{
				LinkedContourVector* pLinkedContourVector = new LinkedContourVector;
				pLinkedContourVector->iColorLevel = iColorLevelOfContour;	// �����ֵ����ɫ����

				if(ReadLinkedContourVectorDataPacket(pLinkedContourVector))
				{
					bExistData = TRUE;
					pLinkedContourVectorList->push_back(pLinkedContourVector);
				}
				else
				{
					_delete(pLinkedContourVector);
					break;
				}
			}// while(2)

		}// while(1)

		return bExistData;
	}

	//FMT.xAF1F ��ȡ��������ͼ������
	//        Radial Data Data
	//        Packet Code = 0xAF1F
	BOOL KRpgDataOperator::ReadRadialImageData(KRadialList* pRadialList)
	{
		int i;

		BOOL bExistData = FALSE;

		// ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// ������ͼ�����ݰ�����
		RadialDataPacketHeader tRadialDataPacketHeader;
		fread(&tRadialDataPacketHeader,sizeof(RadialDataPacketHeader),1,fp);
		this->ReverseBytes(&tRadialDataPacketHeader);

		// �����ļ�ָ��ƫ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		if(tRadialDataPacketHeader.iPacketCode != const_PacketCode_Radial_Data)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(RadialDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			return FALSE;
		}

		// ������������
		int iNumberOfRadials = tRadialDataPacketHeader.iNumberOfRadials;

		for(i=0;i<iNumberOfRadials;i++)
		{
			//Ϊһ���������߷����ڴ�
			Radial* pRadial = new Radial;

			// ��ȡһ���������ݰ�
			if(ReadRadialDataPacket(pRadial))
			{
				bExistData = TRUE;
				pRadialList->push_back(pRadial);	// ���һ����������
			}
			else
			{
				_delete(pRadial);
				break;
			}
		}

		return bExistData;
	}

	//FMT.xBA07 ��ȡդ������ͼ������
	//        Raster Data Data
	//        Packet Code = 0xBA07
	//FMT.xBA0F ��ȡդ������ͼ������
	//        Raster Data Data
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterImageData(KRasterRowList* pRasterRowList)
	{
		int i;

		BOOL bExistData = FALSE;

		// ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// ��ȡդ��ͼ�����ݰ�ͷ
		RasterDataPacketHeader tRasterDataPacketHeader;
		fread(&tRasterDataPacketHeader,sizeof(RasterDataPacketHeader),1,fp);
		this->ReverseBytes(&tRasterDataPacketHeader);

		// �����ļ�ָ��ƫ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		if((   tRasterDataPacketHeader.iPacketCode[0] != const_PacketCode_Raster_Data_Normal
			&& tRasterDataPacketHeader.iPacketCode[0] != const_PacketCode_Raster_Data_Section
			&& tRasterDataPacketHeader.iPacketCode[0] != const_PacketCode_Raster_Data_Slice )
			|| tRasterDataPacketHeader.iPacketCode[1] != const_Raster_Data_Operate_Flag_1
			|| tRasterDataPacketHeader.iPacketCode[2] != const_Raster_Data_Operate_Flag_2
			|| tRasterDataPacketHeader.iPackDescriptor != const_Raster_Data_Pack_Descriptor)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(RasterDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			return FALSE;
		}

		// դ����������
		int iNumberOfRows = tRasterDataPacketHeader.iNumberOfRows;
		for(i=0;i<iNumberOfRows;i++)
		{
			// Ϊһ��դ�����ݷ����ڴ�
			RasterRow* pRasterRow = new RasterRow;

			// ��ȡդ�����ݰ�
			if(ReadRasterRowDataPacket(pRasterRow))
			{
				bExistData = TRUE;
				pRasterRowList->push_back(pRasterRow);	// ��ӵ�դ�����ݰ�����
			}
			else
			{
				_delete(pRasterRow);
				break;
			}
		}

		return bExistData;
	}

	//FMT.xBA07 ��ȡդ������ͼ������
	//        Raster Data Data
	//        Packet Code = 0xBA07
	//FMT.xBA0F ��ȡդ������ͼ������
	//        Raster Data Data
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterImageData_Section(KRasterRowList* pRasterRowList)
	{
		return ReadRasterImageData(pRasterRowList);
	}

	//FMT.xBA07 ��ȡդ������ͼ������
	//        Raster Data Data
	//        Packet Code = 0xBA07
	//FMT.xBA0F ��ȡդ������ͼ������
	//        Raster Data Data
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterImageData_Slice(KRasterRowList* pRasterRowList)
	{
		return ReadRasterImageData(pRasterRowList);
	}

	//FMT.x ��ȡ TAB ���һҳ�ı�
	//
	//
	BOOL KRpgDataOperator::ReadTABWriteTextPage(KWriteTextList* pWriteTextList)
	{
		int i;

		// ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		while(1)
		{
			sfx_int16 iNumberOfCharactersPerLine = 0;	// ÿ�е��ַ���
			fread(&iNumberOfCharactersPerLine,sizeof(sfx_int16),1,fp);
			KBytesOrder::ReverseBytes(&iNumberOfCharactersPerLine);

			if(iNumberOfCharactersPerLine == -1)
			{
				break;
			}

			WriteText* pWriteText = new WriteText;
			pWriteText->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;

			for(i=0;i<iNumberOfCharactersPerLine;i++)
			{
				char cTemp = '\0';
				fread(&cTemp,sizeof(char),1,fp);

				//Character is ASCII when the MSB is set to zero.
				//When the MSB is set to one, the remaining 7 bits define the special symbol
				if(cTemp & 0x80)
				{// special symbol
					//pWriteText->szText[i] = ' ';
					pWriteText->szText[i] = cTemp & 0x7F;
				}
				else
				{
					pWriteText->szText[i] = cTemp;
				}
			}
			pWriteText->szText[i] = '\0';
			pWriteTextList->push_back(pWriteText);
		}

		// ÿҳ��һ����
		WriteText* pWriteText =  new WriteText;
		pWriteText->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;
		pWriteText->szText[0] = '\0';
		pWriteTextList->push_back(pWriteText);

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}
	///װ�ظ��ָ�ʽ���� - end
	///--------------------------------------------------------
#pragma endregion

#pragma region ��ȡ���ݰ� - Read_xxxxx_Packet
	///--------------------------------------------------------
	///��ȡ���ݰ� - beg
	//PACKET.01 ��ɫ�ı����ݰ�
	//     Mono Text Data Packet
	//     Packet Code = 1
	BOOL KRpgDataOperator::ReadMonoTextDataPacket(MonoText* pMonoText)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		MonoTextPacketHeader tMonoTextPacketHeader;
		fread(&tMonoTextPacketHeader,sizeof(MonoTextPacketHeader),1,fp);

		this->ReverseBytes(&tMonoTextPacketHeader);	// �ֽ�ת��

		if(tMonoTextPacketHeader.iPacketCode != const_PacketCode_Mono_Text)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(MonoTextPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}
	
		// 3. ��ȡ���ݿ�
		fread(&pMonoText->oPosition,sizeof(Position),1,fp);	// �ı���ʼλ��	

		this->ReverseBytes(pMonoText);	// �ֽ�ת��

		pMonoText->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;		//û����ɫ����

		// ���ַ��� = ���ݿ鳤�� - �ı���ʼλ����ռ���ֽ���
		int iNumberOfChar = tMonoTextPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16)*2;
		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pMonoText->szText[i] = cTemp;
		}
		pMonoText->szText[i] = '\0';

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.02 ����������ݰ�
	//     Special Symbol Packet
	//     Packet Code = 2
	BOOL KRpgDataOperator::ReadSpecilaSymbolDataPacket(SpecilaSymbol* pSpecilaSymbol)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		SpecilaSymbolPacketHeader tSpecilaSymbolPacketHeader;
		fread(&tSpecilaSymbolPacketHeader,sizeof(SpecilaSymbolPacketHeader),1,fp);

		this->ReverseBytes(&tSpecilaSymbolPacketHeader);	// �ֽ�ת��

		if(tSpecilaSymbolPacketHeader.iPacketCode != const_PacketCode_Special_Symbol)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(SpecilaSymbolPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		//3.��ȡ���ݿ�
		fread(&pSpecilaSymbol->oPosition,sizeof(Position),1,fp);	// �ı���ʼλ��	

		this->ReverseBytes(pSpecilaSymbol);	// �ֽ�ת��

		pSpecilaSymbol->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;		//û����ɫ����

		//���ַ��� = ���ݿ鳤�� - �ı���ʼλ����ռ���ֽ���
		int iNumberOfChar = tSpecilaSymbolPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16)*2;

		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pSpecilaSymbol->szText[i] = cTemp;
		}
		pSpecilaSymbol->szText[i] = '\0';

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.03 ���������ݰ� (�ο�PACKET.11)
	//     Mesocyclone
	//     Packet Code = 3

	//PACKET.04 �繳���ݰ�
	//     Wind Barb Data Packet
	//     Packet Code = 4
	BOOL KRpgDataOperator::ReadWindBarbDataPacket(WindBarb* pWindBarb)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		WindBarbDataPacketHeader tWindBarbDataPacketHeader;
		fread(&tWindBarbDataPacketHeader,sizeof(WindBarbDataPacketHeader),1,fp);

		this->ReverseBytes(&tWindBarbDataPacketHeader);	// �ֽ�ת��

		if(tWindBarbDataPacketHeader.iPacketCode != const_PacketCode_Wind_Barb)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(WindBarbDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pWindBarb,sizeof(WindBarb),1,fp);

		this->ReverseBytes(pWindBarb);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.05 ��ʸ���ݰ�
	//     Vector Arrow Data Packet
	//     Packet Code = 5
	BOOL KRpgDataOperator::ReadVectorArrowDataPacket(VectorArrow* pVectorArrow)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		VectorArrowDataPacketHeader tVectorArrowDataPacketHeader;
		fread(&tVectorArrowDataPacketHeader,sizeof(VectorArrowDataPacketHeader),1,fp);

		this->ReverseBytes(&tVectorArrowDataPacketHeader);	// �ֽ�ת��

		if(tVectorArrowDataPacketHeader.iPacketCode != const_PacketCode_Vector_Arrow)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(VectorArrowDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pVectorArrow,sizeof(VectorArrow),1,fp);

		this->ReverseBytes(pVectorArrow);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.06 ��ɫ����ʸ�����ݰ�
	//     Linked Vector Packet
	//     Packet Code = 6
	BOOL KRpgDataOperator::ReadMonoLinkedVectorDataPacket(MonoLinkedVector* pMonoLinkedVector)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		MonoLinkedVectorPacketHeader tMonoLinkedVectorPacketHeader;
		fread(&tMonoLinkedVectorPacketHeader,sizeof(MonoLinkedVectorPacketHeader),1,fp);

		this->ReverseBytes(&tMonoLinkedVectorPacketHeader);	// �ֽ�ת��

		if(tMonoLinkedVectorPacketHeader.iPacketCode != const_PacketCode_Mono_Linked_Vector)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(MonoLinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		pMonoLinkedVector->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;

		int iLengthOfPointDataBlock = tMonoLinkedVectorPacketHeader.iLengthOfDataBlock;	// ���ݿ鳤��
		int iNumberOfPoint = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 2);			// �������ݵĵ�(I,J)����, I �� J ��ռ2�ֽ�)

		for(i=0;i<iNumberOfPoint;i++)
		{
			sfx_int16 X,Y;

			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt;
			pt.x = X;
			pt.y = Y;
			pMonoLinkedVector->oPointVector.push_back(pt);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.07 ��ɫ������ʸ�����ݰ�
	//     Unlinked Vector Packet
	//     Packet Code = 7
	BOOL KRpgDataOperator::ReadMonoUnlinkedVectorDataPacket(MonoUnlinkedVector* pMonoUnlinkedVector)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		MonoUnlinkedVectorPacketHeader tMonoUnlinkedVectorPacketHeader;
		fread(&tMonoUnlinkedVectorPacketHeader,sizeof(MonoUnlinkedVectorPacketHeader),1,fp);

		this->ReverseBytes(&tMonoUnlinkedVectorPacketHeader);	// �ֽ�ת��

		if(tMonoUnlinkedVectorPacketHeader.iPacketCode != const_PacketCode_Mono_Unlinked_Vector)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(MonoUnlinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		pMonoUnlinkedVector->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;	// �߶ε���ɫ����

		int iLengthOfPointDataBlock = tMonoUnlinkedVectorPacketHeader.iLengthOfDataBlock;	// ���ݿ���ֽ���
		int iNumberOfLine = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 4);				// �����߶�����(4����)

		for(i=0;i<iNumberOfLine;i++)
		{
			sfx_int16 X,Y;

			// �˵�1
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt1;
			pt1.x = X;
			pt1.y = Y;
			pMonoUnlinkedVector->oPointVectorBeg.push_back(pt1);

			// �˵�2
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt2;
			pt2.x = X;
			pt2.y = Y;
			pMonoUnlinkedVector->oPointVectorEnd.push_back(pt2);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.08 ��ɫ�ı����ݰ�
	//     Color Text Data Packet
	//     Packet Code = 8
	BOOL KRpgDataOperator::ReadColorTextDataPacket(ColorText* pColorText)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		ColorTextPacketHeader tColorTextPacketHeader;
		fread(&tColorTextPacketHeader,sizeof(ColorTextPacketHeader),1,fp);
		this->ReverseBytes(&tColorTextPacketHeader);	// �ֽ�ת��

		if(tColorTextPacketHeader.iPacketCode != const_PacketCode_Color_Text)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(ColorTextPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}
	
		// 3. ��ȡ���ݿ�
		fread(&pColorText->iColorLevel,sizeof(sfx_int16),1,fp);	//�ı���ɫ����
		fread(&pColorText->oPosition,sizeof(Position),1,fp);	//�ı���ʼλ��	
		this->ReverseBytes(pColorText);	// �ֽ�ת��

		// ���ַ��� = ���ݿ鳤�� - �ı���ɫ���� - �ı���ʼλ����ռ���ֽ���
		int iNumberOfChar = tColorTextPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16) * 3;	// 3 = (�ı���ɫ���� + �ı���ʼλ��)��ռ������

		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pColorText->szText[i] = cTemp;
		}
		pColorText->szText[i] = '\0';

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.09 ��ɫ����ʸ�����ݰ�ͷ
	//     Linked Vector Packet
	//     Packet Code = 9
	BOOL KRpgDataOperator::ReadColorLinkedVectorDataPacket(ColorLinkedVector* pColorLinkedVector)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		ColorLinkedVectorPacketHeader tColorLinkedVectorPacketHeader;
		fread(&tColorLinkedVectorPacketHeader,sizeof(ColorLinkedVectorPacketHeader),1,fp);
		this->ReverseBytes(&tColorLinkedVectorPacketHeader);	// �ֽ�ת��

		if(tColorLinkedVectorPacketHeader.iPacketCode != const_PacketCode_Color_Linked_Vector)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(ColorLinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(&pColorLinkedVector->iColorLevel,sizeof(sfx_int16),1,fp);	// �ı���ɫ����
		KBytesOrder::ReverseBytes(&pColorLinkedVector->iColorLevel);	// �ֽ�ת��

		int iLengthOfPointDataBlock = tColorLinkedVectorPacketHeader.iLengthOfDataBlock;	// ���ݿ鳤��
		int iNumberOfPoint = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 2);				// �������ݵĵ���(һ������(I,J) I J ��ռ2�ֽ�)

		for(i=0;i<iNumberOfPoint;i++)
		{
			sfx_int16 X,Y;

			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt;
			pt.x = X;
			pt.y = Y;
			pColorLinkedVector->oPointVector.push_back(pt);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.10 ��ɫ������ʸ�����ݰ�ͷ
	//     Unlinked Vector Packet
	//     Packet Code = 10
	BOOL KRpgDataOperator::ReadColorUnlinkedVectorDataPacket(ColorUnlinkedVector* pColorUnlinkedVector)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		ColorUnlinkedVectorPacketHeader tColorUnlinkedVectorPacketHeader;
		fread(&tColorUnlinkedVectorPacketHeader,sizeof(ColorUnlinkedVectorPacketHeader),1,fp);
		this->ReverseBytes(&tColorUnlinkedVectorPacketHeader);	// �ֽ�ת��
 
		if(tColorUnlinkedVectorPacketHeader.iPacketCode != const_PacketCode_Color_Unlinked_Vector)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(ColorUnlinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(&pColorUnlinkedVector->iColorLevel,sizeof(sfx_int16),1,fp);	// �߶���ɫ����
		KBytesOrder::ReverseBytes(&pColorUnlinkedVector->iColorLevel);		// �ֽ�ת��

		int iLengthOfPointDataBlock = tColorUnlinkedVectorPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16);	// ���ݿ����ֽ���
		int iNumberOfLine = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 4);									// �����߶�����(4����)

		for(i=0;i<iNumberOfLine;i++)
		{
			sfx_int16 X,Y;

			// �˵�1
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);		// �ֽ�ת��
			KBytesOrder::ReverseBytes(&Y);		// �ֽ�ת��

			POINT pt1;
			pt1.x = X;
			pt1.y = Y;
			pColorUnlinkedVector->oPointVectorBeg.push_back(pt1);

			// �˵�2
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);		// �ֽ�ת��
			KBytesOrder::ReverseBytes(&Y);		// �ֽ�ת��

			POINT pt2;
			pt2.x = X;
			pt2.y = Y;
			pColorUnlinkedVector->oPointVectorEnd.push_back(pt2);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.11 ���������ݰ�ͷ
	//     Mesocyclone
	//     Packet Code = 11
	BOOL KRpgDataOperator::ReadMesocycloneDataPacket(Mesocyclone* pMesocyclone)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		MesocyclonePacketHeader tMesocyclonePacketHeader;
		fread(&tMesocyclonePacketHeader,sizeof(MesocyclonePacketHeader),1,fp);

		this->ReverseBytes(&tMesocyclonePacketHeader);	// �ֽ�ת��

		if( (tMesocyclonePacketHeader.iPacketCode != const_PacketCode_Mesocyclone_3)
			&& (tMesocyclonePacketHeader.iPacketCode != const_PacketCode_Mesocyclone_11) )
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(MesocyclonePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pMesocyclone,sizeof(Mesocyclone),1,fp);

		this->ReverseBytes(pMesocyclone);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.12 ���������������ݰ�ͷ
	//     Tornado Vortex Signature
	//     Packet Code = 12
	BOOL KRpgDataOperator::ReadTornadoVortexSignatureDataPacket(TornadoVortexSignature* pTornadoVortexSignature)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		TornadoVortexSignaturePacketHeader tTornadoVortexSignaturePacketHeader;
		fread(&tTornadoVortexSignaturePacketHeader,sizeof(TornadoVortexSignaturePacketHeader),1,fp);

		this->ReverseBytes(&tTornadoVortexSignaturePacketHeader);	// �ֽ�ת��

		if(tTornadoVortexSignaturePacketHeader.iPacketCode != const_PacketCode_Tornado_Vortex_Signature)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(TornadoVortexSignaturePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pTornadoVortexSignature,sizeof(TornadoVortexSignature),1,fp);

		this->ReverseBytes(pTornadoVortexSignature);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.13 ����ʵ�����ݰ�ͷ
	//     Hail Positive
	//     Packet Code = 13
	BOOL KRpgDataOperator::ReadHailPositiveDataPacket(HailPositive* pHailPositive)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		HailPositivePacketHeader tHailPositivePacketHeader;
		fread(&tHailPositivePacketHeader,sizeof(HailPositivePacketHeader),1,fp);

		this->ReverseBytes(&tHailPositivePacketHeader);	// �ֽ�ת��

		if(tHailPositivePacketHeader.iPacketCode != const_PacketCode_Hail_Positive)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(HailPositivePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pHailPositive,sizeof(HailPositive),1,fp);

		this->ReverseBytes(pHailPositive);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.14 �����������ݰ�
	//     Hail Probable
	//     Packet Code = 14
	BOOL KRpgDataOperator::ReadHailProbableDataPacket(HailProbable* pHailProbable)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		HailProbablePacketHeader tHailProbablePacketHeader;
		fread(&tHailProbablePacketHeader,sizeof(HailProbablePacketHeader),1,fp);

		this->ReverseBytes(&tHailProbablePacketHeader);	// �ֽ�ת��

		if(tHailProbablePacketHeader.iPacketCode != const_PacketCode_Hail_Probable)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(HailProbablePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pHailProbable,sizeof(HailProbable),1,fp);

		this->ReverseBytes(pHailProbable);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.15 �籩ID���ݰ�
	//     Storm ID
	//     Packet Code = 15
	BOOL KRpgDataOperator::ReadStormIdDataPacket(StormId* pStormId)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		StormIdPacketHeader tStormIdPacketHeader;
		fread(&tStormIdPacketHeader,sizeof(StormIdPacketHeader),1,fp);

		this->ReverseBytes(&tStormIdPacketHeader);	// �ֽ�ת��

		if(tStormIdPacketHeader.iPacketCode != const_PacketCode_Storm_Id)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(StormIdPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(&pStormId->oPosition,sizeof(Position),1,fp);	// �ı���ʼIλ��

		this->ReverseBytes(&pStormId->oPosition);			// �ֽ�ת��

		pStormId->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;	//û����ɫ����

		//���ַ��� = ���ݿ鳤�� - �ı���ʼλ����ռ���ֽ���
		int iNumberOfChar = tStormIdPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16) * 2;

		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pStormId->szText[i] = cTemp;
		}
		pStormId->szText[i] = '\0';

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.16 ���־��������������ݰ�
	//     Digital Radial Data Array Packet
	//     Packet Code = 16
	BOOL KRpgDataOperator::ReadDigitalRadialDataArrayDataPacket(DigitalRadialDataArray* pDigitalRadialDataArray)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		{
			// waiting ...
			Assert(FALSE);
		}


		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return FALSE;
	}

	//PACKET.17 ���ֽ�ˮ�����������ݰ�ͷ
	//     Digital Precipatation Data Array Packet
	//     Packet Code = 17
	BOOL KRpgDataOperator::ReadDigitalPrecipitationDataArrayDataPacket(DigitalPrecipitationDataArray* pDigitalPrecipitationDataArray)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ÿ��դ������ͷ
		fread(&pDigitalPrecipitationDataArray->oEachRowHeader,sizeof(EachRowHeader),1,fp);

		this->ReverseBytes(&pDigitalPrecipitationDataArray->oEachRowHeader);			// �ֽ�ת��

		// 3. ��ȡ���ݿ�
		int iNumberOfBytesInThisRow = pDigitalPrecipitationDataArray->oEachRowHeader.iNumberOfBytesInThisRow;	// �����ֽ���
		int iNumberOfRunColor = iNumberOfBytesInThisRow / 2;							// ����RLE��������
		pDigitalPrecipitationDataArray->pRunColor = new RunColor[iNumberOfRunColor];	// Ϊ TRunColor* ����洢�ռ�

		for(i=0;i<iNumberOfRunColor;i++)
		{
			sfx_uint16 iRunColor = 0;
			fread(&iRunColor,sizeof(sfx_uint16),1,fp);
			pDigitalPrecipitationDataArray->pRunColor[i].Attach(iRunColor);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.18 ��ˮ�������������ݰ�
	//     Precipitation Rate Data Array Packet
	//     Packet Code = 18
	BOOL KRpgDataOperator::ReadPrecipitationRateDataArrayDataPacket(PrecipitationRateDataArray* pPrecipitationRateDataArray)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ÿ��դ������ͷ
		fread(&pPrecipitationRateDataArray->oEachRowHeader,sizeof(EachRowHeader),1,fp);

		this->ReverseBytes(&pPrecipitationRateDataArray->oEachRowHeader);			// �ֽ�ת��

		// 3. ��ȡ���ݿ�
		int iNumberOfBytesInThisRow = pPrecipitationRateDataArray->oEachRowHeader.iNumberOfBytesInThisRow;	//�����ֽ���
		int iNumberOfRunColor = iNumberOfBytesInThisRow;							// ����RLE��������
		pPrecipitationRateDataArray->pRunColor = new RunColor[iNumberOfRunColor];	// Ϊ TRunColor* ����洢�ռ�

		for(i=0;i<iNumberOfRunColor;i++)
		{
			char cRunColor = '\0';
			fread(&cRunColor,sizeof(char),1,fp);
			pPrecipitationRateDataArray->pRunColor[i].Attach(cRunColor);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.19 �����������ݰ� waiting... HDA ???
	//     HDA Hail
	//     Packet Code = 19
	BOOL KRpgDataOperator::ReadHdaHailDataPacket(HdaHail* pHdaHail)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		HdaHailPacketHeader tHdaHailPacketHeader;
		fread(&tHdaHailPacketHeader,sizeof(HdaHailPacketHeader),1,fp);

		this->ReverseBytes(&tHdaHailPacketHeader);	// �ֽ�ת��

		if(tHdaHailPacketHeader.iPacketCode != const_PacketCode_Hda_Hail)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(HdaHailPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		fread(pHdaHail,sizeof(HdaHail),1,fp);

		this->ReverseBytes(pHdaHail);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.20 �������ݰ�

	//PACKET.21 �������ݰ�

	//PACKET.22 �������ݰ�

	//PACKET.23 �籩����·����Ϣ��ȥ���ݰ�
	//     SCIT Past Data
	//     Packet Code = 23
	// ���� 2,6,25 ���ݰ�
	BOOL KRpgDataOperator::ReadScitPastDataPacket(ScitPast* pScitPast)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		ScitPastDataPacketHeader tScitPastDataPacketHeader;
		fread(&tScitPastDataPacketHeader,sizeof(ScitPastDataPacketHeader),1,fp);

		this->ReverseBytes(&tScitPastDataPacketHeader);	// �ֽ�ת��

		if(tScitPastDataPacketHeader.iPacketCode != const_PacketCode_Scit_Past_Data)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(ScitPastDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// ����ָ��λ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 3.��ȡ����
		// 3.1 Packet Code = 2
		ReadSpecilaSymbolData(&pScitPast->oSpecilaSymbolList);
		// 3.2 Packet Code = 6
		ReadMonoLinkedVectorData(&pScitPast->oMonoLinkedVectorList);
		// 3.3 Packet Code = 25
		ReadStiCircleData(&pScitPast->oStiCircleList);

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.24 �籩����·����ϢԤ�����ݰ�
	//     SCIT Forecast Data
	//     Packet Code = 24
	// ���� 2,6,25 ���ݰ�
	BOOL KRpgDataOperator::ReadScitForecastDataPacket(ScitForecast* pScitForecast)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		ScitForecastDataPacketHeader tScitForecastDataPacketHeader;
		fread(&tScitForecastDataPacketHeader,sizeof(ScitForecastDataPacketHeader),1,fp);

		this->ReverseBytes(&tScitForecastDataPacketHeader);	// �ֽ�ת��

		if(tScitForecastDataPacketHeader.iPacketCode != const_PacketCode_Scit_Forecast_Data)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(ScitForecastDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		//����ָ��λ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 3.��ȡ����
		// 3.1 Packet Code = 2
		ReadSpecilaSymbolData(&pScitForecast->oSpecilaSymbolList);
		// 3.2 Packet Code = 6
		ReadMonoLinkedVectorData(&pScitForecast->oMonoLinkedVectorList);
		// 3.3 Packet Code = 25
		ReadStiCircleData(&pScitForecast->oStiCircleList);

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.25 �籩·����ϢȦ���ݰ�
	//     STI Circle
	//     Packet Code = 25
	BOOL KRpgDataOperator::ReadStiCircleDataPacket(StiCircle* pStiCircle)
	{
		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		StiCirclePacketHeader tStiCirclePacketHeader;
		fread(&tStiCirclePacketHeader,sizeof(StiCirclePacketHeader),1,fp);

		this->ReverseBytes(&tStiCirclePacketHeader);	// �ֽ�ת��

		if(tStiCirclePacketHeader.iPacketCode != const_PacketCode_Sti_Circle)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(StiCirclePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		//3.��ȡ���ݿ�
		fread(pStiCircle,sizeof(StiCircle),1,fp);
		this->ReverseBytes(pStiCircle);	// �ֽ�ת��

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.x3501 �����ӵ�ֵ��ʸ�����ݰ�
	//        UnLinked Contour Vectors Packet
	//        Packet Code = 0x3501
	BOOL KRpgDataOperator::ReadUnLinkedContourVectorDataPacket(UnlinkedContourVector* pUnlinkedContourVector)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		UnlinkedContourVectorPacketHeader tUnlinkedContourVectorPacketHeader;
		fread(&tUnlinkedContourVectorPacketHeader,sizeof(UnlinkedContourVectorPacketHeader),1,fp);

		this->ReverseBytes(&tUnlinkedContourVectorPacketHeader);	// �ֽ�ת��

		if(tUnlinkedContourVectorPacketHeader.iPacketCode != const_PacketCode_Unlinked_Contour_Vector)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(UnlinkedContourVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return false;
		}

		// 3. ��ȡ���ݿ�
		int iNumberOfLine = tUnlinkedContourVectorPacketHeader.iLengthOfVector / (sizeof(sfx_int16) * 4);	//�����߶�����(1�߶�������)

		for(i=0;i<iNumberOfLine;i++)
		{
			sfx_int16 X,Y;

			// �˵�1
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt1;
			pt1.x = X;
			pt1.y  =Y;
			pUnlinkedContourVector->oPointVectorBeg.push_back(pt1);

			// �˵�2
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt2;
			pt2.x = X;
			pt2.y  =Y;
			pUnlinkedContourVector->oPointVectorEnd.push_back(pt2);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.x0E03 ���ӵ�ֵ��ʸ�����ݰ�
	//        Linked Contour Vectors Packet
	//        Packet Code = 0x0E03
	BOOL KRpgDataOperator::ReadLinkedContourVectorDataPacket(LinkedContourVector* pLinkedContourVector)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ȡ���ݰ�ͷ
		LinkedContourVectorPacketHeader tLinkedContourVectorPacketHeader;
		fread(&tLinkedContourVectorPacketHeader,sizeof(LinkedContourVectorPacketHeader),1,fp);

		this->ReverseBytes(&tLinkedContourVectorPacketHeader);	// �ֽ�ת��

		if(tLinkedContourVectorPacketHeader.iPacketCode != const_PacketCode_Linked_Contour_Vector)
		{
			// ������������Ҫ�ع��ļ�ָ��
			fseek(fp,-1*(int)sizeof(LinkedContourVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. ��ȡ���ݿ�
		// 3.1 ��ͷ
		if(tLinkedContourVectorPacketHeader.iInitialPointIndicator == const_Initial_Point_Indicator)
		{// ��ֵ���������ڰ���
			sfx_int16 X,Y;

			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt;
			pt.x = X;
			pt.y = Y;
			pLinkedContourVector->oPointVector.push_back(pt);
		}
		else
		{// ��ֵ����㲻�����ڰ���
			POINT pt;
			pt.x = 0;
			pt.y = 0;
			pLinkedContourVector->oPointVector.push_back(pt);
		}

		// 3.2 ��������ֵ�ߵ�������
		// ���������ֵ������������ռ�ĳ���(�ֽ�: 4 to 32764, length = # of vectors * 4)
		sfx_uint16	iLengthOfOtherVector;
		fread(&iLengthOfOtherVector,sizeof(sfx_uint16),1,fp);
		KBytesOrder::ReverseBytes(&iLengthOfOtherVector);	// �ֽ�ת��

		// ���������ڵ�����(1�ڵ� = 2��16λ���� = 4���ֽ�)
		int iNumberOfPoint = iLengthOfOtherVector / (sizeof(sfx_uint16) * 2);	// �ڵ�����

		//�����ȡһ����ֵ�ߵ�����������
		for(i=0;i<iNumberOfPoint;i++)
		{
			sfx_int16 X,Y;

			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt;
			pt.x = X;
			pt.y = Y;
			pLinkedContourVector->oPointVector.push_back(pt);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.xAF1F �������ݰ�
	//        Radial Data Packet
	//        Packet Code = 0xAF1F
	BOOL KRpgDataOperator::ReadRadialDataPacket(Radial* pRadial)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. ��ÿ��������������ͷ
		fread(&pRadial->oEachRadialHeader,sizeof(EachRadialHeader),1,fp);

		this->ReverseBytes(&pRadial->oEachRadialHeader);	// �ֽ�ת��

		// 3. ��ȡ���ݿ�
		int iNumbersOfRunColor = pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;	//ÿ�����ߵ��г̱���(RLECode)��(16bits,ע��Ҫ *2)
		pRadial->pRunColor = new RunColor[iNumbersOfRunColor];								//Ϊ�������г̱���ṹ����

		for(i=0;i<iNumbersOfRunColor;i++)
		{// ���������Ͼ������г̱���
			char cRunColor = '\0';
			fread(&cRunColor,sizeof(char),1,fp);
			pRadial->pRunColor[i].Attach(cRunColor);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.xBA07 դ�����ݰ�
	//        Raster Data Packet
	//        Packet Code = 0xBA07
	//PACKET.xBA0F դ�����ݰ�
	//        Raster Data Packet
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterRowDataPacket(RasterRow* pRasterRow)
	{
		int i;

		// 1. ��λ�ļ�ָ��
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. դ������ÿ������ͷ
		fread(&pRasterRow->oEachRowHeader,sizeof(EachRowHeader),1,fp);

		this->ReverseBytes(&pRasterRow->oEachRowHeader);	// �ֽ�ת��

		// 3. ��ȡ���ݿ�
		int iNumbersOfRunColor = pRasterRow->oEachRowHeader.iNumberOfBytesInThisRow;	// ÿ�����ߵ��г̱���(RLECode)��
		pRasterRow->pRunColor = new RunColor[iNumbersOfRunColor];						// Ϊ TRunColor* ����洢�ռ�

		for(i=0;i<iNumbersOfRunColor;i++)
		{// �����г̱���
			char cRunColor='\0';
			fread(&cRunColor,sizeof(char),1,fp);
			pRasterRow->pRunColor[i].Attach(cRunColor);
		}

		// 4. �����ļ�ָ�뵱ǰλ��
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}
	///��ȡ���ݰ� - end
	///--------------------------------------------------------
#pragma endregion
	///===============
	///��ȡ���� - end
	///========================================================

	///========================================================
	///����ͼ�� - beg
	///===============
#pragma region ����ͼ�� - BuildImage
	// ����ͼ��
	BOOL KRpgDataOperator::BuildImage(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		if(pRpgData->IsEmpty())
			return FALSE;

		// ȡ�ò�Ʒ����
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());

		BOOL bBuildImage = FALSE;

		switch(eProductType)
		{
		case ProductType_RadialImage:				// 1
			bBuildImage = BuildImage_Radial(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_RasterImage:				// 2
			bBuildImage = BuildImage_Raster(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_LinkedContourVectorImage:	// 3
			bBuildImage = BuildImage_LinkedContourVector(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_GeographicAlphaImage:		// 4
			bBuildImage = BuildImage_GeographicAlpha(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_NonGeographicAlphaImage:	// 5
			bBuildImage = BuildImage_NonGeographicAlpha(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_AlphanumericImage:			// 6
			bBuildImage = BuildImage_Alphanumeric(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_RasterImageSection:		// 7
			bBuildImage = BuildImage_Raster_Section(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;

		case ProductType_RasterImageSlice:			// 8
			bBuildImage = BuildImage_Raster_Slice(fZoom,bDrawCoordinate,bDrawInfoArea);
			break;
		}

		return bBuildImage;
	}

	///--------------------------------------------------------
	/// ��������ͼ�� - Beg
	// 1 ʹ�� KRadialList
	BOOL KRpgDataOperator::BuildImage_Radial(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//ͼ��ߴ�
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// Ĭ��ͼ��߶�Ϊ512����
		DWORD dwWidthInfo	 = 170;									// Ĭ��ͼ����Ϣ�����Ϊ170����
		DWORD dwHeightInfo	 = dwHeightImage;						// Ĭ��ͼ����Ϣ���߶�Ϊͼ��߶�

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("Ҫ��ͼ���Ⱥ͸߶�һ��"),_T("����"),MB_OK);
			return FALSE;
		}

		// ����ͼ��ߴ������Ϣ���ߴ�
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// ����ͼ���� + ��Ϣ���ߴ�
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// ͼ����
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// ��Ϣ��
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// ͼ�� + ��Ϣ��
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)

		// �������� DC(ע���ͷ�)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// ��ˢ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// λͼ���
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// ���ݲ���
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// ȷ��ֻ��һ��

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// ������ɫ

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// ������ɫ

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// ������ɫ

			// ����ͼ��
			KRadialList* pRadialList = pRpgData->GetRadialList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pRadialList);

			// ��������
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// ������Ϣ��
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// �ϲ� hImageDC �� hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// ����λͼ���
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// �ͷ�����DC���

		return TRUE;
	}
	// 2 ʹ�� KRasterRowList
	BOOL KRpgDataOperator::BuildImage_Raster(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//ͼ��ߴ�
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// Ĭ��ͼ��߶�Ϊ512����
		DWORD dwWidthInfo	 = 170;									// Ĭ��ͼ����Ϣ�����Ϊ170����
		DWORD dwHeightInfo	 = dwHeightImage;						// Ĭ��ͼ����Ϣ���߶�Ϊͼ��߶�

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("Ҫ��ͼ���Ⱥ͸߶�һ��"),_T("����"),MB_OK);
			return FALSE;
		}

		// ����ͼ��ߴ������Ϣ���ߴ�
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// ����ͼ���� + ��Ϣ���ߴ�
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// ͼ����
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// ��Ϣ��
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// ͼ�� + ��Ϣ��
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)

		// �������� DC(ע���ͷ�)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// ��ˢ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// λͼ���
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// ���ݲ���
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// ȷ��ֻ��һ��

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// ������ɫ

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// ������ɫ

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// ������ɫ

			// ����ͼ��
			KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pRasterRowList);

			// ��������
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// ������Ϣ��
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// �ϲ� hImageDC �� hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// ����λͼ���
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// �ͷ�����DC���

		return TRUE;
	}
	// 3
	BOOL KRpgDataOperator::BuildImage_LinkedContourVector(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//ͼ��ߴ�
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// Ĭ��ͼ��߶�Ϊ512����
		DWORD dwWidthInfo	 = 170;									// Ĭ��ͼ����Ϣ�����Ϊ170����
		DWORD dwHeightInfo	 = dwHeightImage;						// Ĭ��ͼ����Ϣ���߶�Ϊͼ��߶�

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("Ҫ��ͼ���Ⱥ͸߶�һ��"),_T("����"),MB_OK);
			return FALSE;
		}

		// ����ͼ��ߴ������Ϣ���ߴ�
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// ����ͼ���� + ��Ϣ���ߴ�
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// ͼ����
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// ��Ϣ��
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// ͼ�� + ��Ϣ��
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)

		// �������� DC(ע���ͷ�)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// ��ˢ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// λͼ���
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// ���ݲ���
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// ȷ��ֻ��һ��

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// ������ɫ

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// ������ɫ

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// ������ɫ

			// ����ͼ��
			KLinkedContourVectorList* pLinkedContourVectorList = pRpgData->GetLinkedContourVectorList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pLinkedContourVectorList);

			// ��������
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// ������Ϣ��
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// �ϲ� hImageDC �� hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// ����λͼ���
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// �ͷ�����DC���

		return TRUE;
	}
	// 4
	// SWP47 + STI58 + HI59 + M60 + TVS61
	BOOL KRpgDataOperator::BuildImage_GeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();
	
		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//ͼ��ߴ�
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// Ĭ��ͼ��߶�Ϊ512����
		DWORD dwWidthInfo	 = 170;								// Ĭ��ͼ����Ϣ�����Ϊ170����
		DWORD dwHeightInfo	 = dwHeightImage;					// Ĭ��ͼ����Ϣ���߶�Ϊͼ��߶�

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("Ҫ��ͼ���Ⱥ͸߶�һ��"),_T("����"),MB_OK);
			return FALSE;
		}

		// ����ͼ��ߴ������Ϣ���ߴ�
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// ����ͼ���� + ��Ϣ���ߴ�
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// ͼ����
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// ��Ϣ��
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// ͼ�� + ��Ϣ��
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)

		// �������� DC(ע���ͷ�)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// ��ˢ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// λͼ���
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// ���ݲ���
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// ȷ��ֻ��һ��

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// ������ɫ

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// ������ɫ

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// ������ɫ

			// ����ͼ��
			if(iProductCode == SWP47)
			{// SWP47
				KColorTextList* pColorTextList = pRpgData->GetColorTextList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pColorTextList);
			}
			else if(iProductCode == STI58)
			{// STI
				// Pack Code = 2
				KSpecilaSymbolList* pSpecilaSymbolList = pRpgData->GetSpecilaSymbolList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pSpecilaSymbolList,TRUE);

				// Pack Code = 23
				m_bScitPast = TRUE;	m_bScitForecast = FALSE;
				KScitList* pScitPastList = pRpgData->GetScitPastList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pScitPastList);
				m_bScitPast = FALSE;	m_bScitForecast = FALSE;

				// Pack Code = 24
				m_bScitPast = FALSE;	m_bScitForecast = TRUE;
				KScitList* pScitForecastList = pRpgData->GetScitForecastList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pScitForecastList);
				m_bScitPast = FALSE;	m_bScitForecast = FALSE;

				// Pack Code = 15
				KStormIdList* pStormIdList = pRpgData->GetStormIdList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pStormIdList);
			}
			else if(iProductCode == HI59)
			{// HI
				// Pack Code = 15
				KStormIdList* pStormIdList = pRpgData->GetStormIdList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pStormIdList);

				// Pack Code = 19
				KHdaHailList* pHdaHailList = pRpgData->GetHdaHailList();
				this->DrawImage(hImageDC,&rectImage,fZoom,pHdaHailList);
			}
			else if(iProductCode == M60)
			{// M
			}
			else if(iProductCode == TVS61)
			{// TVS
			}


			// ��������
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// ������Ϣ��
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// �ϲ� hImageDC �� hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// ����λͼ���
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// �ͷ�����DC���

		return TRUE;
	}
	// 5
	// 48,84
	BOOL KRpgDataOperator::BuildImage_NonGeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();
	
		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//ͼ��ߴ�
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// Ĭ��ͼ��߶�Ϊ512����
		DWORD dwWidthInfo	 = 170;								// Ĭ��ͼ����Ϣ�����Ϊ170����
		DWORD dwHeightInfo	 = dwHeightImage;					// Ĭ��ͼ����Ϣ���߶�Ϊͼ��߶�

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("Ҫ��ͼ���Ⱥ͸߶�һ��"),_T("����"),MB_OK);
			return FALSE;
		}

		// ����ͼ��ߴ������Ϣ���ߴ�
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// ����ͼ���� + ��Ϣ���ߴ�
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// ͼ����
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// ��Ϣ��
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// ͼ�� + ��Ϣ��
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)

		// �������� DC(ע���ͷ�)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// ��ˢ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// λͼ���
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// ���ݲ���
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// ȷ��ֻ��һ��

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// ������ɫ

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// ������ɫ

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// ������ɫ


			// ��������
			if(bDrawCoordinate)
			{
				// ���Ƶѿ�������
				DrawDescartes(hImageDC,&rectImage,fZoom,pRpgData->GetColorUnlinkedVectorList(),pRpgData->GetColorTextList());
			}

			//����WindBarbͼ��
			if(iProductCode == 48)
			{
				DrawImage(hImageDC,&rectImage,fZoom,pRpgData->GetWindBarbList());
			}
			//���� Linked Vtetor ͼ��
			else if(iProductCode == 84)
			{
				DrawImage(hImageDC,&rectImage,fZoom,pRpgData->GetColorLinkedVectorList());
			}

			// ������Ϣ��
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// �ϲ� hImageDC �� hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// ����λͼ���
			pRpgData->SetImageHandle(hBitmapTotal);
		}// for(i)

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// �ͷ�����DC���

		return TRUE;
	}
	// 6
	// 62,82
	BOOL KRpgDataOperator::BuildImage_Alphanumeric(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		// ����Ҫ����ͼ��?
		return FALSE;
	}
	// 7 ����ͼ
	BOOL KRpgDataOperator::BuildImage_Raster_Section(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//ͼ��ߴ�
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// Ĭ��ͼ��߶�Ϊ512����
		DWORD dwWidthInfo	 = 170;								// Ĭ��ͼ����Ϣ�����Ϊ170����
		DWORD dwHeightInfo	 = dwHeightImage;					// Ĭ��ͼ����Ϣ���߶�Ϊͼ��߶�

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("Ҫ��ͼ���Ⱥ͸߶�һ��"),_T("����"),MB_OK);
			return FALSE;
		}

		// ����ͼ��ߴ������Ϣ���ߴ�
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// ����ͼ���� + ��Ϣ���ߴ�
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// ͼ����
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// ��Ϣ��
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// ͼ�� + ��Ϣ��
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)

		// �������� DC(ע���ͷ�)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// ��ˢ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// λͼ���
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// ���ݲ���
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		// �״��Ʒ��  Raster Iamge Section ��2��
		Assert(iTotalLayers==2);

		//for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// ������ɫ

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// ������ɫ

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// ������ɫ

			// ����ͼ��
			KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pRasterRowList);

			// ��������
			if(bDrawCoordinate)
			{
				// ���Ƶѿ�������
				DrawDescartes(hImageDC,&rectImage,fZoom,pRpgData->GetMonoUnlinkedVectorList(),pRpgData->GetMonoTextList());
			}

			// ������Ϣ��
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// �ϲ� hImageDC �� hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// ����λͼ���
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// �ͷ�����DC���

		return TRUE;
	}
	// 8 ��Ƭͼ
	// waiting...
	BOOL KRpgDataOperator::BuildImage_Raster_Slice(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		return FALSE;
	}
	/// ��������ͼ�� - end
	///--------------------------------------------------------
#pragma endregion


#pragma region �������ͼ�� - DrawImage
	///--------------------------------------------------------
	/// �������ͼ�� - Beg

	// �����ı����ݰ����� [P3-108][P3-110][P3-109][P3-130]
	//     Write Text Data Packet
	//     Packet Code = 1,2,8,15
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWriteTextList* pWriteTextList,BOOL bSpecilaSymbol)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(12.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		//���Ƶѿ��� fZoom û��ʹ��
		if(iProductCode == VWP48 ||iProductCode == RCS50 || iProductCode == VAD84)
		{
			fZoom = 1.0f;
		}

		::SetBkColor(hDC,clrBack);
		::SetBkMode(hDC,TRANSPARENT);

		//���Ʊ�ע�ı�
		KWriteTextList::iterator it;
		KWriteTextList::iterator ibeg = pWriteTextList->begin();
		KWriteTextList::iterator iend = pWriteTextList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// ��ע�ı�
			WriteText* pWriteText = (*it);

			// ��ɫ�ȼ�
			int iColorLevel = pWriteText->iColorLevel;

			// �ı���ɫ
			COLORREF clr = 0;

			// ������ɫ�ȼ�ȡ����ɫֵ
			if(iColorLevel != RPG_DATA_NO_COLOR_LEVEL)
			{
				clr = pLegend[iColorLevel].dwColor;
			}
			else	// == -1 ��Ϊ�����ı���ɫ
			{
				clr = RGB(0,192,255);
			}

			double x,y;

			// SWP47 STI58 HI59 
			if(iProductCode == SWP47 || iProductCode == STI58 || iProductCode == HI59)
			{
				// �� km/4 ת��Ϊ��Ļ����(����Ļ����Ϊ����)
				x = 256 + 1.0 * pWriteText->oPosition.I / 4 * fZoom;
				y = 256 - 1.0 * pWriteText->oPosition.J / 4 * fZoom;
			}
			else
			{
				// ���������Ļ����
				x = 1.0 * pWriteText->oPosition.I * fZoom;
				y = 1.0 * pWriteText->oPosition.J * fZoom;
			}

			x = x * dPixelPerKm + 0.5;
			y = y * dPixelPerKm + 0.5;

			::SetTextColor(hDC,clr);
			::SetTextAlign(hDC,TA_TOP | TA_LEFT);
			if(!bSpecilaSymbol)
			{
				::TextOutA(hDC,(int)(x),(int)(y),pWriteText->szText,strlen(pWriteText->szText));
			}
			else
			{
				//::TextOutA(hDC,x,y,"?",1);
			}
		}

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);

		return TRUE;
	}
	// ���������ݰ� [P3-129]
	//     Mesocyclone
	//     Packet Code = 3,11
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KMesocycloneList* pMesocycloneList)
	{
		return FALSE;
	}
	// �繳���ݰ� [P3-128]
	//     Wind Barb Data Packet
	//     Packet Code = 4
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWindBarbList* pWindBarbList)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// ��ת��Ϊ��/��
		double dknot2mps = 0.514;

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		KWindBarbList::iterator it;
		KWindBarbList::iterator ibeg = pWindBarbList->begin();
		KWindBarbList::iterator iend = pWindBarbList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// �繳ָ��
			WindBarb* pWindBarb = (*it);

			//������ɫ�ȼ�ȡ�÷繳��ɫֵ
			int iColorLevel = pWindBarb->iColorLevelOfWindBarb;
			COLORREF clr = pLegend[iColorLevel].dwColor;

			//�繳����
			double x = pWindBarb->iXCoordinate * dPixelPerKm;
			double y = pWindBarb->iYCoordinate * dPixelPerKm;

			//���򡢷���
			double dWindDirection	= PI*(pWindBarb->iWindDirection-90)/180;
			double dWindSpeed		= pWindBarb->iWindSpeed;
			//dWindSpeed				= pWindBarb->iWindSpeed*dknot2mps;		//�ѽڱ��mps
			dWindSpeed				= pWindBarb->iWindSpeed * 1.25;

			//------------------------------------------------
			double x0 = x;
			double y0 = y;
			double plen = 16.0*dPixelPerKm;			//�繳�˳���
			double blen = 10.0*dPixelPerKm;			//���̳���Barb length
			double bgap = 2.0*dPixelPerKm;			//���̼��
			double padd	= 0.0;						//�繳��������Ƕ�
			double var	= dWindSpeed;				//����(m/s)

			double rad  = 3.0*dPixelPerKm;		//ԲȦ�뾶
			//------------------------------------------------

			// ѡ�뻭��
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);

			// ѡ�뻭ˢ
			::DeleteObject(hBrush);
			hBrush = ::CreateSolidBrush(clr);
			::SelectObject(hDC,hBrush);

			if (var<2.01) //��(x,y)������Բ
			{
				::Ellipse(hDC,(int)(x0-rad),(int)(y0-rad),(int)(x0+rad),(int)(y0+rad));
				continue;
			}
			else
			{// �������ٵ�
				if (var<2.5) padd = bgap;
				while (var>=47.5) { var-=50.0; padd += bgap*1.6;}
				while (var>=7.5) { var-=10.0; padd += bgap;}
				if (var>=2.5) padd += bgap;
			}

			double xp1,yp1,xp2,yp2,xp3,yp3;

			//Draw pointer
			::MoveToEx(hDC,(int)x0,(int)y0,NULL);
			xp1 = x + plen*cos(dWindDirection);
			yp1 = y + plen*sin(dWindDirection);
			::LineTo(hDC,(int)xp1,(int)yp1);

			var	= dWindSpeed;
			double a70 = 90.0*PI/180.0;	//
			//double a70 = 70.0*PI/180.0;	//
			double cosd70 = cos(dWindDirection+a70);
			double sind70 = sin(dWindDirection+a70);

			int flag = 0;
			int flg2 = 0;

			//[47.5, ...)
			while (var>=47.5)
			{//����������
				xp1 = x + plen*cos(dWindDirection);
				yp1 = y + plen*sin(dWindDirection);
				xp2 = xp1 + blen*cosd70;
				yp2 = yp1 + blen*sind70;
				xp3 = x + (plen-bgap*1.45)*cos(dWindDirection);
				yp3 = y + (plen-bgap*1.45)*sin(dWindDirection);
				::MoveToEx(hDC,(int)xp1,(int)yp1,NULL);
				::LineTo(hDC,(int)xp2,(int)yp2);
				::LineTo(hDC,(int)xp3,(int)yp3);
				plen = plen - bgap*1.6;
				var-=50.0;
				flag = 1;	//add by shenyc
				flg2 = 1;
			}

			// [7.5,47.5)
			while (var>=7.5)
			{// 
				if (flg2) {plen-=bgap*0.7; flg2 = 0;}
				xp1 = x + plen*cos(dWindDirection);
				yp1 = y + plen*sin(dWindDirection);
				xp2 = xp1 + blen*cosd70;
				yp2 = yp1 + blen*sind70;
				::MoveToEx(hDC,(int)xp1,(int)yp1,NULL);
				::LineTo(hDC,(int)xp2,(int)yp2);
				plen-=bgap;
				var-=10.0;
				flag = 0;
			}

			// [2.5,7.5)
			if (var>=2.5)
			{
				if (flag) plen-=bgap;
				xp1 = x + plen*cos(dWindDirection);
				yp1 = y + plen*sin(dWindDirection);
				xp2 = xp1 + 0.5*blen*cosd70;
				yp2 = yp1 + 0.5*blen*sind70;
				::MoveToEx(hDC,(int)xp1,(int)yp1,NULL);
				::LineTo(hDC,(int)xp2,(int)yp2);
			}
		}



		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		return TRUE;
	}
	// ��ʸ���ݰ� [P3-127]
	//     Vector Arrow Data Packet
	//     Packet Code = 5
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KVectorArrowList* pVectorArrowList)
	{
		return FALSE;
	}
	// ��������ʸ�����ݰ� [P3- 97][P3-105][P3-105]
	//     Linked Vector Packet
	//     Packet Code = 6,9,0x0e03
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KLinkedVectorList* pLinkedVectorList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm * 230 / dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(6.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetTextAlign(hDC,TA_BASELINE|TA_CENTER);

		KLinkedVectorList::iterator it;
		KLinkedVectorList::iterator ibeg = pLinkedVectorList->begin();
		KLinkedVectorList::iterator iend = pLinkedVectorList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// ȡ��һ������ʸ��ָ��
			LinkedVector* pLinkedVector = (*it);

			// ������ֵ�ߵ���ɫ�ȼ�
			int iColorLevel = pLinkedVector->iColorLevel;
			if(iColorLevel>=0 && (iColorLevel<iThreshold_min || iColorLevel>iThreshold_max))
			{
				continue;
			}

			// ������ɫ�ȼ�ȡ����ɫֵ
			COLORREF clr = RGB(201,201,201);
			if(iColorLevel != RPG_DATA_NO_COLOR_LEVEL)
			{
				clr = pLegend[iColorLevel].dwColor;
			}

			// ѡ�뻭��
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);
			::SetTextColor(hDC,clr);

			// ������ֵ�ߵĵ���
			int iNumberOfPoint = (int)pLinkedVector->oPointVector.size();
			// ����һ����ֵ��
			for(i=0;i<iNumberOfPoint;i++)
			{
				POINT pt = pLinkedVector->oPointVector[i];

				double x = 0;
				double y = 0;

				// km/4 ת��Ϊ���ص�λ��(�����״�����)
				x = (double)iXCenter + 0.25 * pt.x * dPixelPerKm * fZoom + 0.5;
				y = (double)iYCenter - 0.25 * pt.y * dPixelPerKm * fZoom + 0.5;
				if(iProductCode==88)
				{
				}

				if(i==0) // ��ͷ
				{
					::MoveToEx(hDC,(int)x,(int)y,NULL);
				}
				else
				{
					::LineTo(hDC,(int)x,(int)y);

					// ��� 58 �Ų�Ʒ
					if(m_bScitPast)
					{
						::TextOut(hDC,(int)x,(int)y,_T("��"),1);	
					}
					if(m_bScitForecast)
					{
						::TextOut(hDC,(int)x,(int)y,_T("��"),1);	
					}
				}
			}
		}

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);

		return TRUE;
	}
	// ���Ʒ����ӵ�ʸ������ͼ�� [P3-101][P3-102][P3-105]
	//     Unlinked Vector Packet
	//     Packet Code = 7,10,x3501
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(12.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		// ���Ʊ�ע�ı�
		KUnlinkedVectorList::iterator it;
		KUnlinkedVectorList::iterator ibeg = pUnlinkedVectorList->begin();
		KUnlinkedVectorList::iterator iend = pUnlinkedVectorList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// ȡ�õ�ɫ������ʸ�����ݰ�
			UnlinkedVector* pUnlinkedVector = (*it);

			// ��ɫ�ȼ�
			int iColorLevel = pUnlinkedVector->iColorLevel;

			// ������ɫ�ȼ�ȡ����ɫֵ
			COLORREF clr = 0;;
			if(iColorLevel != RPG_DATA_NO_COLOR_LEVEL)
			{
				clr = pLegend[iColorLevel].dwColor;
			}
			else
			{
				clr = RGB(192,192,192);	// �Զ�����ɫ
			}

			// ѡ�뻭��
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);

			int iSizeBeg = pUnlinkedVector->oPointVectorBeg.size();
			int iSizeEnd = pUnlinkedVector->oPointVectorEnd.size();

			if(iSizeBeg==iSizeEnd)
			{
				for(i=0;i<iSizeBeg;i++)
				{
					POINT ptBeg = pUnlinkedVector->oPointVectorBeg[i];
					POINT ptEnd = pUnlinkedVector->oPointVectorEnd[i];

					ptBeg.x = (int)(ptBeg.x*dPixelPerKm);
					ptBeg.y = (int)(ptBeg.y*dPixelPerKm);

					ptEnd.x = (int)(ptEnd.x*dPixelPerKm);
					ptEnd.y = (int)(ptEnd.y*dPixelPerKm);

					::MoveToEx(hDC,ptBeg.x,ptBeg.y,NULL);
					::LineTo(hDC,ptEnd.x,ptEnd.y);
				}
			}
		}

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);

		return TRUE;
	}
	// �������ݰ� [P3-125][P3-117]
	//     Radial Data Packet
	//     Packet Code = 16,0xAF1F
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRadialList* pRadialList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//TODO:==========================OK
		pRpgData->ToRangeBin();
		//BOOL b = DrawImage(hDC, lpRect, fZoom, pRpgData->GetRangeBinList());
		BOOL b = DrawImage(hDC, lpRect, fZoom, pRpgData->GetRangeBinPerRadialList());
		return b;
		//==========================

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(pRpgData->GetProductCode());

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		// ������ͼ�����ݰ�ͷ
		RadialDataPacketHeader* pRadialDataPacketHeader = pRpgData->GetRadialDataPacketHeader();

		int iIndexOfFirstRangeBin		= pRadialDataPacketHeader->iIndexOfFirstRangeBin;		// ��һ������������(0 to 460)
		int iNumberOfRangeBinsPerRadial	= pRadialDataPacketHeader->iNumberOfRangeBinsPerRadial;	// ���һ���������ݵľ������(1 to 460)
		int iICenterOfSweep				= pRadialDataPacketHeader->iICenterOfSweep;				// ɨ�����ĵ�I����(��λ:km/4 -2048 to 2047)
		int iJCenterOfSweep				= pRadialDataPacketHeader->iJCenterOfSweep;				// ɨ�����ĵ�J����(��λ:km/4 -2048 to 2047)
		int iScaleFactor				= pRadialDataPacketHeader->iScaleFactor;				// ÿ���������������(1 to 8000 ����:0.001)
		int iNumberOfRadials			= pRadialDataPacketHeader->iNumberOfRadials;			// ��������(1 to 400 ����:1)

		// ÿ���������������(���ǷŴ�����ͼ���С)0.998
		double dPixelPerBin = (0.001 * iScaleFactor) * dPixelPerKm * fZoom;

		KRadialList::iterator it;
		KRadialList::iterator ibeg = pRadialList->begin();
		KRadialList::iterator iend = pRadialList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// ȡ��һ������
			Radial* pRadial = (*it);

			// ���ߵĿ�ʼ�Ƕ�
			// ����ת90��
			// ��������x������Ϊ0��
			// ֱ��������y������Ϊ0��
			// �������90��
			double alf0 = pRadial->oEachRadialHeader.iRadialStartAngle * 0.1 - 90;	// -90 ��������ʾ0��

			// ����ÿ�����ڿ�ʼ�Ƕ���X��Y���������
			double dw0 = (dPixelPerBin * cos(alf0 * 3.14159 / 180));
			double dh0 = (dPixelPerBin * sin(alf0 * 3.14159 / 180));

			// ����ÿ�����ڿ�ʼ�Ƕ��ϵ���ʼ����
			double x0Beg = dXOrigin + dw0 * iIndexOfFirstRangeBin;
			double y0Beg = dYOrigin + dh0 * iIndexOfFirstRangeBin;

			// ���ߵĽ����Ƕ�
			double alf1 = alf0 + pRadial->oEachRadialHeader.iDeltaAngle * 0.1;	//����չ��
		
			// ����ÿ�����ڽ����Ƕ���X��Y���������
			double dw1 = dPixelPerBin * cos(alf1 * 3.14159 / 180);
			double dh1 = dPixelPerBin * sin(alf1 * 3.14159 / 180);

			// ����ÿ�����ڽ����Ƕ��ϵ���ʼ����
			double x1Beg = dXOrigin + dw1 * iIndexOfFirstRangeBin;
			double y1Beg = dYOrigin + dh1 * iIndexOfFirstRangeBin;

			POINT pt[4];

			pt[0].x = (LONG)(x0Beg+0.5);	//��������
			pt[0].y = (LONG)(y0Beg+0.5);
			pt[1].x = (LONG)(x1Beg+0.5);
			pt[1].y = (LONG)(y1Beg+0.5);

			// ������Ҫ���Ƶľ�����
			int iCurRun = 0;

			// ÿ�����ߵľ��������
			int iNumberOfRangeBinsPerRadial = pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;

			// ���Ƹ��������
			for(i=0;i<iNumberOfRangeBinsPerRadial;i++)
			{
				// ���
				int iRun = pRadial->pRunColor[i].iRun;

				iCurRun += iRun;	// ��ǰ���

				pt[3].x=(int)(x0Beg + dw0 * iCurRun + 0.5);	// pt[3]��pt[0]
				pt[3].y=(int)(y0Beg + dh0 * iCurRun + 0.5);	// ����ʼ�Ƕ���

				pt[2].x=(int)(x1Beg + dw1 * iCurRun + 0.5);	// pt[1]��pt[2]
				pt[2].y=(int)(y1Beg + dh1 * iCurRun + 0.5);	// �ڽ����Ƕ���

				// ����ɫ����
				int iColorIndex = pRadial->pRunColor[i].iColorIndex;
				if(iColorIndex>0 && iColorIndex>=iThreshold_min && iColorIndex<=iThreshold_max)
				{
					COLORREF clr = pLegend[iColorIndex].dwColor;

					::DeleteObject(hPen);
					hPen = CreatePen(PS_SOLID,1,clr);
					::SelectObject(hDC,hPen);

					::DeleteObject(hBrush);
					hBrush = ::CreateSolidBrush(clr);
					::SelectObject(hDC,hBrush);

					// ���ƾ��������
					::Polygon(hDC,pt,4);

				}

				// ����pt[0]��pt[2]
				pt[0].x=pt[3].x;
				pt[0].y=pt[3].y;
				pt[1].x=pt[2].x;
				pt[1].y=pt[2].y;

			}//for(i)

		}// for(it)

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		return TRUE;
	}
	// դ�����ݰ� [P3-121][P3-123][P3-119]
	//     Raster Data Packet
	//     Packet Code = 17,18,xBA07
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRasterRowList* pRasterRowList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		// ����դ������ͼ���ǲ�Ʒ���Ƿ�Χ�����Ѿ������� iXScaleINT �� iYScaleINT
		//if(dRange<230)
		//{
		//	dPixelPerKm = dPixelPerKm * 230 / dRange;
		//}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		// ��ֱ����(50)
		if(iProductCode==50)
		{
			fZoom = 1.0;
		}


		// ��դ��ͼ�����ݰ�ͷ
		RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();

		int iICoordStart	= pRasterDataPacketHeader->iICoordinateStart;	//���ݿ�ʼ����λ��
		int iJCoordStart	= pRasterDataPacketHeader->iJCoordinateStart;	//���ݿ�ʼ����λ��
		double iXScaleINT	= pRasterDataPacketHeader->iXScaleINT;			//X����һ��Ҫ��ֵռ�ĸ����
		double iYScaleINT	= pRasterDataPacketHeader->iYScaleINT;			//Y����һ��Ҫ��ֵռ�ĸ����
		int iNumberOfRows	= pRasterDataPacketHeader->iNumberOfRows;		//����������

		iXScaleINT	= ((double)iXScaleINT * dPixelPerKm * fZoom);		// X����һ��Ҫ��ֵռ�ĸ����
		iYScaleINT	= ((double)iYScaleINT * dPixelPerKm * fZoom);		// Y����һ��Ҫ��ֵռ�ĸ����

		// դ����������(��ǰ��)
		int iRasterRowNumberInCurrentLayer = (int)pRasterRowList->size();

		// ����ȡ����ĳ��ֵ����С����
		double delt = 0.9;

		int iCurLine = 0;
		KRasterRowList::iterator it;
		KRasterRowList::iterator ibeg = pRasterRowList->begin();
		KRasterRowList::iterator iend = pRasterRowList->end();
		for(iCurLine=0,it=ibeg;it!=iend,iCurLine<iRasterRowNumberInCurrentLayer;it++,iCurLine++)
		{
			// ȡ��һ������
			RasterRow* pRasterRow = (*it);

			// �������ֽ���
			int iNumberOfBytesInThisRow = pRasterRow->iNumberOfBytesInThisRow;

			int iMarginLeft	= (int)(dwWidthImage - (int)(iRasterRowNumberInCurrentLayer * iXScaleINT + delt)) / 2;
			int iMarginTop	= (int)(dwHeightImage - (int)(iRasterRowNumberInCurrentLayer * iYScaleINT + delt)) / 2;

			// ���Ͻǵĵ�
			int xx0 = iMarginLeft;
			int yy0 = iMarginTop;

			// ��ֱ����(50)
			if (iProductCode == 50)
			{
				xx0 = (int)(1.0 * iICoordStart * dPixelPerKm * fZoom);
				yy0 = (int)(1.0 * iJCoordStart * dPixelPerKm * fZoom);
			}

			int iCurRun = 0;	// ��ǰ���

			// һ�е��ֽ���
			for(i=0;i<iNumberOfBytesInThisRow;i++)
			{
				int iRun = pRasterRow->pRunColor[i].iRun;				// ���(��)
				int iColorIndex = pRasterRow->pRunColor[i].iColorIndex;	// �����ɫ

				COLORREF clr = pLegend[iColorIndex].dwColor;

				iCurRun += iRun;

				if(iColorIndex<=0 || iColorIndex<iThreshold_min || iColorIndex>iThreshold_max)
				{
					//continue;
				}

				int xx1 = xx0 + (int)(iCurRun * iXScaleINT + delt);		// ��ǰ��
				int yy1 = yy0 + (int)(iCurLine * iYScaleINT + delt);	// ��ǰ��

				for(int nx=0;nx<(int)(iRun*iXScaleINT+delt);nx++)		//X���ظ���
				{
					for(int ny=0;ny<(int)(iYScaleINT+delt);ny++)		//Y���ظ���
					{
						::SetPixel(hDC,xx1-nx-1,yy1+ny,clr);
					}//--ny
				}//--nx

			}// for(i)

			//=====================================================
			//if(iCurLine == 0) break;
			//=====================================================

		}// for(iCurLine,it)

		iCurLine = 5;

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		return TRUE;
	}
	// �����������ݰ� [P3-130]
	//     HDA Hail
	//     Packet Code = 19
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KHdaHailList* pHdaHailList)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(pRpgData->GetProductCode());

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange<230)
		{
			dPixelPerKm = dPixelPerKm * 230 / dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(12.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetBkColor(hDC,clrBack);

		//���Ʊ�ע�ı�
		KHdaHailList::iterator it;
		KHdaHailList::iterator ibeg = pHdaHailList->begin();
		KHdaHailList::iterator iend = pHdaHailList->end();
		for(it=ibeg;it!=iend;it++)
		{
			HdaHail* pHdaHail = (*it);

			// λ��
			int x,y;

			if(iProductCode == 59)
			{
				// �� km/4 ת��Ϊ��Ļ����
				x = 256 + (int)(pHdaHail->oPosition.I / 4 * fZoom);
				y = 256 - (int)(pHdaHail->oPosition.J / 4 * fZoom);
			}
			else
			{
				// ���������Ļ����
				x = (int)(pHdaHail->oPosition.I * fZoom);
				y = (int)(pHdaHail->oPosition.J * fZoom);
			}

			x = (int)((x + 12) * dPixelPerKm);
			y = (int)(y * dPixelPerKm);

			// ����������ɫ
			COLORREF clrProbabilityOfHail = RGB(192,192,192);
			// ǿ����������ɫ
			COLORREF clrProbabilityOfSevereHail = RGB(192,0,0);
			// �������ߴ���ɫ
			COLORREF clrMaxHailSize = RGB(0,192,255);

			sfx_int16	iProbabilityOfHail			= pHdaHail->iProbabilityOfHail;			// ��������
			sfx_int16	iProbabilityOfSevereHail	= pHdaHail->iProbabilityOfSevereHail;	// ǿ��������
			sfx_uint16	iMaxHailSize				= pHdaHail->iMaxHailSize;				// �������ߴ�(Ԥ��)

			char szText[32]						= "";
			char szProbabilityOfHail[16]		= "";	// ��������
			char szProbabilityOfSevereHail[16]	= "";	// ǿ��������
			char szMaxHailSize[16]				= "";	// �������ߴ�(Ԥ��)

			sprintf(szText,						"(%d,%d,%d)",iProbabilityOfHail,iProbabilityOfSevereHail,iMaxHailSize);
			sprintf(szProbabilityOfHail,		"%d",iProbabilityOfHail);
			sprintf(szProbabilityOfSevereHail,	"%d",iProbabilityOfSevereHail);
			sprintf(szMaxHailSize,				"%d",iMaxHailSize);

			::SetTextColor(hDC,clrProbabilityOfHail);
			::SetTextAlign(hDC,TA_TOP | TA_LEFT);
			if(iProbabilityOfHail>0 || iProbabilityOfSevereHail>0 || iMaxHailSize>0)
			{
				::TextOutA(hDC,x,y,szText,strlen(szText));
			}
		}

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);

		return TRUE;
	}
	// �籩������Ϣ·�����ݰ� [P3-130]
	//     SCIT Past Data
	//     Packet Code = 23,24
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KScitList* pScitList)
	{
		// ���Ʒ籩������Ϣ·��
		KScitList::iterator it;
		KScitList::iterator ibeg = pScitList->begin();
		KScitList::iterator iend = pScitList->end();
		for(it=ibeg;it!=iend;it++)
		{
			Scit* pScit = (*it);

			// �������
			// Pack Code = 2
			if(!this->DrawImage(hDC,lpRect,fZoom,&pScit->oSpecilaSymbolList))
			{
				return FALSE;
			}
			// ��ɫ����ʸ��
			// Pack Code = 6
			if(!this->DrawImage(hDC,lpRect,fZoom,&pScit->oMonoLinkedVectorList))
			{
				return FALSE;
			}
			// �籩Ȧ
			// Pack Code = 25
			if(!this->DrawImage(hDC,lpRect,fZoom,&pScit->oStiCircleList))
			{
				return FALSE;
			}
		}

		return TRUE;
	}
	// �籩·����ϢȦ���ݰ� [P3-130]
	//     STI Circle
	//     Packet Code = 25
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KStiCircleList* pStiCircleList)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ͼ��ߴ�
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// Ĭ��ͼ����Ϊ512����
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// Ĭ��ͼ��߶�Ϊ512����

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(pRpgData->GetProductCode());

		// ���ھ�������ͼ���ǲ�Ʒ���Ƿ�Χ
		if(dRange<230)
		{
			dPixelPerKm = dPixelPerKm * 230 / dRange;
		}

		// ������������(�豸����)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// ԭ������
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// ��ֵ
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(12.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkColor(hDC,clrBack);

		// ���Ʊ�ע�ı�
		KStiCircleList::iterator it;
		KStiCircleList::iterator ibeg = pStiCircleList->begin();
		KStiCircleList::iterator iend = pStiCircleList->end();
		for(it=ibeg;it!=iend;it++)
		{
			StiCircle* pStiCircle = (*it);

			// ������ɫ
			COLORREF clr = RGB(255,0,0);
			int index =0;

			//if(m_pOwner->IsScitPast())
			//{
			//	clr = RGB(0,252,0);
			//	index = HS_BDIAGONAL;
			//}

			//if(m_pOwner->IsScitForecast())
			//{
			//	clr = RGB(252,0,0);
			//	index = HS_HORIZONTAL;
			//}

			// ѡ�뻭��
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);

			// ѡ�뻭ˢ
			::DeleteObject(hBrush);
			hBrush = ::CreateHatchBrush(HS_BDIAGONAL,clr);
			::SelectObject(hDC,hBrush);

			// Բ��
			POINT pt;
			pt.x = pStiCircle->oPosition.I;
			pt.y = pStiCircle->oPosition.J;

			if(iProductCode == 58)
			{
				//�� km/4 ת��Ϊ��Ļ����
				pt.x = 256 + (int)(pStiCircle->oPosition.I / 4 * fZoom);
				pt.y = 256 - (int)(pStiCircle->oPosition.J  /4 * fZoom);
			}
			else
			{
				//���������Ļ����
				pt.x = (int)(pStiCircle->oPosition.I * fZoom);
				pt.y = (int)(pStiCircle->oPosition.J * fZoom);
			}

			pt.x = (int)(pt.x * dPixelPerKm);
			pt.y = (int)(pt.y * dPixelPerKm);

			//Բ�뾶(Pixel)
			int iRadiusOfCircle = (int)((double)pStiCircle->iRadiusOfCircle * dPixelPerKm * fZoom);

			int x0 = pt.x;
			int y0 = pt.y;

			int l = x0 - iRadiusOfCircle;
			int t = y0 - iRadiusOfCircle;
			int r = x0 + iRadiusOfCircle;
			int b = y0 + iRadiusOfCircle;

			// ����Բ
			::Ellipse(hDC,l,t,r,b);
		}

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);

		return TRUE;
	}
	//���ƾ��������
	//��Ҫ����Radial��Raster����
	BOOL KRpgDataOperator::DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinList* pRangeBinList)
	{
		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if (pRpgData->IsEmpty())
			return FALSE;

		KDataInfo* pDataInfo = pRpgData->GetDataInfo();
		if (pDataInfo == NULL)
			return FALSE;

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();
		if (pLegend == NULL)
			return FALSE;

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);

		// ��ֵ
		int iThreshold_min(0), iThreshold_max(0);
		pRpgData->GetThreshold(&iThreshold_min, &iThreshold_max);


		// �ֲ�����
		int row, col;	//
		double xx, yy;	//
		POINT pt[4];	// ���������

		int iHeight = pDataInfo->yNum;

		KRangeBinList::iterator itr;
		KRangeBinList::iterator itrBeg = pRangeBinList->begin();
		KRangeBinList::iterator itrEnd = pRangeBinList->end();
		for (itr = itrBeg; itr != itrEnd; itr++)
		{
			//�����
			RangeBin* pRangeBin = *itr;

			// ����ɫ����(��ֵ)
			int iColorIndex = pRangeBin->iColorIndex;
			if (iColorIndex<iThreshold_min || iColorIndex>iThreshold_max)
				continue;

			for (int k = 0; k < 4; k++)
			{
				xx = pRangeBin->x[k];
				yy = pRangeBin->y[k];

				LonLat2RowCol(pDataInfo, xx, yy, row, col);

				pt[k].x = (LONG)col;
				pt[k].y = (LONG)iHeight - row;

			}

			COLORREF clr = pLegend[iColorIndex].dwColor;

			::DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, 1, clr);
			::SelectObject(hDC, hPen);

			::DeleteObject(hBrush);
			hBrush = ::CreateSolidBrush(clr);
			::SelectObject(hDC, hBrush);


			// ���ƾ��������
			::Polygon(hDC, pt, 4);

		}//for (itr = itrBeg; itr != itrEnd; itr++)

		return TRUE;
	}

	BOOL KRpgDataOperator::DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinPerRadialList* pRangeBinPerRadialList)
	{
		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if (pRpgData->IsEmpty())
			return FALSE;

		KDataInfo* pDataInfo = pRpgData->GetDataInfo();
		if (pDataInfo == NULL)
			return FALSE;

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();
		if (pLegend == NULL)
			return FALSE;

		//������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);

		// ��ֵ
		int iThreshold_min(0), iThreshold_max(0);
		pRpgData->GetThreshold(&iThreshold_min, &iThreshold_max);


		// �ֲ�����
		int row, col;	//
		double xx, yy;	//
		POINT pt[4];	// ���������

		int iHeight = pDataInfo->yNum;

		//=========================================
		int n = 214;
		int nDrw = 0;			//��������

		int nBeg = n;			//��ʼ����
		int nEnd = n + nDrw;	//��������

		int nCur = 0;			//��ǰ����
		int nDrwed = 0;			//�Ѿ���������
		TCHAR szBuffer[MAX_PATH] = _T("");

		//this->out(NULL, pRangeBinPerRadialList);
		//=========================================

		KRangeBinPerRadialList::iterator itr;
		KRangeBinPerRadialList::iterator itrBeg = pRangeBinPerRadialList->begin();
		KRangeBinPerRadialList::iterator itrEnd = pRangeBinPerRadialList->end();
		for (itr = itrBeg; itr != itrEnd; itr++)
		{
			RangeBinPerRadial* pRangeBinPerRadial = *itr;

#ifdef _DEBUG
			//=============================================
			nCur++;

			if (nCur < nBeg)
				continue;
			if (nCur > nEnd)
				break;

			//if (nCur - nBeg >= nDrw)
			//	break;

			nDrwed++;

			int size = pRangeBinPerRadial->pRangeBinList->size();
			_stprintf(szBuffer, _T("%03d\t%d\n"), nCur, size);
			OutputDebugString(szBuffer);
			//=============================================
#endif

			KRangeBinList* pRangeBinList = pRangeBinPerRadial->pRangeBinList;

			KRangeBinList::iterator itr1;
			KRangeBinList::iterator itrBeg1 = pRangeBinList->begin();
			KRangeBinList::iterator itrEnd1 = pRangeBinList->end();
			for (itr1 = itrBeg1; itr1 != itrEnd1; itr1++)
			{
				//�����
				RangeBin* pRangeBin = *itr1;

				// ����ɫ����(��ֵ)
				int iColorIndex = pRangeBin->iColorIndex;
				if (iColorIndex<iThreshold_min || iColorIndex>iThreshold_max)
					continue;

				for (int k = 0; k < 4; k++)
				{
					xx = pRangeBin->x[k];
					yy = pRangeBin->y[k];

					LonLat2RowCol(pDataInfo, xx, yy, row, col);

					pt[k].x = (LONG)col;
					pt[k].y = (LONG)iHeight - row;

				}

				COLORREF clr = pLegend[iColorIndex].dwColor;

				::DeleteObject(hPen);
				hPen = CreatePen(PS_SOLID, 1, clr);
				::SelectObject(hDC, hPen);

				::DeleteObject(hBrush);
				hBrush = ::CreateSolidBrush(clr);
				::SelectObject(hDC, hBrush);


				// ���ƾ��������
				::Polygon(hDC, pt, 4);

			}//for (itr1 = itrBeg1; itr1 != itrEnd1; itr1++)


		}//for (itr = itrBeg; itr != itrEnd; itr++)

		return TRUE;
	}

	//���ÿ�������������Ϣ
	BOOL KRpgDataOperator::out(LPCTSTR lpszFile, KRangeBinPerRadialList* pRangeBinPerRadialList)
	{
		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if (pRpgData->IsEmpty())
			return FALSE;

		KDataInfo* pDataInfo = pRpgData->GetDataInfo();
		if (pDataInfo == NULL)
			return FALSE;


		// �ֲ�����
		int row, col;	//
		double xx, yy;	//
		POINT pt[4];	//���������

		int nCur = 0;			//��ǰ����
		TCHAR szBuffer[1024] = _T("");

		int iHeight = pDataInfo->yNum;

		lpszFile = _T("D:\\Temp\\1.txt");
		FILE* fp = _tfopen(lpszFile, _T("wt"));
		if (fp == NULL)
			return FALSE;

		KString strLine = _T("");

		KRangeBinPerRadialList::iterator itr;
		KRangeBinPerRadialList::iterator itrBeg = pRangeBinPerRadialList->begin();
		KRangeBinPerRadialList::iterator itrEnd = pRangeBinPerRadialList->end();
		for (itr = itrBeg; itr != itrEnd; itr++)
		{
			RangeBinPerRadial* pRangeBinPerRadial = *itr;

			nCur++;
			int size = pRangeBinPerRadial->pRangeBinList->size();
			_stprintf(szBuffer, _T("%03d\t%d"), nCur, size);
			//OutputDebugString(szBuffer);

			strLine.Clear();
			strLine.Append(szBuffer);

			//OutputDebugString(strLine);

			KRangeBinList* pRangeBinList = pRangeBinPerRadial->pRangeBinList;

			KRangeBinList::iterator itr1;
			KRangeBinList::iterator itrBeg1 = pRangeBinList->begin();
			KRangeBinList::iterator itrEnd1 = pRangeBinList->end();
			for (itr1 = itrBeg1; itr1 != itrEnd1; itr1++)
			{
				//�����
				RangeBin* pRangeBin = *itr1;

				float fv = pRangeBin->fValue;

				strLine.Append(_T("\t"));
				_stprintf(szBuffer, _T("%.0f"), pRangeBin->fValue);
				strLine.Append(szBuffer);

				for (int k = 0; k < 4; k++)
				{
					xx = pRangeBin->x[k];
					yy = pRangeBin->y[k];

					LonLat2RowCol(pDataInfo, xx, yy, row, col);

					pt[k].x = (LONG)col;
					pt[k].y = (LONG)iHeight - row;

				}

			}//for (itr1 = itrBeg1; itr1 != itrEnd1; itr1++)

			//OutputDebugString(strLine);

			int len = strLine.GetLength();
			strLine.Append(_T("\n"));
			//_stprintf(szBuffer, _T("%s"), (LPCTSTR)strLine);
			_ftprintf(fp, strLine);

		}//for (itr = itrBeg; itr != itrEnd; itr++)

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	/// �������ͼ�� - End
	///--------------------------------------------------------
#pragma endregion

	///===============
	///����ͼ�� - end
	///========================================================

	// ���Ƽ�����
	void KRpgDataOperator::DrawPolar(HDC hDC, LPRECT pRect,float fZoom)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		//ͼ������������
		double dXCenter = pRect->right / 2;
		double dYCenter = pRect->bottom / 2;

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(255,161,123));
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(clrBack);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,::GetStockObject(NULL_BRUSH));

		double dPixel;
		double x1,y1,x2,y2;

		// ����5������Ȧ
		int iNum = 5;
		for(i=0;i<iNum;i++)
		{
			// 50�����ر�ʾ50����
			dPixel = 50.0 * (i + 1) * dPixelPerKm * fZoom;

			// ���ε����Ϻ����½�����
			x1 = dXCenter - dPixel;
			y1 = dYCenter - dPixel;

			x2 = dXCenter + dPixel;
			y2 = dYCenter + dPixel;

			// ���һȦ����30����
			if(i==iNum-1)
			{
				int iTemp = 20;	// 20���� 50-20 = 30km (��Ϊ230km)

				x1 += iTemp * dPixelPerKm * fZoom;
				y1 += iTemp * dPixelPerKm * fZoom;

				x2 -= iTemp * dPixelPerKm * fZoom;
				y2 -= iTemp * dPixelPerKm * fZoom;
			}

			// ������Բ
			::Ellipse(hDC,(int)x1,(int)y1,(int)x2,(int)y2);

			// �� i=iNum-1 ʱ���Ƽ����꾶����
			if(i==iNum-1)
			{
				//0.4 = 20/50
				dPixel = 50.0*((double)iNum-0.4)*dPixelPerKm*fZoom;

				for(double angle=0.0; angle<360.0; angle+=30.0)
				{
					double dx = dPixel*cos(angle*3.14159/180);
					double dy = dPixel*sin(angle*3.14159/180);

					double x3 = dXCenter + dx+0.5;
					double y3 = dYCenter - dy+0.5;

					::MoveToEx(hDC,(int)dXCenter,(int)dXCenter,NULL);
					::LineTo(hDC,(int)x3,(int)y3);
				}//end for angle
			}
		}

		// �����м��СԲȦ
		::SelectObject(hDC,hBrush);
		dPixel = 5.0*dPixelPerKm*fZoom;

		x1 = dXCenter-dPixel;
		y1 = dYCenter-dPixel;

		x2 = dXCenter+dPixel;
		y2 = dYCenter+dPixel;

		// ������Բ
		::Ellipse(hDC,(int)x1,(int)y1,(int)x2,(int)y2);

		// Cleanup GDI
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);
	}

	// ���Ƶѿ�������(����ͼ)
	void KRpgDataOperator::DrawDescartes(HDC hDC, LPRECT pRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList,KWriteTextList* pWriteTextList)
	{
		// 1 ����ֱ������
		this->DrawImage(hDC,pRect,fZoom,pUnlinkedVectorList);

		 // 2 ���Ʊ�ע�ı�
		this->DrawImage(hDC,pRect,fZoom,pWriteTextList);
	}

	// ���Ʋ�Ʒ��Ϣ
	void KRpgDataOperator::DrawInfoArea(HDC hDC, LPRECT pRect)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 1. ׼����Ϣ��
		// ��䱳��ɫ
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,::GetStockObject(NULL_BRUSH));
		::FillRect(hDC,pRect,hBrush);

		// ������Ϣ������
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(128,128,128));
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);
		::Rectangle(hDC,pRect->left+2,pRect->top+1,pRect->right-1,pRect->bottom-1);

		// 2. ���Ʋ�Ʒ��Ϣ�ı�
		RECT rectText = *pRect;
		rectText.left += 6;
		rectText.top += 4;
		rectText.right -= 1;
		rectText.bottom -= 1;

		DrawInfoText(hDC,&rectText);	// ���Ʋ�Ʒ��Ϣ�ı�

		// 3. ���Ʋ�Ʒ��Ϣɫ��
		int iLineHeight = (int)(14.0*dPixelPerKm);

		RECT rectHue = *pRect;
		//rectHue.top = rectHue.top + iLineHeight * 25 + 3;
		rectHue.top = rectHue.top + iLineHeight * 25;

		DrawInfoHue(hDC,&rectHue);		// ���Ʋ�Ʒ��Ϣɫ��

		// Cleanup GDI
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

	}

	// ���Ʋ�Ʒ��Ϣ�ı�
	void KRpgDataOperator::DrawInfoText(HDC hDC, LPRECT pRect)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// �״��Ʒ��Ϣ
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		// ��Ʒ����
		int iProductCode = pRadarInfo->iProductCode;

		// GDI Init
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(12.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetBkColor(hDC,clrBack);

		RECT rectText = *pRect;
		int xTemp,yTemp;
		TCHAR szTemp[MAX_PATH] = _T("");
		double dTemp[3];

		// �о�
		int iLineHeight = (int)(14.0 * dPixelPerKm);
		int iLine=0;
		//----------------------------------------------------
		// ������Ϣ - Beg

		// 1.�״��Ʒ������Ϣ
		// 1.1 ��Ʒ��д
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("��Ʒ: %s"),pRadarInfo->szContraction);

		::SetTextColor(hDC,RGB(254,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.2 ��Ʒ����
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("����: %s"),pRadarInfo->szCnName);

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.3 ���Ƿ�Χ(km)
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;

		if(pRadarInfo->dRange == 0)
		{
			_stprintf(szTemp,_T("��Χ: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("��Χ: %.0f km"),pRadarInfo->dRange);
		}

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.4 �ֱ���(km)
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;

		if(fabs(pRadarInfo->dResolution)<1.0e-12)
		{
			_stprintf(szTemp,_T("����: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("����: %.2f km"),pRadarInfo->dResolution);
		}

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.5 ����
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("����: %4d-%02d-%02d"),pRadarInfo->wYear,pRadarInfo->wMonth,pRadarInfo->wDay);

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.6 ʱ��
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("ʱ��: %02d:%02d:%02d"),pRadarInfo->wHour,pRadarInfo->wMinute,pRadarInfo->wSecond);

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		iLine++;	// ��һ��

		// 2 �״���Ϣ
		// 2.1 �״�վ��
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("�״�: %s(%d)"),pRadarInfo->szStationName,pRadarInfo->id);

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.2 ����
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("����: %.2f m"),pRadarInfo->dAlt);

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.3 γ��
		dTemp[0] = pRadarInfo->dLat;				// ����
		dTemp[1] = (dTemp[0]-(int)dTemp[0]) * 60.0;	// ��
		dTemp[2] = (dTemp[1]-(int)dTemp[1]) * 60.0;	// ��

		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("γ��: %d/%d/%d %s"),(int)dTemp[0],(int)dTemp[1],(int)dTemp[2],dTemp[0]>0 ? "N" : "S");

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		//2.4 ����
		dTemp[0] = pRadarInfo->dLon;				// ����
		dTemp[1] = (dTemp[0]-(int)dTemp[0]) * 60.0;	// ��
		dTemp[2] = (dTemp[1]-(int)dTemp[1]) * 60.0;	// ��

		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("����: %d/%d/%d %s"),(int)dTemp[0],(int)dTemp[1],(int)dTemp[2],dTemp[0]>0 ? "E" : "W");

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.5 �״�����
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		if(iProductCode==50)
		{
			_stprintf(szTemp,_T("����: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("����: 0deg 0km"));
		}

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.6 ������
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		if(iProductCode == 48 || iProductCode == 50 || iProductCode == 53)
		{
			_stprintf(szTemp,_T("����: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("����: %.0fkm 30deg"),pRadarInfo->dPolarRange);
		}

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		iLine++;	// ��һ��

		// 2.7 ����ģʽ
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("����ģʽ: %s"),pRadarInfo->szCnOPModeName);

		::SetTextColor(hDC,RGB(200,96,250));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.8 ��ɨģʽ
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("��ɨģʽ: VCP %d"),pRadarInfo->iVCP);

		::SetTextColor(hDC,RGB(200,96,250));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		iLine++;	//��һ��

		// 3 �״��Ʒ������Ϣ
		for(int p=0;p<7;p++)
		{
			xTemp = rectText.left;
			yTemp = rectText.top + iLineHeight * iLine++;
			_stprintf(szTemp,_T("%s"),pRadarInfo->szSpecialInfo[p]);

			::SetTextColor(hDC,RGB(0,192,192));
			::SetTextAlign(hDC,TA_TOP | TA_LEFT);
			::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));
		}

		// ������Ϣ - End
		//----------------------------------------------------

		// GDI Cleanup
		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);
	}

	// ���Ʋ�Ʒ��Ϣɫ��
	void KRpgDataOperator::DrawInfoHue(HDC hDC, LPRECT pRect)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// ������ɫ
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// ÿ����������
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// �״��Ʒ��Ϣ
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		// ��Ʒ����
		int iProductCode = pRadarInfo->iProductCode;

		// ͼ��ָ��
		Legend* pLegend = pRpgData->GetLegend();

		// ��Ϣɫ��������
		RECT rectHue = *pRect;

		// GDI Init
		HPEN hPen = (HPEN)::GetStockObject(NULL_PEN);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(10.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("����"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetBkColor(hDC,clrBack);

		RECT rectText = *pRect;
		int xTemp,yTemp;
		TCHAR szTemp[MAX_PATH] = _T("");

		int iHCLR = (int)(9.0*dPixelPerKm);	// ɫ��߶�
		int iWCLR = (int)(32.0*dPixelPerKm);	// ɫ����

		// ɫ����ɫ�����
		RECT rectCLR = rectHue;
		rectCLR.left += 6;
		rectCLR.top += 1;
		rectCLR.right -= 1;
		rectCLR.bottom -= 1;

		rectCLR.top += 9;
		rectCLR.right = rectCLR.left + iWCLR;

		// ���ݼ���(ɫ������)
		int iDataLevel = pRpgData->GetProductDataLevel(pRpgData->GetProductCode());
		iDataLevel = iDataLevel>16 ? 16 : iDataLevel;

		for(int i=0;i<iDataLevel;i++)
		{
			// ɫ����ɫ
			COLORREF clrCLR = pRpgData->GetLegend(i)->dwColor;
			// ɫ���Ӧֵ
			double dValue = (*(pLegend+i)).dValue;

			// ����ɫ����ɫ����ε�
			rectCLR.bottom = rectCLR.top + iHCLR;

			::DeleteObject(hBrush);
			hBrush = ::CreateSolidBrush(clrCLR);
			::SelectObject(hDC,hBrush);

			// ����ɫ����ɫ��
			::FillRect(hDC,&rectCLR,hBrush);	// ɫ��

			// ɫ��ֵ����
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,RGB(128,128,128));
			::SelectObject(hDC,hPen);

			// ���ƶ���
			::MoveToEx(hDC,rectCLR.right,rectCLR.top,NULL);
			::LineTo(hDC,rectCLR.right+(int)(8.0*dPixelPerKm),rectCLR.top);

			// ����ɫ��ֵ
			xTemp = rectCLR.right+(int)(8.0*dPixelPerKm)+(int)((8.0*5.0)*dPixelPerKm);
			yTemp = rectCLR.top+(int)(4.0*dPixelPerKm);		//

			if( !(fabs(dValue-RPG_DATA_INVALID_VALUE)<1.0e-12) )
				_stprintf(szTemp,_T("%s"),(*(pLegend+i)).szText);

			::SetTextColor(hDC,RGB(0,192,192));
			::SetTextAlign(hDC,TA_BASELINE | TA_RIGHT);
			::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

			// �ڵڶ���ɫ����ɫ����ע��ֵ��λ
			if(i==2)
			{
				_stprintf(szTemp,_T("%s"),pRpgData->GetProductUnit(iProductCode));

				xTemp += (int)(8.0*dPixelPerKm);

				::SetTextColor(hDC,RGB(0,192,192));
				::SetTextAlign(hDC,TA_BASELINE | TA_LEFT);
				::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));	// ɫ���ʾ
			}

			// ����ɫ����ɫ����ζ�
			rectCLR.top = rectCLR.bottom;
		}

		// GDI Cleanup
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);
	}

	// ���ɸ������
	// 1.������װ������ - LoadData(...)
	// 2.������RadialImage����RasterImage����
	// Threshold = [0,15] ��16��
	//
	//������
	//	pData			- ���������ݵ�ָ��(�����������ڴ�,�����߸����ڴ����)
	//	dataInfo		- ����������Ϣ
	//	iThreshold_min	- ��ֵ����(0-15)
	//	iThreshold_max	- ��ֵ����(0-15)
	//	skip			- �������(0,1,2)
	//�÷���
	//	double* p = NULL;
	//	KDataInfo di;
	//	ToGrid(p,di,0,15,0);
	BOOL KRpgDataOperator::ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min, int iThreshold_max, int skip)
	{
		// ���봫���ָ��:
		//		double* p = NULL;
		//		KDataInfo di;
		//		ToGrid(p,di,...);
		// �ڴ汾������䣬����
		// �����ɵ����߻���
		//Assert(pData==NULL);
		if (pData)
		{
			delete[]pData;
			pData = NULL;
		}

		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// �Ƿ�Ϊ��
		if(pRpgData->IsEmpty())
			return FALSE;

		// ������ֵ
		pRpgData->SetThreshold(iThreshold_min, iThreshold_max);

		// �жϲ�Ʒ����(ֻ��ĳЩ���ݸ�㻯��������)
		// ������RadialImage����RasterImage����

		// ��Ʒ����
		int iProductCode = pRpgData->GetProductCode();

		// ȡ�ò�Ʒ����
		ProductType eProductType = pRpgData->GetProductType(iProductCode);

		// ������RadialImage����RasterImage����
		if (eProductType != ProductType_RadialImage && eProductType != ProductType_RasterImage)
			return FALSE;

		COLORREF clrBack = 0;

		// ���첻����Ϣ����Image
		pRpgData->SetImageBkColor(clrBack);		// ������ɫ
		pRpgData->SetImageWidth(512);		// 512 X 512 ����
		pRpgData->SetImageHeight(512);		// 512 X 512 ����
		if(!BuildImage(1,FALSE,FALSE))		// ����ͼ��
			return FALSE;

		// skip �Ǽ������ 0 ��û�м��
		if (skip < 0)
			skip = 0;

		skip += 1;

		// �״��Ʒ��Ϣ
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		// ���ľ�γ��
		double dCenterX	= pRadarInfo->dLon;
		double dCenterY	= pRadarInfo->dLat;

		// ���Ƿ�Χ(km)
		double dRange = pRadarInfo->dRange;

		// 1.1����=0.008979��γ�� 1.1/111.37
		double dDegreePerKm	= 0.0098769866211726676842955912723355;

		// ÿ�����ش���ľ�γ��
		double dDegreePerPixel	= dDegreePerKm * dRange / 230;

		// GDI
		// Ϊ����ͼ��׼��DC
		HDC hDC = ::GetDC(NULL);	// ȡ������DC���(ע���ͷ�)
		// �������� DC(ע���ͷ�)
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		// ѡ��λͼ���
		::SelectObject(hImageDC,pRpgData->GetImageHandle());

		// 
		int i,j;
		COLORREF clr(0);
		double dValue(0);
		pData = new double[512*512];

		// Ҫ����ֵ
		double dThresholdMin = pRpgData->GetDataLevelThreshold(iThreshold_min)->dThreshold;
		double dThresholdMax = pRpgData->GetDataLevelThreshold(iThreshold_max)->dThreshold;

		for (i = 0; i < 512; i += skip)
		{
			for (j = 0; j < 512; j += skip)
			{
				clr = ::GetPixel(hImageDC, j, 511 - i);	//��ͼ�����½ǿ�ʼ

				if (clr != clrBack && clr != CLR_INVALID)
				{
					dValue = Color2Value(clr);			//��ɫֵת����������ֵ
				}
				else
				{
					dValue = 0;
				}

				*(pData + i * 512 + j) = dValue;

			}
		}

		// GDI Cleanup
		::DeleteDC(hImageDC);
		::ReleaseDC(NULL,hDC);

		// ���������Ϣ KDataInfo
 		//memset(&dataInfo,0,sizeof(KDataInfo));

		//������½Ǿ�γ��
		double x0 = dCenterX - dDegreePerPixel * 512 / 2;
		double y0 = dCenterY - dDegreePerPixel * 512 / 2;

		//��γ�ȼ��
		double dXInterval = dDegreePerPixel+dDegreePerPixel*(skip-1);
		double dYInterval = dDegreePerPixel+dDegreePerPixel*(skip-1);

		// 
		dataInfo.xNum = 512;
		dataInfo.xMin = dCenterX - dDegreePerPixel * 512 / 2;
		dataInfo.xMax = dCenterX + dDegreePerPixel * 512 / 2;
		dataInfo.xInterval = dXInterval;

		dataInfo.yNum = 512;
		dataInfo.yMin = dCenterY - dDegreePerPixel * 512 / 2;
		dataInfo.yMax = dCenterY + dDegreePerPixel * 512 / 2;
		dataInfo.yInterval = dYInterval;

		dataInfo.wYear = pRadarInfo->wYear;
		dataInfo.wMonth = pRadarInfo->wMonth;
		dataInfo.wDay = pRadarInfo->wDay;
		dataInfo.wHour = pRadarInfo->wHour;
		dataInfo.wMinute = pRadarInfo->wMinute;
		dataInfo.wSecond = pRadarInfo->wSecond;

		//_tcscpy(dataInfo.szComment,pRadarInfo->szComment);
		dataInfo.SetComment(pRadarInfo->szComment);

		// �����ֵ������

		return TRUE;
	}

	// ���ɸ������
	// ���浽�ļ� lpszFile ��
	BOOL KRpgDataOperator::ToGrid(LPCTSTR lpszFile,int iThreshold_min,int iThreshold_max,int skip)
	{
		int i,j,k;

		double* pData = NULL;
		KDataInfo di;

		if(!this->ToGrid(pData,di,iThreshold_min,iThreshold_max,skip))
		{
			delete []pData;
			pData = NULL;
			return FALSE;
		}

		if(skip==0)
			skip = 1;

		/// д���ļ�
		///
		// �����ֵ�߼������С�����ֵ
		double cinterval,cmin(999),cmax(-999);
		for(i=0;i<512;i+=skip)
		{
			for(j=0;j<512;j+=skip)
			{
				cmin = min(cmin,*(pData+i*512+j));
				cmax = max(cmax,*(pData+i*512+j));
			}
		}
		cinterval = (cmax - cmin) / 15;

		KDataInfo* pDataInfo = &di;

		// ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("wt"));
		if(fp==NULL)
		{
			delete[] pData;
			pData = NULL;
			return FALSE;
		}

		// �ļ�ͷ
		_ftprintf(fp,_T("SNYC 204 %s \n"),(LPCTSTR)pDataInfo->Comment());
		_ftprintf(fp,_T("0 0 0 0 0 0 0 0 0 0 \n"));

		// 2.д�����ڡ�ʱЧ�����
		_ftprintf(fp,_T("%4d %02d %02d %02d %02d %02d 0 0\n")
			,pDataInfo->wYear
			,pDataInfo->wMonth
			,pDataInfo->wDay
			,pDataInfo->wHour
			,pDataInfo->wMinute
			,pDataInfo->wSecond
			);

		_ftprintf(fp,_T("%d %8.3f %8.3f \n"),pDataInfo->xNum,pDataInfo->xMin,pDataInfo->xInterval);
		_ftprintf(fp,_T("%d %8.3f %8.3f \n"),pDataInfo->yNum,pDataInfo->yMin,pDataInfo->yInterval);
		_ftprintf(fp,_T("9999 %8.3f %8.3f %8.3f \n"),cinterval,cmin,cmax);

		int row = pDataInfo->yNum;
		int col = pDataInfo->xNum;
		for(i=0;i<row;i+=skip)
		{
			k=0;
			for(j=0;j<col;j+=skip)
			{
				_ftprintf(fp,_T("%8.1f "),*(pData+i*col+j));
				//10������һ��
				if(k%10 == 9)
				{
					_ftprintf(fp,_T("\n"));
				}
				k++;
			}
			//ÿ��γ�����������ɺ���
			_ftprintf(fp,_T("\n"));
		}

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		delete[] pData;
		pData = NULL;

		return TRUE;
	}

	// ������ֵת��Ϊ�Ҷ�ͼ
	HBITMAP KRpgDataOperator::ToGrayImage()
	{
		HBITMAP hBitmap = NULL;
		//-------------------------------------------------
		KDataInfo di;
		double* pData = NULL;	// ע���ͷ��ڴ�
		if (!ToGrid(pData, di, 0, 15, 0))
		{
			_delete(pData);
			return NULL;
		}

		// ������Ϣ
		KDataInfo* pDataInfo = &di;

		//���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 24λ DIB ÿ�е��ֽ�
		DWORD dwBytesPerLine = DIBWIDTHBYTES(iWidth * 8 * 3);

		// ���ݳߴ�(24λ)
		DWORD dwSizeImage = dwBytesPerLine * iHeight;

		// ����DIB ��С(�޵�ɫ������)
		DWORD dwDIBSize = sizeof(BITMAPINFOHEADER) + 0 + dwSizeImage;

		// һ�η��� DIB �ڴ�
		PBYTE pDib = new BYTE[dwDIBSize];
		memset(pDib, 0, sizeof(BYTE) * dwDIBSize);

		// ���� BITMAPINFOHRADER
		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)pDib;
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth = iWidth;
		pBitmapInfo->bmiHeader.biHeight = iHeight;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biBitCount = 24;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage = dwSizeImage;	//��BI_RGB��ʽʱ���Ϊ0
		pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		pBitmapInfo->bmiHeader.biClrUsed = 0;
		pBitmapInfo->bmiHeader.biClrImportant = 0;

		// DIB ����ָ��
		PBYTE pDIBits = pDib + sizeof(BITMAPINFOHEADER) + 0;

		int r, g, b;
		for (int i = 0; i < iHeight; i++)
		{
			for (int j = 0; j < iWidth; j++)
			{
				double dv = *(pData + i*iWidth + j);

				int cv = (int)(dv / 70 * 255);	//TODO:��ʱת��
				if (cv <= 0)
				{
					cv = 0;	//�Ҷ�ͼ��������Ϊ��ɫ��
				}
				else if (cv >= 255)
				{
					cv = 255;
					//GetRValue()
				}

				r = g = b = cv;

				int ii = i;
				//int ii = (iHeight - 1 - i);
				*(pDIBits + ii*dwBytesPerLine + j * 3 + 2) = r;	// red
				*(pDIBits + ii*dwBytesPerLine + j * 3 + 1) = g;	// green
				*(pDIBits + ii*dwBytesPerLine + j * 3 + 0) = b;	// blue

			}
		}

		if (m_dibGray.AttachMemory(pDib, TRUE))
		{
			hBitmap = (HBITMAP)m_dibGray;
		}

		_delete(pData);
		//-------------------------------------------------
		return hBitmap;
	}

	// ƴͼ
	BOOL KRpgDataOperator::Union(double*& pData, KDataInfo* pDataInfo,TCollection<KString>* pFileArray)
	{
		KRpgData rpgData;
		KRpgData* pRpgData = &rpgData;

		if(pData)
			_deletea(pData);

		double* pData1 = NULL;
		KDataInfo dataInfo1;

		double* pData2 = NULL;
		KDataInfo dataInfo2;

		int nFile = pFileArray->Count();

		for(int i=0;i<nFile;i++)
		{
			if(i==0 && pRpgData->LoadData(pFileArray->GetAt(i)))
			{
				// ��һ���ļ����� pData,
				// ��ֹֻ��һ���ļ�
				_deletea(pData);
				pRpgData->ToGrid(pData,*pDataInfo);
			}
			else if(pRpgData->LoadData((*pFileArray)[i]))
			{
				// ���� pData �� pData1
				_deletea(pData1);
				pData1 = pData;
				dataInfo1 = *pDataInfo;

				// ��һ���ļ����� pData2
				_deletea(pData2);
				pRpgData->ToGrid(pData2,dataInfo2);

				// �ϲ�
				pRpgData->Union(pData,pDataInfo,pData1,&dataInfo1,pData2,&dataInfo2);

				// �ϲ��Ժ� pData1,pData2 ��������
				_deletea(pData1);
				_deletea(pData2);
			}
		}

		return TRUE;
	}
	BOOL KRpgDataOperator::Union(double*& pData, KDataInfo* pDataInfo,LPCTSTR pszFile[],int nFile)
	{
		KRpgData rpgData;
		KRpgData* pRpgData = &rpgData;

		if(pData)
			_deletea(pData);

		double* pData1 = NULL;
		KDataInfo dataInfo1;

		double* pData2 = NULL;
		KDataInfo dataInfo2;

		for(int i=0;i<nFile;i++)
		{
			if(i==0 && pRpgData->LoadData(pszFile[i]))
			{
				// ��һ���ļ����� pData,
				// ��ֹֻ��һ���ļ�
				_deletea(pData);
				pRpgData->ToGrid(pData,*pDataInfo);
			}
			else if(pRpgData->LoadData(pszFile[i]))
			{
				// ���� pData �� pData1
				_deletea(pData1);
				pData1 = pData;
				dataInfo1 = *pDataInfo;

				// ��һ���ļ����� pData2
				_deletea(pData2);
				pRpgData->ToGrid(pData2,dataInfo2);

				// �ϲ�
				pRpgData->Union(pData,pDataInfo,pData1,&dataInfo1,pData2,&dataInfo2);

				// �ϲ��Ժ� pData1,pData2 ��������
				_deletea(pData1);
				_deletea(pData2);
			}
		}

		return TRUE;
	}
	BOOL KRpgDataOperator::Union(double*& pData, KDataInfo* pDataInfo, double*& pData1, KDataInfo* pDataInfo1, double*& pData2, KDataInfo* pDataInfo2)
	{
		int i,j;

		if(!pData1)
			return FALSE;

		// �Ȱ� pDataInfo1 ���ݸ� pDataInfo
		pDataInfo->xMin = pDataInfo1->xMin;
		pDataInfo->xMax = pDataInfo1->xMax;

		pDataInfo->yMin = pDataInfo1->yMin;
		pDataInfo->yMax = pDataInfo1->yMax;

		// X,Y ����������
		pDataInfo->xInterval = pDataInfo1->xInterval;
		pDataInfo->yInterval = pDataInfo1->yInterval;

		// X,Y ��������
		pDataInfo->xNum = pDataInfo1->xNum;
		pDataInfo->yNum = pDataInfo1->yNum;

		// �ڶ������ݿ���Ϊ NULL
		//if(!pData2)
		//	return FALSE;

		// ����еڶ�������,�ϲ�������Ϣ
		// ȡ�úϲ�������귶Χ
		if(pData2)
		{
			pDataInfo->xMin = min(pDataInfo1->xMin,pDataInfo2->xMin);
			pDataInfo->xMax = max(pDataInfo1->xMax,pDataInfo2->xMax);

			pDataInfo->yMin = min(pDataInfo1->yMin,pDataInfo2->yMin);
			pDataInfo->yMax = max(pDataInfo1->yMax,pDataInfo2->yMax);

			// X,Y ����������
			//pDataInfo->xInterval = max(pDataInfo1->xInterval,pDataInfo2->xInterval);
			//pDataInfo->yInterval = max(pDataInfo1->yInterval,pDataInfo2->yInterval);
			pDataInfo->xInterval = 0.01;
			pDataInfo->yInterval = 0.01;

			// X,Y ��������
			pDataInfo->xNum = (int)(fabs(pDataInfo->xMax - pDataInfo->xMin) / pDataInfo->xInterval) + 1;
			pDataInfo->yNum = (int)(fabs(pDataInfo->yMax - pDataInfo->yMin) / pDataInfo->yInterval) + 1;
		}

		// �����ڴ�
		double* pDataTemp = new double[pDataInfo->xNum * pDataInfo->yNum];
		if(!pDataTemp)
			return FALSE;

		// �ó�ֵΪ��Ч����
		for(i=0;i<pDataInfo->yNum;i++)
		{
			for(j=0;j<pDataInfo->xNum;j++)
			{
				*(pDataTemp + i * pDataInfo->xNum + j) = -9999;
			}
		}

		// ���Ƶ�һ������
		int row,col;
		double x1,y1,v1;
		for(i=0;i<pDataInfo1->yNum;i++)
		{
			y1 = pDataInfo1->yMin + pDataInfo1->yInterval * i;
			for(j=0;j<pDataInfo1->xNum;j++)
			{
				x1 = pDataInfo1->xMin + pDataInfo1->xInterval * j;
				v1 = *(pData1 + i * pDataInfo1->xNum + j);

				if(LonLat2RowCol(pDataInfo,x1,y1,row,col))
				{
					*(pDataTemp + row * pDataInfo->xNum + col) = v1;
				}
			}
		}

		// �ϲ��ڶ�������
		if(pData2!=NULL)
		{
			double x2,y2,v2;
			for(i=0;i<pDataInfo2->yNum;i++)
			{
				y2 = pDataInfo2->yMin + pDataInfo2->yInterval * i;
				for(j=0;j<pDataInfo2->xNum;j++)
				{
					x2 = pDataInfo2->xMin + pDataInfo2->xInterval * j;
					v2 = *(pData2 + i * pDataInfo2->xNum + j);

					if(LonLat2RowCol(pDataInfo,x2,y2,row,col))
					{
						double v = *(pDataTemp + row * pDataInfo->xNum + col);
						// ȡ��ֵ
						*(pDataTemp + row * pDataInfo->xNum + col) = max(v,v2);
					}
				}
			}
		}

		// ��Ч����Ϊ0
		for(i=0;i<pDataInfo->yNum;i++)
		{
			for(j=0;j<pDataInfo->xNum;j++)
			{
				if(fabs(*(pDataTemp + i * pDataInfo->xNum + j) + 9999)<EPS)
				{
					*(pDataTemp + i * pDataInfo->xNum + j) = 0;
				}
			}
		}

		//_deletea(pData);
		pData = pDataTemp;

		return TRUE;
	}
	BOOL KRpgDataOperator::LonLat2RowCol(KDataInfo* pDataInfo, double lon, double lat, int& row, int& col)
	{
		double xnum = pDataInfo->xNum;
		double xmin = pDataInfo->xMin;
		double xmax = pDataInfo->xMax;
		double xinterval = pDataInfo->xInterval;

		double ynum = pDataInfo->yNum;
		double ymin = pDataInfo->yMin;
		double ymax = pDataInfo->yMax;
		double yinterval = pDataInfo->yInterval;

		col = (int)((lon - xmin) / xinterval + 0.5);
		row = (int)((lat - ymin) / yinterval + 0.5);

		return (col>=0 && col<=xnum && row>=0 && row<=ynum);
	}
	BOOL KRpgDataOperator::RowCol2LonLat(KDataInfo* pDataInfo, int row, int col, double& lon, double& lat)
	{
		double xnum = pDataInfo->xNum;
		double xmin = pDataInfo->xMin;
		double xmax = pDataInfo->xMax;
		double xinterval = pDataInfo->xInterval;

		double ynum = pDataInfo->yNum;
		double ymin = pDataInfo->yMin;
		double ymax = pDataInfo->yMax;
		double yinterval = pDataInfo->yInterval;

		lon = xmin + xinterval * col;
		lat = ymin + yinterval * row;

		return ((lon>xmin || fabs(lon-xmin)<EPS) && (lat>ymin || fabs(lat-ymin)<EPS));
	}

	//---------------------------------------------------------
	// ��  ��: Radial2RangeBin
	// ��  ��: �Ѿ�����������ת����(RangeBin)�Ծ�γ����ʽ��ʾ�Ŀ�,
	//         ������ pRangeBinList ������,������ʹ��
	// ��  ��:
	//		[o] pRangeBinList - ���������
	//
	// ��  ��: ��������� KRangeBinList
	// ��  ��: TRUE/FALSE
	//---------------------------------------------------------
	BOOL KRpgDataOperator::Radial2RangeBin(KRangeBinList* pRangeBinList, KRangeBinPerRadialList* pRangeBinPerRadialList)
	{
		if (pRangeBinList == NULL)
			return FALSE;

		if (pRangeBinPerRadialList == NULL)
			return FALSE;

		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if(pRpgData == NULL)
			return FALSE;

		KRadialList* pRadialList = pRpgData->GetRadialList();
		int size = pRadialList->size();
		if(size<1)
			return FALSE;

		int iProductCode = pRpgData->GetProductCode();
		RadialDataPacketHeader* pRadialDataPacketHeader = pRpgData->GetRadialDataPacketHeader();

		// ÿ��һ����������ͼ��
		// һ����������ͼ�����������(ÿ�����߻����Լ�����������)
		int iFirstBin					= pRadialDataPacketHeader->iIndexOfFirstRangeBin;		//Location of first rang bin
		//int iRangeBinNumberPerRadial = pRadialDataPacketHeader->iRangeBinNumberPerRadial;	//���һ���������ݵĿ���
		//int iICenterOfSweep				= pRadialDataPacketHeader->iICenterOfSweep;			//km/4
		//int iJCenterOfSweep				= pRadialDataPacketHeader->iJCenterOfSweep;			//km/4
		int iScaleFactor				= pRadialDataPacketHeader->iScaleFactor;				//ÿ���������������(0.001)
		//int iRadialNumber				= pRadialDataPacketHeader->iRadialNumber;				//��������

		// ÿ�����ر�ʾ�Ĺ�����(230km��һ�����ر�ʾһ����)
		double dKmPerPixel = (double)pRpgData->GetProductRange(iProductCode) / 230;
		dKmPerPixel = dKmPerPixel < 1.0 ? 1.0 :dKmPerPixel;

		// ÿ��������������� (0.998������)
		double dPixelPerBin = 0.001 * iScaleFactor * dKmPerPixel;

		// 1.1����=0.008979��γ�� 1/111.37
		double dDegreePerKm		= 0.0098769866211726676842955912723355;	//1����=0.008979��γ�� 1/111.37

		// ÿ�����ر�ʾ�ľ�γ��
		double dDegreePerdPixel = dDegreePerKm;

		// ȡ��ͼ�������λ��
		double dLon0,dLat0;
		pRpgData->GetImageCenter(&dLon0,&dLat0);

		double dXOrigin = dLon0;
		double dYOrigin = dLat0;

		Legend* pLegend = pRpgData->GetLegend();

		pRangeBinList->Clear();

		KRadialList::iterator itr;
		KRadialList::iterator itrBeg = pRadialList->begin();
		KRadialList::iterator itrEnd = pRadialList->end();
		for(itr=itrBeg; itr!=itrEnd; itr++)
		{
			Radial* pRadial = *itr;

			// �������ߵĿ�ʼ�Ƕ�(��ʱ��)
			double alf0 = pRadial->oEachRadialHeader.iRadialStartAngle * 0.1 - 90;	//ת���������Ǳ�

			// ��������ÿ�����ڿ�ʼ�Ƕ���X��Y���������
			double dw0 = dPixelPerBin * dDegreePerdPixel * cos(alf0 * 3.1415926 / 180);
			double dh0 = dPixelPerBin * dDegreePerdPixel * sin(alf0 * 3.1415926 / 180);

			// ��������ÿ�����ڿ�ʼ�Ƕ��ϵ���ʼ����
			double x0Beg = dXOrigin + dw0 * iFirstBin;
			double y0Beg = dYOrigin - dh0 * iFirstBin;

			// �������ߵĽ����Ƕ�
			double alf1 = alf0 + pRadial->oEachRadialHeader.iDeltaAngle * 0.1;		//����չ��
			//if(alf1>360)	alf1 = alf1-360.0;
			//if(alf1<0)		alf1 = 360.0-alf1;
		
			// ��������ÿ�����ڽ����Ƕ���X��Y���������
			double dw1 = dPixelPerBin * dDegreePerdPixel * cos(alf1 * 3.1415926 / 180);
			double dh1 = dPixelPerBin * dDegreePerdPixel * sin(alf1 * 3.1415926 / 180);

			// ��������ÿ�����ڽ����Ƕ��ϵ���ʼ����
			double x1Beg = dXOrigin + dw1 * iFirstBin;
			double y1Beg = dYOrigin - dh1 * iFirstBin;

			double x2,y2;
			double x3,y3;

			// ÿ�������ϵ�ǰ�Ŀ��
			int iCurRun=0;

			//��ɱ����������ݵĿ���
			int iRangeBinNumberPerRadial = pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;

			RangeBinPerRadial* pRangeBinPerRadial = new RangeBinPerRadial();
			pRangeBinPerRadial->pRangeBinList = new KRangeBinList();

			for(int k=0;k<pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;k++)	//ÿ�����ߵĿ�����
			{
				RangeBin* pRangeBin = new RangeBin();

				if(k==0)
				{
					pRangeBin->x[0] = (float)x0Beg;
					pRangeBin->y[0] = (float)y0Beg;
					pRangeBin->x[1] = (float)x1Beg;
					pRangeBin->y[1] = (float)y1Beg;
				}
				else
				{
					//����pt[0]��pt[2]
					//pt[0]��pt[3]����ʼ�Ƕ���,pt[1]��pt[2]�ڽ����Ƕ���
					pRangeBin->x[0] = (float)x3;
					pRangeBin->y[0] = (float)y3;
					pRangeBin->x[1] = (float)x2;
					pRangeBin->y[1] = (float)y2;
				}

				int iRun = pRadial->pRunColor[k].iRun;			//���� 
				int iColor = pRadial->pRunColor[k].iColorIndex;	//�����ɫ

				iCurRun += iRun;	//��ǰ���

				pRangeBin->x[3] = (float)(x0Beg + dw0 * iCurRun);	//pt[0]��pt[3]����ʼ�Ƕ���
				pRangeBin->y[3] = (float)(y0Beg - dh0 * iCurRun);	//

				pRangeBin->x[2] = (float)(x1Beg + dw1 * iCurRun);	//pt[1]��pt[2]�ڽ����Ƕ���
				pRangeBin->y[2] = (float)(y1Beg - dh1 * iCurRun);	//

				pRangeBin->iColorIndex	= iColor;						//��ǰ�����ɫ(0��ʾ����ɫ)
				pRangeBin->clrBin		= pLegend[iColor].dwColor;		//��ǰ�����ɫ(0��ʾ����ɫ)
				pRangeBin->fValue		= (float)pLegend[iColor].dValue;//��ǰ���ʾ��Ҫ��ֵ(�����ɫΪ0��ʾû��Ҫ��)

				//---------------------
				x3 = pRangeBin->x[3];
				y3 = pRangeBin->y[3];
				x2 = pRangeBin->x[2];
				y2 = pRangeBin->y[2];
				//---------------------

				if (pRangeBin->iColorIndex > 0)
				{
					pRangeBinList->push_back(pRangeBin);	//���һ���⵽����

					pRangeBinPerRadial->pRangeBinList->push_back(new RangeBin(*pRangeBin));
				}
				else
				{
					delete pRangeBin;
					pRangeBin = 0;
				}

			}// for(k)

			pRangeBinPerRadialList->push_back(pRangeBinPerRadial);

		}// for(itr)

	#ifdef _DEBUG
		int n1 = (int)pRangeBinList->size();
		int n2 = (int)pRangeBinPerRadialList->size();
	#endif

		return TRUE;
	}

	//---------------------------------------------------------
	// ��  ��: Raster2RangeBin
	// ��  ��: �Ѱ�դ������ת����(RangeBin)�Ծ�γ����ʽ��ʾ�Ŀ�,
	//         ������ pRangeBinList ������,������ʹ��
	// ������:  
	// ��  ��:
	//		[o] pRangeBinList - ���������
	//
	// ��  ��: ��������� KRangeBinList
	// ��  ��: TRUE/FALSE
	//---------------------------------------------------------
	BOOL KRpgDataOperator::Raster2RangeBin(KRangeBinList* pRangeBinList)
	{
		if(pRangeBinList == NULL)
			return FALSE;

		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if(pRpgData == NULL)
			return FALSE;

		KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
		int size = pRasterRowList->size();
		if(size<1)
			return FALSE;

		int iProductCode = pRpgData->GetProductCode();
		RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();

		int iICoordStart	= pRasterDataPacketHeader->iICoordinateStart;	//���ݿ�ʼ����λ��
		int iJCoordStart	= pRasterDataPacketHeader->iJCoordinateStart;	//���ݿ�ʼ����λ��
		int iXScaleINT		= pRasterDataPacketHeader->iXScaleINT;			//X����һ��Ҫ��ֵռ�ĸ����
		int iYScaleINT		= pRasterDataPacketHeader->iYScaleINT;			//Y����һ��Ҫ��ֵռ�ĸ����
		int iNumberOfRows	= pRasterDataPacketHeader->iNumberOfRows;		//����������

		// ��Ʒ���Ƿ�Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// ÿ�����ر�ʾ�Ĺ�����(230km�Ĳ�Ʒ1�����ر�ʾ1����)
		double dKmPerPixel = dRange / 230;

		// 1.1����=0.008979��γ�� 1.1/111.37
		double dDegreePerKm		= 0.0098769866211726676842955912723355;

		// ÿ�����ر�ʾ�ľ�γ��
		double dDegreePerPixel = dDegreePerKm;

		// ȡ��ͼ�������λ��
		double dLon0,dLat0;
		pRpgData->GetImageCenter(&dLon0,&dLat0);

		// ���ϽǾ�γ��
		double dLonLft = dLon0 - dDegreePerKm * dRange;
		double dLatTop = dLat0 + dDegreePerKm * dRange;

		//ȡ����ɫֵ
		Legend* pLegend = pRpgData->GetLegend();

		pRangeBinList->Clear();

		float xx0,yy0,xx1,yy1,xx2,yy2,xx3,yy3;

		int j = 0;

		KRasterRowList::iterator itr;
		KRasterRowList::iterator itrBeg = pRasterRowList->begin();
		KRasterRowList::iterator itrEnd = pRasterRowList->end();
		for(itr=itrBeg; itr!=itrEnd; itr++)
		{
			// դ����
			RasterRow* pRasterRow = *itr;
			// �������ֽ���
			int iNumberOfBytesInThisRow = pRasterRow->iNumberOfBytesInThisRow;

			int iCurRun=0;	//��ǰ���

			for(int k=0;k<iNumberOfBytesInThisRow;k++)	//һ�е��ֽ���
			{
				RangeBin* pRangeBin = new RangeBin();

				int iRun = pRasterRow->pRunColor[k].iRun;			//���(��)
				int iColor = pRasterRow->pRunColor[k].iColorIndex;	//�����ɫ

				pRangeBin->iColorIndex	= iColor;
				pRangeBin->clrBin 		= pLegend[iColor].dwColor;			//��ǰ�����ɫ(0��ʾ����ɫ)
				pRangeBin->fValue		= (float)pLegend[iColor].dValue;	//��ǰ���ʾ��Ҫ��ֵ(�����ɫΪ0��ʾû��Ҫ��)

				iCurRun += (int)(iRun * iXScaleINT * dKmPerPixel);

				///////////////////////////////////////////
				//	(xx0,yy0)	---- (xx1,yy1)
				//	           |    |
				//             |    |
				//	(xx3,yy3)	---- (xx2,yy2)
				//////////////////////////////////////////

				// ��ǰ����
				xx1 = (float)(dLonLft + dDegreePerPixel * iCurRun);
				// ��ǰγ��
				yy1 = (float)(dLatTop - dDegreePerPixel * j * iYScaleINT * dKmPerPixel);
			
				xx0 = (float)(xx1 - dDegreePerPixel * iRun * iXScaleINT * dKmPerPixel);
				yy0 = (float)yy1;
			
				xx2 = (float)xx1;
				yy2 = (float)(yy1 - dDegreePerPixel * iYScaleINT * dKmPerPixel);
			
				xx3 = (float)xx0;
				yy3 = (float)yy2;

				pRangeBin->x[1] = xx1; 
				pRangeBin->y[1] = yy1; 
						 
				pRangeBin->x[0] = xx0; 
				pRangeBin->y[0] = yy0; 
						 
				pRangeBin->x[2] = xx2;
				pRangeBin->y[2] = yy2; 
						 
				pRangeBin->x[3] = xx3; 
				pRangeBin->y[3] = yy3; 

				if(pRangeBin->iColorIndex>0)
				{
					pRangeBinList->push_back(pRangeBin);	//���һ���⵽����
				}
				else
				{
					delete pRangeBin;
					pRangeBin = 0;
				}
			}// for(k)
		
			j++;

		}// for(itr=itrBeg; itr!=itrEnd; itr++)

		return TRUE;
	}

	//д�ı��ַ��������ļ�
	BOOL KRpgDataOperator::WriteTextData(LPCTSTR lpszFile,KWriteTextList* pWriteTextList)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if(pRpgData == NULL)
			return FALSE;

		double x,y;
		pRpgData->GetImageCenter(&x,&y);

		if(pWriteTextList->size()<=0)
			return FALSE;

		// ���� UNICODE ��ʾ����
		_tsetlocale( LC_ALL, _T("CHS") );

		FILE* fp = _tfopen(lpszFile,_T("wt"));
		if(fp==NULL)
			return FALSE;

		TCollection<KString> a = KString(pRpgData->GetFileName()).Split(_T('\\'));
		int cnt = a.Count();

		_ftprintf(fp,_T(" %-8s  %s\n"),_T("FILENAME"),a[cnt-1]);
		fprintf(fp," %-8s %8.3f %8.3f\n","LON_LAT",x,y);
		fprintf(fp,"\n");

		KWriteTextList::iterator itr;
		KWriteTextList::iterator itrBeg = pWriteTextList->begin();
		KWriteTextList::iterator itrEnd = pWriteTextList->end();
		for(itr=itrBeg; itr!=itrEnd; itr++)
		{
			WriteText* pWriteText = *itr;
			fputs(pWriteText->szText,fp);
			fprintf(fp,"\n");

		}// for(itr=itrBeg; itr!=itrEnd; itr++)

		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	// ���ɵ�ɫ��
	void KRpgDataOperator::GeneratePalette()
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		KPalette* pPalette = pRpgData->GetPalette();
		if(!pPalette)
		{
			pPalette = new KIdxPalette();
			pRpgData->SetPalette(pPalette);
		}

		int iProductCode = pRpgData->GetProductCode();
		int iDataLevel = pRpgData->GetProductDataLevel(iProductCode);

		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();
		Legend* pLegend = pRpgData->GetLegend();

		for(int i=1;i<iDataLevel;i++)
		{
			pPalette->AddColor(pLegend[i].dValue,pLegend[i].dwColor);
		}

		return;

	}

//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------
