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

  bool operator < (const mapPoint &coord) const {
    if (X == coord.X && Y == coord.Y) {
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
  int timesVisitedThisBlock;
  vector<bool> unexploredEntrances; // 0 0 0 0 -> N S E W
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

// teste V
  explorerNotes testeNote1;
  mapPoint testeCoord1;
  explorerNotes testeNote2;
  mapPoint testeCoord2;

  testeNote1.foundCoin = true;
  testeNote1.timesVisitedThisBlock = 33;
  testeNote1.unexploredEntrances = {true, true, false, false};
  testeCoord1.X = 25;
  testeCoord1.Y = 3;

  testeNote2.foundCoin = false;
  testeNote2.timesVisitedThisBlock = 200;
  testeNote2.unexploredEntrances = {false, true, false, false};
  testeCoord2.X = 11;
  testeCoord2.Y = 2;

  explorerNotebook.insert(pair<mapPoint, explorerNotes>(testeCoord1, testeNote1));
  explorerNotebook.insert(pair<mapPoint, explorerNotes>(testeCoord2, testeNote2));
// teste ^

  //while(!done) {
    lookAround(currentPoint,myMaze,currentNote);
      explorerNotebook.insert(pair<mapPoint, explorerNotes>(currentPoint, currentNote));
    //explorerNotebook[currentPoint] = currentNote;
    // done = nextStep(currentPoint,myMaze,explorerNoteBook);
  //}

/*
  for(const auto &value : explorerNotebook) {
    cout << value.foundCoin.first << " " << value.foundCoin.second.first << " " << value.foundCoin.second.second << "\n";
  }
if(explorerNotebook.find(testeCoord1) == explorerNotebook.end()) {cout << "exite";}
else {cout << "nao existe";}

int testcount = 0;
for(auto it = explorerNotebook.cbegin(); it != explorerNotebook.cend(); ++it)
{
    cout << "explorerNotebook("<< it->first.X << "," << it->first.Y << ")" << "\n";
    cout << it->second.foundCoin << "\n";
    cout << it->second.timesVisitedThisBlock << "\n";
    cout << "(" << it->second.unexploredEntrances[0] << "," << it->second.unexploredEntrances[1] << "," << it->second.unexploredEntrances[2] << "," <<it->second.unexploredEntrances[3] << ")" << "\n";

    testcount++;
    cout << testcount << "\n";
}
*/

map<mapPoint, explorerNotes>::iterator p;
p = explorerNotebook.find(testeCoord1);
cout << p->second.timesVisitedThisBlock << "teste" << endl;
map<mapPoint, explorerNotes>::iterator q;
q = explorerNotebook.find(testeCoord2);
cout << q->second.timesVisitedThisBlock << "teste" << endl;
map<mapPoint, explorerNotes>::iterator r;
r = explorerNotebook.find(currentPoint);
cout << r->second.timesVisitedThisBlock << "teste" << endl;


if(explorerNotebook.find(testeCoord1) == explorerNotebook.end()) {cout << "exite1! ";}
else {cout << "nao existe! ";}
if(explorerNotebook.find(testeCoord2) == explorerNotebook.end()) {cout << "exite2! ";}
else {cout << "nao existe! ";}
if(explorerNotebook.find(currentPoint) == explorerNotebook.begin()) {cout << "exiteCurrent! ";}
else {cout << "nao existe! ";}

  cout << "currentPoint(" << currentPoint.X << "," << currentPoint.Y << ")" << endl;
  return true;
}

void lookAround(mapPoint &currentPoint,mazeMap &myMaze,explorerNotes &currentNote) {
  currentNote.timesVisitedThisBlock++;

  if(myMaze.groundCluster[currentPoint.X][currentPoint.Y] == COIN){
      currentNote.foundCoin = true;
  }

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
