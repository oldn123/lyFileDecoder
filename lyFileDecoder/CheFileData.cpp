#include "StdAfx.h"
#include "CheFileData.h"
#include <assert.h>
#include <math.h>

CLyData::CLyData(void)
{
	Clear(true);
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
		fwrite(m_slyData.pDataBuffer, 1, m_slyData.nDataLen, fp);
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
		 if(*(BYTE*)&pDatas[0] != 0x5A) 
		 {
			 break;
		 }

		 if (*(BYTE*)&pDatas[1] == 0xA5)
		 {
			 m_slyData.nNumber = (int*)&pDatas[5];
			 m_slyData.fValues[eData_2] = (float*)&pDatas[9];

			 int nTimePos = 0x0d;
			 m_slyData.wYear = (WORD*)&pDatas[nTimePos];
			 m_slyData.byteMonth = (BYTE*)&pDatas[nTimePos + 2];
			 m_slyData.byteDay = (BYTE*)&pDatas[nTimePos + 3];
			 m_slyData.byteHour = (BYTE*)&pDatas[nTimePos + 4];
			 m_slyData.byteMinute = (BYTE*)&pDatas[nTimePos + 5];

			 float * pfDatas = (float*)&pDatas[0x13];
			 m_slyData.fValues[eData_3] = &pfDatas[14];
			 m_slyData.fValues[eData_4] = &pfDatas[15];
			 m_slyData.fValues[eData_5] = &pfDatas[0];
			 m_slyData.fValues[eData_6] = &pfDatas[1];
			 m_slyData.fValues[eData_7] = &pfDatas[4];
			 m_slyData.fValues[eData_8] = &pfDatas[3];
			 m_slyData.fValues[eData_9] = &pfDatas[17];
			 m_slyData.fValues[eData_10] = &pfDatas[18];

			 m_slyData.fValues[eData_11] = &pfDatas[16];	//int*
			 m_slyData.fValues[eData_12] = &pfDatas[5];
			 m_slyData.fValues[eData_13] = &pfDatas[2];
			 m_slyData.fValues[eData_14] = &pfDatas[13];
			 m_slyData.fValues[eData_15] = &pfDatas[12];
			 m_slyData.fValues[eData_16] = &pfDatas[23];
			 m_slyData.fValues[eData_17] = &pfDatas[11];
			 m_slyData.fValues[eData_18] = &pfDatas[6];
			 m_slyData.fValues[eData_19] = &pfDatas[8];

			 int nPos = (DWORD)&pfDatas[24] - (DWORD)&pDatas[0];
			 m_slyData.fValues[eData_20] = (float*)&pDatas[nPos];
			 m_slyData.fValues[eData_21] = (float*)&pDatas[nPos + 4];
			 m_slyData.fValues[eData_22] = (float*)&pDatas[nPos + 8];
			 m_slyData.fValues[eData_23] = (float*)&pDatas[nPos + 0x0c];
			 m_slyData.fValues[eData_24] = (float*)&pDatas[nPos + 0x10];

			 //NOx=1.53 x NO + NO2 
			 m_slyData.fNox = 1.53 * *(m_slyData.fValues[eData_22]) + *(m_slyData.fValues[eData_23]);	
		 }
		 else if (*(BYTE*)&pDatas[1] == 0x01)
		 {//6По
			 int nTimePos = 0x02;
			 m_slyData.wYear = (WORD*)&pDatas[nTimePos];
			 m_slyData.byteMonth = (BYTE*)&pDatas[nTimePos + 2];
			 m_slyData.byteDay = (BYTE*)&pDatas[nTimePos + 3];
			 m_slyData.byteHour = (BYTE*)&pDatas[nTimePos + 4];
			 m_slyData.byteMinute = (BYTE*)&pDatas[nTimePos + 5];

			 assert(*(BYTE*)&pDatas[8] == 0x5A);
			 m_slyData.byteHour2 = (BYTE*)&pDatas[9];
			 m_slyData.byteMinute2 = (BYTE*)&pDatas[0x0A];

			 int nPos = 0x50;
			 m_slyData.fValues[eData_20] = (float*)&pDatas[nPos];
			 m_slyData.fValues[eData_21] = (float*)&pDatas[nPos + 4];
			 m_slyData.fValues[eData_22] = (float*)&pDatas[nPos + 8];
			 m_slyData.fValues[eData_23] = (float*)&pDatas[nPos + 0x0c];
			 m_slyData.fValues[eData_24] = (float*)&pDatas[nPos + 0x10];

			 //NOx=1.53 x NO + NO2 
			 m_slyData.fNox = 1.53 * *(m_slyData.fValues[eData_22]) + *(m_slyData.fValues[eData_23]);	
		 }
		 else 
		 {
			 assert(false);
			 break;
		 }
		
		
		m_slyData.pDataBuffer = pDatas;
		m_slyData.nDataLen	= nflen;

	} while (false);

	fclose(fp);

	if (m_slyData.pDataBuffer)
	{
		m_strInputfile = sInput;
		return true;
	}
	else
	{
		delete [] pDatas;
	}
	return false;
}

bool CLyData::GetDataByIdx(DataIdxEnum nIdx, int & val)
{
	val = *(int*)&m_slyData.fValues[nIdx];
	return true;
}

bool CLyData::SetDataByIdx(DataIdxEnum nIdx, int val)
{
	*m_slyData.fValues[nIdx] = *(float*)&val;
	return true;
}

bool CLyData::GetDataByIdx(DataIdxEnum nIdx, float & fRetData)
{
	fRetData = *m_slyData.fValues[nIdx];
	return true;
}

bool CLyData::SetDataByIdx(DataIdxEnum nIdx, float fVal)
{
	*m_slyData.fValues[nIdx] = fVal;
	return true;
}

void CLyData::Clear(bool bByInit)
{
	if (bByInit)
	{
		m_slyData.pDataBuffer = NULL;
		m_slyData.nDataLen = 0;
	}
	
	if (m_slyData.pDataBuffer)
	{
		delete [] m_slyData.pDataBuffer;
		m_slyData.pDataBuffer = NULL;
	}
	m_slyData.nDataLen = 0;

	m_slyData.nNumber	= NULL;
	m_slyData.wYear		= NULL;
	m_slyData.byteMonth	= NULL;
	m_slyData.byteDay	= NULL;
	m_slyData.byteHour	= NULL;
	m_slyData.byteMinute= NULL;
	m_slyData.byteHour2	= NULL;
	m_slyData.byteMinute2	= NULL;
	memset(&m_slyData.fValues[0], 0, sizeof(float)*23);
	m_slyData.fNox = 0;
}
