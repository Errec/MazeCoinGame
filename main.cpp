#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <stack>
#include <random>

using namespace std;

struct mapPoint {
  int X;
  int Y;

  bool operator < (const mapPoint &coord) const {
    if (X < coord.X || (X == coord.X && Y < coord.Y)) {
        return true;
     } else {
      return false;
    }
  }
};

struct mazeMap {
  int mazeLength;
  int mazeWidth;
  mapPoint startPoint;
  vector <vector<unsigned char> > groundCluster;
};

struct explorerNotes {
  bool foundCoin;
  bool stacked;
  int timesVisitedThisBlock;
  vector<bool> unexploredEntrances; // 0 0 0 0 -> N S E W
  explorerNotes():foundCoin(false),
                  stacked(false),
                  timesVisitedThisBlock(0),
                  unexploredEntrances(4,false){}
};

void readFile(string, mazeMap &, ifstream &);  //Function prototype for the reading the file function
void findStart(mazeMap &); // function prototype to locate the maze spawn point tagged as 'i'.
void exploreMaze(mazeMap &, map<mapPoint, explorerNotes> &, stack<mapPoint > &);
void lookAround(mapPoint &, mazeMap &, explorerNotes &, map<mapPoint, explorerNotes> &);
void writeSummary(mazeMap &, map<mapPoint, explorerNotes> &, stack<mapPoint > &);

const unsigned char COIN = 'm';
const unsigned char ENTRANCE = 'i';
const unsigned char EXIT = 's';
const unsigned char FREEPATH = '.';
const unsigned char WALL = '#';

int main() {

  mazeMap myMaze;
  ifstream inFile; //Input file
  string strFileName; //File name
  map<mapPoint, explorerNotes> explorerNotebook;
  stack<mapPoint > tracker;

  strFileName = "maze.txt"; // TODO: accept user input cin >>

  readFile(strFileName, myMaze, inFile);
  findStart(myMaze);
  exploreMaze(myMaze, explorerNotebook, tracker);
  writeSummary(myMaze,explorerNotebook, tracker);

  return 0;
}

void readFile(string strFileName, mazeMap &myMaze, ifstream &inFile) {

  vector<unsigned char> tempVector;
  string line;

  inFile.open(strFileName.c_str());
  if (inFile.is_open())
  {
    getline(inFile,line);
    istringstream iss(line);
    iss >> myMaze.mazeLength >> myMaze.mazeWidth;

    while(getline (inFile,line))
    {
      for(int i = 0; i < line.length() - 1; ++i)
      {
        tempVector.push_back(line[i]);
      }
      myMaze.groundCluster.push_back(tempVector);
      tempVector.clear();
    }
    inFile.close();
  }
  else cout << "Unable to open file";
}

void findStart(mazeMap &myMaze) {
  for (int x = 0; x < myMaze.mazeLength; x++)
  {
    for (int y = 0; y < myMaze.mazeWidth; y++) {
      if(myMaze.groundCluster[x][y] == ENTRANCE) {
        myMaze.startPoint.X = x;
        myMaze.startPoint.Y = y;
        break;
      }
    }
  }
}

void exploreMaze(mazeMap &myMaze, map<mapPoint, explorerNotes> &explorerNotebook, stack<mapPoint> &tracker) {
  mapPoint currentPoint;
  explorerNotes initialNote;
  bool done;
  int totalUnexploredEntrances;
  map<mapPoint, explorerNotes>::iterator i;
  random_device rd;     // only used once to initialise (seed) engine
  mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  uniform_int_distribution<int> uni(0,3); // guaranteed unbiased
  auto random_integer = uni(rng);

  currentPoint = myMaze.startPoint;
  tracker.push(currentPoint);
  initialNote.stacked = true;
  lookAround(currentPoint,myMaze,initialNote,explorerNotebook);
  explorerNotebook[currentPoint] = initialNote;
  done = false;
                                    int p = 0;
  while(!done) {
                                    p++;
                                    cout << "Passo " << p << ": "<< tracker.top().X + 1 << ","<< tracker.top().Y + 1<< endl;
    i = explorerNotebook.find(currentPoint);
    i->second.timesVisitedThisBlock++;
    totalUnexploredEntrances = i->second.unexploredEntrances[0] +
                               i->second.unexploredEntrances[1] +
                               i->second.unexploredEntrances[2] +
                               i->second.unexploredEntrances[3];

    if((totalUnexploredEntrances == 0 && myMaze.groundCluster[currentPoint.X][currentPoint.Y] == ENTRANCE) ||
                                         myMaze.groundCluster[currentPoint.X][currentPoint.Y] == EXIT) {
      done = true;
    } else {
        if(totalUnexploredEntrances == 0) {
          tracker.pop();
          i->second.stacked = false;
          currentPoint = tracker.top();
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
              explorerNotes newNote;
              lookAround(currentPoint,myMaze,newNote,explorerNotebook);
              explorerNotebook[currentPoint] = newNote;
            } else {
              i->second.stacked = true;
            }
          }
      }
  }
}

void lookAround(mapPoint &currentPoint, mazeMap &myMaze, explorerNotes &currentNote, map<mapPoint, explorerNotes> &explorerNotebook) {
  mapPoint N, S, E, W;
  N.X = currentPoint.X - 1;
  N.Y = currentPoint.Y;
  S.X = currentPoint.X + 1;
  S.Y = currentPoint.Y;
  E.X = currentPoint.X;
  E.Y = currentPoint.Y + 1;
  W.X = currentPoint.X;
  W.Y = currentPoint.Y - 1;

  /*  if(N.X >= 0 &&
       myMaze.groundCluster[N.X][N.Y] != WALL &&
       explorerNotebook.count(N) == 0) {
      currentNote.unexploredEntrances[0] = true;
      if(myMaze.groundCluster[N.X][N.Y] == EXIT) {
        currentNote.exitPoint = N;
        currentNote.foundExit = true;
      }
    }*/

  if (N.X >= 0) {
    if (myMaze.groundCluster[N.X][N.Y] != WALL &&
     explorerNotebook.count(N) == 0) {
     currentNote.unexploredEntrances[0] = true;
    }
  }

  if (S.X < myMaze.mazeLength) {
    if (myMaze.groundCluster[S.X][S.Y] != WALL &&
     explorerNotebook.count(S) == 0) {
     currentNote.unexploredEntrances[1] = true;
    }
  }

  if (E.Y < myMaze.mazeWidth) {
    if (myMaze.groundCluster[E.X][E.Y] != WALL &&
     explorerNotebook.count(E) == 0) {
     currentNote.unexploredEntrances[2] = true;
    }
  }

  if (W.Y >= 0) {
    if (myMaze.groundCluster[W.X][W.Y] != WALL &&
     explorerNotebook.count(W) == 0) {
     currentNote.unexploredEntrances[3] = true;
    }
  }

  if(myMaze.groundCluster[currentPoint.X][currentPoint.Y] == COIN){
    currentNote.foundCoin = true;
  }

  currentNote.stacked = true;
}

void writeSummary(mazeMap &myMaze, map<mapPoint, explorerNotes> & explorerNotebook, stack<mapPoint> &tracker) {
  int stepsToExit = 0;
  int totalSteps = 0;
  int coinsToExit = 0;
  int totalCoins = 0;

  for(auto it = explorerNotebook.cbegin(); it != explorerNotebook.cend(); ++it) {
    totalSteps += it->second.timesVisitedThisBlock;
    totalCoins += it->second.foundCoin;
    if(it->second.stacked == true) {
      stepsToExit += it->second.timesVisitedThisBlock;
      coinsToExit += it->second.foundCoin;
    }
  }

  cout << totalSteps << " passos no total\n";
  cout << totalCoins << " moedas no total\n";
  if(myMaze.groundCluster[tracker.top().X][tracker.top().Y] == ENTRANCE) {
    cout << "saida nao existente" << endl;
  } else {
      cout << stepsToExit << " passos ate a saida\n";
      cout << coinsToExit << " moedas no caminho certo\n";
    }

    cout << myMaze.mazeLength << "," << myMaze.mazeWidth << endl;
}
