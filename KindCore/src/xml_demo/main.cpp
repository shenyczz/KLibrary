#include <stdio.h>
#include "tinyxml.h"
#include <iostream>
#include <cstring>
using namespace std;

/*
	TiXmlDocument：文档类，它代表了整个xml文件
	TiXmlDeclaration：声明类，它表示文件的声明部分
	TiXmlComment：注释类，它表示文件的注释部分
	TiXmlElement：元素类，它是文件的主要部分，并且支持嵌套结构，一般使用这种结构来分类的存储信息，它可以包含属性类和文本类
	TiXmlAttribute/TiXmlAttributeSet：元素属性，它一般嵌套在元素中，用于记录此元素的一些属性
	TiXmlText：文本对象，它嵌套在某个元素内部
*/
//创建xml文件
int writeXmlFile()
{
	TiXmlDocument *writeDoc = new TiXmlDocument; //xml文档指针
	
	//文档格式声明
	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "UTF-8", "yes");
	writeDoc->LinkEndChild(decl); //写入文档

	int n = 3;	//父节点个数

	TiXmlElement *RootElement = new TiXmlElement("Info");//根元素
	RootElement->SetAttribute("num", n); //属性
	writeDoc->LinkEndChild(RootElement);
	
	for(int i=0; i<n; i++)//n个父节点
	{
		TiXmlElement *StuElement = new TiXmlElement("Stu");//Stu
		//设置属性
		StuElement->SetAttribute("class","A");
		if(2 == i)
		{
				StuElement->SetAttribute("class","B");
		}
		
		StuElement->SetAttribute("id",i+1);
		StuElement->SetAttribute("flag", (i+1)*10);
		RootElement->LinkEndChild(StuElement);//父节点写入文档
	
		//姓名
		TiXmlElement *nameElement = new TiXmlElement("name");
		StuElement->LinkEndChild(nameElement);

		TiXmlText *nameContent = new TiXmlText("mike");
		nameElement->LinkEndChild(nameContent);
		
		//分数
		TiXmlElement *scoreElement = new TiXmlElement("score");
		StuElement->LinkEndChild(scoreElement);

		TiXmlText *scoreContent = new TiXmlText("88");
		scoreElement->LinkEndChild(scoreContent);
		
		//城市
		TiXmlElement *cityElement = new TiXmlElement("city");
		StuElement->LinkEndChild(cityElement);

		TiXmlText *cityContent = new TiXmlText("Shenzhen");
		cityElement->LinkEndChild(cityContent);
		
	}
	
	writeDoc->SaveFile("stu_info.xml");
	delete writeDoc;
	
	return 1;
}

//解析xml文件
int readXmlFile()
{
	TiXmlDocument mydoc("stu_info.xml");//xml文档对象
	bool loadOk=mydoc.LoadFile();//加载文档
	if(!loadOk)
	{
		cout<<"could not load the test file.Error:"<<mydoc.ErrorDesc()<<endl;
		exit(1);
	}

	TiXmlElement *RootElement=mydoc.RootElement();	//根元素, Info
	cout<< "[root name]" << RootElement->Value() <<"\n";
	
	TiXmlElement *pEle=RootElement;

	//遍历该结点
	for(TiXmlElement *StuElement = pEle->FirstChildElement();//第一个子元素
		StuElement != NULL;
		StuElement = StuElement->NextSiblingElement())//下一个兄弟元素
	{
		// StuElement->Value() 节点名称
		cout<< StuElement->Value() <<" ";
		TiXmlAttribute *pAttr=StuElement->FirstAttribute();//第一个属性
		
		while( NULL != pAttr) //输出所有属性
		{
			cout<<pAttr->Name()<<":"<<pAttr->Value()<<" ";
			pAttr=pAttr->Next();
		}
		cout<<endl;
		
		//输出子元素的值
		for(TiXmlElement *sonElement=StuElement->FirstChildElement();
		sonElement;
		sonElement=sonElement->NextSiblingElement())
		{
			cout<<sonElement->FirstChild()->Value()<<endl;
		}
	}
	
	return 1;
}

int main(int argc, char *argv[])
{
	
	writeXmlFile();
	printf("\nafter write\n");
	
	readXmlFile();

	return 0;
}