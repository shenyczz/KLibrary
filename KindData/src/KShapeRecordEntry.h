#pragma once

namespace Shapefile
{

	class KIND_EXT_CLASS KShapeRecordEntry
	{
	public:
		KShapeRecordEntry(long offset, long length);
		virtual ~KShapeRecordEntry(void);

	public:
		long Offset() { return m_lOffset * 2; }
		long Length() { return m_lLength * 2; }

	private:
		unsigned long m_lOffset;	// [BE] ��¼��ƫ�Ƶ�ַ(�ֳ�,*2 ת��Ϊ�ֽڳ���)
		unsigned long m_lLength;	// [BE] ���ݳ���(�ֳ�,*2 ת��Ϊ�ֽڳ���)

	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
