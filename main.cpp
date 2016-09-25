#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iterator>

using namespace std;

struct mapPoint {
  int X;
  int Y;
};

struct mazeMap {
  int mazeLength;
  int mazeWidth;
  mapPoint startPoint;
  vector <vector<unsigned char> > groundCluster;
};

struct explorerNotes {
  bool foundCoin;
  int timesVisitedThisBlock;
  vector <bool> unexploredEntrances; // 0 0 0 0 -> N S E W
  explorerNotes():timesVisitedThisBlock(0),unexploredEntrances(4,false){}
};

//  TODO: Simplify vector declarations using template
void readFile(string, mazeMap &, ifstream &);  //Function prototype for the reading the file function
void findStart(mazeMap &); // function prototype to locate the maze spawn point tagged as 'i'.
bool exploreMaze(mazeMap &, map<mapPoint, explorerNotes> &);
void lookAround(mapPoint &, mazeMap &, explorerNotes &);

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

/*    for (auto &x: noteBook) {
            cout << "P(" << x.first[0] << "," << x.first[1] << ")" << " => " << x.second << '\n';
    }
*/

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
  // explorerNotebook[myMaze.startPoint] = //  add in nootebook the initial point and mark it as your first visited point.
  mapPoint currentPoint;
  explorerNotes currentNote;
  bool done;

  done = false;
  currentPoint = myMaze.startPoint;

  while(!done) {
    lookAround(currentPoint,myMaze,currentNote);
    explorerNotebook[currentPoint] = currentNote; // TODO: checar se ponto ja existe, caso sim atualize timesVisitedThisBlock++ e cheque nodos a serem explorados N S E W
    // done = nextStep(currentPoint,myMaze,explorerNoteBook);
  }

  cout << "currentPoint(" << currentPoint.X << "," << currentPoint.Y << ")" << endl;
  return true;
}

void lookAround(mapPoint &currentPoint,mazeMap &myMaze,explorerNotes &currentNote) {
  if(myMaze.groundCluster[currentPoint.X - 1][currentPoint.Y] != WALL &&
      currentPoint.X > 0) {
    currentNote.unexploredEntrances[0] = true;
  }

  if(myMaze.groundCluster[currentPoint.X + 1][currentPoint.Y] != WALL &&
      currentPoint.X < myMaze.mazeWidth) {
    currentNote.unexploredEntrances[1] = true;
  }

    if(myMaze.groundCluster[currentPoint.X][currentPoint.Y + 1] != WALL &&
      currentPoint.X < myMaze.mazeLength) {
    currentNote.unexploredEntrances[2] = true;
  }

    if(myMaze.groundCluster[currentPoint.X][currentPoint.Y - 1] != WALL &&
      currentPoint.X > 0) {
    currentNote.unexploredEntrances[3] = true;
  }
}
