// reading a text file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct mapPoint
{
  int X;
  int Y;
  int newPaths;
};

//  TODO: Simplify declarations using template
void readFile(string,vector<vector<unsigned char> > &, ifstream &);//Function prototype for the reading the file function
void findStart(vector<vector<unsigned char> > &, mapPoint &); // function prototype to locate the maze spawn point tagged as 'i'. Returns an array with the coordinates: startPoint[i][j]

int main () {
  mapPoint startPoint;
  ifstream inFile; //Input file
  string strFileName; //File name
  vector <vector<unsigned char> > groundCluster;

  strFileName = "maze.txt"; // TODO: user input cin >>
  readFile(strFileName, groundCluster, inFile);
  findStart(groundCluster, startPoint);

  return 0;
}

void readFile(string strFileName,vector<vector<unsigned char> > &groundCluster, ifstream &inFile) {

  vector<unsigned char> tempVector;
  string line;

  inFile.open(strFileName.c_str());
  if (inFile.is_open())
  {
    getline (inFile,line); // skipping first line
    while ( getline (inFile,line) )
    {
      for (int i = 0; i < line.length() - 1; ++i)
      {
        tempVector.push_back(line[i]);
        cout << tempVector[i];
      }
      groundCluster.push_back(tempVector);
      cout << endl;
      tempVector.clear();
    }
    inFile.close();
  }
  else cout << "Unable to open file";
}

void findStart(vector<vector<unsigned char> > &groundCluster , mapPoint &mapPoint) {
  int mazeLength = 5;
  int mazeWidth = 10;
  for (int x = 0; x < mazeLength; x++)
  {
    for (int y = 0; y < mazeWidth; y++)
    {
      if(groundCluster[x][y] == 'i') {
        mapPoint.X = x;
        mapPoint.Y = y;
        cout << endl << groundCluster[x][y] << endl;
        cout << x << endl;
        cout << y << endl;
      }
    }
  }
}
