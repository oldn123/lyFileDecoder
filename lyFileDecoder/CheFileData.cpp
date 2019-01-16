#include "StdAfx.h"
#include "CheFileData.h"
#include <assert.h>
#include <math.h>

CLyData::CLyData(void)
{
	Clear();
}


CLyData::~CLyData(void)
{
}

bool CLyData::SaveFile(LPCTSTR sFile)
{
	USES_CONVERSION;
	FILE * fp = fopen(T2A(sFile), "wb");
	if (!fp)
	{
		return false;
	}

	do 
	{
		

	} while (false);

	fclose(fp);
	return true;
}

bool CLyData::LoadFile(LPCTSTR sInput)
{
	USES_CONVERSION;
	FILE * fp = fopen(CT2A((LPCTSTR)sInput), "rb");

	if (!fp)
	{
		return false;
	}
	Clear();

	fseek(fp, 0, SEEK_END);
	int nflen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char * pDatas = new char[nflen];
	fread(pDatas, 1, nflen, fp);

	do 
	{
		m_slyData.nNum = *(int*)&pDatas[5];
		m_slyData.fUnkown_9 = *(float*)&pDatas[9];
		m_slyData.wYear = *(WORD*)&pDatas[0x0d];
		m_slyData.byteMonth = *(BYTE*)&pDatas[0x0f];
		m_slyData.byteDay = *(BYTE*)&pDatas[0x10];
		m_slyData.byteHour = *(BYTE*)&pDatas[0x11];
		m_slyData.byteMinute = *(BYTE*)&pDatas[0x12];

		float * pfDatas = (float*)&pDatas[0x13];
		m_slyData.fValues[eData_2] = pfDatas[10];
		m_slyData.fValues[eData_3] = pfDatas[14];
		m_slyData.fValues[eData_4] = pfDatas[15];
		m_slyData.fValues[eData_5] = pfDatas[0];
		m_slyData.fValues[eData_6] = pfDatas[1];
		m_slyData.fValues[eData_7] = pfDatas[4];
		m_slyData.fValues[eData_8] = pfDatas[3];
		m_slyData.fValues[eData_9] = pfDatas[17];
		m_slyData.fValues[eData_10] = pfDatas[18];

		m_slyData.fValues[eData_11] = pfDatas[16];	//int
		m_slyData.fValues[eData_12] = pfDatas[5];
		m_slyData.fValues[eData_13] = pfDatas[2];
		m_slyData.fValues[eData_14] = pfDatas[13];
		m_slyData.fValues[eData_15] = pfDatas[12];
		m_slyData.fValues[eData_16] = pfDatas[23];
		m_slyData.fValues[eData_17] = pfDatas[11];
		m_slyData.fValues[eData_18] = pfDatas[6];
		m_slyData.fValues[eData_19] = pfDatas[8];
		
		m_slyData.fValues[eData_20] = pfDatas[24];
		m_slyData.fValues[eData_21] = pfDatas[25];
		m_slyData.fValues[eData_22] = pfDatas[26];
		m_slyData.fValues[eData_23] = pfDatas[27];
		m_slyData.fValues[eData_24] = pfDatas[28];
		//m_slyData.fValues[eData_25] = pfDatas[12];

	} while (false);

	fclose(fp);
	m_strInputfile = sInput;

	return true;
}


int	CLyData::GetDataCnt()
{
	return 0;
}

bool CLyData::GetDataByIdx(int nIdx, double & fRetData)
{

	return false;
}

bool CLyData::SetDataByIdx(int nIdx, double fVal)
{

	return false;
}

void CLyData::Clear()
{
	
}
