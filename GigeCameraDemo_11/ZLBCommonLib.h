/****************************************************************************
 * Copyright (C) 2004-2007  zte-its Co.,Ltd.  http://www.zte-its.com
 *
 * File      : ZTEImageLib.h  
 * Purpose   : Header file for ZTEImage library
 * Note      : 
 * ==========================================================================
 *
 *					Software License Agreement
 * 
 * Permission is granted to anyone to use this library, in source or object 
 * code form, and to modify, compile and redistribute it to anyone else, 
 * subject to the following restrictions:
 * 
 * 1. Redistribution's of source code must retain the above copyright notice,
 *
 * 2. The author makes no warranty of any kind, either expressed or implied, 
 *    about the suitability of this software for any purpose.
 *
 * 3. The author accepts no liability of any kind for damages or other 
 *    consequences of the use of this software, even if they arise from 
 *    defects in the software.
 *
 ****************************************************************************/
// 

#include <string>
#include <vector>
using namespace std;
#pragma once

#ifndef __ZLB_COMMON_LIB_H__
#define __ZTE_COMMON_LIB_H__

	/*���ļ���*/
	extern "C" __declspec(dllexport) bool ZLBOpenFolder(char* chFloderPath,char* chDefaultDir);
	/*�ж��ļ��Ƿ����*/
	extern "C" __declspec(dllexport) bool ZLBIsPathExist(char* pPathName);
	/*��ȡ��ǰ������ļ�·��*/
	extern "C" __declspec(dllexport) char* ZLBGetCurrentPath();
	/*��ȡ��ǰ������*/
	extern "C" __declspec(dllexport) char* ZLBGetExeName();
	/*��ȡ��ǰ�������Լ��ļ�·��*/
	extern "C" __declspec(dllexport) char* ZLBGetCurrentPathAndName();
	/*��ȡ�����ļ�·��*/
	extern "C" __declspec(dllexport) char* ZLBGetConfigPath();
	//��֪�ļ�ȫ·����ò����ļ�����·��
	extern "C" __declspec(dllexport) char* ZLBGetPath(char* sFileFullName);
	/*��ĳ���ļ�*/
	extern "C" __declspec(dllexport) void ZLBOpenFile(char* sFileName);
	/*���ļ���*/
	extern "C" __declspec(dllexport) void ZLBCreateDirectory(char* sFileName);
	/*�ݹ����������һ���ļ���*/
	extern "C" __declspec(dllexport) int ZLBFindLastDir(char * lpPath, vector<string>& aryLastPath);
	//����ļ���Ŀ¼�����дʱ��
	extern "C" __declspec(dllexport) int ZLBDeleteDir(char*  sDirName);
	//�ݹ������������ļ�
	extern "C" __declspec(dllexport) int  ZLBFindAllFiles(char * lpPath, vector<string>& aryFilePath);
	/*�������*/
	extern "C" __declspec(dllexport) int  ZLBRestartSoftWare();
	/*��������ϵͳʱ�ӵĲ�ֵ*/
	extern "C" __declspec(dllexport) int   ZLBDiffMillionSeconds(SYSTEMTIME& tm1, SYSTEMTIME& tm2);

	/************************************�ļ���дAPI*******************************************************************************/
	/*�ļ���д��*/
	extern "C" __declspec(dllexport) void*  ZLBIniFile(char *chIniPath);
	/*��ȡ�ļ���INT����*/
	extern "C" __declspec(dllexport) int    ZLBIniReadInt(char* sSection,char* pFieldName,int iDefaultValue,void *pUserData);
	/*��ȡ�ļ����ַ�������*/
	extern "C" __declspec(dllexport) char*    ZLBIniReadString(char* sSection,char* strFieldName,char* strDefaultValue,void *pUserData);
	/*��ȡ�ļ���bool����*/
	extern "C" __declspec(dllexport) bool   ZLBIniReadBool(char* sSection,char* strFieldName,bool bDefaultValue,void *pUserData);
	/*��ȡ�ļ��е�double����*/
	extern "C" __declspec(dllexport) double  ZLBIniReadDouble(char* sSection,char* strFieldName,double dDefaultValue,void *pUserData);
	/*д���ļ�INT����*/
	extern "C" __declspec(dllexport) bool    ZLBIniWriteInt(char* sSection,char* strFieldName,int iFieldValue,void *pUserData);
	/*д���ļ��ַ�������*/
	extern "C" __declspec(dllexport) bool    ZLBIniWriteString(char* sSection,char* strFieldName,char* strFieldValue,void *pUserData);
	/*д���ļ�bool����*/
	extern "C" __declspec(dllexport) bool   ZLBIniWriteBool(char* sSection,char* strFieldName,bool bFieldValue,void *pUserData);
	/*д���ļ�double����*/
	extern "C" __declspec(dllexport) bool   ZLBIniWriteDouble(char* sSection,char* strFieldName,double dFieldValue,void *pUserData);
	/*��ȡ�ļ����Լ���չ��*/
	extern "C" __declspec(dllexport) bool   ZLBGetFileNameAndFileType(char* chPathName,char* strFileName,char* strFileType);
	//����ļ���Ŀ¼�����дʱ��
	extern "C" __declspec(dllexport) BOOL ZLBGetDirTime(char*   strDirName,   SYSTEMTIME   &stime);
	/*��ȡ��ʱ�����͵��ļ���*/
	extern "C" __declspec(dllexport) char* ZLBGetFormatTime(int iYear,int iMonth,int iDay,int iHour,int iMin,int iSecond,int iMillSecond);


	/************************************************************ͼ�������ʾ��********************************************************/
	//��ʼ����ʾ�ؼ�
	extern "C" __declspec(dllexport) bool   ZLBInitPictureBox(unsigned char *ucPicBoxInfo,int iPicBoxWidth,int iPicHeight,int iPicDepth,int iPicChannels);
    //��ͼ����Ƶ�������
	extern "C" __declspec(dllexport) void   ZLBDrawPicture(CDC *pDC,unsigned char *ucPicBoxInfo,unsigned char *pImageData,int iPicBoxWidth,int iPicBoxHeight);
    //ȫ����ʾͼ��

#endif


