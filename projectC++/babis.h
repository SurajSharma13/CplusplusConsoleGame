#ifndef BABIS_H
#define BABIS_H

#include <string>
#include <iostream>
#include <filesystem>
#include <ncurses.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <climits>
#include "moving.h"

using namespace std;

/*this class implements the algorithm for the movement of Babis*/
class babis : public moving
{
public:
    int check;
    const int COST_UP = 1;
    const int COST_DOWN = 1;
    const int COST_LEFT = 1;
    const int COST_RIGHT = 1;
    const int INF = numeric_limits<int>::max();
    const vector<pair<int, int>> MOVES = {
        make_pair(-1, 0), // Up
        make_pair(1, 0),  // Down
        make_pair(0, -1), // Left
        make_pair(0, 1)   // Right
    };
    vector<vector<int>> computeShortestPath(const vector<vector<char>> &map, const pair<int, int> &start);
    pair<int, int> BabisMv(pair<int, int> currBabPos, pair<int, int> petPos, vector<vector<char>> map);
};

#endif