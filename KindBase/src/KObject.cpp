#include "StdAfx.h"
#include "KObject.h"

KObject::KObject(void)
	:m_Id(0)
	,m_Type(0)
	,m_Name(_T(""))
	,m_Flag(0)
	,m_bSelected(false)
	,m_pOwner(NULL)
{

}

KObject::KObject(const KObject& object)
	:m_Id(object.m_Id)
	,m_Type(object.m_Type)
	,m_Name(object.m_Name)
	,m_Flag(object.m_Flag)
	,m_bSelected(object.m_bSelected)
	,m_Extents(object.m_Extents)
	,m_ExtentsClient(object.m_ExtentsClient)
	,m_pOwner(object.m_pOwner)
{
}

KObject::KObject(LPCTSTR lpszName)
	:m_Id(0)
	,m_Type(0)
	,m_Name(lpszName)
	,m_Flag(0)
	,m_bSelected(false)
	,m_pOwner(NULL)
{
}

KObject::~KObject(void)
{
}

bool KObject::operator ==(const KObject& other)
{
	return true
		&& m_Id				== other.m_Id
		&& m_Type			== other.m_Type
		&& m_Name			== other.m_Name
		&& m_Flag			== other.m_Flag
		&& m_bSelected		== other.m_bSelected
		&& m_Extents		== other.m_Extents
		&& m_ExtentsClient	== other.m_ExtentsClient
		;
}
bool KObject::operator !=(const KObject& other)
{
	return false
		|| m_Id				!= other.m_Id
		|| m_Type			!= other.m_Type
		|| m_Name			!= other.m_Name
		|| m_Flag			!= other.m_Flag
		|| m_bSelected		!= other.m_bSelected
		|| m_Extents		!= other.m_Extents
		|| m_ExtentsClient	!= other.m_ExtentsClient
		;
}
