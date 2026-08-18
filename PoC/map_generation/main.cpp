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


const unsigned int WIDTH = 12;
const unsigned int HEIGHT = 12;
const unsigned int STARTING_X = 6;
const unsigned int STARTING_Y = 6;
const Biome STARTING_BIOME = Biome::Mountain;

typedef std::vector<std::vector<Biome>> Map;

unsigned int get_neightboring_weight(Biome ref, Biome neigh)
{
  switch (ref)
  {
    case Biome::Plain:
      switch (neigh) {
        case Biome::Plain:
          return 8;
        case Biome::Forest:
          return 4;
        case Biome::Mountain:
          return 2;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    case Biome::Forest:
      switch (neigh) {
        case Biome::Plain:
          return 4;
        case Biome::Forest:
          return 8;
        case Biome::Mountain:
          return 2;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    case Biome::Mountain:
      switch (neigh) {
        case Biome::Plain:
          return 2;
        case Biome::Forest:
          return 4;
        case Biome::Mountain:
          return 6;
        case Biome::Sea:
          return 1;
        default:
          return 0;
      }

    case Biome::Sea:
      switch (neigh) {
        case Biome::Plain:
          return 0;
        case Biome::Forest:
          return 0;
        case Biome::Mountain:
          return 0;
        case Biome::Sea:
          return 8;
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

char biome_to_char(Biome b)
{
  switch (b) {
    case Biome::Plain:
      return ' ';
    case Biome::Forest:
      return '|';
    case Biome::Mountain:
      return '^';
    case Biome::Sea:
      return '~';
    default:
      return 'X';
  }
}

unsigned int dist_coord(std::pair<unsigned int, unsigned int> lhs, std::pair<unsigned int, unsigned int> rhs)
{
  return
    std::max(lhs.first, rhs.first) - std::min(lhs.first, rhs.first) +
    std::max(lhs.second, rhs.second) - std::min(lhs.second, rhs.second);
} 

std::vector<std::pair<unsigned int, unsigned int>> get_all_coord_from_dist(unsigned int dist)
{
  std::vector<std::pair<unsigned int, unsigned int>> result;

  for (unsigned int i = 0 ; i < WIDTH ; ++i)
    for (unsigned int j = 0 ; j < HEIGHT ; ++j)
      if (dist_coord(std::pair(i,j), std::pair(STARTING_X, STARTING_Y)) == dist)
        result.push_back(std::pair(i,j));
  return result;
}

int main()
{
  // Init map
  Map map;
  map.resize(WIDTH);
  for (unsigned int i = 0 ; i < WIDTH ; ++i)
    map[i].resize(HEIGHT, STARTING_BIOME);

  // Main loop
  for (unsigned int dist = 1 ; dist < WIDTH+HEIGHT ; ++dist)
  {
    for (auto c: get_all_coord_from_dist(dist))
    {
      Biome b_north = Biome::Undefined;
      Biome b_south = Biome::Undefined;
      Biome b_east = Biome::Undefined;
      Biome b_west = Biome::Undefined;
      
      if (c.first > 0)
        b_west = map[c.first-1][c.second];
      if (c.first < WIDTH-1)
        b_east = map[c.first+1][c.second];
      if (c.second > 0)
        b_north = map[c.first][c.second-1];
      if (c.second < HEIGHT-1)
        b_south = map[c.first][c.second+1];

      map[c.first][c.second] = get_neighbor(b_north, b_south, b_east, b_west);
    }
  }

  for (unsigned int i = 0 ; i < WIDTH ; ++i)
  {
    for (unsigned int j = 0 ; j < HEIGHT ; ++j)
      std::cout << biome_to_char(map[i][j]);
    std::cout << std::endl;
  }

  return 0;
}


