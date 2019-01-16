#pragma once
#include <vector>
using namespace std;

enum DataIdxEnum{
	eData_2 = 0,
	eData_3 ,
	eData_4 ,
	eData_5 ,
	eData_6 ,
	eData_7 ,
	eData_8 ,
	eData_9 ,
	eData_10 ,
	eData_11 ,
	eData_12 ,
	eData_13 ,
	eData_14 ,
	eData_15 ,
	eData_16 ,
	eData_17 ,
	eData_18 ,
	eData_19 ,
	eData_20 ,
	eData_21 ,
	eData_22 ,
	eData_23 ,
	eData_24 ,
	eData_25 
};

struct sLyData
{
	int nNum;
	
	float fUnkown_9;

	WORD wYear;
	BYTE byteMonth;
	BYTE byteDay;
	BYTE byteHour;
	BYTE byteMinute;

	float fValues[24];
};

class CLyData
{
public:
	CLyData(void);
	~CLyData(void);

	bool LoadFile(LPCTSTR);
	bool SaveFile(LPCTSTR);

	sLyData * GetData(){return &m_slyData;}

public:
	static long	DataToValue(long);
	static long	ValueToData(long);

	int		GetDataCnt();
	bool	GetDataByIdx(int, double & );
	bool	SetDataByIdx(int, double);

	CString GetLastErr(){return m_lastErr;}

protected:
	void	Clear();

protected:
	sLyData	m_slyData;
	CString		m_strInputfile;

protected:
	CString		m_lastErr;
};

