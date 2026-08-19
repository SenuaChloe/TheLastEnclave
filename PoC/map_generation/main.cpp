#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <map>

enum class Biome {
  Undefined,
  Plain,
  Forest,
  Mountain,
  City,
  Swamp,
  Desert,
  Tundra,
  Sea,
};

std::string get_biome_color(Biome b)
{
  switch (b)
  {
    case Biome::Plain:
      return "37";
    case Biome::Forest:
      return "92";
    case Biome::Mountain:
      return "33";
    case Biome::City:
      return "31";
    case Biome::Swamp:
      return "32";
    case Biome::Desert:
      return "93";
    case Biome::Tundra:
      return "0";
    case Biome::Sea:
      return "36";
    default:
      return "95";
  }
}

std::string get_biome_character(Biome b)
{
  switch (b)
  {
    case Biome::Plain:
      return "  ";
    case Biome::Forest:
      return "| ";
    case Biome::Mountain:
      return "^ ";
    case Biome::City:
      return "X ";
    case Biome::Swamp:
      return "$ ";
    case Biome::Desert:
      return ". ";
    case Biome::Tundra:
      return "* ";
    case Biome::Sea:
      return "~ ";
    default:
      return "= ";
  }
  
}

std::string biome_to_char(Biome b)
{
  std::string res;
  res += "\033[";
  res += get_biome_color(b);
  res += "m";
  res += get_biome_character(b);
  res += "\033[0m";
  return res;
}


const unsigned int WIDTH = 48;
const unsigned int HEIGHT = 48;
const std::vector<std::pair<unsigned int, unsigned int>> STARTING_COORDS = { {12,12}, {12,24}, {24,12}, {24,24} };
const std::vector<Biome> STARTING_BIOMES = { Biome::Plain, Biome::Tundra, Biome::Desert, Biome::Swamp };

std::map<Biome, std::map<Biome, unsigned int>> G_WEIGHTS;

void init_weights()
{
  G_WEIGHTS[Biome::Undefined][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Plain]     = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Forest]    = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Mountain]  = 0;
  G_WEIGHTS[Biome::Undefined][Biome::City]      = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Swamp]     = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Desert]    = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Tundra]    = 0;
  G_WEIGHTS[Biome::Undefined][Biome::Sea]       = 0;

  G_WEIGHTS[Biome::Plain][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Plain][Biome::Plain]     = 180;
  G_WEIGHTS[Biome::Plain][Biome::Forest]    = 7;
  G_WEIGHTS[Biome::Plain][Biome::Mountain]  = 4;
  G_WEIGHTS[Biome::Plain][Biome::City]      = 2;
  G_WEIGHTS[Biome::Plain][Biome::Swamp]     = 2;
  G_WEIGHTS[Biome::Plain][Biome::Desert]    = 2;
  G_WEIGHTS[Biome::Plain][Biome::Tundra]    = 2;
  G_WEIGHTS[Biome::Plain][Biome::Sea]       = 1;

  G_WEIGHTS[Biome::Forest][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Forest][Biome::Plain]     = 13;
  G_WEIGHTS[Biome::Forest][Biome::Forest]    = 170;
  G_WEIGHTS[Biome::Forest][Biome::Mountain]  = 4;
  G_WEIGHTS[Biome::Forest][Biome::City]      = 3;
  G_WEIGHTS[Biome::Forest][Biome::Swamp]     = 3;
  G_WEIGHTS[Biome::Forest][Biome::Desert]    = 3;
  G_WEIGHTS[Biome::Forest][Biome::Tundra]    = 3;
  G_WEIGHTS[Biome::Forest][Biome::Sea]       = 1;

  G_WEIGHTS[Biome::Mountain][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Mountain][Biome::Plain]     = 16;
  G_WEIGHTS[Biome::Mountain][Biome::Forest]    = 8;
  G_WEIGHTS[Biome::Mountain][Biome::Mountain]  = 160;
  G_WEIGHTS[Biome::Mountain][Biome::City]      = 4;
  G_WEIGHTS[Biome::Mountain][Biome::Swamp]     = 4;
  G_WEIGHTS[Biome::Mountain][Biome::Desert]    = 3;
  G_WEIGHTS[Biome::Mountain][Biome::Tundra]    = 3;
  G_WEIGHTS[Biome::Mountain][Biome::Sea]       = 2;

  G_WEIGHTS[Biome::City][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::City][Biome::Plain]     = 90;
  G_WEIGHTS[Biome::City][Biome::Forest]    = 45;
  G_WEIGHTS[Biome::City][Biome::Mountain]  = 45;
  G_WEIGHTS[Biome::City][Biome::City]      = 0;
  G_WEIGHTS[Biome::City][Biome::Swamp]     = 5;
  G_WEIGHTS[Biome::City][Biome::Desert]    = 5;
  G_WEIGHTS[Biome::City][Biome::Tundra]    = 5;
  G_WEIGHTS[Biome::City][Biome::Sea]       = 5;

  G_WEIGHTS[Biome::Swamp][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Swamp][Biome::Plain]     = 8;
  G_WEIGHTS[Biome::Swamp][Biome::Forest]    = 16;
  G_WEIGHTS[Biome::Swamp][Biome::Mountain]  = 4;
  G_WEIGHTS[Biome::Swamp][Biome::City]      = 4;
  G_WEIGHTS[Biome::Swamp][Biome::Swamp]     = 160;
  G_WEIGHTS[Biome::Swamp][Biome::Desert]    = 4;
  G_WEIGHTS[Biome::Swamp][Biome::Tundra]    = 4;
  G_WEIGHTS[Biome::Swamp][Biome::Sea]       = 0;

  G_WEIGHTS[Biome::Desert][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Desert][Biome::Plain]     = 2;
  G_WEIGHTS[Biome::Desert][Biome::Forest]    = 2;
  G_WEIGHTS[Biome::Desert][Biome::Mountain]  = 2;
  G_WEIGHTS[Biome::Desert][Biome::City]      = 2;
  G_WEIGHTS[Biome::Desert][Biome::Swamp]     = 0;
  G_WEIGHTS[Biome::Desert][Biome::Desert]    = 190;
  G_WEIGHTS[Biome::Desert][Biome::Tundra]    = 0;
  G_WEIGHTS[Biome::Desert][Biome::Sea]       = 3;

  G_WEIGHTS[Biome::Tundra][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Tundra][Biome::Plain]     = 1;
  G_WEIGHTS[Biome::Tundra][Biome::Forest]    = 1;
  G_WEIGHTS[Biome::Tundra][Biome::Mountain]  = 1;
  G_WEIGHTS[Biome::Tundra][Biome::City]      = 2;
  G_WEIGHTS[Biome::Tundra][Biome::Swamp]     = 0;
  G_WEIGHTS[Biome::Tundra][Biome::Desert]    = 0;
  G_WEIGHTS[Biome::Tundra][Biome::Tundra]    = 190;
  G_WEIGHTS[Biome::Tundra][Biome::Sea]       = 5;

  G_WEIGHTS[Biome::Sea][Biome::Undefined] = 0;
  G_WEIGHTS[Biome::Sea][Biome::Plain]     = 1;
  G_WEIGHTS[Biome::Sea][Biome::Forest]    = 1;
  G_WEIGHTS[Biome::Sea][Biome::Mountain]  = 1;
  G_WEIGHTS[Biome::Sea][Biome::City]      = 2;
  G_WEIGHTS[Biome::Sea][Biome::Swamp]     = 1;
  G_WEIGHTS[Biome::Sea][Biome::Desert]    = 1;
  G_WEIGHTS[Biome::Sea][Biome::Tundra]    = 1;
  G_WEIGHTS[Biome::Sea][Biome::Sea]       = 200;
}

typedef std::vector<std::vector<Biome>> Map;

unsigned int get_neightboring_weight(Biome ref, Biome neigh)
{
  return G_WEIGHTS[ref][neigh];
}

Biome get_neighbor(Biome b1, Biome b2, Biome b3, Biome b4)
{
  unsigned int plain_weight = 0;
  unsigned int forest_weight = 0;
  unsigned int mountain_weight = 0;
  unsigned int city_weight = 0;
  unsigned int swamp_weight = 0;
  unsigned int desert_weight = 0;
  unsigned int tundra_weight = 0;
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
  
  city_weight += get_neightboring_weight(b1, Biome::City);
  city_weight += get_neightboring_weight(b2, Biome::City);
  city_weight += get_neightboring_weight(b3, Biome::City);
  city_weight += get_neightboring_weight(b4, Biome::City);
  
  swamp_weight += get_neightboring_weight(b1, Biome::Swamp);
  swamp_weight += get_neightboring_weight(b2, Biome::Swamp);
  swamp_weight += get_neightboring_weight(b3, Biome::Swamp);
  swamp_weight += get_neightboring_weight(b4, Biome::Swamp);
  
  desert_weight += get_neightboring_weight(b1, Biome::Desert);
  desert_weight += get_neightboring_weight(b2, Biome::Desert);
  desert_weight += get_neightboring_weight(b3, Biome::Desert);
  desert_weight += get_neightboring_weight(b4, Biome::Desert);
  
  tundra_weight += get_neightboring_weight(b1, Biome::Tundra);
  tundra_weight += get_neightboring_weight(b2, Biome::Tundra);
  tundra_weight += get_neightboring_weight(b3, Biome::Tundra);
  tundra_weight += get_neightboring_weight(b4, Biome::Tundra);
  
  sea_weight += get_neightboring_weight(b1, Biome::Sea);
  sea_weight += get_neightboring_weight(b2, Biome::Sea);
  sea_weight += get_neightboring_weight(b3, Biome::Sea);
  sea_weight += get_neightboring_weight(b4, Biome::Sea);

  unsigned int total = 
    plain_weight + 
    forest_weight + 
    mountain_weight + 
    city_weight +
    swamp_weight + 
    desert_weight +
    tundra_weight +
    sea_weight;
  unsigned int roll = rand() % total;

  if (roll < plain_weight)
    return Biome::Plain;
  roll -= plain_weight;

  if (roll < forest_weight)
    return Biome::Forest;
  roll -= forest_weight;

  if (roll < mountain_weight)
    return Biome::Mountain;
  roll -= mountain_weight;

  if (roll < city_weight)
    return Biome::City;
  roll -= city_weight;

  if (roll < swamp_weight)
    return Biome::Swamp;
  roll -= swamp_weight;

  if (roll < desert_weight)
    return Biome::Desert;
  roll -= desert_weight;

  if (roll < tundra_weight)
    return Biome::Tundra;

  return Biome::Sea;
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

  for (unsigned int i = 0 ; i < HEIGHT ; ++i)
  {
    for (unsigned int j = 0 ; j < WIDTH ; ++j)
    {
      bool is_dist = false;
      for (unsigned int i = 0 ; i < STARTING_COORDS.size() ; ++i)
        if (dist_coord(std::pair(i,j), STARTING_COORDS[i]) == dist)
          is_dist = true;
      if (is_dist)
        result.push_back(std::pair(i,j));
    }
  }
  return result;
}

int main()
{
  // Init seed
  srand(time(NULL));

  // Init weights
  init_weights();

  // Init map
  Map map;
  map.resize(HEIGHT);
  for (unsigned int i = 0 ; i < HEIGHT ; ++i)
    map[i].resize(WIDTH, Biome::Undefined);

  for (unsigned int i = 0 ; i < std::min(STARTING_COORDS.size(), STARTING_BIOMES.size()) ; ++i)
    map[STARTING_COORDS[i].first][STARTING_COORDS[i].second] = STARTING_BIOMES[i];
  
  for (unsigned int i = 0 ; i < HEIGHT ; ++i)
  {
    map[i][0] = Biome::Sea;
    map[i][WIDTH-1] = Biome::Sea;
  }
  for (unsigned int j = 0 ; j < WIDTH ; ++j)
  {
    map[0][j] = Biome::Sea;
    map[HEIGHT-1][j] = Biome::Sea;
  }

  // Pre-draw
  std::cout << std::endl << std::endl << "\033[30m";
  for (unsigned int i = 0 ; i < WIDTH*2+3; ++i)
    std::cout << "#";
  std::cout << std::endl;

  // Main loop
  for (unsigned int dist = 1 ; dist < HEIGHT+WIDTH ; ++dist)
  {
    for (auto c: get_all_coord_from_dist(dist))
    {
      if (map[c.first][c.second] != Biome::Undefined)
        continue;

      Biome b_north = Biome::Undefined;
      Biome b_south = Biome::Undefined;
      Biome b_east = Biome::Undefined;
      Biome b_west = Biome::Undefined;
      
      if (c.first > 0)
        b_west = map[c.first-1][c.second];
      if (c.first < HEIGHT-1)
        b_east = map[c.first+1][c.second];
      if (c.second > 0)
        b_north = map[c.first][c.second-1];
      if (c.second < WIDTH-1)
        b_south = map[c.first][c.second+1];

      map[c.first][c.second] = get_neighbor(b_north, b_south, b_east, b_west);
    }
  }

  // Post processing: delete single tiles
  for (unsigned int i = 0 ; i < HEIGHT; ++i)
  {
    for (unsigned int j = 0 ; j < WIDTH; ++j)
    {
      Biome b = map[i][j];
      Biome b_north = Biome::Undefined;
      Biome b_south = Biome::Undefined;
      Biome b_east = Biome::Undefined;
      Biome b_west = Biome::Undefined;
      
      if (i > 0)
        b_west = map[i-1][j];
      if (i < HEIGHT-1)
        b_east = map[i+1][j];
      if (j > 0)
        b_north = map[i][j-1];
      if (j < WIDTH-1)
        b_south = map[i][j+1];
      
      if (b != Biome::City && 
          b != b_north &&
          b != b_south &&
          b != b_east &&
          b != b_west)
      {
        switch (rand()%4)
        {
          case 0: map[i][j] = b_north; break;
          case 1: map[i][j] = b_south; break;
          case 2: map[i][j] = b_east; break;
          case 3: map[i][j] = b_west; break;
        }
        if (map[i][j] == Biome::City)
          map[i][j] = Biome::Plain;
      }
    }
  }

  // Post-draw
  for (unsigned int i = 0 ; i < HEIGHT ; ++i)
  {
    std::cout << "\033[30m# ";
    for (unsigned int j = 0 ; j < WIDTH ; ++j)
      std::cout << biome_to_char(map[i][j]);
    std::cout << "\033[30m#" << std::endl;
  }

  std::cout << "\033[30m";
  for (unsigned int i = 0 ; i < WIDTH*2+3; ++i)
    std::cout << "#";
  std::cout << std::endl;


  return 0;
}


