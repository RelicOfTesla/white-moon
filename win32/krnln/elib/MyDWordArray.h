#pragma once

class CMyDWordArray
{
public:
	CMyDWordArray(void);
	~CMyDWordArray(void);
public:
	void Add(DWORD dwValue);
	int GetDWordCount();
	PDWORD GetPtr();
};
