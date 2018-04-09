/****************************************************************************
 * Copyright (C) 2004-2007  ZLB-its Co.,Ltd.  http://www.ZLB-its.com
 *
 * File      : ZLBImageLib.h  
 * Purpose   : Header file for ZLBImage library
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
#pragma once

#ifndef __ZLB_IMAGE_LIB_H__
#define __ZLB_IMAGE_LIB_H__

typedef struct ZLBImageStruct
{
    int width;
    int height;
    int depth;
	int channels;
	int widthstep;
    char* data;
}
ZLBImageStruct;

/*����ͼ���С�ṹ*/
typedef struct 
{
   int width;
   int height;
}
ZLBImageSize;

/*ͼ������ṹ*/
typedef struct
{
	int ix;
	int iy;
}
ZLBPoint;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
}
ZLBImageRect;

#ifdef __cplusplus
extern "C" {
#endif


///* ��ȡͼ�� */
//bool __stdcall ZLBLoadImage(LPSTR imageFileName);

/* �õ�ͼ����� */
//void __stdcall ZLBGetImage(char** imageData, int* width, int* height, int* depth, int* channels);

//* �ͷ�ͼ���ڴ� */
//void __stdcall ZLBReleaseImage();

/* �ͷ�����ռ�õ��ڴ���Դ */
void __stdcall ZLBImageDllRelease();

/* ��ȡͼ�� */
ZLBImageStruct* __stdcall ZLBLoadImage(LPSTR imageFileName);

/*��ȡͼ���ڴ�*/
ZLBImageStruct* __stdcall ZLBLoadMemImage(unsigned char *chJpegLength,int iJpegLength);

/*����ͼ���ڴ�*/
bool __stdcall ZLBSaveMemImage(ZLBImageStruct *pImageStruct,unsigned char **chJpegChar,int *iJpegLength,int iJpegQuality);

/* �ͷ�ͼ���ڴ� */
void __stdcall ZLBReleaseImage(ZLBImageStruct** ppImage,int iReleaseMode);

/* �����ڴ�ͼ���ļ� */
bool __stdcall ZLBSaveImage(LPSTR imageFileName, char* imageData, int width, int height, int depth, int channels,int iJpegQuality);

/*����ͼ��洢��������*/
void __stdcall ZLBSetImageQuality(float fQuality);

/*��ֱ��תͼ������ָ��*/
void __stdcall ZLBFlipImage(char *imageData,int width,int height,int depth,int channels);

/* �õ�ͼ������ x,y ����Ӧ������ָ�� */
void __stdcall ZLBGetPixPointer(char *imageData, int width, int height, int depth, int channels, int x, int y, char **pxy);

/* ����һ��ͼ��ռ� */
ZLBImageStruct* __stdcall ZLBCreateImage(int width, int height, int depth, int channels);

/* Դͼ�񿽱���Ŀ��ͼ����ȥ */
/* ��Ҫ������Դͼ�������Ŀ��ͼ������ֱ��� sx,sy...dx,dy,... ָ�� */
void __stdcall ZLBCopyImage(ZLBImageStruct* psourceImage, int sx, int sy, int swidth, int sheight, 
							ZLBImageStruct* pdestImage, int dx, int dy, int dwidth, int dheight);


/* �����ֵ��ӵ�ͼ���� */
/* x, y Ϊ����λ�� */
/* ������������ɫ����: 0 - ��ɫ, 1 - ��ɫ, 2 - ��ɫ 3 -��ɫ 4 - ��ɫ */
/* �������ַ�͸���� 0.0 - 1.0 */
/* sizeΪ �������ַ�������Ĵ�С*/
/* fCharSpace�ַ����*/
void __stdcall ZLBPutTextToImage(ZLBImageStruct* pImage, char* pText, int x, int y, int color, float size,float fCharSpace, float transparency);

/*����ͼ�������*/
/*ZLBImageRect ���Ƴ�������*/
/*color: 0 - ��ɫ, 1 - ��ɫ, 2 - ��ɫ*/
/*thickness ���ƵĴ�ϸ*/
void __stdcall ZLBDrawPlateRect(ZLBImageStruct *pImage,ZLBImageRect thisImageRect,LONG color,int thickness);

/*����ͼ�������С*/
/*pSourceImage Դͼ������ָ��*/
/*pDestImage   ���ź�ͼ������ָ��*/
void __stdcall ZLBReSizeImage(ZLBImageStruct *pSourceImage,ZLBImageStruct *pdestImage); 

/*ͼ��ͨ��ת��*/
/*pSourceImage Դͼ������ָ��*/
/*pDestImage   ���ź�ͼ������ָ��*/
/*iConvertMode ת��ģʽ 0-��ʾ��ɫת���ڰ� 1-�ڰ�ת��ɫ*/
void __stdcall ZLBConvertImage(ZLBImageStruct *pSourceImage,ZLBImageStruct *pdestImage,int iConvertMode);

/*��ͼ��ֱ��*/
/*pSourceImage Դͼ������ָ��*/
/*startPoint ���Ƶ���ʼ����*/
/*endPoint ���ƵĽ������� */
/*color����ֱ�ߵ���ɫ  0 - ��ɫ, 1 - ��ɫ, 2 - ��ɫ 3-��ɫ 4-��ɫ*/
/*fLineThickness �߶δ�ϸ*/
void __stdcall ZLBDrawLine(ZLBImageStruct *pSourceImage,ZLBPoint startPoint,ZLBPoint endPoint,int color,int iLineThickness);


/*��AVI�ļ�ͷд��AVI�ļ���*/
/*pAviFileName ����д����ļ���*/
/*iVideoFps    AVI��֡��*/
/*iVideoWidth  AVI����Ƶ���*/
/*iVideoHeight  AVI����Ƶ�߶�*/
/*iVideoType   AVI����Ƶ��ʽ 0-��ѹ�� 1-MJpeg  -1ѡ�������*/
void* __stdcall ZLBCreateAviHeader(const char* pAviFileName,int iVideoFps,int iVideoWidth,int iVideoHeight,int iVideoType);

/*��ͼ������֡д��AVI�ļ���*/
/*pAviHandle   AVI���*/
/*pImageData   ͼ������ָ��*/
/*iImageWidth  ��֡���*/
/*iImageHeight ��֡�߶�*/
/*iImageSize   ��֡ͼ���С*/
/*���ز��� 1-����ɹ� 0-����ʧ��*/
bool __stdcall ZLBAddFrameToAvi(void *pAviHandle,unsigned char *pImageData,int iVideoWidth,int iVideoHeight,int iVideoSize);


/*�ر�AVI�ļ�*/
/*pAviHandle   AVI���*/
bool __stdcall ZLBCloseAvi(void *pAviHandle);

#ifdef __cplusplus
}
#endif

#endif 
