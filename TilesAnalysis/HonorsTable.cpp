#include "HonorsTable.h"
#include <string>
#include <bitset>
HonorsTable::HonorsTable()
{
	init();
	//test();
}

std::string HonorsTable::featureToString(Feature f)
{
	std::string result;
	if (f & CAN_HU)
		result += "可胡";
	if (f & HAS_EYE)
		result += "有眼";
	if (f & ONLY_HAS_EYE)
		result += "只有眼";
	if (f & TIN_ONLY_ONE)
		result += "獨聽";
	if (f & EAST)
		result += "東風";
	if (f & SOUTH)
		result += "南風";
	if (f & WEST)
		result += "西風";
	if (f & NORTH)
		result += "北風";
	if (f & RED)
		result += "紅中";
	if (f & GREEN)
		result += "青發";
	if (f & WHITE)
		result += "白板";
	if (f & LITTLE_FOUR_WINDS)
		result += "小四喜";
	if (f & LITTLE_FOUR_WINDS)
		result += "大四喜";
	if (f & LITTE_THREE_DRAGONS)
		result += "小三元";
	if (f & BIG_THREE_DRAGONS)
		result += "大三元";
	return result;
}

void HonorsTable::init()
{
	preTable = new unsigned short[SIZE];
	setCanHu(5, 0, SIZE);
	setWindsAndDragons();
	setTinOnlyOne();
	filter();
	delete[] preTable;
}

void HonorsTable::setCanHu(int sets, int curr, int all)
{
	if (sets)
	{
		for (int i = 0; i < 8; i++)
			if (have(all, melds[i]))
				setCanHu(sets - 1, curr + melds[i], all - melds[i]);
	}
	else
	{
		preTable[curr] |= (BONUS | CAN_HU);
		//有眼
		for (int i = 0; i < 7; i++)
			if (have(all, allEyes[i]))
				preTable[curr + allEyes[i]] |= (BONUS | HAS_EYE | CAN_HU);
	}
}

void HonorsTable::setWindsAndDragons()
{
	for (Tiles i = 0; i < SIZE; i++)
	{
		const Feature w[7] = { EAST, SOUTH, WEST, NORTH, RED, GREEN, WHITE };
		if (preTable[i] & 1)
		{
			for (size_t j = 0; j < 7; j++) {
				if (have(i, melds[j + 1]))
					preTable[i] |= w[j];
			}

			auto winds = i & 07777;
			auto dragons = i & 07770000;
			auto clearWinds = ~(EAST | SOUTH | WEST | NORTH);
			auto clearDragons = ~(RED | GREEN | WHITE);

			// 小四喜
			if(winds == 03332 || winds == 03323 || winds == 03233 || winds == 02333)
				preTable[i] = (preTable[i] | LITTLE_FOUR_WINDS) & clearWinds;
			// 大四喜
			else if(winds == 03333)
				preTable[i] = (preTable[i] | BIG_FOUR_WINDS) & clearWinds;
			// 小三元
			else if(dragons == 03320000 || dragons == 03230000 || dragons == 02330000)
				preTable[i] = (preTable[i] | LITTE_THREE_DRAGONS) & clearDragons;
			// 大三元
			else if(dragons == 03330000)
				preTable[i] = (preTable[i] | BIG_THREE_DRAGONS) & clearDragons;
		}	
	}
}

void HonorsTable::setTinOnlyOne()
{
	for (Tiles tiles = 1; tiles < SIZE; tiles++)
	{
		if (canHu(tiles))
		{
			for (Tiles j = 0; j < 7; j++)
			{
				if (have(tiles, allSingle[j]))
				{
					Feature huFeature = preTable[tiles], zimoFeature = preTable[tiles];
					Tiles tilesOfTin = tiles - allSingle[j];
					int tinCnt = 0;

					for (int k = 0; k < 9; k++)
					{
						auto temp = tilesOfTin + allSingle[k];
						if (temp > SIZE)
							continue;
						if (canHu(temp))
							tinCnt++;
					}
					// 胡刻子，非自摸暗刻數-1，無獨聽
					if (have(tiles, melds[j + 1]))
						if (canHu(tiles - melds[j + 1]))
						{
							
							tinCnt++;
						}
					// 獨聽
					if (tinCnt == 1)
					{
						huFeature |= TIN_ONLY_ONE;
						zimoFeature |= TIN_ONLY_ONE;
					}
					
					//if(temp != 0)
					table[((j + 1) << 27) | tiles] = huFeature;
					table[(1 << 31) | ((j + 1) << 27) | tiles] = zimoFeature;
				}
			}
		}
	}
}

void HonorsTable::filter()
{
	for (int i = 0; i < SIZE; i++)
		if (canHu(i))
			table[i] = preTable[i];
}


void HonorsTable::test()
{
	for (auto[k, v] : table) {
		std::string str;
		for (size_t i = 0; i < 9; i++) {
			int cnt = (k >> i * 3) & 0b111;
			for (int j = 0; j < cnt; j++)
				str += std::to_string(i + 1);
		}
		std::cout << str;
		std::cout << " " << featureToString(v) << std::endl;
	}
}

HonorsTable::~HonorsTable()
{
}
