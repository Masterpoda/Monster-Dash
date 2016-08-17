#include "monster.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <stdlib.h>

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * Comparison function for sorting list of next moves
 *
 * @param[in] n1 - The first node for comparison
 * @param[in] n2 - The second node for comparison
 ******************************************************************************/

Monster::Monster(Playfield* p)
{
    playfield = p;
    N = playfield->size();

}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * Comparison function for sorting list of next moves
 *
 * @param[in] n1 - The first node for comparison
 * @param[in] n2 - The second node for comparison
 ******************************************************************************/

Monster::~Monster()
{
    locs.clear();
}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * Comparison function for sorting list of next moves
 *
 * @param[in] n1 - The first node for comparison
 * @param[in] n2 - The second node for comparison
 ******************************************************************************/


bool nodeCmp(Node n1, Node n2)
{
    return (n1.step < n2.step);
}



/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * Move function used to retrieve the next move of the monster. The monster
 * uses an A* search algorith to find the most direct path to the player.
 * using Djikstra's resulted in some odd diagonals being taken. that still
 * technically gave a shortest path. This way the monster wont take paths
 * that give the player a distinct advantage.
 ******************************************************************************/

// YOUR MAIN TASK IS TO RE-IMPLEMENT THIS METHOD TO DO SOMETHING INTELLIGENT
Site* Monster::move()
{
    Site* monster = playfield->getMonsterSite();
    Site* player  = playfield->getPlayerSite();


    TempAStar(monster, player, nullptr);//calls A* pathfinding Algorithm


    locs.clear();

    return Tgoal.loc;
}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * Function to find the path from the player to the monster using recursion,
 * and the A* algorithm in a general form. A* is basically Djikstras with the
 * Distance from the final goal factored in to the distance.
 *
 * @param[in] start - The starting position to find a path from
 * @param[in] end - The goal position to find a path to
 * @param[in] prev - The last node processed
 ******************************************************************************/

void Monster::TempAStar(Site * start, Site * end, Site * prev)
{
    if(prev == nullptr)//setting initial conditions
    {
        TempNodemap.clear();
        Tfound = false;
        Tmoves.clear();
        Tclosed.clear();
        locs.clear();
        Tstart = start;
        Tlen = 0;

        Node Self;
        Self.loc = start;
        Self.step = 0;
        Self.parent = nullptr;

        Tgoal = Self;

        TempNodemap[make_pair(start->i(), start->j())] = Self;

    }

    Tclosed[make_pair(start->i(), start->j())] = true; //setting node as closed

    int step = TempNodemap[make_pair(start->i(), start->j())].step + 1; //finding step value

    if(start->equals(end))//path to end has been found
    {

        Tgoal.step = step; // Manhattan is zero in this case
        Tgoal.loc = start;
        Tgoal.parent = prev;
        TempNodemap[make_pair(start->i(), start->j())] = Goalnode;
        Tfound = true; //prunes some work.

        //built in short circuited backward traversal


        while(Tgoal.parent != nullptr && !Tstart->equals(Tgoal.parent))
        {
            Tlen++;
            //cout<<Tgoal.loc;
            Tgoal = TempNodemap[make_pair(Tgoal.parent->i(),Tgoal.parent->j())];
        }
        //cout<<Tgoal.loc;
        return;
    }

    //finding all possible moves
    for (int i = -1; i < 2; i++)//loop iterates through all i's
    {
        for (int j = -1; j < 2; j++)//loop iterates through all j's
        {
            Site * site = new Site(start->i()+i, start->j()+j);

            locs.push_back(site);


            //if move is legal and unvisited by search
            if (playfield->isLegalMove(start, site) && !Tclosed[make_pair(site->i(), site->j())])
            {
                Node Move;
                Move.step = step + site->manhattanTo(end);
                Move.loc = site;
                Move.parent = start;

                //this will be true when going to the node being tested is cheaper than any
                //previous route to that node


                if(TempNodemap[make_pair(site->i(), site->j())].step > step)
                {


                    TempNodemap[make_pair(site->i(), site->j())] = Move;
                    Tmoves.push_front(Move);//adding the move to the queue
                }

            }
            if(site->equals(end))
                TempAStar(site, end, start);

        }
    }

    //sorting moves by best number of steps
    Tmoves.sort(nodeCmp);

    while(!Tmoves.empty() && !Tfound)//if we havent found the player and there's still moves in the queue
    {
        Site * next = Tmoves.front().loc;//getting move off front of queue
        Tmoves.pop_front();
        TempAStar(next, end, start);//recursive call

    }


    return;
}
