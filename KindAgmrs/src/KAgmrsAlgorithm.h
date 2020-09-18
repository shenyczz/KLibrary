/******************************************************************************
 ** KAgmrsAlgorithm.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KAgmrsAlgorithm - ũҵ����ң�����ݷ����㷨
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

              //1. 
              use

******************************************************************************/

#pragma once

namespace Agmrs
{

	class KIND_EXT_CLASS KAgmrsAlgorithm : public KDataAlgorithm
	{
	public:
		KAgmrsAlgorithm(void);
		virtual ~KAgmrsAlgorithm(void);

	public:
		enum _channel_
		{
			None = 0,
			Red,		// 0.645um - ����ͨ��
			Nir,		// 0.859um - ������ͨ��
			Blu,		// 0.469um - ����ͨ��
			Grn,		// 0.555um - �̹�ͨ��
			C940,		// 0.940um - 0.940umͨ��
			T11um,		// 11.00um - �Ⱥ���ͨ��
			T12um,		// 12.00um - �Ⱥ���ͨ��
		};

	public:
		// �Ƿ���Ч����
		virtual BOOL IsValidData(double dValue) { return (fabs(dValue-(double)AGMRS_INVALID_DATA)<EPS); }

		// ���ڼ���ָ��������
		void SetData(void* pData) { m_pData = pData; }
		void* GetData() { return m_pData; }

		// LAI���ò���
		void SetLaiConfig(const KStringArray& lc) { m_LaiConfig = lc; }
		const KStringArray& GetLaiConfig() { return m_LaiConfig; }
		bool ConfigLai();

	public:
		//-------------------------------------------------
		///ֲ��ָ��
		//-------------------------------------------------
		// ��һ��ֲ��ָ��
		virtual double Ndvi() = 0;
		// ��ֲֵ��ָ��
		virtual double Rvi() = 0;
		// ��ֲֵ��ָ��
		virtual double Dvi() = 0;
		// ��������ֲ��ָ��
		virtual double Savi() = 0;
		// ��ǿ��ֲ��ָ��
		virtual double Evi() = 0;

		// ֲ��ָ��������Ʒ
		virtual double Lai() = 0;
		//-------------------------------------------------
		///�ɺ�ָ��
		//-------------------------------------------------
		// ֲ����ˮָ��
		virtual double Vswi() = 0;
		// �ر�ˮ��ָ��
		virtual double Swci() = 0;
		//-------------------------------------------------
		///����ָ��
		//-------------------------------------------------
		// ��һ����ѩָ��
		virtual double Ndsi() = 0;
		//-------------------------------------------------
		/// �ر��¶�
		//-------------------------------------------------
		virtual double Lst(int iMethod = 0) = 0;


	protected:
		// ����ָ��
		void* m_pData;

		// LAI ��������
		KStringArray m_LaiConfig;	// �ַ�������{"vix","equ",...}
		KString m_strVix;			// ָ�������ַ�����NDVI��RIV��DIV��SAVI��
		KString m_strEqu;			// ָ����Ӧ�ķ���
	};

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------
