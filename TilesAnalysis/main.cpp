#include "TilesAnalysis.h"
using namespace std;

int main()
{
    auto &calc = ScoreCalc::getInstance();
	HuInfo info;

    //---------手牌
	info.hands[0] = 01100; // 萬
	info.hands[1] = 01001; // 筒
	info.hands[2] = 01212111; // 條
	info.hands[3] = 0112; // 字

	//---------門前
	info.doors[0] = 0; // 萬
	info.doors[1] = 0; // 筒
	info.doors[2] = 0; // 條
	info.doors[3] = 0; // 字

	//---------門前資訊
	info.isZimo = false; // 是否自己摸進
	info.allChow = false; // 門前是否全為吃牌
	info.allPon = false; // 門前是否全為碰槓牌
	info.noBonus = true; // 是否無花牌

	// 門風圈風 bit4:代表門風和圈風一樣 bit:3-0(北西南東)
	info.winds.all = 0b10010; //南風南

    Tiles_ Wall = {0};
    Wall.tiles[0] = SuitTilesMax-info.hands[0]-info.doors[0];
    Wall.tiles[1] = SuitTilesMax-info.hands[1]-info.doors[1];
    Wall.tiles[2] = SuitTilesMax-info.hands[2]-info.doors[2];
    Wall.tiles[3] = HonorTilesMax-info.hands[3]-info.doors[3];

    TilesAnalysis(calc,info,Wall);

    return 0;
}