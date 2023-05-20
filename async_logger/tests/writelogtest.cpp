#include "async_logger.hpp"
#include "utils.hpp"
#include <cassert>

std::string readFirstLine(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string line;
    std::getline(file, line);
    return line;
}

int main(){

  Logger logg;
  // filename based on current date
  std::string fname = CreateNewFileName(); 
  logg.SetOutput(fname);

  for(int64_t i = 0;i<1000000;i++){
    logg.Log("test", i);
  }

  logg.stop_now();
  std::string first = readFirstLine(fname);
  std::size_t len = first.length();

  assert(len != 0);
  

}