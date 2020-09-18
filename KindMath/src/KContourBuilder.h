/******************************************************************************
 ** KContourBuilder.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KContourBuilder - 等值线构造器
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              // 1.设置数据
              SetData();
			  // 2.设置数据信息
			  SetDataInfo();
			  // 3.构造等值线
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
		// 构造等值线(在派生类中定义)
		virtual BOOL BuildContours();

	public:
		// 设置/获取数据
		void SetData(void* pData) { m_pData = pData; }
		void* GetData() { return m_pData; }

		// 设置/获取数据信息
		void SetDataInfo(void* pDataInfo) { m_pDataInfo = pDataInfo; }
		void* GetDataInfo() { return m_pDataInfo; }

		// 取得等值线集
		// eCurveConvert = 0 --> 返回原始等值线集
		// eCurveConvert = 1 --> 返回转换为闭合曲线的等值线集
		KContourPtrCollection* GetContourCollection(CurveConvert eCurveConvert = BeforeConvert)
		{ return eCurveConvert == BeforeConvert ? m_pContourCollection : m_pContourCollection_close; }

		// 取得包围盒
		KExtents* GetBoundingBox() { return m_pBoundingBox; }

	protected:
		// 把所有等值线转化为闭合的曲线
		// 用于绘制等值线色斑图
		void ConvertToCloseCurve();
		// 计算包围盒
		void ComputeBoundingBox();

	private:
		// 取得指定标记的曲线
		KCurve* GetCurve(KContourPtrCollection* pContourPtrCollection, DWORD dwFlag);

	protected:
		// 数据指针(不负责内存管理)
		void* m_pData;
		// 数据信息(不负责内存管理)
		void* m_pDataInfo;

		// 包围盒
		KExtents* m_pBoundingBox;
		// 等值线集
		KContourPtrCollection* m_pContourCollection;
		// 转换成闭合曲线后的等值线集
		KContourPtrCollection* m_pContourCollection_close;


	///--- 等值线追踪值管理 - beg -----------------------------
	public:
		// 增加一个等值线值
		void AddContourValue(double dValue);
		// 移除一个等值线值
		void RemoveContourValue(double dValue);
		// 移除全部等值线值
		void RemoveAllContourValue();
		// 取得等值线值
		double GetContourValue(int index);
		// 取得等值线值数量
		int GetContourValueCount();

		// 设置要追踪的等值线值
		BOOL SetTraceValue(double* va,int n);
		BOOL SetTraceValue(double dInterval, double dValueBeg, double dValueEnd);

	private:
		// 生成要追踪的等值线值
		BOOL GenerateContourValue(double dInterval, double dValueBeg, double dValueEnd);

	private:
		// 要追踪的等值线值
		vector<double> m_dValueVector;

	///--- 等值线追踪值管理 - end ------------------------------
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
