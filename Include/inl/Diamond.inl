#ifndef __INCLUDED_DIAMOND_INL__
#define __INCLUDED_DIAMOND_INL__

namespace Diamond {};
using namespace Diamond;

#define	DIAM_FID_TXT_MARK			(_T("DIAMOND"))

#define DIAM_COMMENT_LENGTH				64		// ע����Ϣ����

// DIAM_FMT_CODE ϵͳ�ļ���ʽ����
//
#define	DIAM_FMT_CODE								// Diamond ϵͳ�ļ���ʽ����
#define	DIAM_FMT_CODE_BEG				1			// Diamond ϵͳ�ļ���ʽ��Ʒ���붨�忪ʼ
#define	DIAM_FMT_CODE_MAX				299			// �ļ���ʽ���ֵ 0-99 100-199 200-299

#define	DIAM_FMT_CODE_NONE				0			// Diamond ϵͳ�ļ���ʽ����
#define	DIAM_FMT_CODE_SURFACE			1			// ����ȫҪ����ͼ����
#define	DIAM_FMT_CODE_HIGH				2			// �߿�ȫҪ����ͼ����
#define	DIAM_FMT_CODE_TIN				3			// ���������������ļ���ʽ(ͨ����ͼ����ɢ���ֵ��)
#define	DIAM_FMT_CODE_GRID				4			// �������������ļ���ʽ
#define	DIAM_FMT_CODE_TLOGP				5			// TLOGP��վ������ͼ����
#define	DIAM_FMT_CODE_FAX				6			// ����ͼ
#define	DIAM_FMT_CODE_TYPHOON_PATH		7			// ̨��·������
#define	DIAM_FMT_CODE_CITY_FORECAST		8			// ����վ��Ԥ��
#define	DIAM_FMT_CODE_MAP				9			// ��ͼ��������

#define	DIAM_FMT_CODE_COMBIN			10			// �ۺ�ͼ����
#define	DIAM_FMT_CODE_VECTOR			11			// �糡ʸ������
#define	DIAM_FMT_CODE_RADAR				12			// �����״�ͼ��(PPI)
#define	DIAM_FMT_CODE_IMAGE				13			// ͼ������(������ͼ���״�ƴͼ������ͼ)
#define	DIAM_FMT_CODE_EDIT				14			// ���汻�༭ͼ�ε�ͼԪ����
#define	DIAM_FMT_CODE_PALETTE			15			// ��ɫ���ļ���ʽ
#define	DIAM_FMT_CODE_STATION			16			// Ԥ��վ������
#define	DIAM_FMT_CODE_FSTATION_INFO		17			// Ԥ��վ��������Ϣ
#define	DIAM_FMT_CODE_GRID_SECTION		18			// �����������ͼ
#define	DIAM_FMT_CODE_COMMAND			19			// Micaps �����в���

#define	DIAM_FMT_CODE_TAB				20			// �������
#define	DIAM_FMT_CODE_21					21			//
#define	DIAM_FMT_CODE_22					22			//
#define	DIAM_FMT_CODE_23					23			//
#define	DIAM_FMT_CODE_24					24			//
#define	DIAM_FMT_CODE_25					25			//
#define	DIAM_FMT_CODE_26					26			//
#define	DIAM_FMT_CODE_27					27			//
#define	DIAM_FMT_CODE_28					28			//
#define	DIAM_FMT_CODE_29					29			//

#define	DIAM_FMT_CODE_110				110			//
#define	DIAM_FMT_CODE_STAMP				111			// ��Ʊͼ
#define	DIAM_FMT_CODE_112				112			//
#define	DIAM_FMT_CODE_113				113			//
#define	DIAM_FMT_CODE_114				114			//
#define	DIAM_FMT_CODE_115				115			//
#define	DIAM_FMT_CODE_116				116			//
#define	DIAM_FMT_CODE_117				117			//
#define	DIAM_FMT_CODE_118				118			//
#define	DIAM_FMT_CODE_119				119			//

#define	DIAM_FMT_CODE_770				770			//
#define	DIAM_FMT_CODE_771				771			//
#define	DIAM_FMT_CODE_772				772			//
#define	DIAM_FMT_CODE_773				773			//
#define	DIAM_FMT_CODE_774				774			//
#define	DIAM_FMT_CODE_775				775			//
#define	DIAM_FMT_CODE_776				776			//
#define	DIAM_FMT_CODE_777				777			//
#define	DIAM_FMT_CODE_778				778			//
#define	DIAM_FMT_CODE_779				779			//

#define DIAM_FMT_CVID_MIN				1		// ��ֵ��������Сֵ
#define DIAM_FMT_CVID_MAX				99		// ��ֵ���������ֵ
#define DIAM_FMT_CVID_CONTOUR			9999	// ��ֵ������ = 9999 �ĺ����˵����

#pragma pack(1)
#pragma pack()

#endif	//#ifndef __INCLUDED_DIAMOND_INL__
