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
