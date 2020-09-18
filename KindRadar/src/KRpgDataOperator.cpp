#include "StdAfx.h"
#include "KRpgDataOperator.h"

#include "KRpgData.h"

namespace Wsr98d { namespace Rpg
{

#pragma region pragma_region_XXX
#pragma endregion

	KRpgDataOperator::KRpgDataOperator(void)
	{
		m_bScitPast = FALSE;		// 过去
		m_bScitForecast = FALSE;	// 预报 
	}

	KRpgDataOperator::~KRpgDataOperator(void)
	{
	}

	///========================================================
	///读取数据 - beg
	///===============
	// 装载数据
	BOOL KRpgDataOperator::LoadData(LPCTSTR lpszFile,LPVOID lpvData)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		///装载 5 大数据块

		//1. 装载 Message Head Block
		if(!loadMHB(lpszFile))
			return FALSE;
		//2. 装载 Product Description Block
		if(!loadPDB(lpszFile))
			return FALSE;

		PDB* pdb = pRpgData->PDBPtr();

		BOOL bSymbology = FALSE;
		BOOL bGraphic = FALSE;
		BOOL bTabular = FALSE;

		//3. 装载 Product Symbology Block
		if(pdb->lOffsetSymbology > 0)
		{
			bSymbology = loadPSB(lpszFile);
		}
		//4. 装载 Graphic Alphanumeric  Block
		if(pdb->lOffsetGraphic > 0) 
		{
			bGraphic = loadGAB(lpszFile);
		}
		//5. 装载 Tabular Alphanumeric  Block
		if(pdb->lOffsetTabular > 0) 
		{
			bTabular = loadTAB(lpszFile);
		}

		// 是否有数据
		BOOL bLoadData = bSymbology || bGraphic || bTabular;
		if(!bLoadData)
			return FALSE;

		// 计算、设置部分数据属性
		int iYear,iMonth,iDay;
		this->JulianDate2GMTDate(pRpgData->PDBPtr()->iVScanDate,&iYear,&iMonth,&iDay);			// 转换儒略日期

		int iHour,iMinute,iSecond;
		this->JulianTime2GMTTime(pRpgData->PDBPtr()->lVScanStartTime,&iHour,&iMinute,&iSecond);	// 转换儒略时间

		// 设置数据的日期时间
		pRpgData->SetDateTime(iYear,iMonth,iDay,iHour,iMinute,iSecond);

		// 填充雷达信息
		pRpgData->FillRadarInfo(pRpgData->GetRadarInfo());

		// 填充数据信息
		pRpgData->FillDataInfo(pRpgData->GetDataInfo());

		// 根据信息头块MHB和产品描述块PDB构造图例
		pRpgData->MakeLegendInfo(pRpgData->GetProductCode());

		// 生成调色板
		this->GeneratePalette();

		return TRUE;
	}

#pragma region 装载5大数据块 - load_XXB
	///--------------------------------------------------------
	///装载5大数据块 - beg
	// 1. 装载 Message Head Block
	BOOL KRpgDataOperator::loadMHB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		MHB* pMhb = pRpgData->MHBPtr();

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		fseek(fp,0,SEEK_SET);
		fread(pMhb,sizeof(MHB),1,fp);

		fclose(fp);

		this->ReverseBytes(pMhb);	// 字节转换

		return (pMhb->iMsgCode >= RPG_DATA_PRODUCT_CODE_MIN && pMhb->iMsgCode <= RPG_DATA_PRODUCT_CODE_MAX);
	}
	// 2. 装载 Product Description  Block
	BOOL KRpgDataOperator::loadPDB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		PDB* pPdb = pRpgData->PDBPtr();

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		fseek(fp,sizeof(MHB),SEEK_SET);
		fread(pPdb,sizeof(PDB),1,fp);

		fclose(fp);

		this->ReverseBytes(pPdb);	// 字节转换

		return (pPdb->iBlockDivider == RPG_DATA_BLOCK_DIVIDER);
	}
	// 3. 装载 Product Symbology Block  
	BOOL KRpgDataOperator::loadPSB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		long lOffset = pRpgData->PSBOffset();	// 取得本数据块偏移地址
		if(lOffset<=0) return FALSE;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 找不到 Symbology 的起始地址(注意 *2)
		if(fseek(fp,lOffset*2,SEEK_SET) != 0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 读取 PSB
		PSB* psb = pRpgData->PSBPtr();
		if(fread(psb,sizeof(PSB),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		this->ReverseBytes(psb);	// 字节转换

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		// 62,82号的PSB比较特殊
		if(iProductCode == 62 || iProductCode == 82)
		{
			// 回滚文件指针
			fseek(fp,-1*(int)sizeof(PSB),SEEK_CUR);
		}
		// 是否 PSB
		else if(  psb->iBlockDivider != RPG_DATA_BLOCK_DIVIDER	// 块分割符(-1)
			   || psb->iBlockID != RPG_DATA_PSB_BLOCK_ID)		// 块ID(1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		//设置文件指针当前偏移
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 关闭文件
		fclose(fp);
		fp = NULL;

		//-------------------------------------------------------------------------
		// PSB块的类型一共有8种
		// 根据图像数据类型调用相应的读取功能函数
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
	// 4. 装载 Graphic Alphanumeric  Block
	BOOL KRpgDataOperator::loadGAB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		long lOffset = pRpgData->GABOffset();	// 取得本数据块偏移地址
		if(lOffset<=0) return FALSE;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 找不到 Graphic 的起始地址(注意 *2)
		if(fseek(fp,lOffset*2,SEEK_SET) != 0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 读取 GAB
		GAB* gab = pRpgData->GABPtr();
		if(fread(gab,sizeof(GAB),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		this->ReverseBytes(gab);	// 字节转换

		if(  gab->iBlockDivider != RPG_DATA_BLOCK_DIVIDER	// 块分割符(-1)
		  || gab->iBlockID != RPG_DATA_GAB_BLOCK_ID			// 块ID(=2)
			)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		//设置文件指针当前偏移
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 关闭文件
		fclose(fp);
		fp = NULL;

		//-------------------------------------------------------------------------
		// GAB块的类型一共有1种
		// 根据图像数据类型调用相应的读取功能函数
		//-------------------------------------------------------------------------
		BOOL bLoadGab = FALSE;
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());

		bLoadGab = gab_LoadWriteTextData(lpszFile,lOffsetCur);

		return bLoadGab;
	}
	// 5. 装载 Tabular Alphanumeric  Block
	BOOL KRpgDataOperator::loadTAB(LPCTSTR lpszFile)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		long lOffset = pRpgData->TABOffset();	// 取得本数据块偏移地址
		if(lOffset<=0) return FALSE;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 找不到 Tabular 的起始地址(注意 *2)
		if(fseek(fp,lOffset*2,SEEK_SET) != 0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 读取 TAB
		TAB* tab = pRpgData->TABPtr();
		if(fread(tab,sizeof(TAB),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		this->ReverseBytes(tab);	// 字节转换

		if(  tab->iBlockDivider != RPG_DATA_BLOCK_DIVIDER	// 块分割符(-1)
		  || tab->iBlockID != RPG_DATA_TAB_BLOCK_ID			// 块ID(=3)
			)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		//设置文件指针当前偏移
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 关闭文件
		fclose(fp);
		fp = NULL;

		//-------------------------------------------------------------------------
		// TAB块的类型一共有1种
		// 根据图像数据类型调用相应的读取功能函数
		//-------------------------------------------------------------------------
		BOOL bLoadTab = FALSE;
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());

		bLoadTab = tab_LoadWriteTextData(lpszFile,lOffsetCur);

		return bLoadTab;
	}
	///装载5大数据块 - end
	///--------------------------------------------------------
#pragma endregion

#pragma region 装载 PSB 数据块的8种数据 - psb_Load_xxxxx_Data
	///--------------------------------------------------------
	///装载 PSB 数据块的8种数据  - beg
	//3.1 装载 Radial Image Data
	BOOL KRpgDataOperator::psb_LoadRadialImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		//雷达产品的  Radial Iamge 目前只有一层
		if( !(iTotalLayers==1) ) throw;

		// 2. 循环读取所有层数据
		// 径向数据图像数据指针
		KRadialList* pRadialList = pRpgData->GetRadialList();
		pRadialList->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// 读数据层头结构
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{// 层分隔符出错 
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 读径向图像数据包数据
			RadialDataPacketHeader* pRadialDataPacketHeader = pRpgData->GetRadialDataPacketHeader();
			fread(pRadialDataPacketHeader,sizeof(RadialDataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(RadialDataPacketHeader),SEEK_CUR);	// 回滚文件指针

			this->ReverseBytes(pRadialDataPacketHeader);

			if(pRadialDataPacketHeader->iPacketCode != const_PacketCode_Radial_Data)
			{// 数据包代码错误
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 保存当前指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// 读入扫描平面的所有径向数据到径向数据链表 pRadialList
			if(!ReadRadialImageData(pRadialList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.2 装载 Raster Image Data
	BOOL KRpgDataOperator::psb_LoadRasterImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		// 雷达产品的  Raster Iamge Normal 目前只有一层
		if( !(iTotalLayers==1) ) throw;

		// 2. 读取数据
		// 径向数据图像数据指针
		KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
		pRasterRowList->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// 读数据层头结构
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{// 层分隔符出错 
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 读取栅格图像数据包头
			RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();
			fread(pRasterDataPacketHeader,sizeof(RasterDataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(RasterDataPacketHeader),SEEK_CUR);

			this->ReverseBytes(pRasterDataPacketHeader);

			// 数据包代码错误
			if(   pRasterDataPacketHeader->iPacketCode[0] != const_PacketCode_Raster_Data_Normal
			   || pRasterDataPacketHeader->iPacketCode[1] != const_Raster_Data_Normal_Operate_Flag_1
			   || pRasterDataPacketHeader->iPacketCode[2] != const_Raster_Data_Normal_Operate_Flag_2
			   || pRasterDataPacketHeader->iPackDescriptor != const_Raster_Data_Pack_Descriptor)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 保存当前指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// 读入栅格图像数据到数据链表 pRasterRowList
			if(!ReadRasterImageData(pRasterRowList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.3 装载 Linked Contour Vector Image Data
	BOOL KRpgDataOperator::psb_LoadLinkedContourVectorImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		// 雷达产品的  Linked Contour Vector Image Data 目前只有一层
		Assert(iTotalLayers==1);

		// 2. 读取数据
		KLinkedContourVectorList* pLinkedContourVectorList = pRpgData->GetLinkedContourVectorList();
		pLinkedContourVectorList->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// 读数据层信息头
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 保存当前指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// 读入等值线数据到数据链表
			if(!ReadLinkedContourVectorData(pLinkedContourVectorList))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.4 装载 Geographic Alphanumeric Image Data
	BOOL KRpgDataOperator::psb_LoadGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件 
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		// 雷达产品的  Geographic Alphanumeric Image Data 目前只有一层
		Assert(iTotalLayers==1);

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		// 2. 读取数据
		pRpgData->GetColorTextList()->Clear();
		pRpgData->GetSpecilaSymbolList()->Clear();
		pRpgData->GetStormIdList()->Clear();
		pRpgData->GetScitPastList()->Clear();
		pRpgData->GetScitForecastList()->Clear();
		pRpgData->GetHdaHailList()->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// 读数据层信息头
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 保存当前指针偏移
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
					// 读取数据头并回滚指针
					DataPacketHeader tDataPacketHeadre;
					fread(&tDataPacketHeadre,sizeof(DataPacketHeader),1,fp);
					fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
					this->ReverseBytes(&tDataPacketHeadre);

					// 查看是否 PacketCode = 2
					// 每组风暴路径信息总是以 SpecialSymbolPacket 开始
					if(tDataPacketHeadre.iPacketCode != const_PacketCode_Special_Symbol)
					{
						// 成功读取过数据
						if(bRead)
						{
							fclose(fp);
							fp = NULL;
							return TRUE;
						}
						else
						{// 成功读取过数据
							fclose(fp); 
							fp = NULL;
							return FALSE; 
						}
					}

					// Code = 2 特殊符号数据 
					BOOL b2 = ReadSpecilaSymbolData(pRpgData->GetSpecilaSymbolList());

					// Code = 15 风暴ID数据
					BOOL b15 = ReadStormIdData(pRpgData->GetStormIdList());

					// Code = 23 风暴中心路径信息过去数据
					BOOL b23 = ReadScitPastData(pRpgData->GetScitPastList());

					// Code  = 24 风暴中心路径信息预报数据
					BOOL b24 = ReadScitForecastData(pRpgData->GetScitForecastList());

					// 是否曾经成功读出过数据
					bRead = b2 || b15 || b23 || b24;
				}
			}
			break;

			case 59:		// HI
			{
				BOOL bRead = FALSE;
				while(1)
				{
					// 读取数据头并回滚指针
					DataPacketHeader tDataPacketHeadre;
					fread(&tDataPacketHeadre,sizeof(DataPacketHeader),1,fp);
					fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
					this->ReverseBytes(&tDataPacketHeadre);

					if(tDataPacketHeadre.iPacketCode != const_PacketCode_Hda_Hail)
					{
						// 成功读取过数据
						if(bRead)
						{
							fclose(fp);
							fp = NULL;
							return TRUE;
						}
						else
						{// 成功读取过数据
							fclose(fp); 
							fp = NULL;
							return FALSE; 
						}
					}

					// Code = 19 THdaHail
					BOOL b19 = ReadHdaHailData(pRpgData->GetHdaHailList());

					// Code = 15 风暴ID数据
					BOOL b15 = ReadStormIdData(pRpgData->GetStormIdList());

					// 是否曾经成功读出过数据
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

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.5 装载 Non Geographic Alphanumeric Image Data
	// 48 + 84
	BOOL KRpgDataOperator::psb_LoadNonGeographicAlphanumericImageData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		// 雷达产品的  Geographic Alphanumeric Image Data 目前只有一层
		Assert(iTotalLayers==1);

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		// 2. 读取数据
		pRpgData->GetColorUnlinkedVectorList()->Clear();
		pRpgData->GetColorTextList()->Clear();
		pRpgData->GetWindBarbList()->Clear();
		pRpgData->GetColorLinkedVectorList()->Clear();
		pRpgData->GetColorUnlinkedVectorList()->Clear();
		for(i=0;i<iTotalLayers;i++)
		{
			// 读数据层信息头
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);
			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 保存当前指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// 2.1 读取直角坐标线段
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
			// 2.2 读取标注文本
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
			// 2.3 读取图形数据
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			// 2.3.1 Wind Barb 图形数据
			if(tDataPacketHeader.iPacketCode == const_PacketCode_Wind_Barb)
			{
				if(!ReadWindBarbData(pRpgData->GetWindBarbList()))
				{
					fclose(fp);
					fp = NULL;
					return FALSE;
				}
			}
			// 2.3.2 Color UnLinked Vector 图形数据
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
			// 2.3.3 Color Linked Vector 图形数据
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
			// 2.4 读取标注文本
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
			// 2.5 非连接矢量数据(48无,84有)
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
			// 结束
			fread(&tDataPacketHeader,sizeof(DataPacketHeader),1,fp);
			fseek(fp,-1*(int)sizeof(DataPacketHeader),SEEK_CUR);
			this->ReverseBytes(&tDataPacketHeader);
			if(tDataPacketHeader.iPacketCode != const_PacketCode_Color_Unlinked_Vector)
			{
				//int xxx = 0;
			}
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.6 装载 Alphanumeric Image Data
	// 62 + 82
	BOOL KRpgDataOperator::psb_LoadAlphanumericImageData(LPCTSTR lpszFile,long lOffset)
	{
		return ReadPSBWriteTextData(lpszFile,lOffset);
	}
	//3.7 装载 Raster Image Data (Section)
	BOOL KRpgDataOperator::psb_LoadRasterImageData_Section(LPCTSTR lpszFile,long lOffset)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		// 雷达产品的  Raster Iamge Section 有2层
		Assert(iTotalLayers==2);

		//
		// 2. 读取数据
		//
		// 2.1 读取剖面图数据层
		DataLayerHeader tDataLayerHeader;
		fread(&tDataLayerHeader,1,sizeof(DataLayerHeader),fp);
		this->ReverseBytes(&tDataLayerHeader);

		if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 读取栅格图像数据包头
		RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();
		fread(pRasterDataPacketHeader,sizeof(RasterDataPacketHeader),1,fp);
		fseek(fp,-1*(int)sizeof(RasterDataPacketHeader),SEEK_CUR);

		this->ReverseBytes(pRasterDataPacketHeader);

		// 数据包代码错误
		if(   pRasterDataPacketHeader->iPacketCode[0] != const_PacketCode_Raster_Data_Section
		   || pRasterDataPacketHeader->iPacketCode[1] != const_Raster_Data_Section_Operate_Flag_1
		   || pRasterDataPacketHeader->iPacketCode[2] != const_Raster_Data_Section_Operate_Flag_2
		   || pRasterDataPacketHeader->iPackDescriptor != const_Raster_Data_Pack_Descriptor)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 保存当前指针偏移
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 栅格图像数据到数据链表指针
		KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
		pRasterRowList->Clear();

		// 读入栅格图像数据到数据链表 pRasterRowList
		if(!ReadRasterImageData(pRasterRowList))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 2.2 坐标和标注层
		DataLayerHeader tDataLayerHeader2;
		fread(&tDataLayerHeader2,1,sizeof(DataLayerHeader),fp);
		this->ReverseBytes(&tDataLayerHeader2);

		if(tDataLayerHeader2.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// 2.2.1 读入坐标数据
		//读取单色非连接矢量数据包头
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

		// 保存当前指针偏移
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 坐标数据到链表指针
		KMonoUnlinkedVectorList* pMonoUnlinkedVectorList = pRpgData->GetMonoUnlinkedVectorList();
		pMonoUnlinkedVectorList->Clear();

		// 读入坐标数据到链表
		if(!ReadMonoUnLinkedVectorData(pMonoUnlinkedVectorList))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		// 2.2.2 读入标注数据
		//读取单色文本数据包头
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

		// 保存当前指针偏移
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 标注数据链表指针
		KMonoTextList* pMonoTextList = pRpgData->GetMonoTextList();
		pMonoTextList->Clear();

		// 读入标注数据链表 m_MonoTextList
		if(!ReadMonoTextData(pMonoTextList))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	//3.8 装载 Raster Image Data (Slice)
	BOOL KRpgDataOperator::psb_LoadRasterImageData_Slice(LPCTSTR lpszFile,long lOffset)
	{
		int i,j;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		PSB* psb = pRpgData->PSBPtr();

		// 数据层数
		int iTotalLayers = psb->iTotalLayers;

		// 雷达产品的  Raster Iamge Slice 有多层
		Assert(iTotalLayers>0);

		//
		// 2. 读取数据
		//
		for(i=0;i<20;i++)
		{
			pRpgData->GetRasterRowList_Slice(i)->Clear();
			pRpgData->GetMonoUnLinkedVectorList_Slice(i)->Clear();
			pRpgData->GetMonoTextList_Slice(i)->Clear();
		}
		// 数据层数包含切片图层数和不可连接矢量层
		// 最后一层数据是不可连接矢量层和各切片图说明文本
		//
		// 2.1 读取多层切片图
		for(i=0;i<iTotalLayers-1;i++)
		{
			// 读数据层头
			DataLayerHeader tDataLayerHeader;
			fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);

			this->ReverseBytes(&tDataLayerHeader);

			if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 保存文件指针和文件指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			// 读取切片图
			if(!ReadRasterImageData(pRpgData->GetRasterRowList_Slice(i)))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}
		// 2.2 最后一层数据
		// 读数据层头
		DataLayerHeader tDataLayerHeader;
		fread(&tDataLayerHeader,sizeof(DataLayerHeader),1,fp);

		this->ReverseBytes(&tDataLayerHeader);

		if(tDataLayerHeader.iLayerDivider != RPG_DATA_LAYER_DIVIDER)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 保存文件指针和文件指针偏移
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 读取非连接矢量和标注文本数据
		for(j=0;j<iTotalLayers-1;j++)
		{
			//2.2.1 读入坐标数据(单色非连接矢量)
			if(!ReadMonoUnLinkedVectorData(pRpgData->GetMonoUnLinkedVectorList_Slice(j)))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
			//2.2.2 读入标注数据(单色文本数据)
			if(!ReadMonoTextData(pRpgData->GetMonoTextList_Slice(j)))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	///装载 PSB 数据块的8种数据  - end
	///--------------------------------------------------------
#pragma endregion

	///--------------------------------------------------------
	///装载 GAB 数据块文本数据 - beg
	// 4.1 
	BOOL KRpgDataOperator::gab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		return ReadGABWriteTextData(lpszFile,lOffset);
	}
	///装载 GAB 数据块文本数据 - end
	///--------------------------------------------------------

	///--------------------------------------------------------
	///装载 TAB 数据块文本数据 - beg
	// 5.1 
	BOOL KRpgDataOperator::tab_LoadWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		return ReadTABWriteTextData(lpszFile,lOffset);
	}
	///装载 TAB 数据块文本数据 - end
	///--------------------------------------------------------

	///--------------------------------------------------------
	///辅助函数 - Beg
	BOOL KRpgDataOperator::ReadPSBWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 读取数据
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

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	BOOL KRpgDataOperator::ReadGABWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 2. 读取数据 
		int iNumberOfPages = pRpgData->GABPtr()->iNumberOfPages;

		pRpgData->GetGABWriteTextList()->Clear();
		pRpgData->GetGABUnlinkedVectorList()->Clear();

		//------------------------------------------------
		//读入 TextPacket(1) - TextPacket(n) 
		//TextPacket 由 Code 8 和 Code 10 组成
		//------------------------------------------------
		for(i=0;i<iNumberOfPages;i++)
		{
			//读入每页页头
			PageHeader tPageHeader;
			fread(&tPageHeader,sizeof(PageHeader),1,fp);
			this->ReverseBytes(&tPageHeader);

			int iCurPageNumber		= tPageHeader.iCurrentPageNumber;	//当前页号
			int iLengthOfPage		= tPageHeader.iLengthOfPage;		//从 TextPacket(1) - TextPacket(n) 总的字节数

			// 保存文件指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			//读入 TextPacket
			//Code 8
			if(!ReadColorTextData(pRpgData->GetGABWriteTextList()))
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}
			else
			{// 添加一空行
				ColorText* pColorText = new ColorText();
				memset(pColorText,-1,sizeof(ColorText));
				pColorText->szText[0] = '\0';
				pRpgData->GetGABWriteTextList()->push_back(pColorText);
			}

			//Code 10
			// 可以没有 Code 10
			if(!ReadColorUnLinkedVectorData(pRpgData->GetGABUnlinkedVectorList()))
			{
				//fclose(fp);
				//fp = NULL;
				//return FALSE;
			}
		}

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	BOOL KRpgDataOperator::ReadTABWriteTextData(LPCTSTR lpszFile,long lOffset)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 1. 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;

		// 定位偏移
		fseek(fp,lOffset,SEEK_SET);

		// 保存文件指针和文件指针偏移
		SetFilePtr(fp);
		long lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// PSB 指针
		TAB* tab = pRpgData->TABPtr();

		// 总页数
		int iNumberOfPages = tab->iNumberOfPages;

		// 雷达产品的  Raster Iamge Slice 有多层
		Assert(iNumberOfPages>0);

		//
		// 2. 读取数据
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

		// 3. 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	///辅助函数 - End
	///--------------------------------------------------------

#pragma region 装载各种格式数据 - Read_xxxxxxx_Data
	///--------------------------------------------------------
	///装载各种格式数据 - beg
	//FMT.01 读取单色文本数据
	//     Mono Text Data
	//     Packet Code = 1
	BOOL KRpgDataOperator::ReadMonoTextData(KMonoTextList* pMonoTextList)
	{
		// 读取数据
		BOOL bExistData = FALSE;
		while(1)
		{
			// 分配数据内存
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

	//FMT.02 读取特殊符号数据
	//     Special Symbol Data
	//     Packet Code = 2
	BOOL KRpgDataOperator::ReadSpecilaSymbolData(KSpecilaSymbolList* pSpecilaSymbolList)
	{
		BOOL bExistData = FALSE;

		while(1)
		{
			// 分配数据内存
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

	//FMT.04 读取风钩数据
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

	//FMT.06 读取单色连接矢量数据
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

	//FMT.07 读取单色非连接矢量数据(一组线段)
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

	//FMT.08 读取彩色文本数据
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

	//FMT.09 读取彩色连接矢量数据
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

	//FMT.10 读取彩色非连接矢量数据
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

	//FMT.15 读取风暴ID数据
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

	//FMT.19 读取冰雹概率数据
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

	//FMT.23 读取风暴中心路径信息过去数据
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

	//FMT.24 读取风暴中心路径信息预报数据
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

	//FMT.25 读取风暴路径信息圈数据
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

	//FMT.x0E03 读取连接等值线矢量数据
	//        Linked Contour Vectors Data
	//        Packet Code = 0x0E03
	BOOL KRpgDataOperator::ReadLinkedContourVectorData(KLinkedContourVectorList* pLinkedContourVectorList)
	{
		// 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);


		BOOL bExistData = FALSE;

		// 循环读取各组等值线数据
		while(1)
		{
			// 读等值线颜色包(每一组等值线有一个 TSetColorLevelPacket)
			// 等值线颜色包头
			SetColorLevelPacketHeader tSetColorLevelPacketHeader;
			fread(&tSetColorLevelPacketHeader,1,sizeof(SetColorLevelPacketHeader),fp);

			this->ReverseBytes(&tSetColorLevelPacketHeader);	// 字节转换

			// 保存文件指针偏移
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			if(tSetColorLevelPacketHeader.iPacketCode != const_PacketCode_Set_Color_Level)
			{
				// 如果包代码错误要回滚文件指针
				fseek(fp,-1*(int)sizeof(SetColorLevelPacketHeader),SEEK_CUR);
				lOffsetCur = ftell(fp);
				SetFilePtrOffset(lOffsetCur);

				return bExistData;
			}

			//等值线颜色级别
			sfx_int16 iColorLevelOfContour = RPG_DATA_NO_COLOR_LEVEL;

			if(tSetColorLevelPacketHeader.iColorValueIndicator == const_Color_Level_Present_Indicator)
			{// 包中存在颜色级别
				fread(&iColorLevelOfContour,sizeof(sfx_int16),1,fp);
				KBytesOrder::ReverseBytes(&iColorLevelOfContour);	// 字节转换

				// 保存文件指针偏移
				lOffsetCur = ftell(fp);
				SetFilePtrOffset(lOffsetCur);
			}

			// 读取一组等值线
			while(2)
			{
				LinkedContourVector* pLinkedContourVector = new LinkedContourVector;
				pLinkedContourVector->iColorLevel = iColorLevelOfContour;	// 本组等值线颜色级别

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

	//FMT.xAF1F 读取径向数据图像数据
	//        Radial Data Data
	//        Packet Code = 0xAF1F
	BOOL KRpgDataOperator::ReadRadialImageData(KRadialList* pRadialList)
	{
		int i;

		BOOL bExistData = FALSE;

		// 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 读径向图像数据包数据
		RadialDataPacketHeader tRadialDataPacketHeader;
		fread(&tRadialDataPacketHeader,sizeof(RadialDataPacketHeader),1,fp);
		this->ReverseBytes(&tRadialDataPacketHeader);

		// 保存文件指针偏移
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		if(tRadialDataPacketHeader.iPacketCode != const_PacketCode_Radial_Data)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(RadialDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			return FALSE;
		}

		// 径向射线条数
		int iNumberOfRadials = tRadialDataPacketHeader.iNumberOfRadials;

		for(i=0;i<iNumberOfRadials;i++)
		{
			//为一条径向射线分配内存
			Radial* pRadial = new Radial;

			// 读取一条径向数据包
			if(ReadRadialDataPacket(pRadial))
			{
				bExistData = TRUE;
				pRadialList->push_back(pRadial);	// 添加一条射线数据
			}
			else
			{
				_delete(pRadial);
				break;
			}
		}

		return bExistData;
	}

	//FMT.xBA07 读取栅格数据图像数据
	//        Raster Data Data
	//        Packet Code = 0xBA07
	//FMT.xBA0F 读取栅格数据图像数据
	//        Raster Data Data
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterImageData(KRasterRowList* pRasterRowList)
	{
		int i;

		BOOL bExistData = FALSE;

		// 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 读取栅格图像数据包头
		RasterDataPacketHeader tRasterDataPacketHeader;
		fread(&tRasterDataPacketHeader,sizeof(RasterDataPacketHeader),1,fp);
		this->ReverseBytes(&tRasterDataPacketHeader);

		// 保存文件指针偏移
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		if((   tRasterDataPacketHeader.iPacketCode[0] != const_PacketCode_Raster_Data_Normal
			&& tRasterDataPacketHeader.iPacketCode[0] != const_PacketCode_Raster_Data_Section
			&& tRasterDataPacketHeader.iPacketCode[0] != const_PacketCode_Raster_Data_Slice )
			|| tRasterDataPacketHeader.iPacketCode[1] != const_Raster_Data_Operate_Flag_1
			|| tRasterDataPacketHeader.iPacketCode[2] != const_Raster_Data_Operate_Flag_2
			|| tRasterDataPacketHeader.iPackDescriptor != const_Raster_Data_Pack_Descriptor)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(RasterDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);

			return FALSE;
		}

		// 栅格数据行数
		int iNumberOfRows = tRasterDataPacketHeader.iNumberOfRows;
		for(i=0;i<iNumberOfRows;i++)
		{
			// 为一行栅格数据分派内存
			RasterRow* pRasterRow = new RasterRow;

			// 读取栅格数据包
			if(ReadRasterRowDataPacket(pRasterRow))
			{
				bExistData = TRUE;
				pRasterRowList->push_back(pRasterRow);	// 添加到栅格数据包集合
			}
			else
			{
				_delete(pRasterRow);
				break;
			}
		}

		return bExistData;
	}

	//FMT.xBA07 读取栅格数据图像数据
	//        Raster Data Data
	//        Packet Code = 0xBA07
	//FMT.xBA0F 读取栅格数据图像数据
	//        Raster Data Data
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterImageData_Section(KRasterRowList* pRasterRowList)
	{
		return ReadRasterImageData(pRasterRowList);
	}

	//FMT.xBA07 读取栅格数据图像数据
	//        Raster Data Data
	//        Packet Code = 0xBA07
	//FMT.xBA0F 读取栅格数据图像数据
	//        Raster Data Data
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterImageData_Slice(KRasterRowList* pRasterRowList)
	{
		return ReadRasterImageData(pRasterRowList);
	}

	//FMT.x 读取 TAB 块的一页文本
	//
	//
	BOOL KRpgDataOperator::ReadTABWriteTextPage(KWriteTextList* pWriteTextList)
	{
		int i;

		// 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		while(1)
		{
			sfx_int16 iNumberOfCharactersPerLine = 0;	// 每行的字符数
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

		// 每页加一空行
		WriteText* pWriteText =  new WriteText;
		pWriteText->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;
		pWriteText->szText[0] = '\0';
		pWriteTextList->push_back(pWriteText);

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}
	///装载各种格式数据 - end
	///--------------------------------------------------------
#pragma endregion

#pragma region 读取数据包 - Read_xxxxx_Packet
	///--------------------------------------------------------
	///读取数据包 - beg
	//PACKET.01 单色文本数据包
	//     Mono Text Data Packet
	//     Packet Code = 1
	BOOL KRpgDataOperator::ReadMonoTextDataPacket(MonoText* pMonoText)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		MonoTextPacketHeader tMonoTextPacketHeader;
		fread(&tMonoTextPacketHeader,sizeof(MonoTextPacketHeader),1,fp);

		this->ReverseBytes(&tMonoTextPacketHeader);	// 字节转换

		if(tMonoTextPacketHeader.iPacketCode != const_PacketCode_Mono_Text)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(MonoTextPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}
	
		// 3. 读取数据块
		fread(&pMonoText->oPosition,sizeof(Position),1,fp);	// 文本起始位置	

		this->ReverseBytes(pMonoText);	// 字节转换

		pMonoText->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;		//没有颜色级别

		// 求字符数 = 数据块长度 - 文本起始位置所占用字节数
		int iNumberOfChar = tMonoTextPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16)*2;
		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pMonoText->szText[i] = cTemp;
		}
		pMonoText->szText[i] = '\0';

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.02 特殊符号数据包
	//     Special Symbol Packet
	//     Packet Code = 2
	BOOL KRpgDataOperator::ReadSpecilaSymbolDataPacket(SpecilaSymbol* pSpecilaSymbol)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		SpecilaSymbolPacketHeader tSpecilaSymbolPacketHeader;
		fread(&tSpecilaSymbolPacketHeader,sizeof(SpecilaSymbolPacketHeader),1,fp);

		this->ReverseBytes(&tSpecilaSymbolPacketHeader);	// 字节转换

		if(tSpecilaSymbolPacketHeader.iPacketCode != const_PacketCode_Special_Symbol)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(SpecilaSymbolPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		//3.读取数据块
		fread(&pSpecilaSymbol->oPosition,sizeof(Position),1,fp);	// 文本起始位置	

		this->ReverseBytes(pSpecilaSymbol);	// 字节转换

		pSpecilaSymbol->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;		//没有颜色级别

		//求字符数 = 数据块长度 - 文本起始位置所占用字节数
		int iNumberOfChar = tSpecilaSymbolPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16)*2;

		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pSpecilaSymbol->szText[i] = cTemp;
		}
		pSpecilaSymbol->szText[i] = '\0';

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.03 中气旋数据包 (参看PACKET.11)
	//     Mesocyclone
	//     Packet Code = 3

	//PACKET.04 风钩数据包
	//     Wind Barb Data Packet
	//     Packet Code = 4
	BOOL KRpgDataOperator::ReadWindBarbDataPacket(WindBarb* pWindBarb)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		WindBarbDataPacketHeader tWindBarbDataPacketHeader;
		fread(&tWindBarbDataPacketHeader,sizeof(WindBarbDataPacketHeader),1,fp);

		this->ReverseBytes(&tWindBarbDataPacketHeader);	// 字节转换

		if(tWindBarbDataPacketHeader.iPacketCode != const_PacketCode_Wind_Barb)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(WindBarbDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pWindBarb,sizeof(WindBarb),1,fp);

		this->ReverseBytes(pWindBarb);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.05 风矢数据包
	//     Vector Arrow Data Packet
	//     Packet Code = 5
	BOOL KRpgDataOperator::ReadVectorArrowDataPacket(VectorArrow* pVectorArrow)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		VectorArrowDataPacketHeader tVectorArrowDataPacketHeader;
		fread(&tVectorArrowDataPacketHeader,sizeof(VectorArrowDataPacketHeader),1,fp);

		this->ReverseBytes(&tVectorArrowDataPacketHeader);	// 字节转换

		if(tVectorArrowDataPacketHeader.iPacketCode != const_PacketCode_Vector_Arrow)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(VectorArrowDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pVectorArrow,sizeof(VectorArrow),1,fp);

		this->ReverseBytes(pVectorArrow);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.06 单色连接矢量数据包
	//     Linked Vector Packet
	//     Packet Code = 6
	BOOL KRpgDataOperator::ReadMonoLinkedVectorDataPacket(MonoLinkedVector* pMonoLinkedVector)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		MonoLinkedVectorPacketHeader tMonoLinkedVectorPacketHeader;
		fread(&tMonoLinkedVectorPacketHeader,sizeof(MonoLinkedVectorPacketHeader),1,fp);

		this->ReverseBytes(&tMonoLinkedVectorPacketHeader);	// 字节转换

		if(tMonoLinkedVectorPacketHeader.iPacketCode != const_PacketCode_Mono_Linked_Vector)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(MonoLinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		pMonoLinkedVector->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;

		int iLengthOfPointDataBlock = tMonoLinkedVectorPacketHeader.iLengthOfDataBlock;	// 数据块长度
		int iNumberOfPoint = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 2);			// 计算数据的点(I,J)数量, I 和 J 各占2字节)

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

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.07 单色非连接矢量数据包
	//     Unlinked Vector Packet
	//     Packet Code = 7
	BOOL KRpgDataOperator::ReadMonoUnlinkedVectorDataPacket(MonoUnlinkedVector* pMonoUnlinkedVector)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		MonoUnlinkedVectorPacketHeader tMonoUnlinkedVectorPacketHeader;
		fread(&tMonoUnlinkedVectorPacketHeader,sizeof(MonoUnlinkedVectorPacketHeader),1,fp);

		this->ReverseBytes(&tMonoUnlinkedVectorPacketHeader);	// 字节转换

		if(tMonoUnlinkedVectorPacketHeader.iPacketCode != const_PacketCode_Mono_Unlinked_Vector)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(MonoUnlinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		pMonoUnlinkedVector->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;	// 线段的颜色级别

		int iLengthOfPointDataBlock = tMonoUnlinkedVectorPacketHeader.iLengthOfDataBlock;	// 数据块的字节数
		int iNumberOfLine = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 4);				// 计算线段数量(4个点)

		for(i=0;i<iNumberOfLine;i++)
		{
			sfx_int16 X,Y;

			// 端点1
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt1;
			pt1.x = X;
			pt1.y = Y;
			pMonoUnlinkedVector->oPointVectorBeg.push_back(pt1);

			// 端点2
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt2;
			pt2.x = X;
			pt2.y = Y;
			pMonoUnlinkedVector->oPointVectorEnd.push_back(pt2);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.08 彩色文本数据包
	//     Color Text Data Packet
	//     Packet Code = 8
	BOOL KRpgDataOperator::ReadColorTextDataPacket(ColorText* pColorText)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		ColorTextPacketHeader tColorTextPacketHeader;
		fread(&tColorTextPacketHeader,sizeof(ColorTextPacketHeader),1,fp);
		this->ReverseBytes(&tColorTextPacketHeader);	// 字节转换

		if(tColorTextPacketHeader.iPacketCode != const_PacketCode_Color_Text)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(ColorTextPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}
	
		// 3. 读取数据块
		fread(&pColorText->iColorLevel,sizeof(sfx_int16),1,fp);	//文本颜色级别
		fread(&pColorText->oPosition,sizeof(Position),1,fp);	//文本起始位置	
		this->ReverseBytes(pColorText);	// 字节转换

		// 求字符数 = 数据块长度 - 文本颜色级别 - 文本起始位置所占用字节数
		int iNumberOfChar = tColorTextPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16) * 3;	// 3 = (文本颜色级别 + 文本起始位置)所占半字数

		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pColorText->szText[i] = cTemp;
		}
		pColorText->szText[i] = '\0';

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.09 彩色连接矢量数据包头
	//     Linked Vector Packet
	//     Packet Code = 9
	BOOL KRpgDataOperator::ReadColorLinkedVectorDataPacket(ColorLinkedVector* pColorLinkedVector)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		ColorLinkedVectorPacketHeader tColorLinkedVectorPacketHeader;
		fread(&tColorLinkedVectorPacketHeader,sizeof(ColorLinkedVectorPacketHeader),1,fp);
		this->ReverseBytes(&tColorLinkedVectorPacketHeader);	// 字节转换

		if(tColorLinkedVectorPacketHeader.iPacketCode != const_PacketCode_Color_Linked_Vector)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(ColorLinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(&pColorLinkedVector->iColorLevel,sizeof(sfx_int16),1,fp);	// 文本颜色级别
		KBytesOrder::ReverseBytes(&pColorLinkedVector->iColorLevel);	// 字节转换

		int iLengthOfPointDataBlock = tColorLinkedVectorPacketHeader.iLengthOfDataBlock;	// 数据块长度
		int iNumberOfPoint = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 2);				// 计算数据的点数(一个点有(I,J) I J 各占2字节)

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

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.10 彩色非连接矢量数据包头
	//     Unlinked Vector Packet
	//     Packet Code = 10
	BOOL KRpgDataOperator::ReadColorUnlinkedVectorDataPacket(ColorUnlinkedVector* pColorUnlinkedVector)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		ColorUnlinkedVectorPacketHeader tColorUnlinkedVectorPacketHeader;
		fread(&tColorUnlinkedVectorPacketHeader,sizeof(ColorUnlinkedVectorPacketHeader),1,fp);
		this->ReverseBytes(&tColorUnlinkedVectorPacketHeader);	// 字节转换
 
		if(tColorUnlinkedVectorPacketHeader.iPacketCode != const_PacketCode_Color_Unlinked_Vector)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(ColorUnlinkedVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(&pColorUnlinkedVector->iColorLevel,sizeof(sfx_int16),1,fp);	// 线段颜色级别
		KBytesOrder::ReverseBytes(&pColorUnlinkedVector->iColorLevel);		// 字节转换

		int iLengthOfPointDataBlock = tColorUnlinkedVectorPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16);	// 数据块点的字节数
		int iNumberOfLine = iLengthOfPointDataBlock / (sizeof(sfx_int16) * 4);									// 计算线段数量(4个点)

		for(i=0;i<iNumberOfLine;i++)
		{
			sfx_int16 X,Y;

			// 端点1
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);		// 字节转换
			KBytesOrder::ReverseBytes(&Y);		// 字节转换

			POINT pt1;
			pt1.x = X;
			pt1.y = Y;
			pColorUnlinkedVector->oPointVectorBeg.push_back(pt1);

			// 端点2
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);		// 字节转换
			KBytesOrder::ReverseBytes(&Y);		// 字节转换

			POINT pt2;
			pt2.x = X;
			pt2.y = Y;
			pColorUnlinkedVector->oPointVectorEnd.push_back(pt2);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.11 中气旋数据包头
	//     Mesocyclone
	//     Packet Code = 11
	BOOL KRpgDataOperator::ReadMesocycloneDataPacket(Mesocyclone* pMesocyclone)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		MesocyclonePacketHeader tMesocyclonePacketHeader;
		fread(&tMesocyclonePacketHeader,sizeof(MesocyclonePacketHeader),1,fp);

		this->ReverseBytes(&tMesocyclonePacketHeader);	// 字节转换

		if( (tMesocyclonePacketHeader.iPacketCode != const_PacketCode_Mesocyclone_3)
			&& (tMesocyclonePacketHeader.iPacketCode != const_PacketCode_Mesocyclone_11) )
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(MesocyclonePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pMesocyclone,sizeof(Mesocyclone),1,fp);

		this->ReverseBytes(pMesocyclone);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.12 龙卷蜗旋特征数据包头
	//     Tornado Vortex Signature
	//     Packet Code = 12
	BOOL KRpgDataOperator::ReadTornadoVortexSignatureDataPacket(TornadoVortexSignature* pTornadoVortexSignature)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		TornadoVortexSignaturePacketHeader tTornadoVortexSignaturePacketHeader;
		fread(&tTornadoVortexSignaturePacketHeader,sizeof(TornadoVortexSignaturePacketHeader),1,fp);

		this->ReverseBytes(&tTornadoVortexSignaturePacketHeader);	// 字节转换

		if(tTornadoVortexSignaturePacketHeader.iPacketCode != const_PacketCode_Tornado_Vortex_Signature)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(TornadoVortexSignaturePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pTornadoVortexSignature,sizeof(TornadoVortexSignature),1,fp);

		this->ReverseBytes(pTornadoVortexSignature);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.13 冰雹实况数据包头
	//     Hail Positive
	//     Packet Code = 13
	BOOL KRpgDataOperator::ReadHailPositiveDataPacket(HailPositive* pHailPositive)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		HailPositivePacketHeader tHailPositivePacketHeader;
		fread(&tHailPositivePacketHeader,sizeof(HailPositivePacketHeader),1,fp);

		this->ReverseBytes(&tHailPositivePacketHeader);	// 字节转换

		if(tHailPositivePacketHeader.iPacketCode != const_PacketCode_Hail_Positive)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(HailPositivePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pHailPositive,sizeof(HailPositive),1,fp);

		this->ReverseBytes(pHailPositive);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.14 冰雹概率数据包
	//     Hail Probable
	//     Packet Code = 14
	BOOL KRpgDataOperator::ReadHailProbableDataPacket(HailProbable* pHailProbable)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		HailProbablePacketHeader tHailProbablePacketHeader;
		fread(&tHailProbablePacketHeader,sizeof(HailProbablePacketHeader),1,fp);

		this->ReverseBytes(&tHailProbablePacketHeader);	// 字节转换

		if(tHailProbablePacketHeader.iPacketCode != const_PacketCode_Hail_Probable)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(HailProbablePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pHailProbable,sizeof(HailProbable),1,fp);

		this->ReverseBytes(pHailProbable);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.15 风暴ID数据包
	//     Storm ID
	//     Packet Code = 15
	BOOL KRpgDataOperator::ReadStormIdDataPacket(StormId* pStormId)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		StormIdPacketHeader tStormIdPacketHeader;
		fread(&tStormIdPacketHeader,sizeof(StormIdPacketHeader),1,fp);

		this->ReverseBytes(&tStormIdPacketHeader);	// 字节转换

		if(tStormIdPacketHeader.iPacketCode != const_PacketCode_Storm_Id)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(StormIdPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(&pStormId->oPosition,sizeof(Position),1,fp);	// 文本起始I位置

		this->ReverseBytes(&pStormId->oPosition);			// 字节转换

		pStormId->iColorLevel = RPG_DATA_NO_COLOR_LEVEL;	//没有颜色级别

		//求字符数 = 数据块长度 - 文本起始位置所占用字节数
		int iNumberOfChar = tStormIdPacketHeader.iLengthOfDataBlock - sizeof(sfx_int16) * 2;

		for(i=0;i<iNumberOfChar;i++)
		{
			char cTemp = getc(fp);
			pStormId->szText[i] = cTemp;
		}
		pStormId->szText[i] = '\0';

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.16 数字径向数据阵列数据包
	//     Digital Radial Data Array Packet
	//     Packet Code = 16
	BOOL KRpgDataOperator::ReadDigitalRadialDataArrayDataPacket(DigitalRadialDataArray* pDigitalRadialDataArray)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		{
			// waiting ...
			Assert(FALSE);
		}


		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return FALSE;
	}

	//PACKET.17 数字降水数据阵列数据包头
	//     Digital Precipatation Data Array Packet
	//     Packet Code = 17
	BOOL KRpgDataOperator::ReadDigitalPrecipitationDataArrayDataPacket(DigitalPrecipitationDataArray* pDigitalPrecipitationDataArray)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读每行栅格数据头
		fread(&pDigitalPrecipitationDataArray->oEachRowHeader,sizeof(EachRowHeader),1,fp);

		this->ReverseBytes(&pDigitalPrecipitationDataArray->oEachRowHeader);			// 字节转换

		// 3. 读取数据块
		int iNumberOfBytesInThisRow = pDigitalPrecipitationDataArray->oEachRowHeader.iNumberOfBytesInThisRow;	// 该行字节数
		int iNumberOfRunColor = iNumberOfBytesInThisRow / 2;							// 该行RLE编码数量
		pDigitalPrecipitationDataArray->pRunColor = new RunColor[iNumberOfRunColor];	// 为 TRunColor* 分配存储空间

		for(i=0;i<iNumberOfRunColor;i++)
		{
			sfx_uint16 iRunColor = 0;
			fread(&iRunColor,sizeof(sfx_uint16),1,fp);
			pDigitalPrecipitationDataArray->pRunColor[i].Attach(iRunColor);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.18 降水率数据阵列数据包
	//     Precipitation Rate Data Array Packet
	//     Packet Code = 18
	BOOL KRpgDataOperator::ReadPrecipitationRateDataArrayDataPacket(PrecipitationRateDataArray* pPrecipitationRateDataArray)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读每行栅格数据头
		fread(&pPrecipitationRateDataArray->oEachRowHeader,sizeof(EachRowHeader),1,fp);

		this->ReverseBytes(&pPrecipitationRateDataArray->oEachRowHeader);			// 字节转换

		// 3. 读取数据块
		int iNumberOfBytesInThisRow = pPrecipitationRateDataArray->oEachRowHeader.iNumberOfBytesInThisRow;	//该行字节数
		int iNumberOfRunColor = iNumberOfBytesInThisRow;							// 该行RLE编码数量
		pPrecipitationRateDataArray->pRunColor = new RunColor[iNumberOfRunColor];	// 为 TRunColor* 分配存储空间

		for(i=0;i<iNumberOfRunColor;i++)
		{
			char cRunColor = '\0';
			fread(&cRunColor,sizeof(char),1,fp);
			pPrecipitationRateDataArray->pRunColor[i].Attach(cRunColor);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.19 冰雹概率数据包 waiting... HDA ???
	//     HDA Hail
	//     Packet Code = 19
	BOOL KRpgDataOperator::ReadHdaHailDataPacket(HdaHail* pHdaHail)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		HdaHailPacketHeader tHdaHailPacketHeader;
		fread(&tHdaHailPacketHeader,sizeof(HdaHailPacketHeader),1,fp);

		this->ReverseBytes(&tHdaHailPacketHeader);	// 字节转换

		if(tHdaHailPacketHeader.iPacketCode != const_PacketCode_Hda_Hail)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(HdaHailPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		fread(pHdaHail,sizeof(HdaHail),1,fp);

		this->ReverseBytes(pHdaHail);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.20 待定数据包

	//PACKET.21 待定数据包

	//PACKET.22 待定数据包

	//PACKET.23 风暴中心路径信息过去数据包
	//     SCIT Past Data
	//     Packet Code = 23
	// 包含 2,6,25 数据包
	BOOL KRpgDataOperator::ReadScitPastDataPacket(ScitPast* pScitPast)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		ScitPastDataPacketHeader tScitPastDataPacketHeader;
		fread(&tScitPastDataPacketHeader,sizeof(ScitPastDataPacketHeader),1,fp);

		this->ReverseBytes(&tScitPastDataPacketHeader);	// 字节转换

		if(tScitPastDataPacketHeader.iPacketCode != const_PacketCode_Scit_Past_Data)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(ScitPastDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 保存指针位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 3.读取数据
		// 3.1 Packet Code = 2
		ReadSpecilaSymbolData(&pScitPast->oSpecilaSymbolList);
		// 3.2 Packet Code = 6
		ReadMonoLinkedVectorData(&pScitPast->oMonoLinkedVectorList);
		// 3.3 Packet Code = 25
		ReadStiCircleData(&pScitPast->oStiCircleList);

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.24 风暴中心路径信息预报数据包
	//     SCIT Forecast Data
	//     Packet Code = 24
	// 包含 2,6,25 数据包
	BOOL KRpgDataOperator::ReadScitForecastDataPacket(ScitForecast* pScitForecast)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		ScitForecastDataPacketHeader tScitForecastDataPacketHeader;
		fread(&tScitForecastDataPacketHeader,sizeof(ScitForecastDataPacketHeader),1,fp);

		this->ReverseBytes(&tScitForecastDataPacketHeader);	// 字节转换

		if(tScitForecastDataPacketHeader.iPacketCode != const_PacketCode_Scit_Forecast_Data)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(ScitForecastDataPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		//保存指针位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		// 3.读取数据
		// 3.1 Packet Code = 2
		ReadSpecilaSymbolData(&pScitForecast->oSpecilaSymbolList);
		// 3.2 Packet Code = 6
		ReadMonoLinkedVectorData(&pScitForecast->oMonoLinkedVectorList);
		// 3.3 Packet Code = 25
		ReadStiCircleData(&pScitForecast->oStiCircleList);

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.25 风暴路径信息圈数据包
	//     STI Circle
	//     Packet Code = 25
	BOOL KRpgDataOperator::ReadStiCircleDataPacket(StiCircle* pStiCircle)
	{
		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		StiCirclePacketHeader tStiCirclePacketHeader;
		fread(&tStiCirclePacketHeader,sizeof(StiCirclePacketHeader),1,fp);

		this->ReverseBytes(&tStiCirclePacketHeader);	// 字节转换

		if(tStiCirclePacketHeader.iPacketCode != const_PacketCode_Sti_Circle)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(StiCirclePacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		//3.读取数据块
		fread(pStiCircle,sizeof(StiCircle),1,fp);
		this->ReverseBytes(pStiCircle);	// 字节转换

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.x3501 非连接等值线矢量数据包
	//        UnLinked Contour Vectors Packet
	//        Packet Code = 0x3501
	BOOL KRpgDataOperator::ReadUnLinkedContourVectorDataPacket(UnlinkedContourVector* pUnlinkedContourVector)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		UnlinkedContourVectorPacketHeader tUnlinkedContourVectorPacketHeader;
		fread(&tUnlinkedContourVectorPacketHeader,sizeof(UnlinkedContourVectorPacketHeader),1,fp);

		this->ReverseBytes(&tUnlinkedContourVectorPacketHeader);	// 字节转换

		if(tUnlinkedContourVectorPacketHeader.iPacketCode != const_PacketCode_Unlinked_Contour_Vector)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(UnlinkedContourVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return false;
		}

		// 3. 读取数据块
		int iNumberOfLine = tUnlinkedContourVectorPacketHeader.iLengthOfVector / (sizeof(sfx_int16) * 4);	//计算线段条数(1线段两个点)

		for(i=0;i<iNumberOfLine;i++)
		{
			sfx_int16 X,Y;

			// 端点1
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt1;
			pt1.x = X;
			pt1.y  =Y;
			pUnlinkedContourVector->oPointVectorBeg.push_back(pt1);

			// 端点2
			fread(&X,sizeof(sfx_int16),1,fp);
			fread(&Y,sizeof(sfx_int16),1,fp);

			KBytesOrder::ReverseBytes(&X);
			KBytesOrder::ReverseBytes(&Y);

			POINT pt2;
			pt2.x = X;
			pt2.y  =Y;
			pUnlinkedContourVector->oPointVectorEnd.push_back(pt2);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.x0E03 连接等值线矢量数据包
	//        Linked Contour Vectors Packet
	//        Packet Code = 0x0E03
	BOOL KRpgDataOperator::ReadLinkedContourVectorDataPacket(LinkedContourVector* pLinkedContourVector)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读取数据包头
		LinkedContourVectorPacketHeader tLinkedContourVectorPacketHeader;
		fread(&tLinkedContourVectorPacketHeader,sizeof(LinkedContourVectorPacketHeader),1,fp);

		this->ReverseBytes(&tLinkedContourVectorPacketHeader);	// 字节转换

		if(tLinkedContourVectorPacketHeader.iPacketCode != const_PacketCode_Linked_Contour_Vector)
		{
			// 如果包代码错误要回滚文件指针
			fseek(fp,-1*(int)sizeof(LinkedContourVectorPacketHeader),SEEK_CUR);
			lOffsetCur = ftell(fp);
			SetFilePtrOffset(lOffsetCur);
			return FALSE;
		}

		// 3. 读取数据块
		// 3.1 线头
		if(tLinkedContourVectorPacketHeader.iInitialPointIndicator == const_Initial_Point_Indicator)
		{// 等值线起点包含在包中
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
		{// 等值线起点不包含在包中
			POINT pt;
			pt.x = 0;
			pt.y = 0;
			pLinkedContourVector->oPointVector.push_back(pt);
		}

		// 3.2 处理本条等值线的其他点
		// 读入该条等值线上其他点所占的长度(字节: 4 to 32764, length = # of vectors * 4)
		sfx_uint16	iLengthOfOtherVector;
		fread(&iLengthOfOtherVector,sizeof(sfx_uint16),1,fp);
		KBytesOrder::ReverseBytes(&iLengthOfOtherVector);	// 字节转换

		// 计算其他节点数量(1节点 = 2个16位整数 = 4个字节)
		int iNumberOfPoint = iLengthOfOtherVector / (sizeof(sfx_uint16) * 2);	// 节点数量

		//下面读取一条等值线的其他点数据
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

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.xAF1F 径向数据包
	//        Radial Data Packet
	//        Packet Code = 0xAF1F
	BOOL KRpgDataOperator::ReadRadialDataPacket(Radial* pRadial)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 读每条径向射线数据头
		fread(&pRadial->oEachRadialHeader,sizeof(EachRadialHeader),1,fp);

		this->ReverseBytes(&pRadial->oEachRadialHeader);	// 字节转换

		// 3. 读取数据块
		int iNumbersOfRunColor = pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;	//每条射线的行程编码(RLECode)数(16bits,注意要 *2)
		pRadial->pRunColor = new RunColor[iNumbersOfRunColor];								//为距离库的行程编码结构分配

		for(i=0;i<iNumbersOfRunColor;i++)
		{// 读入射线上距离库的行程编码
			char cRunColor = '\0';
			fread(&cRunColor,sizeof(char),1,fp);
			pRadial->pRunColor[i].Attach(cRunColor);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}

	//PACKET.xBA07 栅格数据包
	//        Raster Data Packet
	//        Packet Code = 0xBA07
	//PACKET.xBA0F 栅格数据包
	//        Raster Data Packet
	//        Packet Code = 0xBA0F
	BOOL KRpgDataOperator::ReadRasterRowDataPacket(RasterRow* pRasterRow)
	{
		int i;

		// 1. 定位文件指针
		FILE* fp = GetFilePtr();
		long lOffsetCur = GetFilePtrOffset();
		fseek(fp,lOffsetCur,SEEK_SET);

		// 2. 栅格数据每行数据头
		fread(&pRasterRow->oEachRowHeader,sizeof(EachRowHeader),1,fp);

		this->ReverseBytes(&pRasterRow->oEachRowHeader);	// 字节转换

		// 3. 读取数据块
		int iNumbersOfRunColor = pRasterRow->oEachRowHeader.iNumberOfBytesInThisRow;	// 每条射线的行程编码(RLECode)数
		pRasterRow->pRunColor = new RunColor[iNumbersOfRunColor];						// 为 TRunColor* 分配存储空间

		for(i=0;i<iNumbersOfRunColor;i++)
		{// 读入行程编码
			char cRunColor='\0';
			fread(&cRunColor,sizeof(char),1,fp);
			pRasterRow->pRunColor[i].Attach(cRunColor);
		}

		// 4. 设置文件指针当前位置
		lOffsetCur = ftell(fp);
		SetFilePtrOffset(lOffsetCur);

		return TRUE;
	}
	///读取数据包 - end
	///--------------------------------------------------------
#pragma endregion
	///===============
	///读取数据 - end
	///========================================================

	///========================================================
	///生成图像 - beg
	///===============
#pragma region 生成图像 - BuildImage
	// 生成图像
	BOOL KRpgDataOperator::BuildImage(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		if(pRpgData->IsEmpty())
			return FALSE;

		// 取得产品类型
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
	/// 分类生成图像 - Beg
	// 1 使用 KRadialList
	BOOL KRpgDataOperator::BuildImage_Radial(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//图像尺寸
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// 默认图像宽度为512像素
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// 默认图像高度为512像素
		DWORD dwWidthInfo	 = 170;									// 默认图像信息区宽度为170像素
		DWORD dwHeightInfo	 = dwHeightImage;						// 默认图像信息区高度为图像高度

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("要求图像宽度和高度一致"),_T("错误"),MB_OK);
			return FALSE;
		}

		// 根据图像尺寸调整信息区尺寸
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// 计算图像区 + 信息区尺寸
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// 图像区
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// 信息区
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// 图像 + 信息区
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)

		// 创建兼容 DC(注意释放)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// 画刷
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// 位图句柄
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// 数据层数
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// 确保只有一层

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// 背景颜色

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// 背景颜色

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// 背景颜色

			// 绘制图像
			KRadialList* pRadialList = pRpgData->GetRadialList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pRadialList);

			// 绘制坐标
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// 绘制信息区
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// 合并 hImageDC 和 hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// 设置位图句柄
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// 释放桌面DC句柄

		return TRUE;
	}
	// 2 使用 KRasterRowList
	BOOL KRpgDataOperator::BuildImage_Raster(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//图像尺寸
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// 默认图像宽度为512像素
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// 默认图像高度为512像素
		DWORD dwWidthInfo	 = 170;									// 默认图像信息区宽度为170像素
		DWORD dwHeightInfo	 = dwHeightImage;						// 默认图像信息区高度为图像高度

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("要求图像宽度和高度一致"),_T("错误"),MB_OK);
			return FALSE;
		}

		// 根据图像尺寸调整信息区尺寸
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// 计算图像区 + 信息区尺寸
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// 图像区
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// 信息区
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// 图像 + 信息区
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)

		// 创建兼容 DC(注意释放)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// 画刷
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// 位图句柄
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// 数据层数
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// 确保只有一层

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// 背景颜色

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// 背景颜色

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// 背景颜色

			// 绘制图像
			KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pRasterRowList);

			// 绘制坐标
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// 绘制信息区
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// 合并 hImageDC 和 hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// 设置位图句柄
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// 释放桌面DC句柄

		return TRUE;
	}
	// 3
	BOOL KRpgDataOperator::BuildImage_LinkedContourVector(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//图像尺寸
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// 默认图像宽度为512像素
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// 默认图像高度为512像素
		DWORD dwWidthInfo	 = 170;									// 默认图像信息区宽度为170像素
		DWORD dwHeightInfo	 = dwHeightImage;						// 默认图像信息区高度为图像高度

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("要求图像宽度和高度一致"),_T("错误"),MB_OK);
			return FALSE;
		}

		// 根据图像尺寸调整信息区尺寸
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// 计算图像区 + 信息区尺寸
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// 图像区
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// 信息区
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// 图像 + 信息区
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)

		// 创建兼容 DC(注意释放)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// 画刷
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// 位图句柄
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// 数据层数
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// 确保只有一层

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// 背景颜色

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// 背景颜色

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// 背景颜色

			// 绘制图像
			KLinkedContourVectorList* pLinkedContourVectorList = pRpgData->GetLinkedContourVectorList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pLinkedContourVectorList);

			// 绘制坐标
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// 绘制信息区
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// 合并 hImageDC 和 hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// 设置位图句柄
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// 释放桌面DC句柄

		return TRUE;
	}
	// 4
	// SWP47 + STI58 + HI59 + M60 + TVS61
	BOOL KRpgDataOperator::BuildImage_GeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();
	
		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//图像尺寸
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// 默认图像宽度为512像素
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// 默认图像高度为512像素
		DWORD dwWidthInfo	 = 170;								// 默认图像信息区宽度为170像素
		DWORD dwHeightInfo	 = dwHeightImage;					// 默认图像信息区高度为图像高度

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("要求图像宽度和高度一致"),_T("错误"),MB_OK);
			return FALSE;
		}

		// 根据图像尺寸调整信息区尺寸
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// 计算图像区 + 信息区尺寸
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// 图像区
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// 信息区
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// 图像 + 信息区
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)

		// 创建兼容 DC(注意释放)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// 画刷
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// 位图句柄
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// 数据层数
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// 确保只有一层

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// 背景颜色

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// 背景颜色

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// 背景颜色

			// 绘制图像
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


			// 绘制坐标
			if(bDrawCoordinate)
			{
				DrawPolar(hImageDC,&rectImage,fZoom);
			}

			// 绘制信息区
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// 合并 hImageDC 和 hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// 设置位图句柄
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// 释放桌面DC句柄

		return TRUE;
	}
	// 5
	// 48,84
	BOOL KRpgDataOperator::BuildImage_NonGeographicAlpha(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();
	
		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//图像尺寸
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// 默认图像宽度为512像素
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// 默认图像高度为512像素
		DWORD dwWidthInfo	 = 170;								// 默认图像信息区宽度为170像素
		DWORD dwHeightInfo	 = dwHeightImage;					// 默认图像信息区高度为图像高度

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("要求图像宽度和高度一致"),_T("错误"),MB_OK);
			return FALSE;
		}

		// 根据图像尺寸调整信息区尺寸
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// 计算图像区 + 信息区尺寸
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// 图像区
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// 信息区
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// 图像 + 信息区
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)

		// 创建兼容 DC(注意释放)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// 画刷
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// 位图句柄
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// 数据层数
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		Assert(iTotalLayers==1);	// 确保只有一层

		for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// 背景颜色

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// 背景颜色

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// 背景颜色


			// 绘制坐标
			if(bDrawCoordinate)
			{
				// 绘制笛卡尔坐标
				DrawDescartes(hImageDC,&rectImage,fZoom,pRpgData->GetColorUnlinkedVectorList(),pRpgData->GetColorTextList());
			}

			//绘制WindBarb图像
			if(iProductCode == 48)
			{
				DrawImage(hImageDC,&rectImage,fZoom,pRpgData->GetWindBarbList());
			}
			//绘制 Linked Vtetor 图像
			else if(iProductCode == 84)
			{
				DrawImage(hImageDC,&rectImage,fZoom,pRpgData->GetColorLinkedVectorList());
			}

			// 绘制信息区
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// 合并 hImageDC 和 hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// 设置位图句柄
			pRpgData->SetImageHandle(hBitmapTotal);
		}// for(i)

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// 释放桌面DC句柄

		return TRUE;
	}
	// 6
	// 62,82
	BOOL KRpgDataOperator::BuildImage_Alphanumeric(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		// 不需要生成图像?
		return FALSE;
	}
	// 7 剖面图
	BOOL KRpgDataOperator::BuildImage_Raster_Section(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		//图像尺寸
		DWORD dwWidthImage	= pRpgData->GetImageWidth();		// 默认图像宽度为512像素
		DWORD dwHeightImage	= pRpgData->GetImageHeight();		// 默认图像高度为512像素
		DWORD dwWidthInfo	 = 170;								// 默认图像信息区宽度为170像素
		DWORD dwHeightInfo	 = dwHeightImage;					// 默认图像信息区高度为图像高度

		if(dwWidthImage!=dwHeightImage)
		{
			::MessageBox(NULL,_T("要求图像宽度和高度一致"),_T("错误"),MB_OK);
			return FALSE;
		}

		// 根据图像尺寸调整信息区尺寸
		dwWidthInfo = dwWidthInfo * dwWidthImage / 512;

		// 计算图像区 + 信息区尺寸
		DWORD dwWidthTotal  = bDrawInfoArea ? dwWidthImage + dwWidthInfo : dwWidthImage;
		DWORD dwHeightTotal = dwHeightImage;

		// 图像区
		RECT rectImage;
		rectImage.left = 0;
		rectImage.top = 0;
		rectImage.right = dwWidthImage;
		rectImage.bottom = dwHeightImage;

		// 信息区
		RECT rectInfo;
		rectInfo.left = 0;
		rectInfo.top = 0;
		rectInfo.right = dwWidthInfo;
		rectInfo.bottom = dwHeightInfo;

		// 图像 + 信息区
		RECT rectTotal;
		rectTotal.left = 0;
		rectTotal.top = 0;
		rectTotal.right = dwWidthTotal;
		rectTotal.bottom = dwHeightTotal;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)

		// 创建兼容 DC(注意释放)
		HDC hTotalDC = ::CreateCompatibleDC(hDC);
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		HDC hInfoDC = ::CreateCompatibleDC(hDC);

		// 画刷
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);

		// 位图句柄
		HBITMAP hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
		HBITMAP hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
		HBITMAP hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);

		// 数据层数
		int iTotalLayers = pRpgData->PSBPtr()->iTotalLayers;
		// 雷达产品的  Raster Iamge Section 有2层
		Assert(iTotalLayers==2);

		//for(i=0;i<iTotalLayers;i++)
		{
			::DeleteObject(hBitmapTotal);
			hBitmapTotal = ::CreateCompatibleBitmap(hDC,dwWidthTotal,dwHeightTotal);
			::SelectObject(hTotalDC,hBitmapTotal);
			::FillRect(hTotalDC,&rectTotal,hBrush);	// 背景颜色

			::DeleteObject(hBitmapImage);
			hBitmapImage = ::CreateCompatibleBitmap(hDC,dwWidthImage,dwHeightImage);
			::SelectObject(hImageDC,hBitmapImage);
			::FillRect(hImageDC,&rectImage,hBrush);	// 背景颜色

			::DeleteObject(hBitmapInfoArea);
			hBitmapInfoArea = ::CreateCompatibleBitmap(hDC,dwWidthInfo,dwHeightInfo);
			::SelectObject(hInfoDC,hBitmapInfoArea);
			::FillRect(hInfoDC,&rectInfo,hBrush);	// 背景颜色

			// 绘制图像
			KRasterRowList* pRasterRowList = pRpgData->GetRasterRowList();
			this->DrawImage(hImageDC,&rectImage,fZoom,pRasterRowList);

			// 绘制坐标
			if(bDrawCoordinate)
			{
				// 绘制笛卡尔坐标
				DrawDescartes(hImageDC,&rectImage,fZoom,pRpgData->GetMonoUnlinkedVectorList(),pRpgData->GetMonoTextList());
			}

			// 绘制信息区
			if(bDrawInfoArea)
			{
				DrawInfoArea(hInfoDC,&rectInfo);
			}

			// 合并 hImageDC 和 hInfoDC
			// 1. hImageDC
			::BitBlt(hTotalDC,0,0,dwWidthImage,dwHeightImage,hImageDC,0,0,SRCCOPY);

			// 2. hInfoDC
			if(bDrawInfoArea)
			{
				::BitBlt(hTotalDC,dwWidthImage,0,dwWidthInfo,dwHeightInfo,hInfoDC,0,0,SRCCOPY);
			}

			// 设置位图句柄
			pRpgData->SetImageHandle(hBitmapTotal);
		}

		// Cleanup GDI
		::DeleteObject(hBitmapInfoArea);
		::DeleteObject(hBitmapImage);
		::DeleteObject(hBrush);
		::DeleteDC(hInfoDC);
		::DeleteDC(hImageDC);
		::DeleteDC(hTotalDC);
		::ReleaseDC(NULL,hDC);		// 释放桌面DC句柄

		return TRUE;
	}
	// 8 切片图
	// waiting...
	BOOL KRpgDataOperator::BuildImage_Raster_Slice(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		return FALSE;
	}
	/// 分类生成图像 - end
	///--------------------------------------------------------
#pragma endregion


#pragma region 分类绘制图像 - DrawImage
	///--------------------------------------------------------
	/// 分类绘制图像 - Beg

	// 绘制文本数据包链表 [P3-108][P3-110][P3-109][P3-130]
	//     Write Text Data Packet
	//     Packet Code = 1,2,8,15
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWriteTextList* pWriteTextList,BOOL bSpecilaSymbol)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
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
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		//绘制笛卡尔 fZoom 没有使用
		if(iProductCode == VWP48 ||iProductCode == RCS50 || iProductCode == VAD84)
		{
			fZoom = 1.0f;
		}

		::SetBkColor(hDC,clrBack);
		::SetBkMode(hDC,TRANSPARENT);

		//绘制标注文本
		KWriteTextList::iterator it;
		KWriteTextList::iterator ibeg = pWriteTextList->begin();
		KWriteTextList::iterator iend = pWriteTextList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// 标注文本
			WriteText* pWriteText = (*it);

			// 颜色等级
			int iColorLevel = pWriteText->iColorLevel;

			// 文本颜色
			COLORREF clr = 0;

			// 根据颜色等级取得颜色值
			if(iColorLevel != RPG_DATA_NO_COLOR_LEVEL)
			{
				clr = pLegend[iColorLevel].dwColor;
			}
			else	// == -1 人为设置文本颜色
			{
				clr = RGB(0,192,255);
			}

			double x,y;

			// SWP47 STI58 HI59 
			if(iProductCode == SWP47 || iProductCode == STI58 || iProductCode == HI59)
			{
				// 把 km/4 转换为屏幕像素(以屏幕中心为参照)
				x = 256 + 1.0 * pWriteText->oPosition.I / 4 * fZoom;
				y = 256 - 1.0 * pWriteText->oPosition.J / 4 * fZoom;
			}
			else
			{
				// 本身就是屏幕像素
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
	// 中气旋数据包 [P3-129]
	//     Mesocyclone
	//     Packet Code = 3,11
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KMesocycloneList* pMesocycloneList)
	{
		return FALSE;
	}
	// 风钩数据包 [P3-128]
	//     Wind Barb Data Packet
	//     Packet Code = 4
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KWindBarbList* pWindBarbList)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// 节转换为米/秒
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
			// 风钩指针
			WindBarb* pWindBarb = (*it);

			//根据颜色等级取得风钩颜色值
			int iColorLevel = pWindBarb->iColorLevelOfWindBarb;
			COLORREF clr = pLegend[iColorLevel].dwColor;

			//风钩坐标
			double x = pWindBarb->iXCoordinate * dPixelPerKm;
			double y = pWindBarb->iYCoordinate * dPixelPerKm;

			//风向、风速
			double dWindDirection	= PI*(pWindBarb->iWindDirection-90)/180;
			double dWindSpeed		= pWindBarb->iWindSpeed;
			//dWindSpeed				= pWindBarb->iWindSpeed*dknot2mps;		//把节变成mps
			dWindSpeed				= pWindBarb->iWindSpeed * 1.25;

			//------------------------------------------------
			double x0 = x;
			double y0 = y;
			double plen = 16.0*dPixelPerKm;			//风钩杆长度
			double blen = 10.0*dPixelPerKm;			//倒刺长度Barb length
			double bgap = 2.0*dPixelPerKm;			//倒刺间隔
			double padd	= 0.0;						//风钩方向调整角度
			double var	= dWindSpeed;				//风速(m/s)

			double rad  = 3.0*dPixelPerKm;		//圆圈半径
			//------------------------------------------------

			// 选入画笔
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);

			// 选入画刷
			::DeleteObject(hBrush);
			hBrush = ::CreateSolidBrush(clr);
			::SelectObject(hDC,hBrush);

			if (var<2.01) //在(x,y)处绘制圆
			{
				::Ellipse(hDC,(int)(x0-rad),(int)(y0-rad),(int)(x0+rad),(int)(y0+rad));
				continue;
			}
			else
			{// 调整风速等
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
			{//绘制三角形
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
	// 风矢数据包 [P3-127]
	//     Vector Arrow Data Packet
	//     Packet Code = 5
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KVectorArrowList* pVectorArrowList)
	{
		return FALSE;
	}
	// 绘制连接矢量数据包 [P3- 97][P3-105][P3-105]
	//     Linked Vector Packet
	//     Packet Code = 6,9,0x0e03
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KLinkedVectorList* pLinkedVectorList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm * 230 / dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
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
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetTextAlign(hDC,TA_BASELINE|TA_CENTER);

		KLinkedVectorList::iterator it;
		KLinkedVectorList::iterator ibeg = pLinkedVectorList->begin();
		KLinkedVectorList::iterator iend = pLinkedVectorList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// 取得一条连接矢量指针
			LinkedVector* pLinkedVector = (*it);

			// 本条等值线的颜色等级
			int iColorLevel = pLinkedVector->iColorLevel;
			if(iColorLevel>=0 && (iColorLevel<iThreshold_min || iColorLevel>iThreshold_max))
			{
				continue;
			}

			// 根据颜色等级取得颜色值
			COLORREF clr = RGB(201,201,201);
			if(iColorLevel != RPG_DATA_NO_COLOR_LEVEL)
			{
				clr = pLegend[iColorLevel].dwColor;
			}

			// 选入画笔
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);
			::SetTextColor(hDC,clr);

			// 本条等值线的点数
			int iNumberOfPoint = (int)pLinkedVector->oPointVector.size();
			// 绘制一条等值线
			for(i=0;i<iNumberOfPoint;i++)
			{
				POINT pt = pLinkedVector->oPointVector[i];

				double x = 0;
				double y = 0;

				// km/4 转换为像素点位置(距离雷达中心)
				x = (double)iXCenter + 0.25 * pt.x * dPixelPerKm * fZoom + 0.5;
				y = (double)iYCenter - 0.25 * pt.y * dPixelPerKm * fZoom + 0.5;
				if(iProductCode==88)
				{
				}

				if(i==0) // 线头
				{
					::MoveToEx(hDC,(int)x,(int)y,NULL);
				}
				else
				{
					::LineTo(hDC,(int)x,(int)y);

					// 针对 58 号产品
					if(m_bScitPast)
					{
						::TextOut(hDC,(int)x,(int)y,_T("●"),1);	
					}
					if(m_bScitForecast)
					{
						::TextOut(hDC,(int)x,(int)y,_T("＋"),1);	
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
	// 绘制非连接的矢量数据图像 [P3-101][P3-102][P3-105]
	//     Unlinked Vector Packet
	//     Packet Code = 7,10,x3501
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange>0 && dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
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
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		// 绘制标注文本
		KUnlinkedVectorList::iterator it;
		KUnlinkedVectorList::iterator ibeg = pUnlinkedVectorList->begin();
		KUnlinkedVectorList::iterator iend = pUnlinkedVectorList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// 取得单色非连接矢量数据包
			UnlinkedVector* pUnlinkedVector = (*it);

			// 颜色等级
			int iColorLevel = pUnlinkedVector->iColorLevel;

			// 根据颜色等级取得颜色值
			COLORREF clr = 0;;
			if(iColorLevel != RPG_DATA_NO_COLOR_LEVEL)
			{
				clr = pLegend[iColorLevel].dwColor;
			}
			else
			{
				clr = RGB(192,192,192);	// 自定义颜色
			}

			// 选入画笔
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
	// 径向数据包 [P3-125][P3-117]
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

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(pRpgData->GetProductCode());

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange<230)
		{
			dPixelPerKm = dPixelPerKm*230/dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		// 读径向图像数据包头
		RadialDataPacketHeader* pRadialDataPacketHeader = pRpgData->GetRadialDataPacketHeader();

		int iIndexOfFirstRangeBin		= pRadialDataPacketHeader->iIndexOfFirstRangeBin;		// 第一个距离库的索引(0 to 460)
		int iNumberOfRangeBinsPerRadial	= pRadialDataPacketHeader->iNumberOfRangeBinsPerRadial;	// 组成一条径向数据的距离库数(1 to 460)
		int iICenterOfSweep				= pRadialDataPacketHeader->iICenterOfSweep;				// 扫描中心的I坐标(单位:km/4 -2048 to 2047)
		int iJCenterOfSweep				= pRadialDataPacketHeader->iJCenterOfSweep;				// 扫描中心的J坐标(单位:km/4 -2048 to 2047)
		int iScaleFactor				= pRadialDataPacketHeader->iScaleFactor;				// 每个库包含的像素数(1 to 8000 精度:0.001)
		int iNumberOfRadials			= pRadialDataPacketHeader->iNumberOfRadials;			// 射线条数(1 to 400 精度:1)

		// 每个库包含的像素数(考虑放大倍数和图像大小)0.998
		double dPixelPerBin = (0.001 * iScaleFactor) * dPixelPerKm * fZoom;

		KRadialList::iterator it;
		KRadialList::iterator ibeg = pRadialList->begin();
		KRadialList::iterator iend = pRadialList->end();
		for(it=ibeg;it!=iend;it++)
		{
			// 取得一条射线
			Radial* pRadial = (*it);

			// 射线的开始角度
			// 左旋转90度
			// 极坐标以x轴正向为0度
			// 直角坐标以y轴正向为0度
			// 二者相差90度
			double alf0 = pRadial->oEachRadialHeader.iRadialStartAngle * 0.1 - 90;	// -90 正北方表示0度

			// 射线每个库在开始角度上X和Y方向的增量
			double dw0 = (dPixelPerBin * cos(alf0 * 3.14159 / 180));
			double dh0 = (dPixelPerBin * sin(alf0 * 3.14159 / 180));

			// 射线每个库在开始角度上的起始坐标
			double x0Beg = dXOrigin + dw0 * iIndexOfFirstRangeBin;
			double y0Beg = dYOrigin + dh0 * iIndexOfFirstRangeBin;

			// 射线的结束角度
			double alf1 = alf0 + pRadial->oEachRadialHeader.iDeltaAngle * 0.1;	//加上展角
		
			// 射线每个库在结束角度上X和Y方向的增量
			double dw1 = dPixelPerBin * cos(alf1 * 3.14159 / 180);
			double dh1 = dPixelPerBin * sin(alf1 * 3.14159 / 180);

			// 射线每个库在结束角度上的起始坐标
			double x1Beg = dXOrigin + dw1 * iIndexOfFirstRangeBin;
			double y1Beg = dYOrigin + dh1 * iIndexOfFirstRangeBin;

			POINT pt[4];

			pt[0].x = (LONG)(x0Beg+0.5);	//四舍五入
			pt[0].y = (LONG)(y0Beg+0.5);
			pt[1].x = (LONG)(x1Beg+0.5);
			pt[1].y = (LONG)(y1Beg+0.5);

			// 射线上要绘制的距离库号
			int iCurRun = 0;

			// 每条射线的距离库数量
			int iNumberOfRangeBinsPerRadial = pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;

			// 绘制各个距离库
			for(i=0;i<iNumberOfRangeBinsPerRadial;i++)
			{
				// 库号
				int iRun = pRadial->pRunColor[i].iRun;

				iCurRun += iRun;	// 当前库号

				pt[3].x=(int)(x0Beg + dw0 * iCurRun + 0.5);	// pt[3]和pt[0]
				pt[3].y=(int)(y0Beg + dh0 * iCurRun + 0.5);	// 在起始角度上

				pt[2].x=(int)(x1Beg + dw1 * iCurRun + 0.5);	// pt[1]和pt[2]
				pt[2].y=(int)(y1Beg + dh1 * iCurRun + 0.5);	// 在结束角度上

				// 库颜色索引
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

					// 绘制距离库多边形
					::Polygon(hDC,pt,4);

				}

				// 调整pt[0]和pt[2]
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
	// 栅格数据包 [P3-121][P3-123][P3-119]
	//     Raster Data Packet
	//     Packet Code = 17,18,xBA07
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KRasterRowList* pRasterRowList)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 对于径向数据图像考虑产品覆盖范围
		// 对于栅格数据图像考虑产品覆盖范围因素已经包含在 iXScaleINT 和 iYScaleINT
		//if(dRange<230)
		//{
		//	dPixelPerKm = dPixelPerKm * 230 / dRange;
		//}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
		int iThreshold_min,iThreshold_max;
		pRpgData->GetThreshold(&iThreshold_min,&iThreshold_max);

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		// 垂直剖面(50)
		if(iProductCode==50)
		{
			fZoom = 1.0;
		}


		// 读栅格图像数据包头
		RasterDataPacketHeader* pRasterDataPacketHeader = pRpgData->GetRasterDataPacketHeader();

		int iICoordStart	= pRasterDataPacketHeader->iICoordinateStart;	//数据开始的行位置
		int iJCoordStart	= pRasterDataPacketHeader->iJCoordinateStart;	//数据开始的列位置
		double iXScaleINT	= pRasterDataPacketHeader->iXScaleINT;			//X方向一个要素值占的格点数
		double iYScaleINT	= pRasterDataPacketHeader->iYScaleINT;			//Y方向一个要素值占的格点数
		int iNumberOfRows	= pRasterDataPacketHeader->iNumberOfRows;		//数据总行数

		iXScaleINT	= ((double)iXScaleINT * dPixelPerKm * fZoom);		// X方向一个要素值占的格点数
		iYScaleINT	= ((double)iYScaleINT * dPixelPerKm * fZoom);		// Y方向一个要素值占的格点数

		// 栅格数据行数(当前层)
		int iRasterRowNumberInCurrentLayer = (int)pRasterRowList->size();

		// 用于取大于某个值的最小整数
		double delt = 0.9;

		int iCurLine = 0;
		KRasterRowList::iterator it;
		KRasterRowList::iterator ibeg = pRasterRowList->begin();
		KRasterRowList::iterator iend = pRasterRowList->end();
		for(iCurLine=0,it=ibeg;it!=iend,iCurLine<iRasterRowNumberInCurrentLayer;it++,iCurLine++)
		{
			// 取得一条射线
			RasterRow* pRasterRow = (*it);

			// 该行上字节数
			int iNumberOfBytesInThisRow = pRasterRow->iNumberOfBytesInThisRow;

			int iMarginLeft	= (int)(dwWidthImage - (int)(iRasterRowNumberInCurrentLayer * iXScaleINT + delt)) / 2;
			int iMarginTop	= (int)(dwHeightImage - (int)(iRasterRowNumberInCurrentLayer * iYScaleINT + delt)) / 2;

			// 左上角的点
			int xx0 = iMarginLeft;
			int yy0 = iMarginTop;

			// 垂直剖面(50)
			if (iProductCode == 50)
			{
				xx0 = (int)(1.0 * iICoordStart * dPixelPerKm * fZoom);
				yy0 = (int)(1.0 * iJCoordStart * dPixelPerKm * fZoom);
			}

			int iCurRun = 0;	// 当前库号

			// 一行的字节数
			for(i=0;i<iNumberOfBytesInThisRow;i++)
			{
				int iRun = pRasterRow->pRunColor[i].iRun;				// 库号(数)
				int iColorIndex = pRasterRow->pRunColor[i].iColorIndex;	// 库的颜色

				COLORREF clr = pLegend[iColorIndex].dwColor;

				iCurRun += iRun;

				if(iColorIndex<=0 || iColorIndex<iThreshold_min || iColorIndex>iThreshold_max)
				{
					//continue;
				}

				int xx1 = xx0 + (int)(iCurRun * iXScaleINT + delt);		// 当前列
				int yy1 = yy0 + (int)(iCurLine * iYScaleINT + delt);	// 当前行

				for(int nx=0;nx<(int)(iRun*iXScaleINT+delt);nx++)		//X像素个数
				{
					for(int ny=0;ny<(int)(iYScaleINT+delt);ny++)		//Y像素个数
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
	// 冰雹概率数据包 [P3-130]
	//     HDA Hail
	//     Packet Code = 19
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KHdaHailList* pHdaHailList)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(pRpgData->GetProductCode());

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange<230)
		{
			dPixelPerKm = dPixelPerKm * 230 / dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
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
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetBkColor(hDC,clrBack);

		//绘制标注文本
		KHdaHailList::iterator it;
		KHdaHailList::iterator ibeg = pHdaHailList->begin();
		KHdaHailList::iterator iend = pHdaHailList->end();
		for(it=ibeg;it!=iend;it++)
		{
			HdaHail* pHdaHail = (*it);

			// 位置
			int x,y;

			if(iProductCode == 59)
			{
				// 把 km/4 转换为屏幕像素
				x = 256 + (int)(pHdaHail->oPosition.I / 4 * fZoom);
				y = 256 - (int)(pHdaHail->oPosition.J / 4 * fZoom);
			}
			else
			{
				// 本身就是屏幕像素
				x = (int)(pHdaHail->oPosition.I * fZoom);
				y = (int)(pHdaHail->oPosition.J * fZoom);
			}

			x = (int)((x + 12) * dPixelPerKm);
			y = (int)(y * dPixelPerKm);

			// 冰雹概率颜色
			COLORREF clrProbabilityOfHail = RGB(192,192,192);
			// 强冰雹概率颜色
			COLORREF clrProbabilityOfSevereHail = RGB(192,0,0);
			// 最大冰雹尺寸颜色
			COLORREF clrMaxHailSize = RGB(0,192,255);

			sfx_int16	iProbabilityOfHail			= pHdaHail->iProbabilityOfHail;			// 冰雹概率
			sfx_int16	iProbabilityOfSevereHail	= pHdaHail->iProbabilityOfSevereHail;	// 强冰雹概率
			sfx_uint16	iMaxHailSize				= pHdaHail->iMaxHailSize;				// 最大冰雹尺寸(预期)

			char szText[32]						= "";
			char szProbabilityOfHail[16]		= "";	// 冰雹概率
			char szProbabilityOfSevereHail[16]	= "";	// 强冰雹概率
			char szMaxHailSize[16]				= "";	// 最大冰雹尺寸(预期)

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
	// 风暴中心信息路径数据包 [P3-130]
	//     SCIT Past Data
	//     Packet Code = 23,24
	//waiting...
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KScitList* pScitList)
	{
		// 绘制风暴中心信息路径
		KScitList::iterator it;
		KScitList::iterator ibeg = pScitList->begin();
		KScitList::iterator iend = pScitList->end();
		for(it=ibeg;it!=iend;it++)
		{
			Scit* pScit = (*it);

			// 特殊符号
			// Pack Code = 2
			if(!this->DrawImage(hDC,lpRect,fZoom,&pScit->oSpecilaSymbolList))
			{
				return FALSE;
			}
			// 单色连接矢量
			// Pack Code = 6
			if(!this->DrawImage(hDC,lpRect,fZoom,&pScit->oMonoLinkedVectorList))
			{
				return FALSE;
			}
			// 风暴圈
			// Pack Code = 25
			if(!this->DrawImage(hDC,lpRect,fZoom,&pScit->oStiCircleList))
			{
				return FALSE;
			}
		}

		return TRUE;
	}
	// 风暴路径信息圈数据包 [P3-130]
	//     STI Circle
	//     Packet Code = 25
	BOOL KRpgDataOperator::DrawImage(HDC hDC,LPRECT lpRect,float fZoom,KStiCircleList* pStiCircleList)
	{
		//int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 图像尺寸
		DWORD dwWidthImage	= lpRect->right - lpRect->left;		// 默认图像宽度为512像素
		DWORD dwHeightImage	= lpRect->bottom - lpRect->top;		// 默认图像高度为512像素

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 覆盖范围
		double dRange = pRpgData->GetProductRange(pRpgData->GetProductCode());

		// 对于径向数据图像考虑产品覆盖范围
		if(dRange<230)
		{
			dPixelPerKm = dPixelPerKm * 230 / dRange;
		}

		// 矩形中心坐标(设备坐标)
		int iXCenter,iYCenter;
		iXCenter = dwWidthImage / 2;
		iYCenter = dwHeightImage / 2;

		// 原点坐标
		double dXOrigin,dYOrigin;
		dXOrigin = (double)iXCenter;
		dYOrigin = (double)iYCenter;

		// 阈值
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
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkColor(hDC,clrBack);

		// 绘制标注文本
		KStiCircleList::iterator it;
		KStiCircleList::iterator ibeg = pStiCircleList->begin();
		KStiCircleList::iterator iend = pStiCircleList->end();
		for(it=ibeg;it!=iend;it++)
		{
			StiCircle* pStiCircle = (*it);

			// 连线颜色
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

			// 选入画笔
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,clr);
			::SelectObject(hDC,hPen);

			// 选入画刷
			::DeleteObject(hBrush);
			hBrush = ::CreateHatchBrush(HS_BDIAGONAL,clr);
			::SelectObject(hDC,hBrush);

			// 圆心
			POINT pt;
			pt.x = pStiCircle->oPosition.I;
			pt.y = pStiCircle->oPosition.J;

			if(iProductCode == 58)
			{
				//把 km/4 转换为屏幕像素
				pt.x = 256 + (int)(pStiCircle->oPosition.I / 4 * fZoom);
				pt.y = 256 - (int)(pStiCircle->oPosition.J  /4 * fZoom);
			}
			else
			{
				//本身就是屏幕像素
				pt.x = (int)(pStiCircle->oPosition.I * fZoom);
				pt.y = (int)(pStiCircle->oPosition.J * fZoom);
			}

			pt.x = (int)(pt.x * dPixelPerKm);
			pt.y = (int)(pt.y * dPixelPerKm);

			//圆半径(Pixel)
			int iRadiusOfCircle = (int)((double)pStiCircle->iRadiusOfCircle * dPixelPerKm * fZoom);

			int x0 = pt.x;
			int y0 = pt.y;

			int l = x0 - iRadiusOfCircle;
			int t = y0 - iRadiusOfCircle;
			int r = x0 + iRadiusOfCircle;
			int b = y0 + iRadiusOfCircle;

			// 绘制圆
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
	//绘制距离库链表
	//主要用于Radial和Raster数据
	BOOL KRpgDataOperator::DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinList* pRangeBinList)
	{
		// 数据对象
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if (pRpgData->IsEmpty())
			return FALSE;

		KDataInfo* pDataInfo = pRpgData->GetDataInfo();
		if (pDataInfo == NULL)
			return FALSE;

		// 图例
		Legend* pLegend = pRpgData->GetLegend();
		if (pLegend == NULL)
			return FALSE;

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);

		// 阈值
		int iThreshold_min(0), iThreshold_max(0);
		pRpgData->GetThreshold(&iThreshold_min, &iThreshold_max);


		// 局部变量
		int row, col;	//
		double xx, yy;	//
		POINT pt[4];	// 距离库坐标

		int iHeight = pDataInfo->yNum;

		KRangeBinList::iterator itr;
		KRangeBinList::iterator itrBeg = pRangeBinList->begin();
		KRangeBinList::iterator itrEnd = pRangeBinList->end();
		for (itr = itrBeg; itr != itrEnd; itr++)
		{
			//距离库
			RangeBin* pRangeBin = *itr;

			// 库颜色索引(阈值)
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


			// 绘制距离库多边形
			::Polygon(hDC, pt, 4);

		}//for (itr = itrBeg; itr != itrEnd; itr++)

		return TRUE;
	}

	BOOL KRpgDataOperator::DrawImage(HDC hDC, LPRECT lpRect, float fZoom, KRangeBinPerRadialList* pRangeBinPerRadialList)
	{
		// 数据对象
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if (pRpgData->IsEmpty())
			return FALSE;

		KDataInfo* pDataInfo = pRpgData->GetDataInfo();
		if (pDataInfo == NULL)
			return FALSE;

		// 图例
		Legend* pLegend = pRpgData->GetLegend();
		if (pLegend == NULL)
			return FALSE;

		//背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID, 1, 0);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(0);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);

		// 阈值
		int iThreshold_min(0), iThreshold_max(0);
		pRpgData->GetThreshold(&iThreshold_min, &iThreshold_max);


		// 局部变量
		int row, col;	//
		double xx, yy;	//
		POINT pt[4];	// 距离库坐标

		int iHeight = pDataInfo->yNum;

		//=========================================
		int n = 214;
		int nDrw = 0;			//绘制条数

		int nBeg = n;			//开始射束
		int nEnd = n + nDrw;	//结束射束

		int nCur = 0;			//当前条数
		int nDrwed = 0;			//已经绘制条数
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
				//距离库
				RangeBin* pRangeBin = *itr1;

				// 库颜色索引(阈值)
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


				// 绘制距离库多边形
				::Polygon(hDC, pt, 4);

			}//for (itr1 = itrBeg1; itr1 != itrEnd1; itr1++)


		}//for (itr = itrBeg; itr != itrEnd; itr++)

		return TRUE;
	}

	//输出每条射束距离库信息
	BOOL KRpgDataOperator::out(LPCTSTR lpszFile, KRangeBinPerRadialList* pRangeBinPerRadialList)
	{
		// 数据对象
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if (pRpgData->IsEmpty())
			return FALSE;

		KDataInfo* pDataInfo = pRpgData->GetDataInfo();
		if (pDataInfo == NULL)
			return FALSE;


		// 局部变量
		int row, col;	//
		double xx, yy;	//
		POINT pt[4];	//距离库坐标

		int nCur = 0;			//当前条数
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
				//距离库
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

		// 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	/// 分类绘制图像 - End
	///--------------------------------------------------------
#pragma endregion

	///===============
	///生成图像 - end
	///========================================================

	// 绘制极坐标
	void KRpgDataOperator::DrawPolar(HDC hDC, LPRECT pRect,float fZoom)
	{
		int i;

		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		//图像区中心坐标
		double dXCenter = pRect->right / 2;
		double dYCenter = pRect->bottom / 2;

		// GDI
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(255,161,123));
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = ::CreateSolidBrush(clrBack);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,::GetStockObject(NULL_BRUSH));

		double dPixel;
		double x1,y1,x2,y2;

		// 绘制5条距离圈
		int iNum = 5;
		for(i=0;i<iNum;i++)
		{
			// 50个像素表示50公里
			dPixel = 50.0 * (i + 1) * dPixelPerKm * fZoom;

			// 矩形的左上和右下角坐标
			x1 = dXCenter - dPixel;
			y1 = dYCenter - dPixel;

			x2 = dXCenter + dPixel;
			y2 = dYCenter + dPixel;

			// 最后一圈绘制30公里
			if(i==iNum-1)
			{
				int iTemp = 20;	// 20公里 50-20 = 30km (因为230km)

				x1 += iTemp * dPixelPerKm * fZoom;
				y1 += iTemp * dPixelPerKm * fZoom;

				x2 -= iTemp * dPixelPerKm * fZoom;
				y2 -= iTemp * dPixelPerKm * fZoom;
			}

			// 绘制椭圆
			::Ellipse(hDC,(int)x1,(int)y1,(int)x2,(int)y2);

			// 当 i=iNum-1 时绘制极坐标径向线
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

		// 绘制中间的小圆圈
		::SelectObject(hDC,hBrush);
		dPixel = 5.0*dPixelPerKm*fZoom;

		x1 = dXCenter-dPixel;
		y1 = dYCenter-dPixel;

		x2 = dXCenter+dPixel;
		y2 = dYCenter+dPixel;

		// 绘制椭圆
		::Ellipse(hDC,(int)x1,(int)y1,(int)x2,(int)y2);

		// Cleanup GDI
		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

		::SelectObject(hDC,hOldBrush);
		::DeleteObject(hBrush);
	}

	// 绘制笛卡尔坐标(剖面图)
	void KRpgDataOperator::DrawDescartes(HDC hDC, LPRECT pRect,float fZoom,KUnlinkedVectorList* pUnlinkedVectorList,KWriteTextList* pWriteTextList)
	{
		// 1 绘制直角坐标
		this->DrawImage(hDC,pRect,fZoom,pUnlinkedVectorList);

		 // 2 绘制标注文本
		this->DrawImage(hDC,pRect,fZoom,pWriteTextList);
	}

	// 绘制产品信息
	void KRpgDataOperator::DrawInfoArea(HDC hDC, LPRECT pRect)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 1. 准备信息区
		// 填充背景色
		HBRUSH hBrush = ::CreateSolidBrush(clrBack);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,::GetStockObject(NULL_BRUSH));
		::FillRect(hDC,pRect,hBrush);

		// 绘制信息区方框
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(128,128,128));
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);
		::Rectangle(hDC,pRect->left+2,pRect->top+1,pRect->right-1,pRect->bottom-1);

		// 2. 绘制产品信息文本
		RECT rectText = *pRect;
		rectText.left += 6;
		rectText.top += 4;
		rectText.right -= 1;
		rectText.bottom -= 1;

		DrawInfoText(hDC,&rectText);	// 绘制产品信息文本

		// 3. 绘制产品信息色标
		int iLineHeight = (int)(14.0*dPixelPerKm);

		RECT rectHue = *pRect;
		//rectHue.top = rectHue.top + iLineHeight * 25 + 3;
		rectHue.top = rectHue.top + iLineHeight * 25;

		DrawInfoHue(hDC,&rectHue);		// 绘制产品信息色标

		// Cleanup GDI
		::DeleteObject(hBrush);

		::SelectObject(hDC,hOldPen);
		::DeleteObject(hPen);

	}

	// 绘制产品信息文本
	void KRpgDataOperator::DrawInfoText(HDC hDC, LPRECT pRect)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 雷达产品信息
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		// 产品代码
		int iProductCode = pRadarInfo->iProductCode;

		// GDI Init
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(12.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetBkColor(hDC,clrBack);

		RECT rectText = *pRect;
		int xTemp,yTemp;
		TCHAR szTemp[MAX_PATH] = _T("");
		double dTemp[3];

		// 行距
		int iLineHeight = (int)(14.0 * dPixelPerKm);
		int iLine=0;
		//----------------------------------------------------
		// 绘制信息 - Beg

		// 1.雷达产品基本信息
		// 1.1 产品缩写
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("产品: %s"),pRadarInfo->szContraction);

		::SetTextColor(hDC,RGB(254,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.2 产品名称
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("名称: %s"),pRadarInfo->szCnName);

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.3 覆盖范围(km)
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;

		if(pRadarInfo->dRange == 0)
		{
			_stprintf(szTemp,_T("范围: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("范围: %.0f km"),pRadarInfo->dRange);
		}

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.4 分辨率(km)
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;

		if(fabs(pRadarInfo->dResolution)<1.0e-12)
		{
			_stprintf(szTemp,_T("精度: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("精度: %.2f km"),pRadarInfo->dResolution);
		}

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.5 日期
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("日期: %4d-%02d-%02d"),pRadarInfo->wYear,pRadarInfo->wMonth,pRadarInfo->wDay);

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 1.6 时间
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("时间: %02d:%02d:%02d"),pRadarInfo->wHour,pRadarInfo->wMinute,pRadarInfo->wSecond);

		::SetTextColor(hDC,RGB(192,192,192));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		iLine++;	// 空一行

		// 2 雷达信息
		// 2.1 雷达站点
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("雷达: %s(%d)"),pRadarInfo->szStationName,pRadarInfo->id);

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.2 海拔
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("海拔: %.2f m"),pRadarInfo->dAlt);

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.3 纬度
		dTemp[0] = pRadarInfo->dLat;				// 整度
		dTemp[1] = (dTemp[0]-(int)dTemp[0]) * 60.0;	// 分
		dTemp[2] = (dTemp[1]-(int)dTemp[1]) * 60.0;	// 秒

		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("纬度: %d/%d/%d %s"),(int)dTemp[0],(int)dTemp[1],(int)dTemp[2],dTemp[0]>0 ? "N" : "S");

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		//2.4 经度
		dTemp[0] = pRadarInfo->dLon;				// 整度
		dTemp[1] = (dTemp[0]-(int)dTemp[0]) * 60.0;	// 分
		dTemp[2] = (dTemp[1]-(int)dTemp[1]) * 60.0;	// 秒

		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("经度: %d/%d/%d %s"),(int)dTemp[0],(int)dTemp[1],(int)dTemp[2],dTemp[0]>0 ? "E" : "W");

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.5 雷达中心
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		if(iProductCode==50)
		{
			_stprintf(szTemp,_T("中心: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("中心: 0deg 0km"));
		}

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.6 极坐标
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		if(iProductCode == 48 || iProductCode == 50 || iProductCode == 53)
		{
			_stprintf(szTemp,_T("坐标: N/A"));
		}
		else
		{
			_stprintf(szTemp,_T("坐标: %.0fkm 30deg"),pRadarInfo->dPolarRange);
		}

		::SetTextColor(hDC,RGB(0,192,128));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		iLine++;	// 空一行

		// 2.7 操作模式
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("操作模式: %s"),pRadarInfo->szCnOPModeName);

		::SetTextColor(hDC,RGB(200,96,250));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		// 2.8 体扫模式
		xTemp = rectText.left;
		yTemp = rectText.top + iLineHeight * iLine++;
		_stprintf(szTemp,_T("体扫模式: VCP %d"),pRadarInfo->iVCP);

		::SetTextColor(hDC,RGB(200,96,250));
		::SetTextAlign(hDC,TA_TOP | TA_LEFT);
		::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

		iLine++;	//空一行

		// 3 雷达产品特殊信息
		for(int p=0;p<7;p++)
		{
			xTemp = rectText.left;
			yTemp = rectText.top + iLineHeight * iLine++;
			_stprintf(szTemp,_T("%s"),pRadarInfo->szSpecialInfo[p]);

			::SetTextColor(hDC,RGB(0,192,192));
			::SetTextAlign(hDC,TA_TOP | TA_LEFT);
			::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));
		}

		// 绘制信息 - End
		//----------------------------------------------------

		// GDI Cleanup
		::SelectObject(hDC,hOldFont);
		::DeleteObject(hFont);
	}

	// 绘制产品信息色标
	void KRpgDataOperator::DrawInfoHue(HDC hDC, LPRECT pRect)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 背景颜色
		COLORREF clrBack = pRpgData->GetImageBkColor();

		// 每公里像素数
		double dPixelPerKm = pRpgData->GetPixelPerKm();

		// 雷达产品信息
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		// 产品代码
		int iProductCode = pRadarInfo->iProductCode;

		// 图例指针
		Legend* pLegend = pRpgData->GetLegend();

		// 信息色标区矩形
		RECT rectHue = *pRect;

		// GDI Init
		HPEN hPen = (HPEN)::GetStockObject(NULL_PEN);
		HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

		HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC,hBrush);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));							// zero out structure
		lf.lfHeight = (LONG)(10.0 * dPixelPerKm);					// request a 12-pixel-height font
		_tcscpy(lf.lfFaceName,_T("宋体"));							// request a face name "Arial"
		HFONT hFont = ::CreateFontIndirect(&lf);
		HFONT hOldFont = (HFONT)::SelectObject(hDC,hFont);

		::SetBkMode(hDC,TRANSPARENT);
		::SetBkColor(hDC,clrBack);

		RECT rectText = *pRect;
		int xTemp,yTemp;
		TCHAR szTemp[MAX_PATH] = _T("");

		int iHCLR = (int)(9.0*dPixelPerKm);	// 色标高度
		int iWCLR = (int)(32.0*dPixelPerKm);	// 色标宽度

		// 色标颜色块矩形
		RECT rectCLR = rectHue;
		rectCLR.left += 6;
		rectCLR.top += 1;
		rectCLR.right -= 1;
		rectCLR.bottom -= 1;

		rectCLR.top += 9;
		rectCLR.right = rectCLR.left + iWCLR;

		// 数据级别(色标数量)
		int iDataLevel = pRpgData->GetProductDataLevel(pRpgData->GetProductCode());
		iDataLevel = iDataLevel>16 ? 16 : iDataLevel;

		for(int i=0;i<iDataLevel;i++)
		{
			// 色标颜色
			COLORREF clrCLR = pRpgData->GetLegend(i)->dwColor;
			// 色标对应值
			double dValue = (*(pLegend+i)).dValue;

			// 调整色标颜色块矩形底
			rectCLR.bottom = rectCLR.top + iHCLR;

			::DeleteObject(hBrush);
			hBrush = ::CreateSolidBrush(clrCLR);
			::SelectObject(hDC,hBrush);

			// 绘制色标颜色块
			::FillRect(hDC,&rectCLR,hBrush);	// 色标

			// 色标值画笔
			::DeleteObject(hPen);
			hPen = ::CreatePen(PS_SOLID,1,RGB(128,128,128));
			::SelectObject(hDC,hPen);

			// 绘制短线
			::MoveToEx(hDC,rectCLR.right,rectCLR.top,NULL);
			::LineTo(hDC,rectCLR.right+(int)(8.0*dPixelPerKm),rectCLR.top);

			// 绘制色标值
			xTemp = rectCLR.right+(int)(8.0*dPixelPerKm)+(int)((8.0*5.0)*dPixelPerKm);
			yTemp = rectCLR.top+(int)(4.0*dPixelPerKm);		//

			if( !(fabs(dValue-RPG_DATA_INVALID_VALUE)<1.0e-12) )
				_stprintf(szTemp,_T("%s"),(*(pLegend+i)).szText);

			::SetTextColor(hDC,RGB(0,192,192));
			::SetTextAlign(hDC,TA_BASELINE | TA_RIGHT);
			::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));

			// 在第二个色标颜色块后标注数值单位
			if(i==2)
			{
				_stprintf(szTemp,_T("%s"),pRpgData->GetProductUnit(iProductCode));

				xTemp += (int)(8.0*dPixelPerKm);

				::SetTextColor(hDC,RGB(0,192,192));
				::SetTextAlign(hDC,TA_BASELINE | TA_LEFT);
				::TextOut(hDC,xTemp,yTemp,szTemp,_tcslen(szTemp));	// 色标标示
			}

			// 调整色标颜色块矩形顶
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

	// 生成格点数据
	// 1.必须先装载数据 - LoadData(...)
	// 2.必须是RadialImage或者RasterImage类型
	// Threshold = [0,15] 有16级
	//
	//参数：
	//	pData			- 保存格点数据的指针(本函数分配内存,调用者负责内存回收)
	//	dataInfo		- 保存数据信息
	//	iThreshold_min	- 阈值下限(0-15)
	//	iThreshold_max	- 阈值上限(0-15)
	//	skip			- 采样间隔(0,1,2)
	//用法：
	//	double* p = NULL;
	//	KDataInfo di;
	//	ToGrid(p,di,0,15,0);
	BOOL KRpgDataOperator::ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min, int iThreshold_max, int skip)
	{
		// 必须传入空指针:
		//		double* p = NULL;
		//		KDataInfo di;
		//		ToGrid(p,di,...);
		// 内存本程序分配，但是
		// 必须由调用者回收
		//Assert(pData==NULL);
		if (pData)
		{
			delete[]pData;
			pData = NULL;
		}

		// 数据对象
		KRpgData* pRpgData = (KRpgData*)GetOwner();

		// 是否为空
		if(pRpgData->IsEmpty())
			return FALSE;

		// 设置阈值
		pRpgData->SetThreshold(iThreshold_min, iThreshold_max);

		// 判断产品类型(只有某些数据格点化才有意义)
		// 必须是RadialImage或者RasterImage类型

		// 产品代码
		int iProductCode = pRpgData->GetProductCode();

		// 取得产品类型
		ProductType eProductType = pRpgData->GetProductType(iProductCode);

		// 必须是RadialImage或者RasterImage类型
		if (eProductType != ProductType_RadialImage && eProductType != ProductType_RasterImage)
			return FALSE;

		COLORREF clrBack = 0;

		// 构造不含信息区的Image
		pRpgData->SetImageBkColor(clrBack);		// 背景颜色
		pRpgData->SetImageWidth(512);		// 512 X 512 像素
		pRpgData->SetImageHeight(512);		// 512 X 512 像素
		if(!BuildImage(1,FALSE,FALSE))		// 生成图像
			return FALSE;

		// skip 是间隔点数 0 是没有间隔
		if (skip < 0)
			skip = 0;

		skip += 1;

		// 雷达产品信息
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		// 中心经纬度
		double dCenterX	= pRadarInfo->dLon;
		double dCenterY	= pRadarInfo->dLat;

		// 覆盖范围(km)
		double dRange = pRadarInfo->dRange;

		// 1.1公里=0.008979经纬度 1.1/111.37
		double dDegreePerKm	= 0.0098769866211726676842955912723355;

		// 每个象素代表的经纬度
		double dDegreePerPixel	= dDegreePerKm * dRange / 230;

		// GDI
		// 为生成图像准备DC
		HDC hDC = ::GetDC(NULL);	// 取得桌面DC句柄(注意释放)
		// 创建兼容 DC(注意释放)
		HDC hImageDC = ::CreateCompatibleDC(hDC);
		// 选入位图句柄
		::SelectObject(hImageDC,pRpgData->GetImageHandle());

		// 
		int i,j;
		COLORREF clr(0);
		double dValue(0);
		pData = new double[512*512];

		// 要素阈值
		double dThresholdMin = pRpgData->GetDataLevelThreshold(iThreshold_min)->dThreshold;
		double dThresholdMax = pRpgData->GetDataLevelThreshold(iThreshold_max)->dThreshold;

		for (i = 0; i < 512; i += skip)
		{
			for (j = 0; j < 512; j += skip)
			{
				clr = ::GetPixel(hImageDC, j, 511 - i);	//从图象左下角开始

				if (clr != clrBack && clr != CLR_INVALID)
				{
					dValue = Color2Value(clr);			//颜色值转换到物理量值
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

		// 填充数据信息 KDataInfo
 		//memset(&dataInfo,0,sizeof(KDataInfo));

		//求出左下角经纬度
		double x0 = dCenterX - dDegreePerPixel * 512 / 2;
		double y0 = dCenterY - dDegreePerPixel * 512 / 2;

		//经纬度间隔
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

		// 处理等值线数据

		return TRUE;
	}

	// 生成格点数据
	// 保存到文件 lpszFile 中
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

		/// 写入文件
		///
		// 计算等值线间隔、最小、最大值
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

		// 打开文件
		FILE* fp = _tfopen(lpszFile,_T("wt"));
		if(fp==NULL)
		{
			delete[] pData;
			pData = NULL;
			return FALSE;
		}

		// 文件头
		_ftprintf(fp,_T("SNYC 204 %s \n"),(LPCTSTR)pDataInfo->Comment());
		_ftprintf(fp,_T("0 0 0 0 0 0 0 0 0 0 \n"));

		// 2.写入日期、时效、层次
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
				//10个数据一行
				if(k%10 == 9)
				{
					_ftprintf(fp,_T("\n"));
				}
				k++;
			}
			//每组纬向数据输出完成后换行
			_ftprintf(fp,_T("\n"));
		}

		// 关闭文件
		fclose(fp);
		fp = NULL;

		delete[] pData;
		pData = NULL;

		return TRUE;
	}

	// 根据数值转换为灰度图
	HBITMAP KRpgDataOperator::ToGrayImage()
	{
		HBITMAP hBitmap = NULL;
		//-------------------------------------------------
		KDataInfo di;
		double* pData = NULL;	// 注意释放内存
		if (!ToGrid(pData, di, 0, 15, 0))
		{
			_delete(pData);
			return NULL;
		}

		// 数据信息
		KDataInfo* pDataInfo = &di;

		//数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 24位 DIB 每行的字节
		DWORD dwBytesPerLine = DIBWIDTHBYTES(iWidth * 8 * 3);

		// 数据尺寸(24位)
		DWORD dwSizeImage = dwBytesPerLine * iHeight;

		// 整个DIB 大小(无调色板数据)
		DWORD dwDIBSize = sizeof(BITMAPINFOHEADER) + 0 + dwSizeImage;

		// 一次分配 DIB 内存
		PBYTE pDib = new BYTE[dwDIBSize];
		memset(pDib, 0, sizeof(BYTE) * dwDIBSize);

		// 设置 BITMAPINFOHRADER
		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)pDib;
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth = iWidth;
		pBitmapInfo->bmiHeader.biHeight = iHeight;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biBitCount = 24;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage = dwSizeImage;	//用BI_RGB方式时设可为0
		pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
		pBitmapInfo->bmiHeader.biClrUsed = 0;
		pBitmapInfo->bmiHeader.biClrImportant = 0;

		// DIB 数据指针
		PBYTE pDIBits = pDib + sizeof(BITMAPINFOHEADER) + 0;

		int r, g, b;
		for (int i = 0; i < iHeight; i++)
		{
			for (int j = 0; j < iWidth; j++)
			{
				double dv = *(pData + i*iWidth + j);

				int cv = (int)(dv / 70 * 255);	//TODO:临时转换
				if (cv <= 0)
				{
					cv = 0;	//灰度图背景必须为黑色？
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

	// 拼图
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
				// 第一个文件处理到 pData,
				// 防止只有一个文件
				_deletea(pData);
				pRpgData->ToGrid(pData,*pDataInfo);
			}
			else if(pRpgData->LoadData((*pFileArray)[i]))
			{
				// 传递 pData 到 pData1
				_deletea(pData1);
				pData1 = pData;
				dataInfo1 = *pDataInfo;

				// 第一个文件处理到 pData2
				_deletea(pData2);
				pRpgData->ToGrid(pData2,dataInfo2);

				// 合并
				pRpgData->Union(pData,pDataInfo,pData1,&dataInfo1,pData2,&dataInfo2);

				// 合并以后 pData1,pData2 不再有用
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
				// 第一个文件处理到 pData,
				// 防止只有一个文件
				_deletea(pData);
				pRpgData->ToGrid(pData,*pDataInfo);
			}
			else if(pRpgData->LoadData(pszFile[i]))
			{
				// 传递 pData 到 pData1
				_deletea(pData1);
				pData1 = pData;
				dataInfo1 = *pDataInfo;

				// 第一个文件处理到 pData2
				_deletea(pData2);
				pRpgData->ToGrid(pData2,dataInfo2);

				// 合并
				pRpgData->Union(pData,pDataInfo,pData1,&dataInfo1,pData2,&dataInfo2);

				// 合并以后 pData1,pData2 不再有用
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

		// 先把 pDataInfo1 传递给 pDataInfo
		pDataInfo->xMin = pDataInfo1->xMin;
		pDataInfo->xMax = pDataInfo1->xMax;

		pDataInfo->yMin = pDataInfo1->yMin;
		pDataInfo->yMax = pDataInfo1->yMax;

		// X,Y 方向坐标间隔
		pDataInfo->xInterval = pDataInfo1->xInterval;
		pDataInfo->yInterval = pDataInfo1->yInterval;

		// X,Y 方向格点数
		pDataInfo->xNum = pDataInfo1->xNum;
		pDataInfo->yNum = pDataInfo1->yNum;

		// 第二个数据可以为 NULL
		//if(!pData2)
		//	return FALSE;

		// 如果有第二个数据,合并数据信息
		// 取得合并后的坐标范围
		if(pData2)
		{
			pDataInfo->xMin = min(pDataInfo1->xMin,pDataInfo2->xMin);
			pDataInfo->xMax = max(pDataInfo1->xMax,pDataInfo2->xMax);

			pDataInfo->yMin = min(pDataInfo1->yMin,pDataInfo2->yMin);
			pDataInfo->yMax = max(pDataInfo1->yMax,pDataInfo2->yMax);

			// X,Y 方向坐标间隔
			//pDataInfo->xInterval = max(pDataInfo1->xInterval,pDataInfo2->xInterval);
			//pDataInfo->yInterval = max(pDataInfo1->yInterval,pDataInfo2->yInterval);
			pDataInfo->xInterval = 0.01;
			pDataInfo->yInterval = 0.01;

			// X,Y 方向格点数
			pDataInfo->xNum = (int)(fabs(pDataInfo->xMax - pDataInfo->xMin) / pDataInfo->xInterval) + 1;
			pDataInfo->yNum = (int)(fabs(pDataInfo->yMax - pDataInfo->yMin) / pDataInfo->yInterval) + 1;
		}

		// 分配内存
		double* pDataTemp = new double[pDataInfo->xNum * pDataInfo->yNum];
		if(!pDataTemp)
			return FALSE;

		// 置初值为无效数据
		for(i=0;i<pDataInfo->yNum;i++)
		{
			for(j=0;j<pDataInfo->xNum;j++)
			{
				*(pDataTemp + i * pDataInfo->xNum + j) = -9999;
			}
		}

		// 复制第一个数据
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

		// 合并第二个数据
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
						// 取大值
						*(pDataTemp + row * pDataInfo->xNum + col) = max(v,v2);
					}
				}
			}
		}

		// 无效数据为0
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
	// 名  称: Radial2RangeBin
	// 功  能: 把径向射线数据转换成(RangeBin)以经纬度形式表示的库,
	//         保存在 pRangeBinList 链表中,供程序使用
	// 输  入:
	//		[o] pRangeBinList - 距离库链表
	//
	// 输  出: 距离库链表 KRangeBinList
	// 返  回: TRUE/FALSE
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

		// 每层一个径向数据图像
		// 一个径向数据图像的属性数据(每条射线还有自己的属性数据)
		int iFirstBin					= pRadialDataPacketHeader->iIndexOfFirstRangeBin;		//Location of first rang bin
		//int iRangeBinNumberPerRadial = pRadialDataPacketHeader->iRangeBinNumberPerRadial;	//组成一条径向数据的库数
		//int iICenterOfSweep				= pRadialDataPacketHeader->iICenterOfSweep;			//km/4
		//int iJCenterOfSweep				= pRadialDataPacketHeader->iJCenterOfSweep;			//km/4
		int iScaleFactor				= pRadialDataPacketHeader->iScaleFactor;				//每个库包含的像素数(0.001)
		//int iRadialNumber				= pRadialDataPacketHeader->iRadialNumber;				//射线条数

		// 每个像素表示的公里数(230km是一个像素表示一公里)
		double dKmPerPixel = (double)pRpgData->GetProductRange(iProductCode) / 230;
		dKmPerPixel = dKmPerPixel < 1.0 ? 1.0 :dKmPerPixel;

		// 每个库包含的像素数 (0.998个像素)
		double dPixelPerBin = 0.001 * iScaleFactor * dKmPerPixel;

		// 1.1公里=0.008979经纬度 1/111.37
		double dDegreePerKm		= 0.0098769866211726676842955912723355;	//1公里=0.008979经纬度 1/111.37

		// 每个像素表示的经纬度
		double dDegreePerdPixel = dDegreePerKm;

		// 取得图像的中心位置
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

			// 本条射线的开始角度(逆时针)
			double alf0 = pRadial->oEachRadialHeader.iRadialStartAngle * 0.1 - 90;	//转换到上面是北

			// 本条射线每个库在开始角度上X和Y方向的增量
			double dw0 = dPixelPerBin * dDegreePerdPixel * cos(alf0 * 3.1415926 / 180);
			double dh0 = dPixelPerBin * dDegreePerdPixel * sin(alf0 * 3.1415926 / 180);

			// 本条射线每个库在开始角度上的起始坐标
			double x0Beg = dXOrigin + dw0 * iFirstBin;
			double y0Beg = dYOrigin - dh0 * iFirstBin;

			// 本条射线的结束角度
			double alf1 = alf0 + pRadial->oEachRadialHeader.iDeltaAngle * 0.1;		//加上展角
			//if(alf1>360)	alf1 = alf1-360.0;
			//if(alf1<0)		alf1 = 360.0-alf1;
		
			// 本条射线每个库在结束角度上X和Y方向的增量
			double dw1 = dPixelPerBin * dDegreePerdPixel * cos(alf1 * 3.1415926 / 180);
			double dh1 = dPixelPerBin * dDegreePerdPixel * sin(alf1 * 3.1415926 / 180);

			// 本条射线每个库在结束角度上的起始坐标
			double x1Beg = dXOrigin + dw1 * iFirstBin;
			double y1Beg = dYOrigin - dh1 * iFirstBin;

			double x2,y2;
			double x3,y3;

			// 每条射线上当前的库号
			int iCurRun=0;

			//组成本条径向数据的库数
			int iRangeBinNumberPerRadial = pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;

			RangeBinPerRadial* pRangeBinPerRadial = new RangeBinPerRadial();
			pRangeBinPerRadial->pRangeBinList = new KRangeBinList();

			for(int k=0;k<pRadial->oEachRadialHeader.iNumbersOfRLECodeInRadial * 2;k++)	//每条射线的库数量
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
					//调整pt[0]和pt[2]
					//pt[0]和pt[3]在起始角度上,pt[1]和pt[2]在结束角度上
					pRangeBin->x[0] = (float)x3;
					pRangeBin->y[0] = (float)y3;
					pRangeBin->x[1] = (float)x2;
					pRangeBin->y[1] = (float)y2;
				}

				int iRun = pRadial->pRunColor[k].iRun;			//库数 
				int iColor = pRadial->pRunColor[k].iColorIndex;	//库的颜色

				iCurRun += iRun;	//当前库号

				pRangeBin->x[3] = (float)(x0Beg + dw0 * iCurRun);	//pt[0]和pt[3]在起始角度上
				pRangeBin->y[3] = (float)(y0Beg - dh0 * iCurRun);	//

				pRangeBin->x[2] = (float)(x1Beg + dw1 * iCurRun);	//pt[1]和pt[2]在结束角度上
				pRangeBin->y[2] = (float)(y1Beg - dh1 * iCurRun);	//

				pRangeBin->iColorIndex	= iColor;						//当前库的颜色(0表示背景色)
				pRangeBin->clrBin		= pLegend[iColor].dwColor;		//当前库的颜色(0表示背景色)
				pRangeBin->fValue		= (float)pLegend[iColor].dValue;//当前库表示的要素值(如果颜色为0表示没有要素)

				//---------------------
				x3 = pRangeBin->x[3];
				y3 = pRangeBin->y[3];
				x2 = pRangeBin->x[2];
				y2 = pRangeBin->y[2];
				//---------------------

				if (pRangeBin->iColorIndex > 0)
				{
					pRangeBinList->push_back(pRangeBin);	//添加一个库到链表

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
	// 名  称: Raster2RangeBin
	// 功  能: 把把栅格数据转换成(RangeBin)以经纬度形式表示的库,
	//         保存在 pRangeBinList 链表中,供程序使用
	// 调用者:  
	// 输  入:
	//		[o] pRangeBinList - 距离库链表
	//
	// 输  出: 距离库链表 KRangeBinList
	// 返  回: TRUE/FALSE
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

		int iICoordStart	= pRasterDataPacketHeader->iICoordinateStart;	//数据开始的行位置
		int iJCoordStart	= pRasterDataPacketHeader->iJCoordinateStart;	//数据开始的列位置
		int iXScaleINT		= pRasterDataPacketHeader->iXScaleINT;			//X方向一个要素值占的格点数
		int iYScaleINT		= pRasterDataPacketHeader->iYScaleINT;			//Y方向一个要素值占的格点数
		int iNumberOfRows	= pRasterDataPacketHeader->iNumberOfRows;		//数据总行数

		// 产品覆盖范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 每个像素表示的公里数(230km的产品1个像素表示1公里)
		double dKmPerPixel = dRange / 230;

		// 1.1公里=0.008979经纬度 1.1/111.37
		double dDegreePerKm		= 0.0098769866211726676842955912723355;

		// 每个像素表示的经纬度
		double dDegreePerPixel = dDegreePerKm;

		// 取得图像的中心位置
		double dLon0,dLat0;
		pRpgData->GetImageCenter(&dLon0,&dLat0);

		// 左上角经纬度
		double dLonLft = dLon0 - dDegreePerKm * dRange;
		double dLatTop = dLat0 + dDegreePerKm * dRange;

		//取得颜色值
		Legend* pLegend = pRpgData->GetLegend();

		pRangeBinList->Clear();

		float xx0,yy0,xx1,yy1,xx2,yy2,xx3,yy3;

		int j = 0;

		KRasterRowList::iterator itr;
		KRasterRowList::iterator itrBeg = pRasterRowList->begin();
		KRasterRowList::iterator itrEnd = pRasterRowList->end();
		for(itr=itrBeg; itr!=itrEnd; itr++)
		{
			// 栅格行
			RasterRow* pRasterRow = *itr;
			// 该行上字节数
			int iNumberOfBytesInThisRow = pRasterRow->iNumberOfBytesInThisRow;

			int iCurRun=0;	//当前库号

			for(int k=0;k<iNumberOfBytesInThisRow;k++)	//一行的字节数
			{
				RangeBin* pRangeBin = new RangeBin();

				int iRun = pRasterRow->pRunColor[k].iRun;			//库号(数)
				int iColor = pRasterRow->pRunColor[k].iColorIndex;	//库的颜色

				pRangeBin->iColorIndex	= iColor;
				pRangeBin->clrBin 		= pLegend[iColor].dwColor;			//当前库的颜色(0表示背景色)
				pRangeBin->fValue		= (float)pLegend[iColor].dValue;	//当前库表示的要素值(如果颜色为0表示没有要素)

				iCurRun += (int)(iRun * iXScaleINT * dKmPerPixel);

				///////////////////////////////////////////
				//	(xx0,yy0)	---- (xx1,yy1)
				//	           |    |
				//             |    |
				//	(xx3,yy3)	---- (xx2,yy2)
				//////////////////////////////////////////

				// 当前经度
				xx1 = (float)(dLonLft + dDegreePerPixel * iCurRun);
				// 当前纬度
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
					pRangeBinList->push_back(pRangeBin);	//添加一个库到链表
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

	//写文本字符到磁盘文件
	BOOL KRpgDataOperator::WriteTextData(LPCTSTR lpszFile,KWriteTextList* pWriteTextList)
	{
		KRpgData* pRpgData = (KRpgData*)GetOwner();
		if(pRpgData == NULL)
			return FALSE;

		double x,y;
		pRpgData->GetImageCenter(&x,&y);

		if(pWriteTextList->size()<=0)
			return FALSE;

		// 设置 UNICODE 显示汉字
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
	// 生成调色板
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
