/******************************************************************************
 ** KContourBuilder.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KContourBuilder - ��ֵ�߹�����
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              // 1.��������
              SetData();
			  // 2.����������Ϣ
			  SetDataInfo();
			  // 3.�����ֵ��
			  BuildContours();

******************************************************************************/

#pragma once

namespace ContourBuilder
{

	class KIND_EXT_CLASS KContourBuilder : public KProgressObject
	{
	public:
		enum CurveConvert : int
		{
			BeforeConvert = 0,
			AfterConvert,
		};

	public:
		KContourBuilder(void);
		virtual ~KContourBuilder(void);

	public:
		void Empty();
		BOOL IsEmpty();

	public:
		// �����ֵ��(���������ж���)
		virtual BOOL BuildContours();

	public:
		// ����/��ȡ����
		void SetData(void* pData) { m_pData = pData; }
		void* GetData() { return m_pData; }

		// ����/��ȡ������Ϣ
		void SetDataInfo(void* pDataInfo) { m_pDataInfo = pDataInfo; }
		void* GetDataInfo() { return m_pDataInfo; }

		// ȡ�õ�ֵ�߼�
		// eCurveConvert = 0 --> ����ԭʼ��ֵ�߼�
		// eCurveConvert = 1 --> ����ת��Ϊ�պ����ߵĵ�ֵ�߼�
		KContourPtrCollection* GetContourCollection(CurveConvert eCurveConvert = BeforeConvert)
		{ return eCurveConvert == BeforeConvert ? m_pContourCollection : m_pContourCollection_close; }

		// ȡ�ð�Χ��
		KExtents* GetBoundingBox() { return m_pBoundingBox; }

	protected:
		// �����е�ֵ��ת��Ϊ�պϵ�����
		// ���ڻ��Ƶ�ֵ��ɫ��ͼ
		void ConvertToCloseCurve();
		// �����Χ��
		void ComputeBoundingBox();

	private:
		// ȡ��ָ����ǵ�����
		KCurve* GetCurve(KContourPtrCollection* pContourPtrCollection, DWORD dwFlag);

	protected:
		// ����ָ��(�������ڴ����)
		void* m_pData;
		// ������Ϣ(�������ڴ����)
		void* m_pDataInfo;

		// ��Χ��
		KExtents* m_pBoundingBox;
		// ��ֵ�߼�
		KContourPtrCollection* m_pContourCollection;
		// ת���ɱպ����ߺ�ĵ�ֵ�߼�
		KContourPtrCollection* m_pContourCollection_close;


	///--- ��ֵ��׷��ֵ���� - beg -----------------------------
	public:
		// ����һ����ֵ��ֵ
		void AddContourValue(double dValue);
		// �Ƴ�һ����ֵ��ֵ
		void RemoveContourValue(double dValue);
		// �Ƴ�ȫ����ֵ��ֵ
		void RemoveAllContourValue();
		// ȡ�õ�ֵ��ֵ
		double GetContourValue(int index);
		// ȡ�õ�ֵ��ֵ����
		int GetContourValueCount();

		// ����Ҫ׷�ٵĵ�ֵ��ֵ
		BOOL SetTraceValue(double* va,int n);
		BOOL SetTraceValue(double dInterval, double dValueBeg, double dValueEnd);

	private:
		// ����Ҫ׷�ٵĵ�ֵ��ֵ
		BOOL GenerateContourValue(double dInterval, double dValueBeg, double dValueEnd);

	private:
		// Ҫ׷�ٵĵ�ֵ��ֵ
		vector<double> m_dValueVector;

	///--- ��ֵ��׷��ֵ���� - end ------------------------------
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
