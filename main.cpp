// reading a text file
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

int main () {
  string line;
  vector< vector<char> > block;
  vector<char> tempVector;

  ifstream myfile ("maze.txt");

  if (myfile.is_open())
  {
    getline (myfile,line); // skipping first line
    while ( getline (myfile,line) )
    {
      for (int i = 0; i < line.length() - 1; ++i)
      {
        tempVector.push_back(line[i]);
      }
      block.push_back(tempVector);
      tempVector.clear();
    }
    myfile.close();
  }
  else cout << "Unable to open file";

  return 0;
}
