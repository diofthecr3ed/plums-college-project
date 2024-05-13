#ifndef DATABASE_H
#define DATABASE_H
#include <map>
#include <tuple>
#include <unordered_map>
#include <chrono>
using namespace std;
class DatabaseManager 
{
    protected:
        map<int, tuple<int, char,int, char>> realTimeData; // slot num, landmark, carno., via
        map<int, tuple<int, int, string, char>> dataBase; 
        int tEntries = 0; // total entries in database
        void addRealTimeData(int slotNum, char landmark, int carNum, char via); // add real time data to real time data map
        void updateRtData(int slotNum, char landmk,int status, int carNum, char via); // update real time data
        void addDbEntry(int slotNum, int carNum, string day, char via) ; // add entry to database
        void importParkingInfo(const string &filename) ; // import file data into program;
        void importDatabase(const string &filename) ; // import file data into program;
        void printParkingLot() const ; // print parking lot;
        void saveDatabase(); // save database to file
        int totalAvailableSlots;   // Total available slots
        unordered_map<int, std::chrono::system_clock::time_point> entryTimes; // entry time of car
    public:
        DatabaseManager()
        {
            importParkingInfo("ParkingInfo");
            importDatabase("Database");
        }
        DatabaseManager(string parkingInfo, string database)
        {
            importParkingInfo(parkingInfo);
            importDatabase(database);
        }
};

#endif