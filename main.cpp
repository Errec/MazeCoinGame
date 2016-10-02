/*
 * Maze Explorer - Random Search mt19937 Engine
 *
 * Tested with G++ using C++ std 2014
 *
 * Created: 22/09/2016
 *
 * Author: Raniro Coelho - errecme@gmail.com
 *
 * https://github.com/Errec/MazeCoinGame
 */

#include <sys/stat.h>
#include <unistd.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct coord {
  int X;
  int Y;

  bool operator < (const coord &coord) const {
    if (X < coord.X || (X == coord.X && Y < coord.Y)) {
        return true;
     } else {
      return false;
    }
  }
};

struct maze {
  int mazeHeight;
  int mazeLength;
  coord startPoint;
  vector <vector<unsigned char> > mazeMap;
};

struct pointIformation {
  int coordCheckins; // number of times the coordinate point is accessed
  bool foundCoin;
  bool inStack; // verify if the coordinate point is currently stacked
  vector<bool> unexploredPath; // (0, 0, 0, 0) -> North South East Weast
  pointIformation():coordCheckins(0),
                    foundCoin(false),
                    inStack(false),
                    unexploredPath(4,false){}
};

void readMap(string, ifstream &, maze &);
void exploreMaze(maze &, map<coord, pointIformation> &, stack<coord > &);
void lookAround(coord &, maze &, pointIformation &, map<coord, pointIformation> &);
void writeSummary(string, maze &, map<coord, pointIformation> &, stack<coord > &);
void printMaze(maze &, coord &);
inline stack<coord> reverseStack(stack<coord> &);

/*
 * Specific characters of the maze file components
 */
const unsigned char COIN = 'm';
const unsigned char START = 'i';
const unsigned char EXIT = 's';
const unsigned char FREEPATH = '.';
const unsigned char WALL = '#';

int main() {
  string strFileName;
  ifstream inFile;
  maze myMaze; // contains the maze character map, the start coordinate and the size HxL
  map<coord, pointIformation> explorersDiary; // store the accessed coordinates and its current information
  stack<coord> myTracker; // store the coordinates for the right path
  struct stat buf;

  system("clear");
  cout << "\n\t\t\tMaze Explorer - Random Search mt19937 Engine\n\n ";
  cout << "Enter Maze File Name: ";
  cin >> strFileName; //User enters the file name
  /*
   * While the file does not open display error message and asks for another file name
  */
  while (stat(strFileName.c_str(), &buf) == -1) {
    system("clear");
    cout << "\n\t\t\tMaze Explorer - Random Search mt19937 Engine\n\n";
    cout << " Error: File Not Found.\n ";
    cout << "Enter Maze File Name: ";
    cin >> strFileName;
  }

  readMap(strFileName, inFile, myMaze);
  exploreMaze(myMaze, explorersDiary, myTracker);
  writeSummary(strFileName, myMaze, explorersDiary, myTracker);

  return 0;
}

/* This function opens a .txt file with the specific maze format to store
 * the content in a 2D char vector
 */
void readMap(string strFileName, ifstream &inFile, maze &myMaze) {
  vector<unsigned char> tempVector; // store the chars of the string line
  string line;

  inFile.open(strFileName.c_str());
  if (inFile.is_open())
  {
    //Get the .txt first line and store the int values representing the maze size
    getline(inFile,line);
    istringstream iss(line);
    iss >> myMaze.mazeHeight >> myMaze.mazeLength;

    int j = 0;
    while(getline (inFile,line))
    {
      for(int i = 0; i < myMaze.mazeLength; i++)
      {
        tempVector.push_back(line[i]);
        if (line[i] == START) // find the start point
        {
          myMaze.startPoint.X = i;
          myMaze.startPoint.Y = j;
        }
      }
      myMaze.mazeMap.push_back(tempVector);
      tempVector.clear();
      j++;
    }
    inFile.close();
  }
  else cout << "Unable to open file";
}

/* This function receives the struct with the maze information(start point, size, 2D vector of
 * chars) , map (coordinates as keys and information about the point as value) and a stack of coordinates.
 * The function analyses each position using the map information updating the coordinate according
 * the neighbors coordinate configuration. The call to select the next position is made using random
 * library. The function loops until theres no more free paths to go or the maze exit is found
 */
void exploreMaze(maze &myMaze, map<coord, pointIformation> &explorersDiary, stack<coord> &myTracker) {
  coord currentPoint;
  pointIformation startPointInfo;
  map<coord, pointIformation>::iterator it;

  // Generate the next step random position: http://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
  random_device rd;     // only used once to initialise (seed) engine
  mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  uniform_int_distribution<int> uni(0,3); // guaranteed unbiased
  auto randomInteger = uni(rng);

  // Initialize the stack and the map<> with the point of the start coordinate
  currentPoint = myMaze.startPoint;
  myTracker.push(currentPoint);
  lookAround(currentPoint, myMaze,startPointInfo, explorersDiary);
  explorersDiary[currentPoint] = startPointInfo;

  bool done = false;
  while(!done) {
    printMaze(myMaze,currentPoint);
    it = explorersDiary.find(currentPoint); // used to iterate trough the map elements
    // verify if a position is free to move. E.g.: one free position: unexploredPath == {0,0,1,0}
    int sumUnexplorePaths = it->second.unexploredPath[0] +
                            it->second.unexploredPath[1] +
                            it->second.unexploredPath[2] +
                            it->second.unexploredPath[3];

    if((sumUnexplorePaths == 0 && myMaze.mazeMap[currentPoint.X][currentPoint.Y] == START) ||
                                  myMaze.mazeMap[currentPoint.X][currentPoint.Y] == EXIT) {
      done = true;
    } else {
      // no more positions to go on? Then pop() back
        if(sumUnexplorePaths == 0) {
          myTracker.pop();
          currentPoint = myTracker.top();
          it->second.inStack = false;
          it->second.coordCheckins++;
        } else {
            do {
              randomInteger = uni(rng);
            } while (it->second.unexploredPath[randomInteger] == 0);

            switch(randomInteger) {
              case 0:
                currentPoint.X =  currentPoint.X - 1;
                it->second.unexploredPath[0] = false;
              break;

              case 1:
                currentPoint.X =  currentPoint.X + 1;
                it->second.unexploredPath[1] = false;
              break;

              case 2:
                currentPoint.Y =  currentPoint.Y + 1;
                it->second.unexploredPath[2] = false;
              break;

              case 3:
                currentPoint.Y =  currentPoint.Y - 1;
                it->second.unexploredPath[3] = false;
              break;
            }
            myTracker.push(currentPoint);
            if (explorersDiary.count(currentPoint) == 0) { // check if the map value key already exists: if no, add one
              pointIformation newNote;
              lookAround(currentPoint,myMaze,newNote,explorersDiary);
              explorersDiary[currentPoint] = newNote;
            } else { // else, just update the existing one
              it->second.inStack = true;
              it->second.coordCheckins++;
            }
          }
      }
  }
}

/* This function receives a maze struct and the current coordinate of exploreMaze() function's loop iteration
 * then update the character of that point to '*', representing the explorer. The function clean the current screen for
 * each iteration using system() and prints the updated maze within a interval of time using sleep()
 */
void printMaze(maze &myMaze,coord &currentPoint) {
  unsigned int microseconds = 45000;
  unsigned char tempChar;
  const unsigned char EXPLORER = '*';
  system("clear");
  cout << "\n\t\t\tMaze Explorer - Random Search mt19937 Engine\n\n";
  cout << " Maze Size: "<< myMaze.mazeHeight << "x" << myMaze.mazeLength << endl;
  cout << " i: Start  s: Exit  m: Coin  .:Free path  #: Wall\n\n";
  for (int i = 0; i < myMaze.mazeHeight; i++)
  {
    for (int j = 0; j < myMaze.mazeLength; j++)
    {
      cout << " ";
      if (i == currentPoint.X && j == currentPoint.Y)
      {
        tempChar = myMaze.mazeMap[i][j];
        myMaze.mazeMap[i][j] = EXPLORER;
      }
      cout << myMaze.mazeMap[i][j];
    }
    cout << endl;
  }
  cout << endl;
    usleep(microseconds);
    myMaze.mazeMap[currentPoint.X][currentPoint.Y] = tempChar;
}

/* This function receives and analyses the current point inside the exploreMaze() function's loop iteration then updates
 * the map element for that point: if the neighbor char was not yet visited and its != #, then the direction pointing
 * to that char is updated to true
 */
void lookAround(coord &currentPoint, maze &myMaze, pointIformation &currentNote, map<coord, pointIformation> &explorersDiary) {
  coord N, S, E, W; // North, South, East, West
  N.X = currentPoint.X - 1;
  N.Y = currentPoint.Y;
  S.X = currentPoint.X + 1;
  S.Y = currentPoint.Y;
  E.X = currentPoint.X;
  E.Y = currentPoint.Y + 1;
  W.X = currentPoint.X;
  W.Y = currentPoint.Y - 1;

  if (N.X >= 0 && // check the inferior boundary
      myMaze.mazeMap[N.X][N.Y] != WALL &&
      explorersDiary.count(N) == 0) { // check if this neighbor point was already visited
     currentNote.unexploredPath[0] = true;
    }

  if (S.X < myMaze.mazeHeight && // check the superior boundary
      myMaze.mazeMap[S.X][S.Y] != WALL &&
      explorersDiary.count(S) == 0) {
     currentNote.unexploredPath[1] = true;
    }

  if (E.Y < myMaze.mazeLength &&
      myMaze.mazeMap[E.X][E.Y] != WALL &&
      explorersDiary.count(E) == 0) {
     currentNote.unexploredPath[2] = true;
    }

  if (W.Y >= 0 &&
      myMaze.mazeMap[W.X][W.Y] != WALL &&
      explorersDiary.count(W) == 0) {
     currentNote.unexploredPath[3] = true;
    }

  currentNote.inStack = true;
  currentNote.coordCheckins++;
  if(myMaze.mazeMap[currentPoint.X][currentPoint.Y] == COIN){
    currentNote.foundCoin = true;
  }
}
/* This function prints the maze running results on console and creates a .txt file containing the correct paths
 * coordinates
 */
void writeSummary(string strFileName, maze &myMaze, map<coord, pointIformation> & explorersDiary, stack<coord> &myTracker) {
  int stepsToExit = 0;
  int totalSteps = 0;
  int coinsToExit = 0;
  int totalCoins = 0;

  stepsToExit = myTracker.size();

  for(auto it = explorersDiary.cbegin(); it != explorersDiary.cend(); ++it) {
    totalCoins += it->second.foundCoin;
    totalSteps += it->second.coordCheckins;
    if(it->second.inStack == true) {
      coinsToExit += it->second.foundCoin;
    }
  }

  cout << " "<< strFileName << " Solved!\n\n ";
  cout << totalSteps << " passos no total\n ";
  cout << totalCoins << " moedas no total\n ";
  if(myMaze.mazeMap[myTracker.top().X][myTracker.top().Y] == START) {
    cout << "saida nao existente\n ";
  } else {
      cout << stepsToExit << " passos ate a saida\n ";
      cout << coinsToExit << " moedas no caminho certo\n\n ";
    }

  ofstream outputFile ("output_" + strFileName);
  if (outputFile.is_open()) {
    outputFile << totalSteps << " passos no total\n";
    outputFile << totalCoins << " moedas no total\n";
    if(myMaze.mazeMap[myTracker.top().X][myTracker.top().Y] == START) {
      outputFile << "saida nao existente\n";
      myTracker = reverseStack(myTracker);
      while (!myTracker.empty()) {
        outputFile << myTracker.top().X << " " << myTracker.top().X << endl;
        myTracker.pop();
      }
    } else {
        outputFile << stepsToExit << " passos ate a saida\n";
        outputFile << coinsToExit << " moedas no caminho certo\n";
        myTracker = reverseStack(myTracker);
        while (!myTracker.empty()) {
          outputFile << myTracker.top().X << " " << myTracker.top().Y << endl;
          myTracker.pop();
        }
    }
    cout << "Summary file output_" << strFileName << " saved.\n\n";
  } else {
    cout << "Unable to open file";
    }
  outputFile.close();
}

/*
 * This function receives the paths coordinates stack and reverses it returning the reversed stack
 */
inline stack<coord> reverseStack(stack<coord> &myTracker) {
  stack<coord> tempStack;
  while (!myTracker.empty()) {
    tempStack.push(myTracker.top());
    myTracker.pop();
  }
  return tempStack;
}
