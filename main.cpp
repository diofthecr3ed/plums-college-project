#include "ParkingManager.h"
#include "Database.h"
#include <iostream>
using namespace std;

int main() 
{

  cout << "            (####(                                                            " << endl;
    cout << "                ((((((( &&/%((                                                  " << endl;
    cout << "                       &%(((                                                    " << endl;
    cout << "               /%&&%%%%%%%%%%%%&%                                               " << endl;
    cout << "             %&%%%%%%%%%%%%%%%%%%%%%                                             " << endl;
    cout << "           %&%%%%%%%%%%%%%%%%%%%%&%%%%                                          " << endl;
    cout << "          %%%%%%%%%%%(            .&%%%                                         " << endl;
    cout << "         %&%%%%%%%.                   %%                                        " << endl;
    cout << "         %%%%%%%,                                                               " << endl;
    cout << "        ,&%%%%%        ///////*    /.         //     //    //////     //////  " << endl;
    cout << "         %%%%%%        &&     #&   &,         &&     &&  /&  && *&   &#         " << endl;
    cout << "         %%%%%#        &&&&&&&%    &,         &&     &&  /&  && ,&    %&&&&&&#  " << endl;
    cout << "          %%%%%        &&          &&&&&&&&.  &&&&&&&&&  /&  && ,&   &&&&&&&&*  " << endl;
    cout << "           %%%%%                                                                " << endl;
    cout << "             %%%%/                                                              " << endl;
    cout << "               *%%%%                                                            " << endl;

    
  cout << "--------------------------------------------------------------------" 
  "----------------" << endl;
  cout << "Welcome to Parking Lot Management System PLUMS : )" << endl;
  cout << "Enter the day of the week: " << endl;
  string day;
  cin >> day;
  for (char &c : day) {
    c = std::tolower(c);
  }
  ParkingManager parkingLot(day,"ParkingInfo.txt", "Database.txt");

  do {
    cout << "Enter 1 to park a vehicle" << endl;
    cout << "Enter 2 to unpark a vehicle" << endl;
    cout << "Enter 3 to print parking lot" << endl;
    cout << "Enter 4 to exit" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
    case 1:
      parkingLot.Parkvehicle();
      break;
    case 2:
      parkingLot.unParkVehicle();
      break;
    case 3:
      parkingLot.viewParkingLot();
      break;
    case 4:
      parkingLot.Exit();
      cout << "Thank you for using PLUMS : )" << endl;
      cout << "----------------------------------------------------------------"
              "--------------------"
           << endl
           << endl
           << endl;
           return 0;
      return 0;
    default:
      cout << "Invalid choice" << endl;
    }
  } while (1);
  return 0;
}
