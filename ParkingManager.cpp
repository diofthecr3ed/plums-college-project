#include "ParkingManager.h"
#include <iostream>
#include <fstream>
#include <climits>
using namespace std;



ParkingManager::ParkingManager(string day, string parkingInfo, string database) : DatabaseManager(parkingInfo, database)
{
  popped = 0;
     makePriorityQueue();
    landmarkMap = {{'L', "Lift"}, {'G', "Gate"}, {'S', "Stairs"}};
    this->day = day;
    int count = 13 * 0.4;
    vector<pair<int, int>> tempPremiumSlots; // Temporary vector to store premium slots
    while (!pq.empty() && count > 0) {
      int slot = pq.top().second;
      topPremiumSlots2[slot] = true; // Mark slot as premium
      tempPremiumSlots.push_back(
          pq.top()); // Store the top premium slots in the temporary vector
      pq.pop();      // Remove the slot from the priority queue
      count--;
    }
    // Push back the popped premium slots into the priority queue
    for (auto &slot : tempPremiumSlots) {
      pq.push(slot);
    }
}
void ParkingManager::viewParkingLot()
{
    printParkingLot();
}

ParkingManager::ParkingManager(): DatabaseManager()
{
    landmarkMap = {{'L', "Lift"}, {'G', "Gate"}, {'S', "Stairs"}};
    totalAvailableSlots = 0;
};

void ParkingManager::Exit()
{
    saveDatabase();
}


void ParkingManager:: assignPremiumSlot(int carNum)
{
    if (pq.empty())
    {
        cout << "No premium slots available" << endl;
        return;
    }

    // Store the top 5 slots segregated by landmark
    map<char, vector<int>> topSlotsByLandmark;
    vector<pair<int, int>> tempPremiumSlots; // Temporary vector to store premium slots
    int count = (13 * 0.4) - popped;


    while (!pq.empty() && count > 0)
    {
        int slot = pq.top().second;
        char landmark = get<1>(realTimeData[slot]); // Get landmark of the slot
        topSlotsByLandmark[landmark].push_back(slot); // Store slot in the landmark's vector
        tempPremiumSlots.push_back(pq.top()); // Store the top premium slots in the temporary vector
        pq.pop();      // Remove the slot from the priority queue
        count--;
    }

    // Present the top 5 slots by landmark and prompt the user to choose
    for (const auto &entry : topSlotsByLandmark)
    {
        char landmark = entry.first;
        cout << "Available Slots in " << landmarkMap[landmark] << ":" << endl;
        bool availableSlots = false;
        for (int slot : entry.second)
        {
        cout << "Slot Number: " << slot << endl;
        availableSlots = true;
        }
        if (!availableSlots)
        {
        cout << "No available slots." << endl;
        continue;
        }
    }

    cout << endl << "Enter your preferred slot: ";
    int preferredSlot;
    cin >> preferredSlot;

    // Update the chosen premium slot
    updateRtData(preferredSlot, get<1>(realTimeData[preferredSlot]),1, carNum,'P'); // Update real-time data for premium slot
    addDbEntry(preferredSlot, carNum, dayMap[day],'P'); // Update database for premium slot

    // Update the priority queue pq by pushing back the remaining premium slots
    // from the temporary vector
    for (auto &slot : tempPremiumSlots) 
    {
        if (slot.second != preferredSlot)
        { // Skip the selected slot
        pq.push(slot);
        }
    }
    //int count = totalAvailableSlots * 0.4;
    ++popped;
}


void ParkingManager:: makePriorityQueue()
{
    unordered_map<int, int> occurrences;
    for (const auto &entry : dataBase)
    {
      if (get<3>(entry.second) == 'M')
      {

        occurrences[get<0>(entry.second)]++;
      }
    }
    for (const auto &entry : occurrences)
    {
      pq.push({entry.second, entry.first}); // Push {occurrences, slot_number} pair
    }
}

void ParkingManager:: autoAssingment(int carnum)
{
    int slot = findMinOccurrenceSlot();
    if (slot == -1)
    {
        cout << "No available slots" << endl;
        return;
    }
    cout << "The slot number is: " << slot << endl;
    updateRtData(slot, get<1>(realTimeData[slot]),1 ,carnum,'A'); // A == auto booking
    addDbEntry(slot, carnum, dayMap[day], 'A'); // A == auto booking
}


int ParkingManager:: findMinOccurrenceSlot() {
    unordered_map<int, int> occurrenceCount;
    for (const auto &entry :dataBase) 
    { // some modification here to only be day specific
        int num = get<0>(entry.second); // Get the first element of the tuple
        occurrenceCount[num]++;
    }
    // Find the number with minimum occurrence count
    int minOccurrence = INT_MAX;
    int minOccurrenceNumber = -1;

    for (const auto &entry : occurrenceCount)
    {
      // Check if the slot is not already occupied
      if (entry.second < minOccurrence && get<2>(realTimeData[entry.first]) == 0)
      {
        minOccurrence = entry.second;
        minOccurrenceNumber = entry.first;
      }
    }
    return minOccurrenceNumber;
}

double ParkingManager::calculateParkingDuration(std::chrono::system_clock::time_point entryTime,std::chrono::system_clock::time_point exitTime)
{
    // Calculate duration in minutes
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(exitTime - entryTime);
    return duration.count();
}


double ParkingManager::calculateParkingCost(double duration, int num, int ispremium = 0, char via = 'M') {
        double basePrice = 20.0; // Default base price for parking
        if (ispremium == 1) {
            basePrice += 30.0; // Add premium price
        }
        else if (via == 'A') {
            basePrice = 10.0; // Change the base price to 10 for auto assignment
        }
        const double priceIncrement = 3.0; // Price increment after every 15 minutes
        // Calculate total price
        double totalPrice = basePrice;
        int quarterHours = static_cast<int>(duration / 1.0); // Calculate quarter hours
        totalPrice += quarterHours * priceIncrement; // Add price increment for each quarter hour
        return totalPrice;
    }


void ParkingManager::unParkVehicle() {
        cout << "Enter car number: \n\n" << endl;
        int num;
        cin >> num;
        cout << "Enter the slot number: " << endl;
        int slotNum;
        cin >> slotNum;
        if (realTimeData.find(slotNum) == realTimeData.end()) {
            cout << "Slot not found" << endl;
            return;
        } else if (get<2>(realTimeData[slotNum]) != num) {
            cout << "Car number does not match" << endl;
            return;
        } else if (get<3>(realTimeData[slotNum]) == 0) {
            cout << "Slot is already empty" << endl;
            return;
        } else {
            // Get entry time from system clock
            auto entryTime = entryTimes[num];
            char via = get<3>(realTimeData[slotNum]); // Save the via indicator
            // Update real-time data
            get<2>(realTimeData[slotNum]) = 0;
            get<3>(realTimeData[slotNum]) = 0;
            cout << "Vehicle exited successfully" << endl;

            // Calculate duration
            auto exitTime = std::chrono::system_clock::now();
            double durationMinutes = calculateParkingDuration(entryTime, exitTime);

            // Calculate parking cost
            double cost;
            if (via == 'P') { // If premium slot
                int occurrenceCount = 0;
                for (const auto &entry : dataBase) {
                    if (get<0>(entry.second) == slotNum) {
                        occurrenceCount++;
                    }
                }
                pq.push({occurrenceCount, slotNum});
                cost = calculateParkingCost(durationMinutes, num, 1, via); // Pass via parameter
            } else { // For other slots
                cost = calculateParkingCost(durationMinutes, num, 0, via); // Pass via parameter
            }

            cout << "Duration parked: " << durationMinutes << " minutes" << endl;
            cout << "Parking cost: Rs. " << cost << endl;
        }
    }

void ParkingManager::Parkvehicle()
{
    cout << "Total available slots: " << totalAvailableSlots << endl;
    if (totalAvailableSlots == 0) {
      cout << "No available slots" << endl;
      return;
    }

    cout << "Enter car number: " << endl;
    int num;
    cin >> num;

    cout << "Enter A for automatic assignment or M for manual assignment"
         << endl;
    char choice;
    cin >> choice;

    if (choice == 'A' || choice == 'a') {
      autoAssingment(num);
    } else if (choice == 'M' || choice == 'm') {
      cout << "Want to park in premium slots? (Y/N): ";
      char premiumChoice;
      cin >> premiumChoice;
      if (premiumChoice == 'Y' || premiumChoice == 'y') {
        assignPremiumSlot(num);
      } else {
        assignNormal(num);
      } // M == manual booking
    }
}

void ParkingManager:: assignNormal(int num)
  {
    cout << "Enter the landmark (L for lift, G for Gate, S for Stairs): ";
    char landmarkChoice;
    cin >> landmarkChoice;
    if (landmarkMap.find(landmarkChoice) == landmarkMap.end()) {
      cout << "Invalid landmark choice." << endl;
      return;
    }

    // Get the top 5 premium slots from the priority queue

    cout << "Available Non-Premium Slots in " << landmarkMap[landmarkChoice]
         << ":" << endl;
    bool availableSlots = false;
    for (const auto &entry :DatabaseManager:: realTimeData) {
      int slotNumber = get<0>(entry.second);
      // Check if the slot's la ndmark matches the user's choice, if the slot is
      // unoccupied, and if the slot is not marked as premium
        if (get<1>(entry.second) == landmarkChoice && get<2>(entry.second) == 0 && topPremiumSlots2.find(slotNumber) == topPremiumSlots2.end())
        {
            availableSlots = true;
            cout << "Slot Number: " << slotNumber << endl;
        }
    }
    if (!availableSlots)
    {
      cout << "No available non-premium slots." << endl;
      return;
    }

    cout << endl << "Enter your preferred slot: ";
    int chr;
    cin >> chr;

    // Edit the chosen parking slot
    updateRtData(chr, landmarkChoice,1, num, 'M');
    addDbEntry(chr, num, dayMap[day], 'M');
  }




