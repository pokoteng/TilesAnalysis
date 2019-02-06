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
		CAN_HU = 1,         // bit0: 可胡
		HAS_EYE = 2,        // bit1: 有眼
		ONLY_HAS_EYE = 4,   // bit2: 只有眼
		TIN_ONLY_ONE = 8,   // bit3: 獨聽
		EAST = 16,          // bit4: 東風
		SOUTH = 32,         // bit5: 南風
		WEST = 64,          // bit6: 西風
		NORTH = 128,        // bit7: 北風
		BONUS = 256,        // bit8: 1
		RED = 512,          // bit9: 紅中
		GREEN = 1024,        // bit10: 青發
		WHITE = 2048,       // bit11: 白板
		LITTLE_FOUR_WINDS = 4096, // 小四喜
		BIG_FOUR_WINDS = 8192, // 大四喜
		LITTE_THREE_DRAGONS = 16384, // 小三元
		BIG_THREE_DRAGONS = 32768, // 大三元
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

