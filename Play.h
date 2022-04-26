#ifndef PLAY_H
#define PLAY_H
#include <iostream>
#include <ctime>
#include "Field.h"

class Play: Field{
public:
    Play(int setrows, int setcolumns, int setmines, vector<vector<int>>setfieldstate, vector<vector<char>>setfieldvalue, bool demostatus);
    int gameloop(vector <string> &savesteps, long &seedref);
private:
    int rows;
    int columns;
    int mines;
    void preparegame(long &seedref);
    char calcfieldvalue(int x, int y);
    void fieldprint();
    int positioninput(int *flagptr, vector <string> &savesteps3);
    void uncover(int pos);
    string stringtrim(string);
    bool demo;
    vector <string> demosteps;
    unsigned int democounter;
};

#endif // PLAY_H
