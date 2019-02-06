#include "HuTable.h"

Feature HuTable::operator[](Tiles idx) const
{
	auto it = table.find(idx);
	if (it == table.end())
		return 0;
	else
		return it->second;
}

HuTable::HuTable()
{
	// 0
	melds.push_back(0);
	// 刻子
	for (size_t i = 0; i < 9; i++)
		melds.push_back(3 << i * 3);
	// 順子
	for (size_t i = 0; i < 7; i++)
		melds.push_back(0111 << i * 3);
	// 眼
	for (size_t i = 0; i < 9; i++)
		allEyes.push_back(2 << i * 3);

	for (size_t i = 0; i < 9; i++)
		allSingle.push_back(1 << i * 3);
}

// 檢查手牌是否包含特定的牌組
// now為手牌 check為要檢查的牌組
// 若手牌有，return true 
bool HuTable::have(Tiles now, Tiles check)
{
	for (int i = 0; i < 9; i++)
		if (((now >> i * 3) & 7) < ((check >> i * 3) & 7))
			return false;
	return true;
}


bool HuTable::canHu(const Tiles& t)
{
	return (preTable[t & 0777777777] & 1);
}

HuTable::~HuTable()
{
}

std::ostream& operator<<(std::ostream& out, const HuTable& huTable)
{
	for (const auto& m : huTable.table)
		out << m.first << " " << m.second << std::endl;
	return out;
}