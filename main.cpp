// reading a text file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

struct mapPoint
{
  int X;
  int Y;
  int newPaths;
};

//  TODO: Simplify vector declarations using template
void readFile(int &, int &, string,vector<vector<unsigned char> > &, ifstream &);//Function prototype for the reading the file function
void findStart(int &, int &, vector<vector<unsigned char> > &, mapPoint &); // function prototype to locate the maze spawn point tagged as 'i'.

int main () {
  mapPoint startPoint;
  int mazeLength;
  int mazeWidth;
  ifstream inFile; //Input file
  string strFileName; //File name
  vector <vector<unsigned char> > groundCluster;

  strFileName = "maze.txt"; // TODO: accept user input cin >>

  readFile(mazeLength, mazeWidth, strFileName, groundCluster, inFile);
  findStart(mazeLength, mazeWidth, groundCluster, startPoint);

  return 0;
}

void readFile(int &mazeLength, int &mazeWidth,string strFileName,vector<vector<unsigned char> > &groundCluster, ifstream &inFile) {

  vector<unsigned char> tempVector;
  string line;

  inFile.open(strFileName.c_str());
  if (inFile.is_open())
  {
    getline(inFile,line);
    istringstream iss(line);
    iss >> mazeLength >> mazeWidth;

    while(getline (inFile,line))
    {
      for(int i = 0; i < line.length() - 1; ++i)
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

void findStart(int &mazeLength, int &mazeWidth, vector<vector<unsigned char> > &groundCluster , mapPoint &mapPoint) {
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
