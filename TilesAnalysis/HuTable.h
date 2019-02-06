#pragma once
#include <iostream>
#include <vector>
#include <map>
#include<string>

typedef unsigned int Tiles;
typedef unsigned short Feature;
#define SIZE 0444444444

class HuTable
{
public:
	friend std::ostream& operator<<(std::ostream&, const HuTable&);
	Feature operator[](Tiles) const;
	~HuTable();
protected:
	HuTable();
	
	std::vector<int> melds, allEyes, allSingle;
	Feature *preTable;
	std::map<Tiles, Feature> table;

	virtual void init() = 0;
	bool have(Tiles now, Tiles check);
	bool canHu(const Tiles&);
	void filter() {}
};

