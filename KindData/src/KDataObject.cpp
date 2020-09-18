#include "StdAfx.h"
#include "KDataObject.h"

KDataObject::KDataObject(void)
	:KProgressObject()
	,m_px(NULL)
	,m_py(NULL)
	,m_pz(NULL)
	,m_pu(NULL)
	,m_pv(NULL)
	,m_pw(NULL)
	,m_pszUint(NULL)
	,m_pszMessage(NULL)
	,m_pPalette(NULL)
	,m_pDataInfo(NULL)
	,m_strFileName(_T(""))
	,m_pDataProcessor(NULL)
{
	m_pDataInfo = new KDataInfo();
}

KDataObject::KDataObject(LPCTSTR lpszFile)
	: KProgressObject()
	,m_px(NULL)
	,m_py(NULL)
	,m_pz(NULL)
	,m_pu(NULL)
	,m_pv(NULL)
	,m_pw(NULL)
	,m_pszUint(NULL)
	,m_pszMessage(NULL)
	,m_pPalette(NULL)
	,m_pDataInfo(NULL)
	,m_strFileName(_T(""))
	,m_pDataProcessor(NULL)
{
	m_pDataInfo = new KDataInfo();
}

KDataObject::~KDataObject(void)
{
	Empty();

	_delete(m_pDataInfo);
	_delete(m_pDataProcessor);
}

// ������ݲ������ڴ�
void KDataObject::Empty()
{
	// ��Ҫ�ͷ� m_pDataInfo,m_pDataAlgorithm,m_pDataProcessor
	_deletea(m_px);
	_deletea(m_py);
	_deletea(m_pz);
	_deletea(m_pu);
	_deletea(m_pv);
	_deletea(m_pw);

	_deletea(m_pszUint);
	_deletea(m_pszMessage);

	_delete(m_pPalette);
	// ��Ҫ�ͷ� m_pDataInfo,m_pDataAlgorithm,m_pDataProcessor
}

//���������Ƿ����(������Ҫ���������и�����Ҫȡ������)
BOOL KDataObject::IsEmpty()
{
	return TRUE
			&& m_px == NULL
			&& m_py == NULL
			&& m_pz == NULL
			&& m_pu == NULL
			&& m_pv == NULL
			&& m_pw == NULL
			&& m_pszUint == NULL
			&& m_pszMessage == NULL
			&& m_pPalette == NULL
			;
}

// װ������
BOOL KDataObject::LoadData(LPCTSTR lpszFile)
{
	if(!KFile::Exists(lpszFile))
		return FALSE;

	this->SetFileName(lpszFile);

	if(KFile::IsBinary(lpszFile))
		return LoadData_bin(lpszFile);
	else
		return LoadData_txt(lpszFile);
}

BOOL KDataObject::LoadData_bin(LPCTSTR lpszFile)
{
	m_pDataInfo->DataStoreType = 2;
	return TRUE;
}

BOOL KDataObject::LoadData_txt(LPCTSTR lpszFile)
{
	m_pDataInfo->DataStoreType = 1;
	return TRUE;
}

// ��������
BOOL KDataObject::SaveData(LPCTSTR lpszFile)
{
	return FALSE;
}

// ����ͼ��
BOOL KDataObject::BuildImage(COLORREF clrBack,LPCTSTR lpszFile)
{
	if(!m_pDataProcessor)
		return FALSE;

	m_pDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
	return m_pDataProcessor->BuildImage(clrBack,lpszFile);
}

Image* KDataObject::GetImage()
{
	KDataProcessor* pDataProcessor = GetDataProcessor();
	if(!pDataProcessor)
		return NULL;

	return pDataProcessor->GetImage();
}

// Ӧ�õ�ɫ��
BOOL KDataObject::UsePalette(KPalette* pPalette)
{
	SetPalette(pPalette);
	return BuildImage(RGB(0,0,0),NULL);
}

// ��Ч����
BOOL KDataObject::IsInvalidData(double value)
{
	KDataInfo* pDataInfo = this->GetDataInfo();

	return !pDataInfo
		|| fabs(value-pDataInfo->InvalidData())<EPS	// ָ������Чֵ
		|| fabs(value - SFX_INVALID_DATA)<EPS		// 0xCCCCCCCC
		|| fabs(value - AXIN_INVALID_DATA)<EPS		// -9999
		;
}
// �ض�ֵ
BOOL KDataObject::IsSpecialData(double value)
{
	KDataInfo* pDataInfo = this->GetDataInfo();

	DWORD dwValue = (DWORD)(value+0.0001);
	DWORD dwSdmin = pDataInfo->SpecificDataMin();
	DWORD dwSdmax = pDataInfo->SpecificDataMax();

	return !pDataInfo
		// ָ�����ض�ֵ
		|| (dwValue>=dwSdmin) && (dwValue<=dwSdmax)
		|| fabs(value - SFX_INVALID_DATA)<EPS		// 0xCCCCCCCC
		|| fabs(value - AXIN_INVALID_DATA)<EPS		// -9999
		;
}

// Ϊ���ݷ����ڴ�
void KDataObject::PrepareMemory(UVW eUvw)
{
	int i,j;
	KDataInfo* pDataInfo = this->GetDataInfo();
	if(!pDataInfo)
		return;
	//-----------------------------
	if(m_px) {delete []m_px; m_px=NULL;}
	if(m_py) {delete []m_py; m_py=NULL;}
	if(m_pz) {delete []m_pz; m_pz=NULL;}
	if(m_pu) {delete []m_pu; m_pu=NULL;}
	if(m_pv) {delete []m_pv; m_pv=NULL;}
	if(m_pw) {delete []m_pw; m_pw=NULL;}
	//-----------------------------
	int nx = pDataInfo->Width();
	double x0 = pDataInfo->MinX();
	double xInterval = pDataInfo->XInterval();

	int ny = pDataInfo->Height();
	double y0 = pDataInfo->MinY();
	double yInterval = pDataInfo->YInterval();

	int nz = pDataInfo->zNum;
	double z0 = pDataInfo->zMin;
	double zInterval = pDataInfo->zInterval;
	//-----------------------------
	if(nx<=0 || ny<=0) return;
	//-----------------------------
	m_px = new double[nx]; //����
	for(j=0;j<nx;j++)
		*(m_px+j) = x0+j*xInterval;

	m_py = new double[ny]; //γ��
	for(i=0;i<ny;i++)
		*(m_py+i) = y0+i*yInterval;

	if(nz>0)
	{
		m_pz = new double[nz]; //�߶�
		for(i=0;i<nz;i++) *(m_pz+i) = z0+i*zInterval;
	}
	//-----------------------------
	if(eUvw & eUvw_u)
	{
		m_pu = new double[nx*ny];	// U����
		memset(m_pu,0,sizeof(double)*nx*ny);
	}

	if(eUvw & eUvw_v)
	{
		m_pv = new double[nx*ny];	// V����/Ҫ��
		memset(m_pv,0,sizeof(double)*nx*ny);
	}

	if(eUvw & eUvw_w)
	{
		m_pw = new double[nx*ny];	// DWORD
		memset(m_pw,0,sizeof(double)*nx*ny);
	}
	
}

//---------------------------------------------------------
// ��̬���� - beg
//---------------
//---------------
// ��̬���� - end
//---------------------------------------------------------
