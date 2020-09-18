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
		unsigned long m_lOffset;	// [BE] 记录的偏移地址(字长,*2 转换为字节长度)
		unsigned long m_lLength;	// [BE] 内容长度(字长,*2 转换为字节长度)

	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
