#pragma once
#include "HuTable.h"
class HonorsTable : public HuTable
{
public:
	HonorsTable();
	std::string featureToString(Feature);
	~HonorsTable();
private:
	enum feature
	{
		CAN_HU = 1,         // bit0: �i�J
		HAS_EYE = 2,        // bit1: ����
		ONLY_HAS_EYE = 4,   // bit2: �u����
		TIN_ONLY_ONE = 8,   // bit3: �Wť
		EAST = 16,          // bit4: �F��
		SOUTH = 32,         // bit5: �n��
		WEST = 64,          // bit6: �護
		NORTH = 128,        // bit7: �_��
		BONUS = 256,        // bit8: 1
		RED = 512,          // bit9: ����
		GREEN = 1024,        // bit10: �C�o
		WHITE = 2048,       // bit11: �ժO
		LITTLE_FOUR_WINDS = 4096, // �p�|��
		BIG_FOUR_WINDS = 8192, // �j�|��
		LITTE_THREE_DRAGONS = 16384, // �p�T��
		BIG_THREE_DRAGONS = 32768, // �j�T��
	};
	
private:
	void init();
	void setCanHu(int sets, int curr, int all);
	void setWindsAndDragons();
	void setTinOnlyOne();
	void filter();

private:
	void test();
};

