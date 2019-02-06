#pragma once
#include "HuTable.h"

class SimplesTable : public HuTable
{
public:
	SimplesTable();
	Feature operator[](Tiles) const;
	std::string toString(const Feature&);

private:
	enum feature
	{
		CAN_HU            = 1,      // bit0: 可胡
		HAS_EYE           = 2,      // bit1: 有眼
		ONLY_HAS_EYE      = 4,      // bit2: 只有眼
		TIN_ONLY_ONE      = 8,      // bit3: 獨聽
		CLEAN             = 16,     // bit4: 門清
		PING_HU           = 32,     // bit5: 平胡
		MIX_SUIT          = 64,     // bit6: 湊一色
		SAME_SUIT         = 128,    // bit7: 清一色
		PON_PAIR          = 256,    // bit8: 碰碰胡
		CONCEALED_PON_CNT = 01000,  // bit9-11: 暗刻數
		EYE_NUMBER        = 0xf000, // bit12-15: 眼睛位置
	};

private:
	void init();
	void setCanHu(int sets, Tiles curr, Tiles all);
	void setOnlyHasEye();
	void setPingHu(int sets, Tiles curr, Tiles c);
	void setSameSuit();
	void setPonPair(int sets, Tiles curr, Tiles c);
	void setConcealedPonCnt();
	void setEyeNumber();
	void setTinOnlyOne();

	void filter();
};

