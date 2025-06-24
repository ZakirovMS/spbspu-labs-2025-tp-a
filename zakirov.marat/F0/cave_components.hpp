#ifndef CAVE_COMPONENTS_HPP
#define CAVE_COMPONENTS_HPP
#include <map>
#include <iostream>

namespace zakirov
{
  class Cave
  {
  public:
    Cave(std::istream & in);
    Cave getPath();
    void addEdge(int u, char v);
  private:
    std::map< int, std::map< int, CaveComponent > > cave_map;
  };

  class CaveComponent
  {
    public:
      bool isWall();
      bool isPath();
      bool isEntry();
      bool isExit();
    private:
      char symbol;
  };
}

#endif
