/* 
 * File:   mazegenerator.cpp
 * Author: jmcogan
 *
 * Created on July 18, 2015, 11:26 PM
 * 
 * Recursively creates a maze in ASCII form. Starts to overflow the stack around
 * 202500 square units or so. Created somewhat hastily mostly for use with
 * mazesolver.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/*
 * checks to see if (x, y) is a good spot to remove a wall
 */
bool goodSpace(vector<vector<bool> >& wall, int x, int y, int length, int height)
{
  if (x == 0 || y == 0 || x == length - 1 || y == height - 1)
    return false;
  
  int openSpots = 0;
  if (wall[x + 1][y] == false)
    openSpots++;
  if (wall[x - 1][y] == false)
    openSpots++;
  if (wall[x][y + 1] == false)
    openSpots++;
  if (wall[x][y - 1] == false)
    openSpots++;
  
  return openSpots < 2;
}

/*
 * recursive function that does the majority of the work. does all the wall
 * removing to form the actual maze
 */
void checkNext(vector<vector<bool> >& wall, int x, int y, int length,
               int height, int prevDir)
{
  vector<bool> checked(4, false);
  switch (prevDir)
  {
    /*
     * 0 == North
     * 1 == South
     * 2 == East
     * 3 == West
     */
    case 0:
      checked[1] = true;
      break;
    case 1:
      checked[0] = true;
      break;
    case 2:
      checked[3] = true;
      break;
    case 3:
      checked[2] = true;
      break;
  }
  
  // check all directions
  while (checked[0] == false ||
         checked[1] == false ||
         checked[2] == false ||
         checked[3] == false)
  {
    int nextDir = rand() % 4;
    int nextX = x;
    int nextY = y;
    switch (nextDir)
    {
        /*
         * 0 == North
         * 1 == South
         * 2 == East
         * 3 == West
         */
      case 0:
        nextY++;
        break;
      case 1:
        nextY--;
        break;
      case 2:
        nextX++;
        break;
      case 3:
        nextX--;
        break;
    }
    
    //if a valid direciton
    if (checked[nextDir] == false &&
        goodSpace(wall, nextX, nextY, length, height))
    {
      wall[nextX][nextY] = false;
      checkNext(wall, nextX, nextY, length, height, nextDir);
    }
    checked[nextDir] = true;
  }
}
/*
 * argv[1] == length
 * argv[2] == width
 * argv[3] == char to use for walls (optional, default is '#')
 */
int main(int argc, char** argv)
{
  srand (time(NULL));
  int length, height;
  length = atoi(argv[1]);
  height = atoi(argv[2]);
  char wallChar = '#';
  if (argc == 4)
    wallChar = argv[3][0];
  
  vector< vector<bool> > wall;
  wall.resize(length, vector<bool>(height, true));
  
  wall[1][1] = false;
  checkNext(wall, 1, 1, length, height, 2);
  
  ostringstream out;
  out << length << '\n';
  out << height << '\n';
  out << "1, 1\n";
  int endX = length - 2;
  while (wall[endX][height - 2] == true)
    endX--;
  out << endX << ", " << height - 2 << endl;
  for (int x = 0; x < length; ++x)
  {
    for (int y = 0; y < height; ++y)
    {
      if (wall[x][y] == true)
        out << wallChar;
      else
        out << ' ';
    }
    out << '\n';
  }
  cout << out.str();
  return 0;
}

