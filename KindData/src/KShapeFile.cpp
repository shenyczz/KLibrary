#include "StdAfx.h"
#include "KShapeFile.h"

namespace Shapefile
{

	KShapeFile::KShapeFile(void)
		: KDataObject()
		, m_bHasDbf(false)
		, m_bHasIndexFile(false)
		, m_strDbfFile(_T(""))
		, m_strShxFile(_T(""))
		, m_pDbf(NULL)
		, m_pShapeFileIndex(NULL)
		, m_pShapeFileHeader(NULL)
	{
		SetFileName(_T(""));
	}

	KShapeFile::KShapeFile(LPCTSTR lpszFile) throw(KException)
		: KDataObject(lpszFile)
		, m_bHasDbf(false)
		, m_bHasIndexFile(false)
		, m_strDbfFile(_T(""))
		, m_strShxFile(_T(""))
		, m_pDbf(NULL)
		, m_pShapeFileIndex(NULL)
		, m_pShapeFileHeader(NULL)
	{
	}

	KShapeFile::~KShapeFile(void)
	{
		_delete(m_pDbf);
		_delete(m_pShapeFileIndex);
		_delete(m_pShapeFileHeader);
	}

	// ��
	BOOL KShapeFile::LoadData_bin(LPCTSTR lpszFile)
	{
		if(!KFile::Exists(lpszFile))
			return false;

		KFile file(lpszFile);

		KString strTemp;
		strTemp.Format(_T("%s%s")
			,(LPCTSTR)file.GetDirectory()
			,(LPCTSTR)file.GetFileName(false)
			);

		m_strDbfFile = strTemp + _T(".dbf");
		m_strShxFile = strTemp + _T(".shx");

		m_bHasDbf = KFile::Exists(m_strDbfFile);
		m_bHasIndexFile = KFile::Exists(m_strShxFile);

		// 1. ��ȡ��������
		if(m_bHasDbf)
		{
			// ��ȡ��������
			try
			{
				m_pDbf = new KDbaseFile(m_strDbfFile);
			}
			catch(KException& e)
			{
				_delete(m_pDbf);
				return false;
				throw KException(e);
			}
		}

		// 2. ��ȡ��������
		if(m_bHasIndexFile)
		{
			try
			{
				m_pShapeFileIndex = new KShapeFileIndex(this);
			}
			catch(KException& e)
			{
				_delete(m_pShapeFileIndex);
				return false;
				throw KException(e);
			}
		}

		// 3. ��ȡ��״��������
		if(!ReadShape(lpszFile))
		{
			return false;
			throw KException(_T("Read Shape file error."));
		}

		return true;
	}

	// ��ȡ Shape ����
	bool KShapeFile::ReadShape(LPCTSTR lpszFile)
	{
		return m_bHasIndexFile
			? ReadShape_WithIndexfile(lpszFile)
			: ReadShape_WithoutIndexfile(lpszFile);
	}
	bool KShapeFile::ReadShape_WithIndexfile(LPCTSTR lpszFile)
	{
		KShapeFileIndex* pShapeFileIndex = m_pShapeFileIndex;
		if(!pShapeFileIndex)
			return false;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		int iShapeCount = pShapeFileIndex->Count();
		for(int i=0;i<iShapeCount;i++)
		{
			KShapeRecordEntry* pShapeRecordEntry = pShapeFileIndex->GetAt(i);
			if(!pShapeRecordEntry)
				break;

			long loffset = pShapeRecordEntry->Offset();
			KGeometryPtrCollection* pGeometryPtrCollection = this->readOneShape(fp,loffset);

			if(pGeometryPtrCollection)
			{
				this->Add(pGeometryPtrCollection);
			}

		}// next i

		fclose(fp);
		fp = NULL;

		return true;
	}
	bool KShapeFile::ReadShape_WithoutIndexfile(LPCTSTR lpszFile)
	{
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		// ��ȡ�ļ�ͷ����
		SHAPE_FILE_HEADER shapeFileHeader;
		memset(&shapeFileHeader, 0, sizeof(SHAPE_FILE_HEADER) - sizeof(double) * 8);
		fread(&shapeFileHeader, sizeof(SHAPE_FILE_HEADER) - sizeof(double) * 8, 1, fp);

		fclose(fp);
		fp = NULL;

		_delete(m_pShapeFileHeader);
		m_pShapeFileHeader = new KShapeFileHeader(shapeFileHeader);

		bool bRead = false;
		ShapeType eShapeType = (ShapeType)this->GetType();
		switch(eShapeType)
		{
		case ShapeType_Point:
			bRead = readPointData_WithoutIndexfile(lpszFile);
			break;

		case ShapeType_Polyline:
			bRead = readPolylineData_WithoutIndexfile(lpszFile);
			break;

		case ShapeType_Polygon:
			bRead = readPolygonData_WithoutIndexfile(lpszFile);
			break;
		}

		return bRead;
	}

	//-----------------------------------------------------Without Index File - beg
	bool KShapeFile::readPointData_WithoutIndexfile(LPCTSTR lpszFile)
	{
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		// �����ļ�ͷ
		fseek(fp,sizeof(SHAPE_FILE_HEADER),SEEK_SET);

		while(!feof(fp))
		{
			// ��ȡ��¼ͷ
			ShapeRecordHead shapeRecordHead;
			memset(&shapeRecordHead,0,sizeof(ShapeRecordHead));
			fread(&shapeRecordHead, sizeof(ShapeRecordHead), 1, fp);

			KBytesOrder::ReverseBytes(&shapeRecordHead.lRecordNo);
			KBytesOrder::ReverseBytes(&shapeRecordHead.lContentLength);

			// ��¼�� lRecordNo �� 1 ��ʼ
			// ��¼�ŵ���0��ʾ Null Shape
			if(shapeRecordHead.lRecordNo==0)
				break;

			// ��ȡͼ������
			long lShapeType = ShapeType_Null;
			fread(&lShapeType, sizeof(long), 1, fp);
			if( lShapeType != ShapeType_Point )
			{
				fclose(fp);
				fp = NULL;
				return false;
			}

			// �㼯
			KGeometryPtrCollection* pGeometryPtrCollection = new KGeometryPtrCollection();
			pGeometryPtrCollection->SetType(GeometryType_PointCollection);

			// ������Ŀ���ж��������� - beg
			{
				// ��������(Point)�ڴ�
				KPoint* pPoint = new KPoint();

				// ��ȡ������
				double x, y;
				fread(&x , sizeof(double), 1, fp);
				fread(&y , sizeof(double), 1, fp);

				*pPoint = KPoint(x,y);
				pPoint->SetExtents(KExtents(x,y,x,y));

				pGeometryPtrCollection->Add(pPoint);
			}
			// ������Ŀ���ж��������� - end

			this->Add(pGeometryPtrCollection);

		}//while(!feof(fp))

		fclose(fp);
		fp = NULL;

		return true;
	}
	bool KShapeFile::readPolylineData_WithoutIndexfile(LPCTSTR lpszFile)
	{
		int i;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		// �����ļ�ͷ
		fseek(fp,sizeof(SHAPE_FILE_HEADER),SEEK_SET);

		while(!feof(fp))
		{
			// ��ȡ��¼ͷ
			ShapeRecordHead shapeRecordHead;
			memset(&shapeRecordHead,0,sizeof(ShapeRecordHead));
			fread(&shapeRecordHead, sizeof(ShapeRecordHead), 1, fp);

			KBytesOrder::ReverseBytes(&shapeRecordHead.lRecordNo);
			KBytesOrder::ReverseBytes(&shapeRecordHead.lContentLength);

			// ��¼�� lRecordNo �� 1 ��ʼ
			// ��¼�ŵ���0��ʾ Null Shape 
			if(shapeRecordHead.lRecordNo==0)
				break;

			// ��ȡͼ������
			long lShapeType = ShapeType_Null;
			fread(&lShapeType, sizeof(long), 1, fp);
			if( lShapeType != ShapeType_Polyline )
			{
				fclose(fp);
				fp = NULL;
				return false;
			}

			// ��ȡShape�����Χ������
			double objMinX, objMinY, objMaxX, objMaxY;
			fread(&objMinX, sizeof(double), 1, fp);
			fread(&objMinY, sizeof(double), 1, fp);
			fread(&objMaxX, sizeof(double), 1, fp);
			fread(&objMaxY, sizeof(double), 1, fp);

			// ���߼�
			KGeometryPtrCollection* pGeometryPtrCollection = new KGeometryPtrCollection();
			pGeometryPtrCollection->SetType(GeometryType_PolylineCollection);
			pGeometryPtrCollection->SetExtents(KExtents(objMinX,objMinY,objMaxX,objMaxY));

			// ��ȡͼ������
			int numParts;	// ��������
			int numPoints;	// �ܵ���
			fread(&numParts, sizeof(int), 1, fp);
			fread(&numPoints, sizeof(int), 1, fp);

			// ���������������
			int* Parts = new int[numParts];
			for(  i = 0; i < numParts; i++ )
			{
				int indexFirstPoint;
				fread(&indexFirstPoint, sizeof(int), 1, fp);
				Parts[i] = indexFirstPoint;
			}

			// ������Ŀ���ж����ߵ����� - beg
			for(i = 0; i < numParts; i++)
			{
				// ����һ�� KPolyline �����ڴ�
				KPolyline* pPolyline = new KPolyline();

				int curPosIndex = Parts[i];
				int nxtPosIndex = 0;
				int curPointCount = 0;

				if( i == numParts - 1 )
				{
					curPointCount = numPoints - curPosIndex ;
				}
				else
				{
					nxtPosIndex = Parts[i + 1] ;
					curPointCount = nxtPosIndex - curPosIndex ;
				}

				//����һ���߶ε�����
				for( int iteratorPoint=0; iteratorPoint<curPointCount; iteratorPoint++ )
				{
					double x , y ;
					fread(&x, sizeof(double), 1, fp) ;
					fread(&y, sizeof(double), 1, fp) ;

					pPolyline->Add(new KPoint(x,y));
				}

				pGeometryPtrCollection->Add(pPolyline);	// ����һ����

			}// next i
			// ������Ŀ���ж����ߵ����� - end

			_deletea(Parts);
			this->Add(pGeometryPtrCollection);

		}//while(!feof(fp))

		fclose(fp);
		fp = NULL;

		return true;
	}
	bool KShapeFile::readPolygonData_WithoutIndexfile(LPCTSTR lpszFile)
	{
		int i;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		// �����ļ�ͷ
		fseek(fp,sizeof(SHAPE_FILE_HEADER),SEEK_SET);

		while(!feof(fp))
		{
			// ��ȡ��¼ͷ
			ShapeRecordHead shapeRecordHead;
			memset(&shapeRecordHead,0,sizeof(ShapeRecordHead));
			fread(&shapeRecordHead, sizeof(ShapeRecordHead), 1, fp);

			KBytesOrder::ReverseBytes(&shapeRecordHead.lRecordNo);
			KBytesOrder::ReverseBytes(&shapeRecordHead.lContentLength);

			// ��¼�� lRecordNo �� 1 ��ʼ
			// ��¼�ŵ���0��ʾ Null Shape 
			if(shapeRecordHead.lRecordNo==0)
				break;

			// ��ȡͼ������
			long lShapeType = ShapeType_Null;
			fread(&lShapeType, sizeof(long), 1, fp);
			if( lShapeType != ShapeType_Polygon )
			{
				fclose(fp);
				fp = NULL;
				return false;
			}

			// ��ȡShape�����Χ������
			double objMinX, objMinY, objMaxX, objMaxY;
			fread(&objMinX, sizeof(double), 1, fp);
			fread(&objMinY, sizeof(double), 1, fp);
			fread(&objMaxX, sizeof(double), 1, fp);
			fread(&objMaxY, sizeof(double), 1, fp);

			// ����μ�
			KGeometryPtrCollection* pGeometryPtrCollection = new KGeometryPtrCollection();
			pGeometryPtrCollection->SetType(GeometryType_PolygonCollection);
			pGeometryPtrCollection->SetExtents(KExtents(objMinX,objMinY,objMaxX,objMaxY));

			// ��ȡͼ������
			int numParts;	// ��������
			int numPoints;	// �ܵ���
			fread(&numParts, sizeof(int), 1, fp);
			fread(&numPoints, sizeof(int), 1, fp);

			// ���������������
			int* Parts = new int[numParts];
			for(  i = 0; i < numParts; i++ )
			{
				int indexFirstPoint;
				fread(&indexFirstPoint, sizeof(int), 1, fp);
				Parts[i] = indexFirstPoint;
			}

			// ������Ŀ���ж����ߵ����� - beg
			for(i = 0; i < numParts; i++)
			{
				// ����һ�� KPolygon �����ڴ�
				KPolygon* pPolygon = new KPolygon();

				int curPosIndex = Parts[i];
				int nxtPosIndex = 0;
				int curPointCount = 0;

				if( i == numParts - 1 )
				{
					curPointCount = numPoints - curPosIndex ;
				}
				else
				{
					nxtPosIndex = Parts[i + 1] ;
					curPointCount = nxtPosIndex - curPosIndex ;
				}

				//����һ���߶ε�����
				for( int iteratorPoint=0; iteratorPoint<curPointCount; iteratorPoint++ )
				{
					double x , y ;
					fread(&x, sizeof(double), 1, fp) ;
					fread(&y, sizeof(double), 1, fp) ;

					pPolygon->Add(new KPoint(x,y));
				}

				pGeometryPtrCollection->Add(pPolygon);	// ����һ�����

			}// next i
			// ������Ŀ���ж����ߵ����� - end

			_deletea(Parts);
			this->Add(pGeometryPtrCollection);

		}//while(!feof(fp))

		fclose(fp);
		fp = NULL;

		return true;
	}
	//-----------------------------------------------------Without Index File - end

	// ��ȡ����ͼ�μ���
	KGeometryPtrCollection* KShapeFile::readOneShape(FILE* fp, long lOffset)
	{
		KGeometryPtrCollection* pGeometryPtrCollection = NULL;

		ShapeType eShapeType = (ShapeType)this->GetType();
		switch(eShapeType)
		{
		case ShapeType_Point:
			pGeometryPtrCollection = this->readOnePointShape(fp,lOffset);
			break;

		case ShapeType_Polyline:
			pGeometryPtrCollection = this->readOnePolylineShape(fp,lOffset);
			break;

		case ShapeType_Polygon:
			pGeometryPtrCollection = this->readOnePolygonShape(fp,lOffset);
			break;
		}

		return pGeometryPtrCollection;
	}
	KGeometryPtrCollection* KShapeFile::readOnePointShape(FILE* fp, long lOffset)
	{
		KGeometryPtrCollection* pGeometryPtrCollection = NULL;

		fseek(fp,lOffset,SEEK_SET);

		// ��ȡ��¼ͷ
		ShapeRecordHead shapeRecordHead;
		memset(&shapeRecordHead,0,sizeof(ShapeRecordHead));
		fread(&shapeRecordHead, sizeof(ShapeRecordHead), 1, fp);

		KBytesOrder::ReverseBytes(&shapeRecordHead.lRecordNo);
		KBytesOrder::ReverseBytes(&shapeRecordHead.lContentLength);

		// ��¼�� lRecordNo �� 1 ��ʼ
		// ��¼�ŵ���0��ʾ Null Shape
		if(shapeRecordHead.lRecordNo==0)
			return NULL;

		// ��ȡͼ������
		long lShapeType = ShapeType_Null;
		fread(&lShapeType, sizeof(long), 1, fp);
		if( lShapeType != ShapeType_Point )
			return NULL;

		// �㼯
		pGeometryPtrCollection = new KGeometryPtrCollection();
		pGeometryPtrCollection->SetType(GeometryType_PointCollection);

		// ������Ŀ���ж��������� - beg
		{
			// ��ȡ������
			double x, y;
			fread(&x , sizeof(double), 1, fp);
			fread(&y , sizeof(double), 1, fp);

			// ��������(Point)�ڴ�
			KPoint* pPoint = new KPoint(x,y);
			pPoint->SetExtents(KExtents(x,y,x,y));

			pGeometryPtrCollection->Add(pPoint);
		}
		// ������Ŀ���ж��������� - end

		return pGeometryPtrCollection;
	}
	KGeometryPtrCollection* KShapeFile::readOnePolylineShape(FILE* fp, long lOffset)
	{
		int i;
		KGeometryPtrCollection* pGeometryPtrCollection = NULL;

		fseek(fp,lOffset,SEEK_SET);

		// ��ȡ��¼ͷ
		ShapeRecordHead shapeRecordHead;
		memset(&shapeRecordHead,0,sizeof(ShapeRecordHead));
		fread(&shapeRecordHead, sizeof(ShapeRecordHead), 1, fp);

		KBytesOrder::ReverseBytes(&shapeRecordHead.lRecordNo);
		KBytesOrder::ReverseBytes(&shapeRecordHead.lContentLength);

		// ��¼�� lRecordNo �� 1 ��ʼ
		// ��¼�ŵ���0��ʾ Null Shape
		if(shapeRecordHead.lRecordNo==0)
			return NULL;

		// ��ȡͼ������
		long lShapeType = ShapeType_Null;
		fread(&lShapeType, sizeof(long), 1, fp);
		if( lShapeType != ShapeType_Polyline )
			return NULL;

		// ��ȡShape�����Χ������
		double objMinX, objMinY, objMaxX, objMaxY;
		fread(&objMinX, sizeof(double), 1, fp);
		fread(&objMinY, sizeof(double), 1, fp);
		fread(&objMaxX, sizeof(double), 1, fp);
		fread(&objMaxY, sizeof(double), 1, fp);

		// ���߼�
		pGeometryPtrCollection = new KGeometryPtrCollection();
		pGeometryPtrCollection->SetType(GeometryType_PolylineCollection);
		pGeometryPtrCollection->SetExtents(KExtents(objMinX,objMinY,objMaxX,objMaxY));

		// ��ȡͼ������
		int numParts;	// ��������
		int numPoints;	// �ܵ���
		fread(&numParts, sizeof(int), 1, fp);
		fread(&numPoints, sizeof(int), 1, fp);

		// ���������������
		int* Parts = new int[numParts];
		for( i = 0; i < numParts; i++ )
		{
			int indexFirstPoint;
			fread(&indexFirstPoint, sizeof(int), 1, fp);
			Parts[i] = indexFirstPoint;
		}

		// ������Ŀ���ж����ߵ����� - beg
		for(i = 0; i < numParts; i++)
		{
			// ����һ�� KPolyline �����ڴ�
			KPolyline* pPolyline = new KPolyline();

			int curPosIndex = Parts[i];
			int nxtPosIndex = 0;
			int curPointCount = 0;

			if( i == numParts - 1 )
			{
				curPointCount = numPoints - curPosIndex ;
			}
			else
			{
				nxtPosIndex = Parts[i + 1] ;
				curPointCount = nxtPosIndex - curPosIndex ;
			}

			//����һ���߶ε�����
			for( int iteratorPoint=0; iteratorPoint<curPointCount; iteratorPoint++ )
			{
				double x , y ;
				fread(&x, sizeof(double), 1, fp) ;
				fread(&y, sizeof(double), 1, fp) ;

				pPolyline->Add(new KPoint(x,y));
			}

			pGeometryPtrCollection->Add(pPolyline);	// ����һ����

		}// next i
		// ������Ŀ���ж����ߵ����� - end

		_delete(Parts);
		return pGeometryPtrCollection;
	}
	KGeometryPtrCollection* KShapeFile::readOnePolygonShape(FILE* fp, long lOffset)
	{
		int i;
		KGeometryPtrCollection* pGeometryPtrCollection = NULL;

		fseek(fp,lOffset,SEEK_SET);

		// ��ȡ��¼ͷ
		ShapeRecordHead shapeRecordHead;
		memset(&shapeRecordHead,0,sizeof(ShapeRecordHead));
		fread(&shapeRecordHead, sizeof(ShapeRecordHead), 1, fp);

		KBytesOrder::ReverseBytes(&shapeRecordHead.lRecordNo);
		KBytesOrder::ReverseBytes(&shapeRecordHead.lContentLength);

		// ��¼�� lRecordNo �� 1 ��ʼ
		// ��¼�ŵ���0��ʾ Null Shape
		if(shapeRecordHead.lRecordNo==0)
			return NULL;

		// ��ȡͼ������
		long lShapeType = ShapeType_Null;
		fread(&lShapeType, sizeof(long), 1, fp);
		if( lShapeType != ShapeType_Polygon )
			return NULL;

		// ��ȡShape�����Χ������
		double objMinX, objMinY, objMaxX, objMaxY;
		fread(&objMinX, sizeof(double), 1, fp);
		fread(&objMinY, sizeof(double), 1, fp);
		fread(&objMaxX, sizeof(double), 1, fp);
		fread(&objMaxY, sizeof(double), 1, fp);

		// ���߼�
		pGeometryPtrCollection = new KGeometryPtrCollection();
		pGeometryPtrCollection->SetType(GeometryType_PolygonCollection);
		pGeometryPtrCollection->SetExtents(KExtents(objMinX,objMinY,objMaxX,objMaxY));

		// ��ȡͼ������
		int numParts;	// ��������
		int numPoints;	// �ܵ���
		fread(&numParts, sizeof(int), 1, fp);
		fread(&numPoints, sizeof(int), 1, fp);

		// ���������������
		int* Parts = new int[numParts];
		for( i = 0; i < numParts; i++ )
		{
			int indexFirstPoint;
			fread(&indexFirstPoint, sizeof(int), 1, fp);
			Parts[i] = indexFirstPoint;
		}

		// ������Ŀ���ж����ߵ����� - beg
		for(i = 0; i < numParts; i++)
		{
			// ����һ�� KPolygon �����ڴ�
			KPolygon* pPolygon = new KPolygon();

			int curPosIndex = Parts[i];
			int nxtPosIndex = 0;
			int curPointCount = 0;

			if( i == numParts - 1 )
			{
				curPointCount = numPoints - curPosIndex ;
			}
			else
			{
				nxtPosIndex = Parts[i + 1] ;
				curPointCount = nxtPosIndex - curPosIndex ;
			}

			//����һ���߶ε�����
			for( int iteratorPoint=0; iteratorPoint<curPointCount; iteratorPoint++ )
			{
				double x , y ;
				fread(&x, sizeof(double), 1, fp) ;
				fread(&y, sizeof(double), 1, fp) ;

				pPolygon->Add(new KPoint(x,y));
			}

			pGeometryPtrCollection->Add(pPolygon);	// ����һ�����

		}// next i
		// ������Ŀ���ж����ߵ����� - end

		_delete(Parts);
		return pGeometryPtrCollection;
	}

	// ��������
	BOOL KShapeFile::SaveData(LPCTSTR lpszFile)
	{
		return FALSE;
	}

	//---------------------------------------------------------
	// �Ƿ�Shape�ļ�
	//static
	BOOL KShapeFile::IsShapeFile(LPCTSTR lpszFile)
	{
		FILE* fp = _tfopen(lpszFile,_T("rb"));

		if(fp==NULL)
			return FALSE;

		// ��ȡ�ļ�ͷ����
		SHAPE_FILE_HEADER shapeFileHead;
		memset(&shapeFileHead,0,sizeof(SHAPE_FILE_HEADER));
		fread(&shapeFileHead, sizeof(SHAPE_FILE_HEADER), 1, fp);

		KBytesOrder::ReverseBytes(&shapeFileHead.lFileCode);
		KBytesOrder::ReverseBytes(&shapeFileHead.lFileLength);

		fclose(fp);
		fp = NULL;

		// �ļ���ʽ
		return (shapeFileHead.lFileCode == 0x270A);
	}
	//---------------------------------------------------------

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
