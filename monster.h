#ifndef __MONSTER_H
#define __MONSTER_H
#include <stdlib.h>
#include <vector>
#include <map>
#include <utility>
#include <list>
#include "site.h"
#include "playfield.h"

using namespace std;

struct Node
{
    Site* parent; //location of the node's parent
    Site * loc;
    int step = 30000000; //distance from player. initially set very high
};

class Monster
{
    Playfield* playfield;
    int N;
    int Tlen;//length of path for general A* search
    int P2M; //distance from player to monster
    void getMoves(Site * start, Site* end, Site* prev);
    void TempAStar(Site * start, Site * end, Site * prev);

    list<Site *> locs; //list of location pointers to be cleaned up after search
    list<Node> Tmoves; //list of moves for general A* search

    map<pair<int, int>, bool> Tclosed;//temporary closed array

    map<pair<int, int>, Node> Nodemap; //map of nodes for monster pathfinding
    map<pair<int, int>, Node> TempNodemap;//temporary node map

    Node Goalnode; // node to aim for, longest path from monster
    Node Tgoal;//node used as goal in general A* search
    Site * Tstart; //beginning of path for Temp A* search

    bool found = false;
    bool Tfound = false;//temporary found variable




public:
    Monster(Playfield* play);
    ~Monster();
    Site* move();
};

#endif
