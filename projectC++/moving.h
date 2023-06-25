#ifndef MOVING_H
#define MOVING_H

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
#include <random>

using namespace std;

/*This class will be used by both the characters when they move to check if the move is valid*/
class moving
{
private:
    int mv;
    pair<int, int> mapSize;
    bool didLukasMove = false;

public:
    moving();
    pair<int, int> DoMove(const int move, const pair<int, int> pos, const vector<vector<char>> map);
    bool collisionDetector(const int move, const pair<int, int> pos, const vector<vector<char>> map);
    bool winCheck(int move, pair<int, int> pos, vector<vector<char>> map);
};

#endif