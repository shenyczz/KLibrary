#ifndef __INCLUDED_DBF_INL__
#define __INCLUDED_DBF_INL__

enum FieldType
{
	FieldType_None		= 0,

	FieldType_Binary	= 'B',	//				
	FieldType_Character	= 'C',	// Lmax=254		
	FieldType_Date		= 'D',	// L=8
	FieldType_DateTime	= 'T',	// L=8
	FieldType_Float		= 'F',	// L=32
	FieldType_General	= 'G',	// L=?			ͨ���� or OLE
	FieldType_Interger	= 'I',	// L=4			����
	FieldType_Logical	= 'L',	// L=1
	FieldType_Memo		= 'M',	// L=10
	FieldType_Numeric	= 'N',	// L=19
};

#pragma pack(1)

// һ��DBF �ļ�ͷ(32 �ֽ�)
struct DBFHeader
{
   unsigned char	cVersion;			// 0		�汾
										// 02H - FoxBase
										// 03H - DBASE III (FoxBase/CLIPPER/FoxPro ��֮����) foxbase+, foxpro, dbase III+, dbaseIV, no memo - 0x03
										// 04H - dBase IV w/o memo file
										// 05H - dBase V w/o memo file
										// 30H - Visual FoxPro / Visual FoxPro w. DBC
										// 31H - Visual FoxPro w. AutoIncrement field
										// 43H - .dbv memo var size (Flagship)
										// 7BH - dBASE IV with memo
										// 83H - FoxBase+  2.0/2.1  foxbase+, dbase III+ with memo - File with DBT
										// 8BH - dBase IV with memo
										// CBH - dBase IV with SQL table - 0x8e
										// E5H - Clipper SIX driver w. SMT memo file.
										//		 Note! Clipper SIX driver sets lowest 3 bytes to 110 in descriptor of crypted databases.
										//		 So, 3->6, 83h->86h, F5->F6, E5->E6 etc.
										// F5H - FoxPro 2.0/2.5  foxpro with memo - 0xf5

   unsigned char	cYear;				// 1		�ļ��޸�����(yymmdd): ��   5FH,   03H,   0BH   ��ʾ   1995.03.11
   unsigned char	cMonth;				// 2		�ļ��޸�����(yymmdd): ��   5FH,   03H,   0BH   ��ʾ   1995.03.11
   unsigned char	cDay;				// 3		�ļ��޸�����(yymmdd): ��   5FH,   03H,   0BH   ��ʾ   1995.03.11

   unsigned long	lNumberOfRecord;	// 4-7		�ܼ�¼��
   unsigned short	iLengthOfHeader;	// 8-9		�ļ�ͷ + �ֶνṹ�������ֵĳ��� 
   unsigned short	iLengthOfRecord;	// 10-11	ÿ����¼�ĳ���

   unsigned char	cReserved[17];		// 12-28	����
   unsigned char	cLdid;				// 29		Language Driver ID
   unsigned char	cFlag[2];			// 30-31	
};

// �����ֶεĽṹ������(ÿ���ֶε�˵��ռ 32 �ֽ�, �ֶ������Ϊ 128 ��)
// 2.1
struct FieldDescriptor
{
   char szName[11];				// �ֶ���
								// ����ĸ/�»���/����/�������, ����һ���ַ�����������,�м�Ҳ�������пո�

   unsigned char cType;			// �ֶ�����
								// 'C' - Character
								// 'N' - Numeric
								// 'D' - Date
								// 'L' - Logical
								// 'M' - Memo
								// 'F' - Float (FoxPro)

   unsigned long lOffset;		// �ֶ���������ڸü�¼��ƫ����
								// ��һ���ֶ�һ��Ϊ00000001H,
								// ��Ϊ 00000000H �� ���� 00001000H ����Ҫͨ������ȷ��

   unsigned char cFieldLength;	// �ֶο��:
								// Character 254 �ֽ�
								// Numeric   19 λ
								// Date      08H
								// Logical   01H
								// Memo:     0AH

   char cFieldDecimal;			// С�����λ��(Character/Date/Logical/Memo 00H)

   char cReserved1[2];			// 
   char cWorkArea;				// dBASE IV work area id
   char cReserved2[2];			// 
   char cSetFieldsFlag;			// 
   char cReserved3[7];			// 
   char cProductionIndex;		// �������һ��MDX ��ʽ�������ļ�����ô�����¼��Ϊ�棬����Ϊ��
};

// 2.2 �ֶνṹ����������(0DH)

// ����ÿ����¼����(���ֶμ�͸�����¼����޷ָ���,˳����)   
		// 1.	ÿ����¼�ĵ�һ�ֽ�Ϊɾ����־:   20H(' ')   ---   ������¼   
		//										2AH('*')   ---   ��ɾ����ǵļ�¼   
		// 2.   ÿ����¼�ַ������Ϊ   4000   
		// 3.   Char      �������ұ��Կո���   
		// 4.   Number    ������Ҳ��ASCII����
		//                ����ǰ��'-'��),�Ҷ���
		//			      С������'.',�ұ߲����С��λ��'0'����,����Կո���   
		// 5.   Date      ��������ASCII����,   ��'   ','19950311',   '20990517'   
		// 6.   Logical   ��������ASCII����,   'F'   ��   '   '   ��ʾ   .F.,   'T'   ��ʾ   .T.   
		// 7.   Memo      ��������ASCII��������Ӧ .DBT/.FPT �ļ��е���ʼ���,
		//			      ռ10���ֽ�,�Ҷ���,��  '5' ��ʾ .DBT �ĵ� 6 ��,
		//			      �� 10 ���ֽ�ȫΪ�ո�,��ʾ������   

// �ġ��ļ�������:   1�ֽ� 1AH

#pragma pack()

#endif	//#ifndef __INCLUDED_DBF_INL__
