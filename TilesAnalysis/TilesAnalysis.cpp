#include "TilesAnalysis.h"
using namespace std;

bool operator<(const Tiles_& lhs, const Tiles_& rhs)
{
    for(int i=0;i<4;i++)
    {
        if(lhs.tiles[i]<rhs.tiles[i])
            return true;
        else if(lhs.tiles[i]>rhs.tiles[i])
            return false;
    }
    return false;
}

bool operator<(const Groups& lhs, const Groups& rhs)
{
    Tiles_ l = GroupsToTiles(lhs);
    Tiles_ r = GroupsToTiles(rhs);
    return l<r;
}

bool operator==(const Groups& lhs, const Groups& rhs)
{
    return lhs.code==rhs.code;
}

bool Have(int t, int m)
{
	for (int i = 0; i < 9; i++)
		if (((t >> i * 3) & 7) < ((m >> i * 3) & 7))
			return false;
	return true;
}

bool Have(Tiles_ T, Tiles_ s)
{
    for(int i=0;i<4;i++)
    {
        if(!Have(T.tiles[i],s.tiles[i]))
            return false;
    }
    return true;
}

int Count(Tiles_ T)
{
    int n = 0;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<9;j++)
            n += (T.tiles[i]>>(j*3))&7;
    }
    return n;
}

int RemainCount(Tiles_ All,int m)
{
    return (All.tiles[m/9]>>(m%9*3))&7;
}

Tiles_ Diff(Tiles_ x, Tiles_ y)
{
    Tiles_ ret = x;
    int a,b;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<9;j++)
        {
            a = (ret.tiles[i]>>(3*j))&7;
            b = (y.tiles[i]>>(3*j))&7;
            if(a>=b)
                ret.tiles[i] -= y.tiles[i]&(7<<3*j);
            else 
                ret.tiles[i] -= ret.tiles[i]&(7<<3*j);
        }
    }
    return ret;
}

Tiles_ GroupsToTiles(Groups G)
{
    Tiles_ ret = {0};
    for(int i=0;i<G.code.size();i++)
    {
        int c = G.code[i];
        if(c<138)
            ret.tiles[c/40] += GroupMask[c%40];
        else 
            ret.tiles[(c-180)/9] += 1<<(c-180)%9*3;
    }
    return ret;
}

Tiles_ IntToTiles(int t)
{
    Tiles_ T = {0};
    T.tiles[t/9] |= 1<<(t%9*3);
    return T;
}

string TilesToString(Tiles_ T)
{
    string result = "";
    int x,i,j,k;
    for(i=0;i<3;i++)
    {
        x = T.tiles[i];
        for(j=0;j<9;j++,x>>=3)
            for(k=0;k<(x&7);k++)
                result+=to_string(j+1)+StrSuit[i]+" ";
    }

    x = T.tiles[3];
    for(i=0;i<7;i++,x>>=3)
        for(j=0;j<(x&7);j++)
            result+=StrHonor[i]+" ";
    return result;
}

string GroupsToString(Groups G)
{
    string s = "";
    for(int i=0;i<G.code.size();i++)
    {
        int type = G.code[i]/40;
        int loc = G.code[i]%40;
        if(G.code[i]>=180)
        {
             if(G.code[i]%180<27)
                 s += to_string(G.code[i]%9+1)+StrSuit[(G.code[i]-180)/9]+" | ";
             else
                 s += StrHonor[G.code[i]%9]+" | ";
         }
        else if(loc<9)
        {
            if(type<3)
                s += to_string(loc%9+1)+StrSuit[type]+to_string(loc%9+1)+StrSuit[type]+" | ";
            else
                s += StrHonor[loc%9]+StrHonor[loc%9]+" | ";
        }
        else if(loc<18)
        {
            if(type<3)
                s += to_string(loc%9+1)+StrSuit[type]+to_string(loc%9+1)+StrSuit[type]+to_string(loc%9+1)+StrSuit[type]+" | ";
            else
                s += StrHonor[loc%9]+StrHonor[loc%9]+StrHonor[loc%9]+" | ";
        }
        else if(loc<25)
            s += to_string(loc%9+1)+StrSuit[type]+to_string(loc%9+2)+StrSuit[type]+to_string(loc%9+3)+StrSuit[type]+" | ";
        else if(loc<33)
            s += to_string((loc-25)%9+1)+StrSuit[type]+to_string((loc-25)%9+2)+StrSuit[type]+" | ";
        else 
            s += to_string((loc-33)%9+1)+StrSuit[type]+to_string((loc-33)%9+3)+StrSuit[type]+" | ";
    }
    return s;
}

vector<int> GetComponet(Tiles_ T)
{
    vector<int> ret;
    for(int i=0;i<138;i++)
    {   
        if(Have(T.tiles[i/40],GroupMask[i%40]))
            ret.push_back(i);
    }
    return ret;
}

void FindGroups(Tiles_ T, const vector<int>& Componet, vector<Groups>& result, vector<int> tmp, int start)
{
    bool f = true;
    for(int i=start;i<Componet.size();i++)
    {
        if(Have(T.tiles[Componet[i]/40],GroupMask[Componet[i]%40]))
        {
            f = false;
            Tiles_ t = T;
            t.tiles[Componet[i]/40] -= GroupMask[Componet[i]%40];
            tmp.push_back(Componet[i]);
            FindGroups(t,Componet,result,tmp,i);
            tmp.pop_back();
        }
    }

    if(f)
    {
        for(int i=0;i<34;i++)
        {
			if ((T.tiles[i / 9] >> (i % 9 * 3)) & 7)
            {
				for (int j = 0; j < ((T.tiles[i / 9] >> (i % 9 * 3)) & 7); j++)
					tmp.push_back(180 + i);
            }
        }
        Groups G;
        G.code = tmp;
        result.push_back(G);
    }
}

vector<Groups> CountSteps(vector<Groups>& G)
{
    vector<Groups> ret;

    int min = 100;
    int total,single,pair,group,pre_group,steps;
    for(int i=0;i<G.size();i++)
    {
		single = 0, pair = 0, group = 0, pre_group = 0;
        for(int j=0;j<G[i].code.size();j++)
        {
            if(G[i].code[j]>=180)
                single++;
			else if (G[i].code[j] % 40 < 9)
                pair++;
			else if (G[i].code[j] % 40 < 25)
                group++;
            else
                pre_group++;
        }
		total = group * 3 + pre_group * 2 + pair * 2 + single;
		steps = total / 3 * 2 + 1;
		steps -= group * 2 + pre_group + pair;

		if (group + pre_group + pair > total / 3 + 1)
			steps += group + pre_group + pair - (total / 3 + 1);

			if (pre_group != 0 && pre_group >= (total / 3 + 1 - group) && !pair)
				steps += 1;

        if(steps<min)
            min = steps;

        G[i].steps = steps;
    }

	for (int i = 0; i < G.size(); i++)
    {
		if (G[i].steps <= min + 0)
            ret.push_back(G[i]);
    }

    return ret;
}

vector<Groups> GenerateHuGroups(Groups G, Tiles_ Wall)
{
    vector<int> MS,PS;
    vector<int> Meld,Pre,Pair,Single;
    for(int i=0;i<G.code.size();i++)
    {
        if(G.code[i]>=180)
            Single.push_back(G.code[i]-180);
        else if(G.code[i]%40<9)
            Pair.push_back(G.code[i]);
        else if(G.code[i]%40<18)
            Meld.push_back(G.code[i]);
        else if(G.code[i]%40<25)
            Meld.push_back(G.code[i]);
        else if(G.code[i]%40<33)
            Pre.push_back(G.code[i]);
        else if(G.code[i]%40<40)
            Pre.push_back(G.code[i]);
    }

    int s = Single.size();
    int m = Meld.size();
    int p = Pair.size();
    int pr = Pre.size();
    int total =  m*3+p*2+pr*2+s;

    if((p+pr)>=(total/3-m))
    {
        if(p)
        {
            PS = Pair;
            for(int i=0;i<p;i++)
                MS.push_back(Pair[i]+9);
            for(int i=0;i<pr;i++)
            {
                int c = Pre[i];
                if(c%40<26)
                    MS.push_back(c-7);
                else if(c%40<32)
                {
                    MS.push_back(c-7);
                    MS.push_back(c-8);
                }
                else if(c%40<33)
                    MS.push_back(c-8);
                else
                    MS.push_back(c-15);
            }
        }
        else
        {
            PS = Pair;
            for(int i=0;i<s;i++)
            PS.push_back(Single[i]/9*40+Single[i]%9);
            int ary[27] = {0};
            for(int i=0;i<pr;i++)
            {
                int c = Pre[i];
                if(c%40<33)
                {
                    ary[c/40*9+(c%40-25)]++;
                    ary[c/40*9+(c%40-24)]++;
                }
                else if(c%40<40)
                {
                    ary[c/40*9+(c%40-33)]++;
                    ary[c/40*9+(c%40-31)]++;
                }
            }
            for(int i=0;i<27;i++)
            {
                if(ary[i])
                    PS.push_back(i/9*40+i%9);
            }
            for(int i=0;i<p;i++)
                MS.push_back(Pair[i]+9);
            for(int i=0;i<pr;i++)
            {
                int c = Pre[i];
                if(c%40<26)
                    MS.push_back(c-7);
                else if(c%40<32)
                {
                    MS.push_back(c-7);
                    MS.push_back(c-8);
                }
                else if(c%40<33)
                    MS.push_back(c-8);
                else
                    MS.push_back(c-15);
            }
        }
    }
    else
    {
        PS = Pair;
        for(int i=0;i<s;i++)
            PS.push_back(Single[i]/9*40+Single[i]%9);
        for(int i=0;i<p;i++)
            MS.push_back(Pair[i]+9);
        for(int i=0;i<pr;i++)
        {
            int c = Pre[i];
            if(c%40<26)
                MS.push_back(c-7);
            else if(c%40<32)
            {
                MS.push_back(c-7);
                MS.push_back(c-8);
            }
            else if(c%40<33)
                MS.push_back(c-8);
            else
                MS.push_back(c-15);
        }
        for(int i=0;i<s;i++)
        {
            int c = Single[i];
            MS.push_back(c/9*40+c%9+9);
            if(c<27&&c%9==0)
                MS.push_back(c/9*40+c%9+18);
            else if(c<27&&c%9==1)
            {
                MS.push_back(c/9*40+c%9+17);
                MS.push_back(c/9*40+c%9+18);
            }
            else if(c<27&&c%9==7)
            {
                MS.push_back(c/9*40+c%9+16);
                MS.push_back(c/9*40+c%9+17);
            }
            else if(c<27&&c%9==8)
            {
                MS.push_back(c/9*40+c%9+16);
            }
            else if(c<27)
            {
                MS.push_back(c/9*40+c%9+16);
                MS.push_back(c/9*40+c%9+17);
                MS.push_back(c/9*40+c%9+18);
            }
        }
    }

    CheckMeldSet(MS);

    vector<vector<int>> result;
    vector<int> tmp;
    CombMS(result,tmp,MS,total/3-Meld.size(),0);

    vector<Groups> HuGroups;
    for(int i=0;i<PS.size();i++)
    {
        for(int j=0;j<result.size();j++)
        {
            Groups g;
            g.code = Meld;
            g.code.push_back(PS[i]);
            g.code.insert(g.code.end(),result[j].begin(),result[j].end());
            int n = Count(GroupsToTiles(g));
            if(n==total)
                HuGroups.push_back(g);
        }
    }

    vector<Groups> ret = CheckHuGroups(HuGroups,G,Wall);

    return ret;
}

void CombMS(vector<vector<int>> &result, vector<int> tmp, const vector<int> &Meld, int n, int s)
{
    if(tmp.size()==n)
    {
        result.push_back(tmp);
        tmp.clear();
        return;
    }
    for(int i=s;i<Meld.size();i++)
    {
        tmp.push_back(Meld[i]);
        CombMS(result,tmp,Meld,n,i+1);
        tmp.pop_back();
    }
}

vector<Groups> CheckHuGroups(const vector<Groups>& vG, Groups G, Tiles_ Wall)
{
    vector<Groups> ret;
    Tiles_ t = GroupsToTiles(G);
    for(int i=0;i<vG.size();i++)
    {
        Tiles_ x = GroupsToTiles(vG[i]);
        Tiles_ d = Diff(x,t);
        int n = Count(d);

        if(n<=G.steps&&Have(Wall,d))
        {
            ret.push_back(vG[i]);
            ret.back().Hu_Tiles = TilesToVec(d);
        }
    }
    return ret;
}

void CheckMeldSet(vector<int>& MS)
{
    sort(MS.begin(),MS.end());
    MS.erase(unique(MS.begin(),MS.end()),MS.end());
}

vector<int> TilesToVec(Tiles_ T)
{
    vector<int> ret;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<9;j++)
        {
            if((T.tiles[i]>>(j*3))&7)
                ret.push_back(i*9+j);
        }
    }
    return ret;
}

void TilesAnalysis(ScoreCalc& calc, HuInfo info, Tiles_ Wall)
{
    Tiles_ Hand,Door;
    memcpy(&Hand,&info.hands,sizeof(Tiles_));
    memcpy(&Door,&info.doors,sizeof(Tiles_));

    printf("Door: %s\n",TilesToString(Door).c_str());
    printf("Hand: %s\n\n",TilesToString(Hand).c_str());


    //計算牌型組合與進聽數
    vector<int> componet = GetComponet(Hand);
    vector<Groups> result;
    vector<int> tmp;
    FindGroups(Hand,componet,result,tmp,0);
    vector<Groups> G = CountSteps(result);

    for(int i=0;i<G.size();i++)
        printf("Steps to Ting: %d  -  %s\n",G[i].steps-1,GroupsToString(G[i]).c_str());


    //產生胡牌牌型
    vector<Groups> HuGroups;
    for(int i=0;i<G.size();i++)
    {
        vector<Groups> HG = GenerateHuGroups(G[i],Wall);
        HuGroups.insert(HuGroups.end(),HG.begin(),HG.end());
    }

    
    //刪除重複胡牌牌型
    for(int i=0;i<HuGroups.size();i++)
        sort(HuGroups[i].code.begin(),HuGroups[i].code.end());

    set<Groups> sG(HuGroups.begin(),HuGroups.end());
    HuGroups.assign(sG.begin(),sG.end());


    //計算聽的張數
    vector<HuData> HuDatas;
    map<Tiles_,int> TingDatas;
    for(int i=0;i<HuGroups.size();i++)
    {
        HuData HD;
        HD.HuTiles = GroupsToTiles(HuGroups[i]);
        for(int j=0;j<HuGroups[i].Hu_Tiles.size();j++)
        {
            HD.Hu_Tile = HuGroups[i].Hu_Tiles[j];
            HuDatas.push_back(HD);
            TingDatas[Diff(HD.HuTiles,IntToTiles(HD.Hu_Tile))]+=RemainCount(Diff(Wall,Diff(Diff(HD.HuTiles,IntToTiles(HD.Hu_Tile)),Hand)),HD.Hu_Tile);
        }
    }

    for(int i=0;i<HuDatas.size();i++)
        HuDatas[i].TingCount = TingDatas[Diff(HuDatas[i].HuTiles,IntToTiles(HuDatas[i].Hu_Tile))];


    //計算台數
    vector<string> vTai,vTingCount;
    int Tai_T=0,Tai_TC=0;
    int TingCount_T=0,TingCount_TC=0;
    string s;

    for(int i=0;i<HuDatas.size();i++)
    {
        memcpy(&info.hands,&HuDatas[i].HuTiles,sizeof(Tiles_));
        info.certainTile = HuDatas[i].Hu_Tile;
	    auto[tai, str] = calc.countTai(info);
        s = ScoreCalc::huInfoToString(info)+"\n台: "+to_string(tai)+" 牌型: "+str+" Ting Count: "+to_string(HuDatas[i].TingCount)+"\n";
        if(tai>=Tai_T)
        {
            if(tai>Tai_T)
            {
                vTai.clear();
                Tai_TC = 0;
            }
            if(HuDatas[i].TingCount>Tai_TC)
            {
                vTai.clear();
                vTai.push_back(s);
                Tai_TC = HuDatas[i].TingCount;
            }
            else if(HuDatas[i].TingCount==Tai_TC)
                vTai.push_back(s);
            Tai_T = tai;
        }
        if(HuDatas[i].TingCount>=TingCount_TC)
        {
            if(HuDatas[i].TingCount>TingCount_TC)
            {
                vTingCount.clear();
                TingCount_T = 0;
            }
            if(tai>TingCount_T)
            {
                vTingCount.clear();
                vTingCount.push_back(s);
                TingCount_T = tai;
            }
            else if(tai==TingCount_T)
                vTingCount.push_back(s);
            TingCount_TC = HuDatas[i].TingCount;
        }
    }

    
    printf("\nTai->Ting Count\n");
    for(int i=0;i<vTai.size();i++)
        printf("%s",vTai[i].c_str());

    printf("\nTing Count->Tai\n");
    for(int i=0;i<vTingCount.size();i++)
        printf("%s",vTingCount[i].c_str());
}