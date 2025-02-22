#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "ScoreCalc.h"
#include "Tiles.h"
using namespace std;

#define SuitTilesMax 0444444444
#define HonorTilesMax 04444444

static const int GroupMask[40] = {02,020,0200,02000,020000,0200000,02000000,020000000,0200000000,\
                    03,030,0300,03000,030000,0300000,03000000,030000000,0300000000,\
                    0111,01110,011100,0111000,01110000,011100000,0111000000,\
                    011,0110,01100,011000,0110000,01100000,011000000,0110000000,\
                    0101,01010,010100,0101000,01010000,010100000,0101000000};

static const string StrHonor[7] = {"東","南","西","北","中","發","白"};
static const string StrSuit[3] = {"萬","筒","條"};

//static const string StrHonor[7] = {"Don","Nan","Shi","Bei","Zhong","Fa","By"};
//static const string StrSuit[3] = {"Wan","Ton","Tiao"};

typedef struct
{
    int tiles[4];
}Tiles_;

typedef struct
{
    vector<int> code;
    vector<int> Hu_Tiles;
    int steps;
}Groups;

typedef struct
{
    Tiles_ HuTiles;
    int Hu_Tile;
    int TingCount;
}HuData;

bool operator<(const Tiles_& lhs, const Tiles_& rhs);
bool operator<(const Groups& lhs, const Groups& rhs);
bool operator==(const Groups& lhs, const Groups& rhs);

bool Have(int t, int m);
bool Have(Tiles_ T, Tiles_ s);

int Count(Tiles_ T);
int RemainCount(Tiles_ All,int m);
Tiles_ Diff(Tiles_ x, Tiles_ y);
Tiles_ GroupsToTiles(Groups G);
Tiles_ IntToTiles(int t);

string TilesToString(Tiles_ T);
string GroupsToString(Groups G);

vector<int> GetComponet(Tiles_ T);

void FindGroups(Tiles_ T, const vector<int>& Componet, vector<Groups>& result, vector<int> tmp, int start);

vector<Groups> CountSteps(vector<Groups>& G);

vector<Groups> GenerateHuGroups(Groups G, Tiles_ Wall);
void CombMS(vector<vector<int>> &result, vector<int> tmp, const vector<int> &Meld, int n, int s);
vector<Groups> CheckHuGroups(const vector<Groups>& vG, Groups G, Tiles_ Wall);
void CheckMeldSet(vector<int>& MS);

vector<int> TilesToVec(Tiles_ T);

void TilesAnalysis(ScoreCalc& calc, HuInfo info, Tiles_ Wall);