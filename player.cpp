#include <vector>
#include "player.h"

using namespace std;

void getMnext(Site *player, Site *monster); //function declaration for getting monster's next move

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * A Constructor to initialize the playfield and size N
 *
 * @param[in] Playfield - The initial starting configuration of the Playfield
 ******************************************************************************/

Player::Player(Playfield* p)
{
    playfield = p;
    N = playfield->size();
}


/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * A deconstructor to remove any leftover data created by the new sites
 ******************************************************************************/

Player::~Player()
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


bool nodeCmp(PNode n1, PNode n2)
{
    return (n1.step < n2.step);
}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * Comparison function for sorting list of next moves by farthest distance
 * instead of nearest
 *
 * @param[in] n1 - The first node for comparison
 * @param[in] n2 - The second node for comparison
 ******************************************************************************/

bool nodeCmp2(PNode n1, PNode n2)
{
    return (n1.step > n2.step);
}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * The function used to determine the next move the player should take.
 * The player will take the longest path that never decreases it's distance from
 * the monster. The distance is determined by an A* path finding algorithm in
 * this case. This will not succeed on all paths, but should usually avoid
 * the monster fairly well.
 ******************************************************************************/

Site* Player::move()
{
    // Player strategy is to predict the monster movement based on every
    // possible player movement weighed against the most reasonable monster movement

    Site* player  = playfield->getPlayerSite();
    Site* monster  = playfield->getMonsterSite();


    PNode Pself;//player current location node
    PNode Mself;//monster current location node
    PNode Pnext;//next player move


    Pself.parent = nullptr;
    Pself.step = 0;
    Pself.loc = player;

    Mself.parent = nullptr;
    Mself.step = 0;
    Mself.loc = monster;

    Goalnode = Pself; //this will only be unchanged if monster cant reach player, or player cant escape with any move

    //will set Goalnode to optimal path
    //this way the parent nodes can be traced back to find first move

    Nodemap[make_pair(monster->i(), monster->j())] = Mself;

    int maxlen = 0;
    //get a new goal if you havent hit your old one.


    closed.clear();
    locs.clear();
    moves.clear();

    Nodemap.clear();

    Nodemap[make_pair(player->i(), player->j())] = Pself;


    found = false;

    maxlen = 0;

    getPlayerMove(player, nullptr, maxlen);

    closed.clear();
    moves.clear();
    locs.clear();

    Pnext = Goalnode;

    while(Pnext.parent != nullptr && !player->equals(Pnext.parent))//short circuited backward traversal
    {
        Pnext = Nodemap[make_pair(Pnext.parent->i(),Pnext.parent->j())];
    }

    Nodemap.clear();


    return Pnext.loc;// function will return next move for player to make

}


/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * A function used to determine the next optimal move of the player based on
 * the predicted movements of the monster
 *
 * @param[in] player- The start position of the Player
 * @param[in] monster - The position of the monster
 * @param[in] prev - The last position evaluated. The parent location of the
 *                   current node.
 ******************************************************************************/

void Player::getPlayerMove(Site * start, Site * prev, int &maxlen)
{

    //this will set P2M equal to the length of the path from the monster to the player
    //when the function is called for the first time.
    if(prev == nullptr)
    {
        TempAStar(playfield->getPlayerSite(), playfield->getMonsterSite(), nullptr);
        P2M = Tlen;
    }

    closed[make_pair(start->i(), start->j())] = true; //close the current node

    // how many steps it will take to get to our neighbors
    int step = Nodemap[make_pair(start->i(), start->j())].step + 1;

    if(step > maxlen)//farthest node from monster. Paradise.
    {

        maxlen = step;//set new max length. should be about 14 for A.in

        Goalnode.step = step;
        Goalnode.loc = start;
        Goalnode.parent = prev;

        Nodemap[make_pair(start->i(), start->j())] = Goalnode;

    }



    for (int i = -1; i < 2; i++)//loop iterates through all i's
    {
        for (int j = -1; j < 2; j++)//loop iterates through all j's
        {
            Site * site = new Site(start->i()+i, start->j()+j);

            locs.push_back(site);

            //if move is legal and unvisited by search
            if (playfield->isLegalMove(start, site) && !closed[make_pair(site->i(), site->j())])
            {
                //calling this function will set Tlen equal to the A* path length from the site to
                //the monster. this will be used momentarily.
                TempAStar(site, playfield->getMonsterSite(), nullptr);
                PNode Move;
                Move.step = step;
                Move.loc = site;
                Move.parent = start;

                //dont add any paths that result in getting closer to the monster,
                //in terms of A* path length.
                if(Nodemap[make_pair(site->i(), site->j())].step > step && !(Tlen < P2M))
                {
                    Nodemap[make_pair(site->i(), site->j())] = Move;
                    moves.push_front(Move);//adding the move to the queue
                }
                if(Tlen < P2M)
                {
                    //close paths that you dont want, otherwise they wont get
                    //closed in the recursive call
                    closed[make_pair(site->i(), site->j())] = true;
                }

            }

        }
    }


    //sorting moves by best number of steps
    moves.sort(nodeCmp);


    while(!moves.empty())//if we havent found the farthest move yet
    {
        Site * next = moves.front().loc;
        moves.pop_front();
        getPlayerMove(next, start, maxlen);//recursive call

    }

    return;
    return;


}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * General A* search used to find shortest path between any two points on map
 * Also provides path length by mutating Tlen variable
 *
 * @param[in] player- The start position of the Player
 * @param[in] monster - The position of the monster
 * @param[in] prev - The last position evaluated. The parent location of the
 *                   current node.
 ******************************************************************************/

void Player::TempAStar(Site * start, Site * end, Site * prev)
{
    if(prev == nullptr)
    {
        TempNodemap.clear();
        Tfound = false;
        Tmoves.clear();
        Tclosed.clear();
        locs.clear();
        Tstart = start;
        Tlen = 0;

        PNode Self;
        Self.loc = start;
        Self.step = 0;
        Self.parent = nullptr;

        Tgoal = Self;

        TempNodemap[make_pair(start->i(), start->j())] = Self;

    }

    Tclosed[make_pair(start->i(), start->j())] = true;

    int step = TempNodemap[make_pair(start->i(), start->j())].step + 1;

    if(start->equals(end))//path to end has been found
    {

        Tgoal.step = step;
        Tgoal.loc = start;
        Tgoal.parent = prev;
        TempNodemap[make_pair(start->i(), start->j())] = Goalnode;
        Tfound = true; //prunes some work.

        //built in short circuited backward traversal

        while(Tgoal.parent != nullptr && !Tstart->equals(Tgoal.parent))
        {
            Tlen++;
            Tgoal = TempNodemap[make_pair(Tgoal.parent->i(),Tgoal.parent->j())];
        }
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
                PNode Move;
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

    while(!Tmoves.empty() && !Tfound && Tmoves.size()<300)//if we havent found the player and there's still moves in the queue
    {
        Site * next = Tmoves.front().loc;//getting move off front of queue
        Tmoves.pop_front();
        TempAStar(next, end, start);//recursive call

    }


    return;
}
