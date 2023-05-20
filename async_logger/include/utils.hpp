#pragma once
#include <chrono>
#include <string>

std::string CreateNewFileName() {
    
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

  std::tm* currentDate = std::localtime(&currentTime);

  int currentYear = currentDate->tm_year + 1900; 
  int currentMonth = currentDate->tm_mon + 1;   
  int currentDay = currentDate->tm_mday;        
  std::string result = std::to_string(currentYear)+"_"+std::to_string(currentMonth)+"_"+std::to_string(currentDay)+".txt";

  return result;
}

int CurrDay() { 

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* currentDate = std::localtime(&currentTime);

    int currentDay = currentDate->tm_mday;

    return currentDay;
}


