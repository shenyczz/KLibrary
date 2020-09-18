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
	FieldType_General	= 'G',	// L=?			通用型 or OLE
	FieldType_Interger	= 'I',	// L=4			整型
	FieldType_Logical	= 'L',	// L=1
	FieldType_Memo		= 'M',	// L=10
	FieldType_Numeric	= 'N',	// L=19
};

#pragma pack(1)

// 一、DBF 文件头(32 字节)
struct DBFHeader
{
   unsigned char	cVersion;			// 0		版本
										// 02H - FoxBase
										// 03H - DBASE III (FoxBase/CLIPPER/FoxPro 与之兼容) foxbase+, foxpro, dbase III+, dbaseIV, no memo - 0x03
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

   unsigned char	cYear;				// 1		文件修改日期(yymmdd): 如   5FH,   03H,   0BH   表示   1995.03.11
   unsigned char	cMonth;				// 2		文件修改日期(yymmdd): 如   5FH,   03H,   0BH   表示   1995.03.11
   unsigned char	cDay;				// 3		文件修改日期(yymmdd): 如   5FH,   03H,   0BH   表示   1995.03.11

   unsigned long	lNumberOfRecord;	// 4-7		总记录数
   unsigned short	iLengthOfHeader;	// 8-9		文件头 + 字段结构描述部分的长度 
   unsigned short	iLengthOfRecord;	// 10-11	每条记录的长度

   unsigned char	cReserved[17];		// 12-28	保留
   unsigned char	cLdid;				// 29		Language Driver ID
   unsigned char	cFlag[2];			// 30-31	
};

// 二、字段的结构描述器(每个字段的说明占 32 字节, 字段数最多为 128 个)
// 2.1
struct FieldDescriptor
{
   char szName[11];				// 字段名
								// 由字母/下划线/汉字/数字组成, 但第一个字符不能是数字,中间也不允许有空格

   unsigned char cType;			// 字段类型
								// 'C' - Character
								// 'N' - Numeric
								// 'D' - Date
								// 'L' - Logical
								// 'M' - Memo
								// 'F' - Float (FoxPro)

   unsigned long lOffset;		// 字段数据相对于该记录的偏移量
								// 第一个字段一般为00000001H,
								// 若为 00000000H 或 大于 00001000H 则需要通过计算确定

   unsigned char cFieldLength;	// 字段宽度:
								// Character 254 字节
								// Numeric   19 位
								// Date      08H
								// Logical   01H
								// Memo:     0AH

   char cFieldDecimal;			// 小数点后位数(Character/Date/Logical/Memo 00H)

   char cReserved1[2];			// 
   char cWorkArea;				// dBASE IV work area id
   char cReserved2[2];			// 
   char cSetFieldsFlag;			// 
   char cReserved3[7];			// 
   char cProductionIndex;		// 如果存在一个MDX 格式的索引文件，那么这个记录项为真，否则为空
};

// 2.2 字段结构描述结束符(0DH)

// 三、每条记录内容(各字段间和各条记录间均无分隔符,顺序存放)   
		// 1.	每条记录的第一字节为删除标志:   20H(' ')   ---   正常记录   
		//										2AH('*')   ---   有删除标记的记录   
		// 2.   每条记录字符数最多为   4000   
		// 3.   Char      型数据右边以空格补齐   
		// 4.   Number    型数据也以ASCII码存放
		//                负数前有'-'号),右对齐
		//			      小数包括'.',右边不足的小数位以'0'补齐,左边以空格补齐   
		// 5.   Date      型数据以ASCII码存放,   如'   ','19950311',   '20990517'   
		// 6.   Logical   型数据以ASCII码存放,   'F'   或   '   '   表示   .F.,   'T'   表示   .T.   
		// 7.   Memo      型数据以ASCII码存放在相应 .DBT/.FPT 文件中的起始块号,
		//			      占10个字节,右对齐,如  '5' 表示 .DBT 的第 6 块,
		//			      若 10 个字节全为空格,表示无内容   

// 四、文件结束符:   1字节 1AH

#pragma pack()

#endif	//#ifndef __INCLUDED_DBF_INL__
