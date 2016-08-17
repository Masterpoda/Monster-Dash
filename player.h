#ifndef __PLAYER_H
#define __PLAYER_H
#include <stdlib.h>
#include <vector>
#include <map>
#include <utility>
#include <list>
#include "site.h"
#include "playfield.h"

using namespace std;

/**
 * @brief Structure that represents node Data
 */

struct PNode
{
    Site* parent; //location of the node's parent
    Site * loc;
    int step = 30000000; //distance from goal. initially set very high
};


/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * Class used to handle player pathfinding and decision making
 *
 * @param[in] Playfield - the starting playfield
 * @param[in] argv - a pointer array full of the character strings signifying
 * the files and function calls needed by the user
 *
 * @returns 0 program ran successful.
 * @returns 1 the program failed or there was an error.
 ******************************************************************************/


class Player
{
    Playfield* playfield;
    int N;
    int Tlen;//length of path for general A* search
    int P2M; //distance from player to monster
    void getPlayerMove(Site * start, Site* end, int &maxlen);
    void getMnext(Site *player, Site * monster);

    map<pair<int, int>, bool> closed;
    map<pair<int, int>, bool> Tclosed;//temporary closed array

    map<pair<int, int>, PNode> Nodemap; //map of nodes for monster pathfinding
    map<pair<int, int>, PNode> TempNodemap;//temporary node map

    PNode Goalnode; // node to aim for, longest path from monster
    PNode Tgoal;//node used as goal in general A* search
    Site * Tstart; //beginning of path for Temp A* search

    bool found = false;
    bool Tfound = false;//temporary found variable
    bool Goalreached = true;

    list<PNode> moves; //list of nodes to check ("open set")
    list<PNode> Tmoves;
    list<Site *> locs;

    void TempAStar(Site * start, Site * end, Site * prev);//General implementation A* search

public:
    Player(Playfield* p);
    ~Player();
    Site* move();
};

#endif
