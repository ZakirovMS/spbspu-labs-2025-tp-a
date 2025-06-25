#include "cave_components.hpp"
#include <iostream>
#include <stream_guardian.hpp>

namespace
{
  struct MinorSymbol
  {
    char symbol;
  };

  std::istream & operator>>(std::istream & in, MinorSymbol && sym)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    char symbol;
    if (!(in >> symbol) || symbol != sym.symbol)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }
}

char zakirov::CaveComponent::getCell() const noexcept
{
  return cell_;
}

char zakirov::CaveComponent::setCell(char cell) noexcept
{
  cell_ = cell;
}

bool zakirov::CaveComponent::isWall() const noexcept
{
  return cell_ == '#';
}

bool zakirov::CaveComponent::isPath() const noexcept
{
  return cell_ == '.';
}

bool zakirov::CaveComponent::isEntry() const noexcept
{
  return cell_ == '+';
}

bool zakirov::CaveComponent::isExit() const noexcept
{
 return (cell_ == '=' || (cell_ > 'a' && cell_ < 'z') || (cell_ > 'A' && cell_ < 'Z'));
}

std::pair< int, int > zakirov::CaveLayer::getCords() const noexcept
{
  return cords_;
}

void zakirov::CaveLayer::setCords(std::pair< int, int > cords) noexcept
{
  cords_ = cords;
}

bool zakirov::CaveLayer::isScopes()
{
  return scopes_.first != -1 || scopes_.second != -1;
}

char zakirov::CaveLayer::getName() const noexcept
{
  return name_;
}

void zakirov::CaveLayer::setName(char name) noexcept
{
  name_ = name;
}

void zakirov::CaveLayer::addElement(int key, CaveComponent & cell)
{
  std::list< int > edges;
  cave_map_.insert({key, {cell, {}}});
  addEdges(key);
  if (cell.isEntry())
  {
    scopes_.first = key;
  }
  else if (cell.isExit())
  {
    scopes_.second = key;
  }
}

void zakirov::CaveLayer::addEdge(int key1, int key2)
{
  cave_map_[key1].second.push_back(key2);
  cave_map_[key2].second.push_back(key1);
}

void zakirov::CaveLayer::addEdges(int key)
{
  if (key % cords_.first != 0)
  {
    addEdge(key, key - 1);
  }
  
  if (key - cords_.first >= 0)
  {
    addEdge(key, key - cords_.first);
  }
}

std::istream & zakirov::operator>>(std::istream & in, zakirov::CaveComponent & comp_c)
{
  char element_c;
  in >> element_c;
  comp_c.setCell(element_c);
  if (!comp_c.isWall() && !comp_c.isPath() && !comp_c.isEntry() && !comp_c.isExit())
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, zakirov::CaveLayer & layer_c)
{
  Guardian guard(in);
  char name;
  int width = 0;
  int length = 0;
  in >> std::noskipws;
  in >> name >> MinorSymbol{' '};
  in >> length >> MinorSymbol{' '} >> width >> MinorSymbol{'\n'};
  layer_c.setName(name);
  layer_c.setCords({length, width});
  size_t key = 0;
  CaveComponent inserter;
  for (size_t i = 0; i < width; ++i)
  {
    in >> inserter;
    layer_c.addElement(key, inserter);
    for (size_t j = 0; j < length - 1; ++j)
    {
      in >> MinorSymbol{' '} >> inserter;
      layer_c.addElement(key, inserter);
    }

    in >> MinorSymbol{'\n'};
  }

  return in;
}
