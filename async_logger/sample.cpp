#include "async_logger.hpp"
#include "utils.hpp"
using namespace std::chrono_literals;


int main(int argc, char *argv[]) {
  Logger logg;
  // filename based on current date
  std::string fname = CreateNewFileName(); 
  logg.SetOutput(fname);

  for(int64_t i = 0;i<30;i++){
    logg.Log("test", i);
    //logg.stop();
  }
  
  return 0;
}