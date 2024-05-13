#ifndef PARKINGMANAGER_H
#define PARKINGMANAGER_H

#include <map>
#include <tuple>
#include <vector>
#include <queue>
#include <unordered_map>
#include <chrono>
# include "Database.h"

using namespace std;


struct CompareOccurrences {
  bool operator()(const pair<int, int> &a, const pair<int, int> &b)
  {
    return a.first < b.first;
  }
};

class ParkingManager : private DatabaseManager
{
    private:
    int popped;
        map<int, bool> topPremiumSlots2;
        map<char, string> landmarkMap; // map of landmarks
        string day; // current day
        map<string, string> dayMap; // map of days
        priority_queue<pair<int, int>, vector<pair<int, int>>, CompareOccurrences> pq; // no_of_occurances , slot_no

        

        void assignPremiumSlot(int carNum); // assign premium slot to car
        void makePriorityQueue(); // make priority queue
        void assignNormal(int num) ; // assign normal slot to car
        void autoAssingment(int carNum); // auto assign slot to car
        int findMinOccurrenceSlot(); // find slot with minimum occurrence
        double calculateParkingDuration(std::chrono::system_clock::time_point entryTime,std::chrono::system_clock::time_point exitTime) ; // calculate parking duration
        double calculateParkingCost(double duration, int num, int ispremium, char via) ; // calculate parking cost
    public:
        ParkingManager(string day, string parkingInfo, string database);
        ParkingManager();
        void Parkvehicle(); // park vehicle
        void unParkVehicle() ; // unpark vehicle
        void viewParkingLot(); // view parking lot
        void Exit() ; // exit

};

#endif
