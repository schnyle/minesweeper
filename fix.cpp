#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

int main()
{
  std::stringstream ss;
  std::ifstream f("in.txt");
  if (!f.is_open())
  {
    throw std::runtime_error("AHHHH");
  }

  std::string line;
  while (std::getline(f, line))
  {
    if (line.substr(0, 2) == "0x")
    {
      std::stringstream hexs(line);
      std::string word;
      while (hexs >> word)
      {
        std::cout << "go\n";
        if (std::count(word.begin(), word.end(), 'f') > 3 && std::count(word.begin(), word.end(), 'e') > 0)
        {
          ss << "0xffffffff,";
        }
        else
        {
          ss << word;
        }
      }
    }
  }

  std::ofstream o("out.txt");
  o << ss.str();
}
