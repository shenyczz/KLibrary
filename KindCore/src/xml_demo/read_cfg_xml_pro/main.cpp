#include <stdio.h>
#include "tinyxml.h"
#include <iostream>
#include <cstring>
using namespace std;


/**********************************************************/
//功能：解析xml文件内容
//参数：str: xml文件路径
//返回值：0：成功	-1: 失败
/**********************************************************/
int readXmlCfg(const char *str)
{
	if (str == NULL) 
	{
		 printf("please input a path\n");
		 return -1;
	}

	TiXmlDocument xml(str);
	if(!xml.LoadFile())
	{
		printf("invalid xml format");
		return -1;
	}

	//根节点元素
	TiXmlElement* root = xml.FirstChildElement("configuration");
	
	//标签名称为"log"的节点元素
	TiXmlElement* log = root->FirstChildElement("log");
	if(NULL != log)//不为空时
	{
		if(NULL == log->Attribute("path"))
		{
			fprintf(stderr, "get [%s] failed\n", "path");
			return -1;
		}
		else
		{
			printf("path = %s\n", log->Attribute("path"));
		}

		if(NULL == log->Attribute("level"))
		{
			fprintf(stderr, "get [%s] failed\n", "level");
			return -1;
		}
		else
		{
			printf("level = %s\n", log->Attribute("level"));
		}
	}
	else
	{
		fprintf(stderr, "get [%s] failed\n", "log");
		return -1;
	}

	//标签名称为"oracle"的节点元素
	TiXmlElement* rdb = root->FirstChildElement("oracle");
	if(NULL != rdb)
	{
		if(NULL == rdb->Attribute("connstr"))
		{
			fprintf(stderr, "get [%s] failed\n", "connstr");
			return -1;
		}
		else
		{
			printf("connstr=%s\n", rdb->Attribute("connstr"));
		}
	}
	else
	{
		fprintf(stderr, "get [%s] failed\n", "oracle");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	readXmlCfg("./sms_aoc_cfg.xml");

	return 0;
}