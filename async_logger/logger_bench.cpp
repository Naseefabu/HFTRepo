#include "async_logger.hpp"
#include "utils.hpp"
using namespace std::chrono_literals;


int main(int argc, char *argv[]) {


  std::string fname = CreateNewFileName();
  Logger::SetOutput(fname);

  for(int i = 0;i<3;i++){
    Logger::Log("test", i, i * i);
    
  }


  return 0;
}