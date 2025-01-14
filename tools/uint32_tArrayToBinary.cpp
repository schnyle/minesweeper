#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// uint32_t array data to be converted
const std::string outputFilename = "one"; // 'bin' extension will be added
constexpr size_t dataSize = 2500;
uint32_t data[dataSize] = {

};

void saveArrayToBinary(const uint32_t *array, size_t size, const std::string &filename)
{
  std::ofstream file(filename, std::ios::binary);
  if (!file)
  {
    throw std::runtime_error("failed to open file: " + filename);
  }

  file.write(reinterpret_cast<const char *>(array), size * sizeof(uint32_t));
  if (!file)
  {
    throw std::runtime_error("failed to write to file: " + filename);
  }
}

int main()
{
  try
  {
    const std::string filename = outputFilename + ".bin";
    saveArrayToBinary(data, dataSize, filename);
    std::cout << "converted array to binary file at " + filename + "\n";
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}