// reading a text file
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
  int forksToGo;  // number of paths nodes with entries yet to be explored.
  mapPoint startPoint;
  vector <vector<unsigned char> > groundCluster;
};

struct explorerNotes {
  bool foundCoin;
  bool linkedToExitNode;
  int timesVisitedThisBlock;
  vector <bool> blockUnexploredEntrances; // 0 0 0 0 -> N S E W
};

//  TODO: Simplify vector declarations using template
void readFile(string,mazeMap &, ifstream &);//Function prototype for the reading the file function
void findStart(mazeMap &); // function prototype to locate the maze spawn point tagged as 'i'.
bool exploreMaze(mazeMap &, map<mapPoint, explorerNotes> &);

int main () {

  mazeMap myMaze;
  ifstream inFile; //Input file
  string strFileName; //File name
  map<mapPoint, explorerNotes> explorerNotebook; //  explorerNotebook[{a,b}] = c;
  bool foundExit;

  strFileName = "maze.txt"; // TODO: accept user input cin >>

  readFile(strFileName, myMaze, inFile);
  findStart(myMaze);
  foundExit = exploreMaze(myMaze, explorerNotebook);

  if(foundExit) {
    cout << "true" << endl;
 // TODO
  } else {
    cout << "false" << endl;
    // TODO
  }

// -------------------------------- remove this block when done --------------------------------------------------

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

// -------------------------------- remove this block when done --------------------------------------------------
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
      if(myMaze.groundCluster[x][y] == 'i') {
        myMaze.startPoint.X = x;
        myMaze.startPoint.Y = y;
        break;
      }
    }
  }
}

bool exploreMaze(mazeMap &myMaze, map<mapPoint, explorerNotes> &explorerNotebook) {
  mapPoint currentPoint;
  cout << myMaze.groundCluster[1][1] << endl;

  return true;
}
