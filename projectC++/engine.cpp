#include "engine.h"

using namespace std;
/*This atomic boolean is for the thread to terminate*/
atomic<bool> terminateFlag(false);

engine::engine(moving &mov, babis &bab) : move(mov), babi(bab)
{
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
}
/* This function generates a random position in the map*/
pair<int, int> engine::RandomMapPlaceGenerator(int row, int col) const
{
    row--;
    col--;
    srand(time(0));
    int coordinate1 = rand() % row;
    int coordinate2 = rand() % col;
    return make_pair(coordinate1, coordinate2);
}

/*Here the map gets created*/
void engine::CreateMap(const string &userMap)
{
    string line;
    ifstream infile(userMap);
    while (getline(infile, line))
    {
        vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        map.push_back(row);
    }
}

/*In this function we find valid random starting positions for our characters*/
void engine::AddCharacters()
{
    rowSize = map.size();
    colSize = map[0].size();
    pair<int, int> lukasPlace, babisPlace;
    bool found_place_for_babis = false;
    bool found_place_for_lukas = false;
    do
    {
        if (found_place_for_lukas == false)
        {
            lukasPlace = RandomMapPlaceGenerator(rowSize, colSize);
            if (map.at(lukasPlace.first).at(lukasPlace.second) == '.')
            {
                found_place_for_lukas = true;
                map.at(lukasPlace.first).at(lukasPlace.second) = 'L';
            }
        }
        if (found_place_for_babis == false)
        {
            babisPlace = RandomMapPlaceGenerator(rowSize, colSize);
            if (map.at(babisPlace.first).at(babisPlace.second) == '.')
            {
                found_place_for_babis = true;
                map.at(babisPlace.first).at(babisPlace.second) = 'B';
            }
        }

    } while (!found_place_for_babis || !found_place_for_lukas);
}
/*When the game ends if the user decides to restart we remove every character so they can get a new place*/
void engine::RemoveCharacters()
{
    pair<int, int> lukasPlace = getPosition('L');
    pair<int, int> babisPlace = getPosition('B');
    pair<int, int> petPlace = getPosition('P');

    if (lukasPlace.first != 0)
        map[lukasPlace.first][lukasPlace.second] = '.';
    if (babisPlace.first != 0)
        map[babisPlace.first][babisPlace.second] = '.';
    if (petPlace.first != 0)
        map[petPlace.first][petPlace.second] = '.';
}

/*The magical stone that the characters are chasing
but because its magical it changes places every random times*/
void engine::Petradi()
{
    rowSize = map.size();
    colSize = map[0].size();
    pair<int, int> prevPetradiPlace = make_pair(0, 0);
    pair<int, int> petradiPlace;
    pair<int, int> lukasPos;
    bool found_place_for_petradi = false;
    srand(time(nullptr));
    default_random_engine delayGenerator(time(nullptr));
    uniform_int_distribution<int> delayDistribution(1, 2);
    while (!terminateFlag)
    {
        do
        {
            if (found_place_for_petradi == false)
            {
                petradiPlace = RandomMapPlaceGenerator(rowSize, colSize);
                if (map.at(petradiPlace.first).at(petradiPlace.second) == '.')
                {
                    // remove the previous petradi
                    if (prevPetradiPlace != make_pair(0, 0))
                    {
                        map.at(prevPetradiPlace.first).at(prevPetradiPlace.second) = '.';
                    }
                    found_place_for_petradi = true;
                    map.at(petradiPlace.first).at(petradiPlace.second) = 'P';
                    prevPetradiPlace = petradiPlace;
                    ShowMap();
                }
            }
        } while (!found_place_for_petradi);
        found_place_for_petradi = false;

        int delaySeconds = delayDistribution(delayGenerator);
        this_thread::sleep_for(chrono::seconds(delaySeconds));
    }
}

/*This function helps us find the position of any character*/
pair<int, int> engine::getPosition(char character) const
{
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == character)
            {
                return make_pair(i, j);
            }
        }
    }
    return make_pair(0, 0);
}

/*The movement of Lukas which is controled by the user*/
pair<bool, bool> engine::setMove(int c)
{

    bool didMove = true;
    pair<int, int> oldPos = getPosition('L');
    pair<int, int> newPos = move.DoMove(c, oldPos, map);
    bool gameWon = updateMap(newPos.first, newPos.second, 'L');
    if (gameWon || oldPos == newPos)
        didMove = false;

    return make_pair(gameWon, didMove);
}

/*This starts everything*/
void engine::startGame()
{
    int ch;
    do
    {
        gameWon.first = false;
        gameLost = false;
        terminateFlag = false;
        RemoveCharacters();
        AddCharacters();
        thread petradiThread([this]()
                             { Petradi(); });

        int direction;
        while (!gameWon.first && !gameLost)
        {
            direction = getch();
            if (direction == 27) // ESC key
                break;
            gameWon = setMove(direction);

            if (gameWon.second || (char)direction == ' ')
            {
                gameLost = moveBabis();
            }
        }

        if (gameWon.first)
        {
            Teleporting();
            this_thread::sleep_for(chrono::seconds(3));
            YouWon();
            this_thread::sleep_for(chrono::seconds(3));
        }
        else if (gameLost)
        {
            YouLost();
            this_thread::sleep_for(chrono::seconds(2));
        }
        else
        {
            terminateFlag = true;
            quitter();
            this_thread::sleep_for(chrono::seconds(2));
        }

        petradiThread.join();
        key();
        ch = getch();

        while (ch != 82 && ch != 114 && ch != 27)
        {
            ch = getch();
        }
        if (ch == 27) // ESC key
            break;
    } while (ch == 82 || ch == 114);
}

/*The movement of Lukas which is controlled by the computer*/
bool engine::moveBabis()
{
    pair<int, int> currBabisPos = getPosition('B');
    pair<int, int> currPetPos = getPosition('P');
    pair<int, int> newBabPos = babi.BabisMv(currBabisPos, currPetPos, map);
    bool gameLost = updateMap(newBabPos.first, newBabPos.second, 'B');

    return gameLost;
}

/*This functuon changes the positions of the characters every time they move and checks if any character has won*/
bool engine::updateMap(int row, int col, char Char)
{
    pair<int, int> pos = getPosition(Char);
    map[pos.first][pos.second] = '.';
    if (map[row][col] == 'P')
    {

        map[row][col] = Char;

        terminateFlag = true;
        ShowMap();

        return true;
    }
    map[row][col] = Char;
    ShowMap();
    return false;
}

/*This function simply just prints the map every time its modified*/
void engine::ShowMap() const
{
    clear();

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == 'B')

            {

                init_pair(1, COLOR_BLACK, COLOR_RED);
                attron(COLOR_PAIR(1));
                mvprintw(i, j, "%c", map[i][j]);
                attroff(COLOR_PAIR(1));
            }
            else if (map[i][j] == 'L')
            {
                init_pair(2, COLOR_BLACK, COLOR_GREEN);
                attron(COLOR_PAIR(2));
                mvprintw(i, j, "%c", map[i][j]);
                attroff(COLOR_PAIR(2));
            }
            else if (map[i][j] == 'P')
            {
                init_pair(3, COLOR_BLACK, COLOR_MAGENTA);
                attron(COLOR_PAIR(3));
                mvprintw(i, j, "%c", map[i][j]);
                attroff(COLOR_PAIR(3));
            }
            else if (map[i][j] == '.')
            {
                init_pair(4, COLOR_BLACK, COLOR_BLACK);
                attron(COLOR_PAIR(4));
                mvprintw(i, j, "%c", map[i][j]);
                attroff(COLOR_PAIR(4));
            }
            else
            {
                init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
                attron(COLOR_PAIR(5));
                mvprintw(i, j, "%c", map[i][j]);
                attroff(COLOR_PAIR(5));
            }
        }
    }
    refresh();
}
/*Every funtion from down here is just print something on the screen*/
void engine::YouWon() const
{
    clear();
    ifstream infile("youwon.txt");
    string line;
    while (getline(infile, line))
    {
        printw("%s", line.c_str());
        printw("\n");
    }
    refresh();
    infile.close();
}

void engine::Teleporting() const
{
    clear();
    ifstream infile("teleporting.txt");
    string line;
    while (getline(infile, line))
    {
        printw("%s", line.c_str());
        printw("\n");
    }
    refresh();
    infile.close();
}

void engine::YouLost() const
{
    clear();
    ifstream infile("youlost.txt");
    string line;
    while (getline(infile, line))
    {
        printw("%s", line.c_str());
        printw("\n");
    }
    refresh();
    infile.close();
}
void engine::key() const
{
    clear();
    ifstream infile("press.txt");
    string line;
    while (getline(infile, line))
    {
        printw("%s", line.c_str());
        printw("\n");
    }
    refresh();
    infile.close();
}

void engine::quitter() const
{
    clear();
    ifstream infile("quit.txt");
    string line;
    while (getline(infile, line))
    {
        printw("%s", line.c_str());
        printw("\n");
    }
    refresh();
    infile.close();
}

void engine::welcome() const
{
    clear();
    ifstream infile("welcomescreen.txt");
    string line;
    while (getline(infile, line))
    {
        printw("%s", line.c_str());
        printw("\n");
    }
    refresh();
    infile.close();
}
