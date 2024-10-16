// Brij Malhotra
// hw5.cpp
// Purpose: Maze navigation using Stacks and its functions

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

// Struct for the room

struct Room {
    string direction;
    int x;
    int y;
};

// Class declaration and function implementation

class Stack {
    
    private:
        struct Room * stackArray;
        int stackSize;
        int top;
        void resize(){
            struct Room * newArray;
            int newSize = 2*stackSize;

            newArray = new struct Room[newSize];
            
            for (int i = 0; i < stackSize; i++){
                newArray[i] = stackArray[i];
            }

            delete [] stackArray;
            stackArray = newArray;
            stackSize = newSize;
        }

    public: 
        
        Stack(){
            int size = 10;
            stackArray = new struct Room[size];
            stackSize = size;
            top = -1;
        }

        ~Stack(){
            delete [] stackArray;
        }

        void push(struct Room room){
            
            if (isFull()){
                cout << "The stack is full.\n";
                resize();
            } else {
                top++;
                stackArray[top] =  room;
            }
        }

        struct Room pop(){
            struct Room room;

            if (isEmpty()){
                throw("The stack is empty.\n");
            } else {
                room = stackArray[top];
                top--;
            }

            return room;
        }

        bool isFull() const {
            bool status;

            if (top == stackSize - 1){
                status = true;
            } else {
                status = false;
            }

            return status;
        } 

        bool isEmpty() const {
            bool status;

            if (top == -1){
                status = true;
            } else {
                status = false;
            }

            return status;
        }
};

// File reading function, the first two getlines get the rows and columns
// It then creates a 2D Array based on the parsed directions
// I used stringstream to read the whole line rather than figuring out a way
// to go through different sizes of files 

struct Room ** fileRead( char *argv[]){
    ifstream infile;
    infile.open(argv[1]);
    
    string s1;
    string s2; 
    int rows;
    int columns; 

    getline(infile, s1, ' ');
    getline(infile, s2, '\n');

    rows = stoi(s1);
    columns = stoi(s2);

    struct Room ** mazeRoom = new struct Room * [rows];
    for(int i = 0; i < rows; i++){
        mazeRoom[i] = new struct Room[columns];
    }
    
    for (int j = 0; !infile.eof(); j++){
        string line;
        getline(infile, line, '\n');

        stringstream s (line);
        for(int k = 0; !s.eof(); k++)
        {
            string parsedDirection;
            getline(s, parsedDirection, ' ');

            mazeRoom[j][k].direction = parsedDirection;
            mazeRoom[j][k].x = j;
            mazeRoom[j][k].y = k;
        }
    }

    return mazeRoom;
}

// Maze navigation function, I set the current indexes to 0,0
// The for loop basically reads through the lines and wherever there are 
// combinations of multiple rooms you can enter, based on that it pushes 
// and pops, I did that so that I would not have to write down several combinations
// and have to do it manually. Function returns true and false appropriately

bool mazeNav(struct Room ** mazeRoom){
    Stack * nav = new Stack();

    int currentIndexRow = 0;
    int currentIndexColumn = 0;
    nav->push(mazeRoom[currentIndexRow][currentIndexColumn]);

    while (!nav->isEmpty()){
        struct Room result = nav->pop();
        string str = result.direction;
        currentIndexRow = result.x;
        currentIndexColumn = result.y;
        
        for(int i = 0; i < (int) str.length(); i++){
            if (result.direction[i] == 'Z'){
                return true;
            } else if (result.direction[i] == 'N') {
                nav->push(mazeRoom[currentIndexRow-1][currentIndexColumn]);
            } else if (result.direction[i] == 'S') {
                nav->push(mazeRoom[currentIndexRow+1][currentIndexColumn]);
            } else if (result.direction[i] == 'E') {
                nav->push(mazeRoom[currentIndexRow][currentIndexColumn+1]);
            } else if (result.direction[i] == 'W') {
                nav->push(mazeRoom[currentIndexRow][currentIndexColumn-1]);
            }
        }
    }

    return false;
}

// Main simply returns 0 if we get the exit via the mazeNav functions
// else it returns 1 and program ends

int main(int argc, char * argv[]){
    
    cout << endl;
    if (mazeNav(fileRead(argv))){
        cout << "The navigation was successful and the exit was reached.";
        return 0;
    } else {
        return 1;
    }

    cout << endl << endl;
}