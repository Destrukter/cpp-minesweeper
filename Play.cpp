#include<Highscore.h>
#include<Play.h>
#include<unistd.h>

Play::Play(int setrows, int setcolumns, int setmines, vector<vector<int>>setfieldstate, vector<vector<char>>setfieldvalue, bool demostatus){
    rows = setrows;
    columns = setcolumns;
    mines = setmines;
    fieldstate = setfieldstate;
    fieldvalue = setfieldvalue;
    democounter = 0;
    demo = demostatus;
}

int Play::gameloop(vector <string> &savesteps2, long &seedref){
    int position;
    int score = 0;
    int flag;
    system("clear");
    bool win = 0;
    preparegame(seedref);                                           //calculate fieldvalue and set mines
    for(int y = 0;y!=columns;y++){                           //cover field
        for (int x = 0;x!=rows;x++){
            fieldstate[x][y] = 0;
        }
    }
    time_t start = time(NULL);                              //start timer
    while(true){                                            //start gameloop
        int fieldscovered;
        position = 0;
        do{
            fieldprint();                                   //output field
            if(position==-1){                               //get position until valid
                cout<<"Invalid position try again!"<<endl;
            }
            position = positioninput(&flag, savesteps2);
        }while(position==-1);
        if (position==-2){                                  //game loop ends(surrender)
            win = 0;
            break;
        }
        if(flag==1){
            fieldstate [position/rows][position%columns] = 2;
        }
        else if(flag==-1){
            fieldstate [position/rows][position%columns] = 0;
        }
        else{
            fieldstate [position/rows][position%columns] = 1;          //set position in fieldstate to uncovered
            if(fieldvalue[position/rows][position%columns] == '_'){    //if position value was 0 uncover all surrounding positions
                uncover(position);
            }
            if(fieldvalue[position/rows][position%columns] == 'O'){    //game loop ends(lost)
                win = 0;
                break;
            }
        }
        if(demo){ //wait if demo is active so player can see new field before next input
            sleep(2);
        }
        fieldscovered = 0;
        for(int y = 0;y!=columns;y++){                           //check if all remaining positions covered in fieldstate are mined if so game loop ends(won)
            for (int x = 0;x!=rows;x++){
                if (fieldstate[x][y] != 1){
                    fieldscovered++;
                }
            }
        }
        if(fieldscovered==mines){
            win = 1;
            break;
        }
    }
    time_t end = time(NULL);                        //end timer
    for(int y = 0;y!=columns;y++){                       //calculate score
        for (int x = 0;x!=rows;x++){
            if (fieldstate[x][y] == 1){
                score++;
            }
        }
    }
    if(win){
        score += rows*columns;
    }
    score = score*mines-(end-start);
    if(score<0){
        score = 0;
    }
    for(int y = 0;y!=columns;y++){                       //uncover field
        for (int x = 0;x!=rows;x++){
            fieldstate[x][y] = 1;
        }
    }
    fieldprint();     //output uncovered field
    if(demo){
    cout<<"Input: "<<savesteps2[savesteps2.size()-1]<<endl;
    }
    if(!win){
        cout<<"You have lost!"<<endl;
    }
    if(win){
        cout<<"You have won!"<<endl;
    }
    if(!demo){
        cout<<"Score:"<<score<<endl;
    }
    return score;
}

void Play::preparegame(long &seedref){
    vector<int> mine;
    if(!demo){ //if not demo get new seed
        seedref = time(NULL);
    }
    srand(seedref);
    for(int y = 0;y!=columns;y++){                        //set fieldvalue to X completly for later comparison
        for (int x = 0;x!=rows;x++){
            fieldvalue[x][y] = 'X';
        }
    }
    for(int counter = 0; counter!=mines; counter++){   //set mine positions with time seed
        bool repeat;
        int ms;
        mine.push_back(1);
        repeat = 1;
        while(repeat==1){
            mine[mine.size()-1]=rand() % (columns*rows-1);
            repeat = 0;
            ms = mine.size()-1;
            for(int counter2 = 0; counter2 != ms; counter2++){ //set new position if its already mined
                if(mine[mine.size()-1]==mine[counter2]){
                    repeat = 1;
                }
            }
        }
    }
    for(int counter = 0; counter != mines; counter++) {  //insert minepositions in fieldvalue
        fieldvalue[mine[counter]/rows][mine[counter]%columns]='O';
    }
    for(int x = 0;x!=rows; x++){
        for (int y = 0; y!=columns; y++){

            fieldvalue[x][y] = calcfieldvalue(x, y);
        }
    } //return fieldvalue
}

char Play::calcfieldvalue(int x, int y){
    char value = 0;
    if(fieldvalue[x][y] == 79){                           //If position is mined dont change value
        return 'O';
    }
    if(y+1<columns && y+1>-1 && fieldvalue[x][y+1] == 'O'){   //check all surrounding positions(check if they are still in array) and count the mines
        value++;
    }
    if(x+1<rows && x+1>-1 && fieldvalue[x+1][y] == 'O'){
        value++;
    }
    if(y-1<columns && y-1>-1 && fieldvalue[x][y-1] == 'O'){
        value++;
    }
    if(x-1<rows && x-1>-1 && fieldvalue[x-1][y] == 'O'){
        value++;
    }
    if(y+1<columns && y+1>-1 && x+1<rows && x+1>-1 && fieldvalue[x+1][y+1] == 'O'){
        value++;
    }
    if( y+1<columns && y+1>-1 && x-1<rows && x-1>-1 && fieldvalue[x-1][y+1] == 'O'){
        value++;
    }
    if(y-1<columns && y-1>-1 && x+1<rows && x+1>-1 && fieldvalue[x+1][y-1] == 'O'){
        value++;
    }
    if(y-1<columns && y-1>-1 && x-1<rows && x-1>-1 && fieldvalue[x-1][y-1] == 'O'){
        value++;
    }
    if(value == 0){                             //if no mine was found
        return '_';
    }
    string output = to_string(value);           //convert int to string for return
    return output[0];                           //return first and only value of string(max number of mines around position is 9)
}

void Play::fieldprint(){
    system("clear");
    cout<<"   ";
    for (char i = 'A';i<=columns+64;i += 1){      //output overlay top
        cout<<i;
        cout<<" ";
    }
    cout<<endl;
    for (int x = 0;x!=rows;x++){               //output field
        cout<<x+1;                          //output overlay side
        if(x+1<10){
            cout<<" ";
        }
        for (int y = 0;y!=columns;y++){       //output fieldvalue if position is uncovered in fieldstate
            cout<<" ";
            if (fieldstate[x][y] == 1){
                cout<<fieldvalue[x][y];
            }
            else if(fieldstate[x][y] == 2){ //output flags
                cout<<'B';
            }
            else{                       //output * otherwise
                cout<<"*";
            }
        }
        cout<<endl;                         //new line every after coulmn value reached
    }
}

string Play:: stringtrim(string oldstring){
    string newstring;
    short int size = oldstring.size();
    for(short int c=1; c<size; c++){    //insert old string -1st pos in new string
        newstring.push_back('_');
        newstring[c-1]=oldstring[c];
    }
    return newstring;
}
int Play:: positioninput(int *flagptr, vector <string> &savesteps3){
    string input;
    *flagptr = 0;                   //flagptr = 0 default no flag added
    char x;
    char y;
    if(!demo){ //if not demo get input and safe it
    cout<<"Which field(f.e.A5) do you want to uncover?(# surrender, + add flag, - remove flag)";
    cin>>input;                  //get input
    savesteps3.push_back(input);
    }
    if(demo){//if demo get input out of vector
        if(democounter==savesteps3.size()){
            cout<<"Error"<<endl;
            return -2;
        }
        input = savesteps3[democounter];
        cout <<"Input: "<<savesteps3[democounter]<<"\n";
        democounter++;
    }
    if(input[0]=='#'){                  //surrender
        return -2;
    }
    if(input[0]=='+'){              //flagadd -> flagptr = 1
        *flagptr = 1;
        input = stringtrim(input);
    }
    else if(input[0]=='-'){       //flagremove -> flagptr = -1
        *flagptr = -1;
        input = stringtrim(input);
    }
    y = input[0];                //safe column
    if(2 == input.length()){     //safe and convert row to int-1(single digit)
        x = input[1]-49;
    }
    else if(3 == input.length()){ //safe and convert row to int-1(two digit)
        if(input[1]<48 || input[1]>57 || input[2]<48 || input[2]>57){ //intercept invalid entry
            return -1;
        }
        if(input[1]!=48 && input[2]==48){  //safe and convert row to int-1(two digit, ten digits break)
            x = (input[1]-49)*10+input[2]-39;
        }
        else{
            x = (input[1]-48)*10+input[2]-49; //safe and convert row to int-1(two digit, other)
        }
    }
    else{
        return -1;
    }
    if(y>='a' && y<='z'){               //convert column lowercase to uppercase
        y-=32;
    }
    y -= 65;                            //convert column letter to number
    if(y<0 || y>columns-1 || x<0 || x>rows-1 || (*flagptr == 0 && fieldstate[x][y] == 2) ||(*flagptr == 1 && fieldstate[x][y] != 0) || (*flagptr == -1 && fieldstate[x][y]!= 2)){  //intercept invalid entries
        return -1;
    }
    return x*columns+y;                  //calculate and return one dimensional value of position
}

void Play::uncover(int pos){
    bool loop;                                      //uncover all surrounding poss of one set pos
    if((pos/rows+1)<rows && (pos/rows+1)>-1){                    //check if pos about to be uncovered would be still in the field
        loop = 0;
        if(fieldstate[pos/rows+1][pos%columns] == 0){                     //if pos is still uncovered ->
            loop = 1;
            fieldstate[pos/rows+1][pos%columns] = 1;
        }                                                                 //uncover
        if(fieldvalue[pos/rows+1][pos%columns] == '_' && loop == 1){          //and also has 0(_) value uncover all surrounding poss of that pos aswell
            uncover(pos+columns);
        }
    }
    if((pos/rows-1)<rows && (pos/rows-1)>-1){                    //repeat for all remaining poss
        loop = 0;
        if(fieldstate[pos/rows-1][pos%columns] == 0){
            loop = 1;
            fieldstate[pos/rows-1][pos%columns] = 1;
        }
        if(fieldvalue[pos/rows-1][pos%columns] == '_' && loop == 1){
            uncover(pos-columns);
        }
    }
    if((pos%columns+1)<columns && (pos%columns+1)>-1){
        loop = 0;
        if(fieldstate[pos/rows][pos%columns+1] == 0){
            loop = 1;
            fieldstate[pos/rows][pos%columns+1] = 1;
        }
        if(fieldvalue[pos/rows][pos%columns+1] == '_' && loop == 1){
            uncover(pos+1);
        }
    }
    if((pos%columns-1)<columns && (pos%columns-1)>-1){
        loop = 0;
        if(fieldstate[pos/rows][pos%columns-1] == 0){
            loop = 1;
            fieldstate[pos/rows][pos%columns-1] = 1;
        }
        if(fieldvalue[pos/rows][pos%columns-1] == '_' && loop == 1){
            uncover(pos-1);
        }
    }
    if((pos%columns+1)<columns && (pos%columns+1)>-1 && (pos/rows+1)<rows && (pos/rows+1)>-1 && fieldvalue[pos/rows+1][pos%columns+1] != '_'){
        loop = 0;
        if(fieldstate[pos/rows+1][pos%columns+1] == 0){
            loop = 1;
            fieldstate[pos/rows+1][pos%columns+1] = 1;
        }
        if(fieldvalue[pos/rows+1][pos%columns+1] == '_' && loop == 1){
            uncover(pos+1+columns);
        }
    }
    if((pos%columns+1)<columns && (pos%columns+1)>-1 && (pos/rows-1)<rows && (pos/rows-1)>-1 && fieldvalue[pos/rows-1][pos%columns+1] != '_'){
        loop = 0;
        if(fieldstate[pos/rows-1][pos%columns+1] == 0){
            loop = 1;
            fieldstate[pos/rows-1][pos%columns+1] = 1;
        }
        if(fieldvalue[pos/rows-1][pos%columns+1] == '_' && loop == 1){
            uncover(pos-columns+1);
        }
    }
    if((pos%columns-1)<columns && (pos%columns-1)>-1 && (pos/rows+1)<rows && (pos/rows+1)>-1 && fieldvalue[pos/rows+1][pos%columns-1] != '_'){
        loop = 0;
        if(fieldstate[pos/rows+1][pos%columns-1] == 0){
            loop = 1;
            fieldstate[pos/rows+1][pos%columns-1] = 1;
        }
        if(fieldvalue[pos/rows+1][pos%columns-1] == '_' && loop == 1){
            uncover(pos+columns-1);
        }
    }
    if((pos%columns-1)<columns && (pos%columns-1)>-1 && (pos/rows-1)<rows && (pos/rows-1)>-1 && fieldvalue[pos/rows-1][pos%columns-1] != '_'){
        loop = 0;
        if(fieldstate[pos/rows-1][pos%columns-1] == 0){
            loop = 1;
            fieldstate[pos/rows-1][pos%columns-1] = 1;
        }
        if(fieldvalue[pos/rows-1][pos%columns-1] == '_' && loop == 1){
            uncover(pos-1-columns);
        }
    }
}
