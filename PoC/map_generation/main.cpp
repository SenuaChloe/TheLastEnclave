#include <iostream>
#include <vector>
#include <cstdlib>

enum class Biome {
  Undefined,
  Plain,
  Forest,
  Mountain,
  Sea = 99,
};

typedef std::vector<std::vector<Biome>> Map;

unsigned int get_neightboring_weight(Biome ref, Biome neigh)
{
  switch (ref)
  {
    case Biome::Plain:
      switch (neigh) {
        case Biome::Plain:
          return 1;
        case Biome::Forest:
          return 1;
        case Biome::Mountain:
          return 1;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    case Biome::Forest:
      switch (neigh) {
        case Biome::Plain:
          return 1;
        case Biome::Forest:
          return 1;
        case Biome::Mountain:
          return 1;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    case Biome::Mountain:
      switch (neigh) {
        case Biome::Plain:
          return 1;
        case Biome::Forest:
          return 1;
        case Biome::Mountain:
          return 1;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    case Biome::Sea:
      switch (neigh) {
        case Biome::Plain:
          return 1;
        case Biome::Forest:
          return 1;
        case Biome::Mountain:
          return 1;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    default:
      return 0;
  }
  return 1;
}

Biome get_neighbor(Biome b1, Biome b2, Biome b3, Biome b4)
{
  unsigned int plain_weight = 0;
  unsigned int forest_weight = 0;
  unsigned int mountain_weight = 0;
  unsigned int sea_weight = 0;

  plain_weight += get_neightboring_weight(b1, Biome::Plain);
  plain_weight += get_neightboring_weight(b2, Biome::Plain);
  plain_weight += get_neightboring_weight(b3, Biome::Plain);
  plain_weight += get_neightboring_weight(b4, Biome::Plain);

  forest_weight += get_neightboring_weight(b1, Biome::Forest);
  forest_weight += get_neightboring_weight(b2, Biome::Forest);
  forest_weight += get_neightboring_weight(b3, Biome::Forest);
  forest_weight += get_neightboring_weight(b4, Biome::Forest);
  
  mountain_weight += get_neightboring_weight(b1, Biome::Mountain);
  mountain_weight += get_neightboring_weight(b2, Biome::Mountain);
  mountain_weight += get_neightboring_weight(b3, Biome::Mountain);
  mountain_weight += get_neightboring_weight(b4, Biome::Mountain);
  
  sea_weight += get_neightboring_weight(b1, Biome::Sea);
  sea_weight += get_neightboring_weight(b2, Biome::Sea);
  sea_weight += get_neightboring_weight(b3, Biome::Sea);
  sea_weight += get_neightboring_weight(b4, Biome::Sea);

  unsigned int total = plain_weight + forest_weight + mountain_weight + sea_weight;
  unsigned int roll = rand() % total;

  if (roll < plain_weight)
    return Biome::Plain;
  roll -= plain_weight;

  if (roll < forest_weight)
    return Biome::Forest;
  roll -= forest_weight;

  if (roll < mountain_weight)
    return Biome::Mountain;

  return Biome::Sea;
}

int main()
{
  get_neighbor(Biome::Forest, Biome::Plain, Biome::Undefined, Biome::Undefined);
  return 0;
}


