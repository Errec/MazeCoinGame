#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <stack>

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
  explorerNotes():stacked(false),timesVisitedThisBlock(0),unexploredEntrances(4,false){}
};

//  TODO: Simplify vector declarations using template
void readFile(string, mazeMap &, ifstream &);  //Function prototype for the reading the file function
void findStart(mazeMap &); // function prototype to locate the maze spawn point tagged as 'i'.
bool exploreMaze(mazeMap &, map<mapPoint, explorerNotes> &);
void lookAround(mapPoint &, mazeMap &, explorerNotes &);
void stepIn(mapPoint &, map<mapPoint, explorerNotes> &, stack<mapPoint > &);
//mapPoint nextStep(mazeMap &, map<mapPoint, explorerNotes> &);

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
  map<mapPoint, explorerNotes>::iterator i;
  int totalUnexploredEntrances;
  stack<mapPoint > tracker;

  currentPoint = myMaze.startPoint;
  tracker.push(currentPoint);
  currentNote.stacked = true;

  done = false;
  while(!done) {
    i = explorerNotebook.find(currentPoint);
    if (i->second.timesVisitedThisBlock == 0)
    {
      lookAround(currentPoint,myMaze,currentNote);
      explorerNotebook[currentPoint] = currentNote;
    }

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
      } else {
        stepIn(currentPoint,explorerNotebook,tracker);
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

    if(myMaze.groundCluster[currentPoint.X][currentPoint.Y] == COIN){
        currentNote.foundCoin = true;
    }

  if(myMaze.groundCluster[N.X][N.Y] != WALL &&
     currentPoint.X > 0 &&
     explorerNotebook.count(N) != 0) {
    currentNote.unexploredEntrances[0] = true;
  }

  if(myMaze.groundCluster[S.X][S.Y] != WALL &&
     currentPoint.X < myMaze.mazeWidth &&
     explorerNotebook.count(S) != 0) {
    currentNote.unexploredEntrances[1] = true;
  }

  if(myMaze.groundCluster[E.X][E.Y] != WALL &&
    currentPoint.X < myMaze.mazeLength &&
    explorerNotebook.count(E) != 0) {
    currentNote.unexploredEntrances[2] = true;
  }

  if(myMaze.groundCluster[W.X][W.Y] != WALL &&
    currentPoint.X > 0 &&
    explorerNotebook.count(W) != 0) {
    currentNote.unexploredEntrances[3] = true;
  }
}

void stepIn(mapPoint &currentPoint, map<mapPoint, explorerNotes> &explorerNotebook, stack<mapPoint > &tracker) {

}
