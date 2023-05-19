#pragma once
#include <chrono>
#include <string>

std::string CreateNewFileName() {
  // Get the current system time
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

  // Convert the current time to a struct tm
  std::tm* currentDate = std::localtime(&currentTime);

  // Extract the year, month, and day from the struct tm
  int currentYear = currentDate->tm_year + 1900; 
  int currentMonth = currentDate->tm_mon + 1;   
  int currentDay = currentDate->tm_mday;        
  std::string result = std::to_string(currentYear)+"_"+std::to_string(currentMonth)+"_"+std::to_string(currentDay)+".txt";

  return result;
}