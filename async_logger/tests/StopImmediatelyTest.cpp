#include <cassert>
#include "async_logger.hpp"
#include "utils.hpp"
#include <string>
#include <fstream>
#include <sstream>



int main(){
  Logger logg;
  // filename based on current date
  std::string fname = CreateNewFileName(); 
  logg.SetOutput(fname);

  for (int64_t i = 0; i < 100; i++) {
    logg.Log("stopnowtest", i);
    logg.stop_now();
  }

  // Read the contents of the file
  std::ifstream file(fname);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string fileContents = buffer.str();

  // Assert that the file does not contain "stopnowtest 99"
  assert(fileContents.find("stopnowtest 99") == std::string::npos);

  return 0;
}