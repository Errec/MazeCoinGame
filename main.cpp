// reading a text file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

void readFile(string,const vector<vector<unsigned char> > &, ifstream &);//Function prototype for the reading the file function

int main () {

  ifstream inFile; //Input file
  string strFileName; //File name
  vector <vector<unsigned char> > groundCluster;

  strFileName = "maze.txt";
  readFile(strFileName, groundCluster, inFile);

  return 0;
}

void readFile(string strFileName,const vector<vector<unsigned char> > &groundCluster, ifstream &inFile) {

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
      tempVector.clear();
    }
    inFile.close();
  }
  else cout << "Unable to open file";
}
