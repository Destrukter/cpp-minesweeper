#include"Highscore.h"
#include"Play.h"


void menu(){
    int rows = 10;
    int columns = 10;
    int mines = 15;
    vector<Highscore> highscores;

    string line;
    Highscore OldPlayer;
    ifstream myfileread("/home/stud/Minesweeper Highscores");
    int i = 0;
    if (myfileread.is_open()){                  //read safed highscores in file and safe them at beginning of the program
        while(getline(myfileread, line)){
            if(i%2 == 0){
                int linescore;
                linetoint(line, linescore);
                OldPlayer.score = linescore;
            }
            if(i%2 == 1){
                OldPlayer.name = line;
                highscores.push_back(OldPlayer);
            }
            i++;
        }
    }

    while(true){
        string input;
        vector<vector<int>>fieldstate(rows , vector<int> (columns, 0));
        vector<vector<char>>fieldvalue(rows , vector<char> (columns, 0));
        system("clear");
        cout<<"Welcome to Minesweeper"<<endl<<" (1)Tutorial"<<endl<<" (2)Play"<<endl<<" (3)Demoplay"<<endl<<" (4)Highscores"<<endl<<" (5)Settings"<<endl<<" (6)Leave"<<endl;
        cin>>input;                             //what to do
        if(input.length() > 1){                 //invalid entries
            continue;
        }
        else if("1"==input){                    //tutorial
            tutorial();
        }
        else if("2"==input){//Play
            vector <string> savesteps1; //safes steps in case new highscore will be reached
            long seed; //safes seed ""
            string name;
            int score;
            Play current(rows, columns, mines, fieldstate, fieldvalue, 0);                                        //start game, create object
            score = current.gameloop(savesteps1, seed);
            cout<<"Please insert name: "<<endl;
            cin>>name;                                  //safe player name and score
            Highscore Player;                           //create struct Highscores and safe in vector
            Player.name = name;
            Player.score = score;
            highscores.push_back(Player);
            int length = highscores.size()-1;
            for (int i = 1; i<=length; i++){            //sort highscores bubble sort
                for(int j = length; j>=i; j--){
                    if(highscores[j-1].score<highscores[j].score){
                        swap(highscores[j-1], highscores[j]);
                    }
                }
            }
            if (Player.name == highscores[0].name && Player.score == highscores[0].score){//if new highscore create new demoplay
                length = savesteps1.size()-1;
                ofstream myfilewrite("/home/stud/Demo");
                if(myfilewrite.is_open()){
                    myfilewrite <<seed<<"\n"<<mines<<"\n"<<columns<<"\n"<<rows<<"\n";
                    for(int i = 0;i<=length; i++){
                        myfilewrite<<savesteps1[i]<<"\n";
                    }
                }
            }
        }
        else if("3"==input){//Demoplay
            if(highscores.size()!=0){//check if there was a game played yet
                demoplay();
            }
            else{
                cout<<"There is no game played yet. Press any key to continue.";
                string wait;
                cin>>wait;
            }
        }
        else if("4"==input){//highscores
            printhighscores(highscores);
        }
        else if("5"==input){
            columns = fieldsizesettings(columns, "columns");
            rows = fieldsizesettings(rows, "rows");
            mines = minesettings(columns, rows, mines);
        }
        else if("6"==input){//end program
            system("clear");
            int length = highscores.size()-1;
            for (int i = 1; i<=length; i++){            //sort highscores bubble sort
                for(int j = length; j>=i; j--){
                    if(highscores[j-1].score<highscores[j].score){
                        swap(highscores[j-1], highscores[j]);
                    }
                }
            }
            ofstream myfilewrite("/home/stud/Minesweeper Highscores");
            for(int i = 0; i<10 && i<=length; i++){     //safe best 10 highscores or end if end of vector is reached
                if (myfilewrite.is_open()){
                    myfilewrite <<highscores[i].score<<"\n"<<highscores[i].name<<"\n";
                }
            }
            cout<<"Bye bye, until next time!"<<endl;
            break;
        }
        else{                                   //invalid entries
            continue;
        }
    }
}

void linetoint(string linetemp, int &result){
    int length = linetemp.length();
    int temp;
    result = 0;
    for(int i = 0; i<length; i++){
        int digit = 1;
        for(int c = length-1-i; c>0; c--){
            digit *= 10;
        }
        temp = (linetemp[i]-48)*digit;
        result += temp;
    }
}

void linetolong(string linetemp, long &result){
    int length = linetemp.length();
    int temp;
    result = 0;
    for(int i = 0; i<length; i++){
        int digit = 1;
        for(int c = length-1-i; c>0; c--){
            digit *= 10;
        }
        temp = (linetemp[i]-48)*digit;
        result += temp;
    }
}

void tutorial(){
    char dummy;
    system("clear");
    cout<<"You need to uncover every field, except the ones with mines underneath."<<endl;
    cout<<"A '_' means there is no mine located under the 9 surrounding fields."<<endl;
    cout<<"If there are mines located under the surrounding fields, there will be number shown."<<endl;
    cout<<"The value of this number tells you how many mines there are. Good Luck!"<<endl;
    cout<<"Press any key + Enter to continue.";
    cin>>dummy;
}

int minesettings(const int c, const int r, int m){
    string setmines;
    while(true){
        system("clear");
        cout<<"Set number mines max. "<<r*c-1<<", min. 1, default 15, current "<<m<<endl;
        cin>>setmines;
        if(setmines.length()==3){                                                       //for three digits
            if(setmines[0]<48 || setmines[0]>57 || setmines[1]<48 || setmines[1]>57 || setmines[2]<48 || setmines[2]>57){ //intercept invalid entries(<max||>min)
                continue;
            }
            else{
                m=(setmines[2]-48)+(setmines[1]-48)*10+(setmines[0]-48)*100;                //safe value
            }
        }
        else if(setmines.length()==2){                                                  //for two digits
            if(setmines[0]<48 || setmines[0]>57 || setmines[1]<48 || setmines[1]>57){       //intercept invalid entries(<max||>min)
                continue;
            }
            else{
                m=(setmines[1]-48)+(setmines[0]-48)*10;                                     //safe value
            }
        }
        else if(setmines.length()==1){                                                  //for one digit
            if(setmines[0]<48 || setmines[0]>57){                                           //intercept invalid entries(<max||>min)
                continue;
            }
            else{
                m=setmines[0]-48;                                                           //safe value
            }
        }
        else{
            continue;                                                                   //intercept invalid entries(more than three digits)
        }
        if(r*c==1 && m==0){
            return 0;
        }
        else if(m>=r*c || m == 0){                                                           //intercept invalid entries(value = 0)
            continue;
        }
        else{
            return m;                                                                   //return value
        }
    }
}

int fieldsizesettings(int current, string what){
    string entry;
    while(true){
        system("clear");
        cout<<"Set currenths"<<what<<" max.26, min. 1, default 10, current "<<current<<endl;
        cin>>entry;
        if(entry.length()==2){                                              //for two digits
            if(entry[0]>57 || entry[0]<48 || entry[1]>57 || entry[1]<48){       //intercept invalid entries(<max||>min)
                continue;
            }
            else{
                current = (entry[0]-48)*10+(entry[1]-48);                       //safe value
            }
        }
        else if(entry.length()==1){                                         //for one digit
            if(entry[0]<48 || entry[0]>57){                                     //intercept invalid entries(<max||>min)
                continue;
            }
            else{
                current = entry[0]-48;                                          //safe value
            }
        }
        else{                                                               //intercept invalid entries(more than two digits)
            continue;
        }
        if(current>26 || current == 0){                                     //intercept invalid entries(value = 0)
            continue;
        }
        else{
            return current;                                                 //return value
        }
    }
}

void printhighscores(vector<Highscore> currenths){
    int length;
    char dummy;
    system("clear");
    cout<<"Highscores"<<endl;
    length = currenths.size()-1;
    for (int i = 1; i<=length; i++){            //sort highscores bubble sort
        for(int j = length; j>=i; j--){
            if(currenths[j-1].score<currenths[j].score){
                swap(currenths[j-1], currenths[j]);
            }
        }
    }
    for (int i = 0; i<=length && i<10; i++){
        cout<<i+1<<". "<<currenths[i].score<<" "<<currenths[i].name<<endl;
    }
    cout<<"Press any key + Enter to continue.";
    cin>>dummy;
}

void demoplay(){
    int c = 1;
    string line;
    int columnsdemo = 0;
    int minesdemo = 0;
    int rowsdemo = 0;
    long seed;
    vector <string> demosteps;
    ifstream myfileread("/home/stud/Demo");//get values of demoplay
    if (myfileread.is_open()){
        while(getline(myfileread, line)){
            if(c == 1){
                linetolong(line, seed);
            }
            else if(c == 2){
                linetoint(line, minesdemo);
            }
            else if(c == 3){
                linetoint(line, columnsdemo);
            }
            else if(c == 4){
                linetoint(line, rowsdemo);
            }
            else{
                demosteps.push_back(line);
            }
            c++;
        }
    }
    vector<vector<int>>fieldstatedemo(rowsdemo , vector<int> (columnsdemo, 0));
    vector<vector<char>>fieldvaluedemo(rowsdemo , vector<char> (columnsdemo, 0));
    Play demoplay(rowsdemo, columnsdemo, minesdemo, fieldstatedemo, fieldvaluedemo, 1);//create demoplay
    demoplay.gameloop(demosteps, seed);//start demoplay and give seed for mines and safed steps
    cout<<"Demo finished, press any key to continue.";
    string puffer;
    cin>>puffer;
}
