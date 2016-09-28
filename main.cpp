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

//  TODO: Simplify vector declarations using template
void readFile(string, mazeMap &, ifstream &);  //Function prototype for the reading the file function
void findStart(mazeMap &); // function prototype to locate the maze spawn point tagged as 'i'.
bool exploreMaze(mazeMap &, map<mapPoint, explorerNotes> &);
void lookAround(mapPoint &, mazeMap &, explorerNotes &, map<mapPoint, explorerNotes> &);

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
  bool foundExit;

  strFileName = "maze.txt"; // TODO: accept user input cin >>

  readFile(strFileName, myMaze, inFile);
  findStart(myMaze);
  foundExit = exploreMaze(myMaze, explorerNotebook);

  if(foundExit) {
    cout << "true" << endl;
    // TODO: print required infos
  } else {
      cout << "false" << endl;
      // TODO: print required infos + maze without exit
  }

// ---------------------- remove this block when done ---------------------------
for (int i = 0; i < myMaze.mazeLength; i++)
{
  for (int j = 0; j < myMaze.mazeWidth; j++)
  {
    cout << myMaze.groundCluster[i][j];
  }
  cout << endl;
}

cout << "startPoint(" << myMaze.startPoint.X << "," << myMaze.startPoint.Y << ")" << endl;
// ---------------------- remove this block when done --------------------------
  return 0;
}

void readFile(string strFileName,mazeMap &myMaze, ifstream &inFile) {

  vector<unsigned char> tempVector;
  string line;

  inFile.open(strFileName.c_str());
  if (inFile.is_open())
  {
    getline(inFile,line);
    istringstream iss(line);
    iss >> myMaze.mazeLength >> myMaze.mazeWidth;
    cout << "mazeLength = " << myMaze.mazeLength << ", mazeWidth = " << myMaze.mazeWidth << endl;
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

bool exploreMaze(mazeMap &myMaze, map<mapPoint, explorerNotes> &explorerNotebook) {
  mapPoint currentPoint;
  explorerNotes currentNote;
  bool done;
  int totalUnexploredEntrances;
  map<mapPoint, explorerNotes>::iterator i;
  stack<mapPoint > tracker;
  random_device rd;     // only used once to initialise (seed) engine
  mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  uniform_int_distribution<int> uni(0,3); // guaranteed unbiased
  auto random_integer = uni(rng);

  currentPoint = myMaze.startPoint;
  tracker.push(currentPoint);
  currentNote.stacked = true;
  lookAround(currentPoint,myMaze,currentNote,explorerNotebook);
  explorerNotebook[currentPoint] = currentNote;
  done = false;

  while(!done) {
    i = explorerNotebook.find(currentPoint);
    i->second.timesVisitedThisBlock++;
    totalUnexploredEntrances = i->second.unexploredEntrances[0] +
                               i->second.unexploredEntrances[1] +
                               i->second.unexploredEntrances[2] +
                               i->second.unexploredEntrances[3];
cout << "unexploredEntrances for P(" << currentPoint.X + 1<< "," << currentPoint.Y + 1<< ") : " << "[" <<
i->second.unexploredEntrances[0] << "," << i->second.unexploredEntrances[1] << ","  << i->second.unexploredEntrances[2] <<
"," << i->second.unexploredEntrances[3] << "]" << endl;
    if((totalUnexploredEntrances == 0 && myMaze.groundCluster[currentPoint.X][currentPoint.Y] == ENTRANCE) ||
                                         myMaze.groundCluster[currentPoint.X][currentPoint.Y] == EXIT) {
                                                                      cout << "saida encontrada ou sem saida" << endl;
      done = true;
    } else {
        if(totalUnexploredEntrances == 0) {
          currentPoint = tracker.top();
          tracker.pop();
          i->second.stacked = false;
        } else {
            do {
              random_integer = uni(rng);
            } while (i->second.unexploredEntrances[random_integer] == 0);
                                                      cout << "choose coord:" << random_integer + 1<< endl;
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
              lookAround(currentPoint,myMaze,currentNote,explorerNotebook);
              explorerNotebook[currentPoint] = currentNote;
                                                          cout<<"Key does not exist"<<endl;
            } else {
              i->second.stacked = true;
                                                                        cout<<"Key exist"<<endl;
            }
          }
      }
  }
  return true;
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

  if(currentPoint.X > 0 &&
     myMaze.groundCluster[N.X][N.Y] != WALL &&
     explorerNotebook.count(N) == 0) {
    currentNote.unexploredEntrances[0] = true;
  } else {
        currentNote.unexploredEntrances[0] = false;
  }

  if(currentPoint.X < myMaze.mazeLength - 1 &&
     myMaze.groundCluster[S.X][S.Y] != WALL &&
     explorerNotebook.count(S) == 0) {
    currentNote.unexploredEntrances[1] = true;
  } else {
        currentNote.unexploredEntrances[1] = false;
  }

  if(currentPoint.Y < myMaze.mazeWidth - 1 &&
     myMaze.groundCluster[E.X][E.Y] != WALL &&
     explorerNotebook.count(E) == 0) {
    currentNote.unexploredEntrances[2] = true;
  } else {
        currentNote.unexploredEntrances[2] = false;
  }

  if(currentPoint.Y > 0 &&
     myMaze.groundCluster[W.X][W.Y] != WALL &&
     explorerNotebook.count(W) == 0) {
    currentNote.unexploredEntrances[3] = true;
  } else {
        currentNote.unexploredEntrances[3] = false;
  }

  if(myMaze.groundCluster[currentPoint.X][currentPoint.Y] == COIN){
    currentNote.foundCoin = true;
  }
  currentNote.stacked = true;
}
