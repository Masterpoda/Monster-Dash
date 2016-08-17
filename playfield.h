#ifndef __PLAYFIELD_H
#define __PLAYFIELD_H
#include <iostream>
#include <string>
#include "site.h"

using namespace std;

class Playfield
{
    bool **Room;        // is v-w a room site?
    bool **Corridor;    // is v-w a corridor site?
    int N;                // dimension of dungeon
    Site* playerSite;
    Site* monsterSite;

public:
    Playfield(char **board, int N);
    int size() {
        return N ;
    }
    bool isCorridor(Site *v);
    bool isRoom(Site *v);
    bool isWall(Site *v);
    bool isLegalMove(Site* v, Site* w);
    void setPlayerSite(Site *v);
    void setMonsterSite(Site *v);
    Site* getPlayerSite() {
        return playerSite ;
    }
    Site* getMonsterSite() {
        return monsterSite ;
    }

    friend ostream& operator<<(ostream &os, Playfield *);
};

#endif
