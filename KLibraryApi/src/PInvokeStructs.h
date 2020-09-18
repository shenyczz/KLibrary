#pragma once


#pragma pack(1)

struct Simple
{
	short Value;
};

struct Student
{
	SHORT Age;
	TCHAR Name[32];
	TCHAR Address[128];
	DOUBLE Scores[3];
};

struct Group
{
	INT16 GroupID;
	TCHAR Leader[32];
	Student Students[6];
};


#pragma pack()


