#include "Database.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>

using namespace std;
// adds real time data to real time data map
void DatabaseManager:: addRealTimeData(int slotNum, char landmark, int carNum, char via) 
{
    realTimeData[slotNum] = make_tuple(slotNum, landmark, carNum, via);
}

// updates real time data
void DatabaseManager::updateRtData(int slotNum, char landmk,int status, int carNum, char via)
  {
    auto it = realTimeData.find(slotNum);
    if (it == realTimeData.end()) {
      cout << "Slot not found" << endl;
      return;
    }

    if (get<2>(realTimeData[slotNum]) == 0) {
      get<1>(realTimeData[slotNum]) = landmk;
      get<2>(realTimeData[slotNum]) = carNum;
      get<3>(realTimeData[slotNum]) = via;
      totalAvailableSlots--;
      entryTimes[carNum] = std::chrono::system_clock::now(); // Record entry time
      cout << "Vehicle Parked successfully" << endl;
    }

    else
    {
      cout << "Slot already occupied" << endl;
    }
}

// adds entry to database
void DatabaseManager::addDbEntry(int slotNum, int carNum, string day, char via)
{
    dataBase[tEntries] = make_tuple(slotNum, carNum, day, via);
    tEntries++;
}

// imports file data into program
void DatabaseManager::importParkingInfo(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
      cerr << "Error: Unable to open file " << filename << endl;
      return;
    }
    file >> totalAvailableSlots; // taking total available slots
    cout << "Total available slots: " << totalAvailableSlots << endl;
    int slotNum, status;
    char landmark, via;
    int carNum; // Use a string to store car number

    while (file >> slotNum >> landmark >> carNum >> status >>via) { // slot num, landmark, carno., status, via
      addRealTimeData(slotNum, landmark, carNum, via);
    }
    file.close();
  }


// imports file data into program
void DatabaseManager::importDatabase(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
      cerr << "Error: Unable to open file " << filename << endl;
      return;
    }
    int slotNum, carNum;
    string day;
    char via;
    while (file >> slotNum >> carNum >> day >> via) {
      addDbEntry(slotNum, carNum, day, via);
      tEntries++;
    }
    file.close();
  }

// prints parking lot
void DatabaseManager::printParkingLot() const {
    cout << endl << "Parking Lot Info.............." << endl;
    cout << endl;
    cout << "----------------------------------------------------------"
         << endl;
    cout << "|   Slot     |   Near     |   Car      |   Via      |"
         << endl;
    cout << "----------------------------------------------------------"
         << endl;
    for (const auto &entry : realTimeData) {
        cout << "|   " << setw(9) << left << get<0>(entry.second) << "|   "
            << setw(9) << left << get<1>(entry.second) << "|   " << setw(9)
            << left << get<2>(entry.second) << "|   " << setw(9) << left
            << get<3>(entry.second) << "|   " << endl;
    }
    cout << "----------------------------------------------------------" << endl;
}



// saves database to file
void DatabaseManager::saveDatabase()
{
    ofstream file("Database.txt");
    if (!file.is_open()) {
      cerr << "Error: Unable to open file "
           << "dl_data.txt" << endl;
      return;
    }
    for (const auto &entry : dataBase) {
      file << get<0>(entry.second) << " " << get<1>(entry.second) << " "
           << get<2>(entry.second) << " " << get<3>(entry.second)
           << endl; // Include 'via'
    }
    file.close();
    cout << "database saved\n";
  }

