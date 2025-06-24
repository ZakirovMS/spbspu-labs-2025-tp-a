#ifndef CAVE_COMPONENTS_HPP
#define CAVE_COMPONENTS_HPP
#include <map>
#include <iostream>
#include <string>
namespace zakirov
{
  class CaveComponent
  {
    public:
      char getCell() const noexcept;
      char setCell(char cell) noexcept;
      bool isWall() const noexcept;
      bool isPath() const noexcept;
      bool isEntry() const noexcept;
      bool isExit() const noexcept;
    private:
      char cell_;
  };

  class CaveLayer
  {
  public:
    CaveLayer(std::istream & in);
    CaveLayer getPath();
    std::pair< int, int > getCords() const noexcept;
    void setCords(std::pair< int, int > cords) noexcept;
    char getName() const noexcept;
    void setName(char name) noexcept;
    void addElement(int key, CaveComponent cell);
    void addEdge(int u, char v);
  private:
    std::map< int, std::map< int, CaveComponent > > cave_map_;
    char name_;
    std::pair< int, int > cords_;
  };

  std::istream & operator>>(std::istream & in, CaveComponent & comp_c);
  std::istream & operator>>(std::istream & in, CaveLayer & layer_c);
}

#endif
