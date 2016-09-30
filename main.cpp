#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <stack>
#include <random>
#include <unistd.h>
#include <cstdlib>

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
  bool foundCoin;
  bool inStack;
  int coordCheckins;
  vector<bool> unexploredEntrances; // 0 0 0 0 -> N S E W
  pointIformation():foundCoin(false),
                  inStack(false),
                  coordCheckins(0),
                  unexploredEntrances(4,false){}
};

void readMap(string, maze &);  //Function prototype for the reading the file function
void exploreMaze(maze &, map<coord, pointIformation> &, stack<coord > &);
void lookAround(coord &, maze &, pointIformation &, map<coord, pointIformation> &);
void writeSummary(string, maze &, map<coord, pointIformation> &, stack<coord > &);
void printMaze(maze &, coord &);

const unsigned char COIN = 'm';
const unsigned char START = 'i';
const unsigned char EXIT = 's';
const unsigned char FREEPATH = '.';
const unsigned char WALL = '#';

int main() {
  maze myMaze;
  map<coord, pointIformation> explorerNotebook;
  stack<coord > tracker;
  string strFileName = "maze.txt"; // TODO: accept user input cin >>

  readMap(strFileName, myMaze);
  exploreMaze(myMaze, explorerNotebook, tracker);
  writeSummary(strFileName, myMaze,explorerNotebook, tracker);

  return 0;
}

void readMap(string strFileName,maze &myMaze) {
  ifstream inFile;
  vector<unsigned char> tempVector;
  string line;
  int j;

  inFile.open(strFileName.c_str());
  if (inFile.is_open())
  {
    getline(inFile,line);
    istringstream iss(line);
    iss >> myMaze.mazeHeight >> myMaze.mazeLength;

    j = 0;
    while(getline (inFile,line))
    {
      for(int i = 0; i < myMaze.mazeLength; i++)
      {
        tempVector.push_back(line[i]);
        if (line[i] == START)
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

void exploreMaze(maze &myMaze, map<coord, pointIformation> &explorerNotebook, stack<coord> &tracker) {
  coord currentPoint;
  pointIformation initialNote;
  bool done;
  int totalUnexploredEntrances;
  map<coord, pointIformation>::iterator i;
  random_device rd;     // only used once to initialise (seed) engine
  mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  uniform_int_distribution<int> uni(0,3); // guaranteed unbiased
  auto random_integer = uni(rng);

  currentPoint = myMaze.startPoint;
  tracker.push(currentPoint);
  lookAround(currentPoint,myMaze,initialNote,explorerNotebook);
  explorerNotebook[currentPoint] = initialNote;
  done = false;

  while(!done) {
    printMaze(myMaze,currentPoint);

    i = explorerNotebook.find(currentPoint);
    i->second.coordCheckins++;
    totalUnexploredEntrances = i->second.unexploredEntrances[0] +
                               i->second.unexploredEntrances[1] +
                               i->second.unexploredEntrances[2] +
                               i->second.unexploredEntrances[3];

    if((totalUnexploredEntrances == 0 && myMaze.mazeMap[currentPoint.X][currentPoint.Y] == START) ||
                                         myMaze.mazeMap[currentPoint.X][currentPoint.Y] == EXIT) {
      done = true;
    } else {
        if(totalUnexploredEntrances == 0) {
          tracker.pop();
          currentPoint = tracker.top();
          i->second.inStack = false;
        } else {
            do {
              random_integer = uni(rng);
            } while (i->second.unexploredEntrances[random_integer] == 0);

            switch(random_integer) {
              case 0:
                currentPoint.X =  currentPoint.X - 1;
                i->second.unexploredEntrances[0] = false;
              break;

              case 1:
                currentPoint.X =  currentPoint.X + 1;
                i->second.unexploredEntrances[1] = false;
              break;

              case 2:
                currentPoint.Y =  currentPoint.Y + 1;
                i->second.unexploredEntrances[2] = false;
              break;

              case 3:
                currentPoint.Y =  currentPoint.Y - 1;
                i->second.unexploredEntrances[3] = false;
              break;
            }
            tracker.push(currentPoint);
            if (explorerNotebook.count(currentPoint) == 0) {
              pointIformation newNote;
              lookAround(currentPoint,myMaze,newNote,explorerNotebook);
              explorerNotebook[currentPoint] = newNote;
            } else {
              i->second.inStack = true;
            }
          }
      }
  }
}

void printMaze(maze &myMaze,coord &currentPoint) {
  unsigned int microseconds = 50000;
  unsigned char tempChar;
  const unsigned char EXPLORER = '*';
  system("clear");
  cout << "\n\t\t\t Maze Explorer - Random Search\n\n";
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

void lookAround(coord &currentPoint, maze &myMaze, pointIformation &currentNote, map<coord, pointIformation> &explorerNotebook) {
  coord N, S, E, W;
  N.X = currentPoint.X - 1;
  N.Y = currentPoint.Y;
  S.X = currentPoint.X + 1;
  S.Y = currentPoint.Y;
  E.X = currentPoint.X;
  E.Y = currentPoint.Y + 1;
  W.X = currentPoint.X;
  W.Y = currentPoint.Y - 1;

  if (N.X >= 0) {
    if (myMaze.mazeMap[N.X][N.Y] != WALL &&
     explorerNotebook.count(N) == 0) {
     currentNote.unexploredEntrances[0] = true;
    }
  }

  if (S.X < myMaze.mazeHeight) {
    if (myMaze.mazeMap[S.X][S.Y] != WALL &&
     explorerNotebook.count(S) == 0) {
     currentNote.unexploredEntrances[1] = true;
    }
  }

  if (E.Y < myMaze.mazeLength) {
    if (myMaze.mazeMap[E.X][E.Y] != WALL &&
     explorerNotebook.count(E) == 0) {
     currentNote.unexploredEntrances[2] = true;
    }
  }

  if (W.Y >= 0) {
    if (myMaze.mazeMap[W.X][W.Y] != WALL &&
     explorerNotebook.count(W) == 0) {
     currentNote.unexploredEntrances[3] = true;
    }
  }

  currentNote.inStack = true;
  if(myMaze.mazeMap[currentPoint.X][currentPoint.Y] == COIN){
    currentNote.foundCoin = true;
  }
}

void writeSummary(string strFileName, maze &myMaze, map<coord, pointIformation> & explorerNotebook, stack<coord> &tracker) {
  int stepsToExit = 0;
  int totalSteps = 0;
  int coinsToExit = 0;
  int totalCoins = 0;

  for(auto it = explorerNotebook.cbegin(); it != explorerNotebook.cend(); ++it) {
    totalSteps += it->second.coordCheckins;
    totalCoins += it->second.foundCoin;
    if(it->second.inStack == true) {
      stepsToExit += it->second.coordCheckins;
      coinsToExit += it->second.foundCoin;
    }
  }

  cout << totalSteps << " passos no total\n";
  cout << totalCoins << " moedas no total\n";
  if(myMaze.mazeMap[tracker.top().X][tracker.top().Y] == START) {
    cout << "saida nao existente\n";
  } else {
      cout << stepsToExit << " passos ate a saida\n";
      cout << coinsToExit << " moedas no caminho certo\n\n";
    }

  ofstream outputFile ("output_" + strFileName);
  if (outputFile.is_open()) {
    outputFile << totalSteps << " passos no total\n";
    outputFile << totalCoins << " moedas no total\n";
    if(myMaze.mazeMap[tracker.top().X][tracker.top().Y] == START) {
      outputFile << "saida nao existente\n";
      while (!tracker.empty()) {
        outputFile << tracker.top().X << " " << tracker.top().X << endl;
        tracker.pop();
      }
    } else {
        outputFile << stepsToExit << " passos ate a saida\n";
        outputFile << coinsToExit << " moedas no caminho certo\n";
        while (!tracker.empty()) {
          outputFile << tracker.top().X << " " << tracker.top().Y << endl;
          tracker.pop();
        }
    }
    cout << "output_" << strFileName << " file saved.\n\n";
  } else {
    cout << "Unable to open file";
    }
}
