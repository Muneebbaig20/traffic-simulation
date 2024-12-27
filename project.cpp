#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <conio.h>
using namespace std;

const int MAX_VERTICES = 4;

class Stack
{
private:
    int top;
    int array[MAX_VERTICES * 2];

public:
    Stack() : top(-1) {}

    bool isEmpty() 
    {
    	if (top==-1){
    		return true;
		}
       
    }

    void push(int value)
    {
        if (top < MAX_VERTICES * 2 - 1)
        {++top;
            array[top] = value;
        }
        else
        {
            cout << "Stack overflow!" << endl;
        }
    }

    int pop()
    {
        if (!isEmpty())
        {top--;
            return array[top];
        }
        else
        {
            cout << "Stack underflow!" << endl;
            return -1;
        }
    }
};

class Queue
{
private:
    int front, rear;
    int array[MAX_VERTICES * 2];

public:
    Queue() : front(-1), rear(-1) {}

    bool isFull() 
    {
        return (front == 0 && rear == MAX_VERTICES * 2 - 1) || (front == rear + 1);
    }

    bool isEmpty()  
    {
        return front == -1;
    }

    void enqueue(int value)
    {
        if (!isFull())
        {
            if (isEmpty())
            {
                front = rear = 0;
            }
            else
            {
                rear = (rear + 1) % (MAX_VERTICES * 2);
            }

            array[rear] = value;
        }
        else
        {
            cout << "Queue overflow!" << endl;
        }
    }

    int dequeue()
    {
        if (!isEmpty())
        {
            int value = array[front];

            if (front == rear)
            {
                front = rear = -1;
            }
            else
            {
                front = (front + 1) % (MAX_VERTICES * 2);
            }

            return value;
        }
        else
        {
            cout << "Queue underflow!" << endl;
            return -1;
        }
    }
};

class RoadNetwork
{
private:
public:
    int numVertices;
    bool adjMatrix[MAX_VERTICES][MAX_VERTICES];
    int trafficAmount[MAX_VERTICES];

    RoadNetwork(int vertices) : numVertices(vertices)
    {
        for (int i = 0; i < MAX_VERTICES; ++i)
        {
            for (int j = 0; j < MAX_VERTICES; ++j)
            {
                adjMatrix[i][j] = false;
            }
            trafficAmount[i] = 0;
        }
    }

    void addEdge(int src, int dest, int traffic)
    {
        if (src < MAX_VERTICES && dest < MAX_VERTICES)
        {
            adjMatrix[src][dest] = true;
            adjMatrix[dest][src] = true;
            trafficAmount[src] = traffic;
        }
        else
        {
            cout << "Invalid vertices!" << endl;
        }
    }
};

class TrafficSimulator
{
private:
public:
    RoadNetwork roadNetwork;
    Stack trafficStack;
    Queue trafficQueue;

    TrafficSimulator(int vertices) : roadNetwork(vertices) {}

    void simulateTraffic()
    {
        cout << "Simulating traffic flow using a stack..." << endl;

        for (int i = 0; i < MAX_VERTICES; ++i)
        {
            for (int j = 0; j < MAX_VERTICES; ++j)
            {
                if (roadNetwork.adjMatrix[i][j])
                {
                    trafficStack.push(i);
                    trafficStack.push(j);
                    cout << "Vehicles moving from " << getDirection(i) << " to " << getDirection(j) << endl;
                }
            }
        }
    }

    void controlTrafficLights(int greaterOutflowSide)
    {
        cout << "Controlling traffic lights using a queue..." << endl;

        // Initially open the first group of road segments
        for (int i = 0; i < MAX_VERTICES / 2; ++i)
        {
            for (int j = MAX_VERTICES / 2; j < MAX_VERTICES; ++j)
            {
                if (i == greaterOutflowSide || j == greaterOutflowSide)
                {
                    trafficQueue.enqueue(i);
                    trafficQueue.enqueue(j);
                    cout << "Traffic light turned green for vehicles from " << getDirection(i) << " to " << getDirection(j) << endl;
                }
            }
        }

        // Simulate switching to the other group of road segments after some time
        usleep(5000000); // Simulating the first group being open for 5 seconds (5000000 microseconds)

        for (int i = MAX_VERTICES / 2; i < MAX_VERTICES; ++i)
        {
            for (int j = 0; j < MAX_VERTICES / 2; ++j)
            {
                if (i == greaterOutflowSide || j == greaterOutflowSide)
                {
                    trafficQueue.enqueue(i);
                    trafficQueue.enqueue(j);
                    cout << "Traffic light turned green for vehicles from " << getDirection(i) << " to " << getDirection(j) << endl;
                }
            }
        }
    }

    void manualTrafficControl()
    {
        cout << "Enter the amount of traffic for two perpendicular road segments (north and west or east or south):" << endl;

        int north, south, east, west;
        cout << "Enter traffic amount for north segment: ";
        cin >> north;
        cout << "Enter traffic amount for south segment: ";
        cin >> south;
        cout << "Enter traffic amount for east segment: ";
        cin >> east;
        cout << "Enter traffic amount for west segment: ";
        cin >> west;

        int greaterOutflowSide;
        if (north + south > east + west)
        {
            greaterOutflowSide = 0; // North and South have greater traffic
        }
        else
        {
            greaterOutflowSide = 1; // East and West have greater traffic
        }

        cout << "Traffic flow on side " << getDirection(greaterOutflowSide) << " will be given priority." << endl;

        // Enqueue traffic lights status after manual control
        for (int i = 0; i < MAX_VERTICES; ++i)
        {
            if (i % 2 == greaterOutflowSide)
            {
                trafficQueue.enqueue(i);
                trafficQueue.enqueue(0); // Green
            }
            else
            {
                trafficQueue.enqueue(i);
                trafficQueue.enqueue(2); // Red
            }
        }

        // Enqueue traffic lights status after manual control for the perpendicular road segments
        for (int i = 1; i < MAX_VERTICES; i += 2)
        {
            for (int j = MAX_VERTICES / 2; j < MAX_VERTICES; ++j)
            {
                if (roadNetwork.adjMatrix[i][j])
                {
                    if (i % 2 == greaterOutflowSide)
                    {
                        trafficQueue.enqueue(i);
                        trafficQueue.enqueue(0); // Green
                    }
                    else
                    {
                        trafficQueue.enqueue(i);
                        trafficQueue.enqueue(2); // Red
                    }
                }
            }
        }
    }

    void showTrafficLightsStatus()
    {
        cout << "Traffic Lights Status:" << endl;

        while (!trafficQueue.isEmpty())
        {
            int roadSegment = trafficQueue.dequeue();
            int trafficStatus = trafficQueue.dequeue();

            cout << "Road segment " << getDirection(roadSegment) << ": ";

            if (trafficStatus == 0)
            {
                cout << "Traffic light is Green." << endl;
            }
            else if (trafficStatus == 1)
            {
                cout << "Traffic light is Yellow." << endl;
            }
            else
            {
                cout << "Traffic light is Red." << endl;
            }
        }
    }

    void simulateTrafficLights()
    {
        cout << "Simulating traffic and lights are at yellow" << endl;
        usleep(3000000); // Sleep for 1 second (1000000 microseconds)

        for (int i = 0; i < MAX_VERTICES; ++i)
        {
            for (int j = 0; j < MAX_VERTICES; ++j)
            {
                if (roadNetwork.adjMatrix[i][j])
                {
                    int trafficStatus = rand() % 3; // 0: Green, 1: Yellow, 2: Red
                    trafficQueue.enqueue(i);
                    trafficQueue.enqueue(trafficStatus);
                    cout << "Simulating traffic and lights are at yellow" << endl;
                    usleep(3000000); // Sleep for 1 second (1000000 microseconds)
                }
            }
        }
    }

    void handleEmergency()
    {
        char emergency;
        cout << "Do you want to stop traffic? (y/n): ";
        cin >> emergency;

        while (emergency == 'y' || emergency == 'Y')
        {
            int emergencySide;
            cout << "Enter the side number where there is an emergency (0 for North, 1 for East, 2 for South, 3 for West): ";
            cin >> emergencySide;

            // Stop traffic on all sides except the one where there is an emergency
            for (int i = 0; i < MAX_VERTICES * 2; i += 2)
            {
                if (i / 2 == emergencySide)
                {
                    trafficQueue.enqueue(i);
                    trafficQueue.enqueue(0); // Green for the emergency side
                }
                else
                {
                    trafficQueue.enqueue(i);
                    trafficQueue.enqueue(2); // Red for other sides
                }
            }

            cout << "Emergency situation! Traffic stopped on all sides except the affected side." << endl;

            cout << "Is there another emergency situation? (y/n): ";
            cin >> emergency;
        }
    }

    string getDirection(int side)
    {
        switch (side)
        {
        case 0:
            return "North";
        case 1:
            return "East";
        case 2:
            return "South";
        case 3:
            return "West";
        default:
            return "Invalid Direction";
        }
    }

    void generateChallan(int vehicleType,   string &ownerName,   string &registrationNumber)
    {
        // Assuming vehicleType: 1 for Car, 2 for Motorcycle, 3 for Truck, etc.
        double fineAmount = 0.0;

        switch (vehicleType)
        {
        case 1: // Car
            fineAmount = 50.0;
            break;
        case 2: // Motorcycle
            fineAmount = 30.0;
            break;
        case 3: // Truck
            fineAmount = 100.0;
            break;
        // Add more cases for other vehicle types as needed
        default:
            fineAmount = 20.0; // Default fine for an unknown vehicle type
            break;
        }

        // Print the challan details
        cout << "\n\n\t\t===== Traffic Challan =====" << endl;
        cout << "Owner Name: " << ownerName << endl;
        cout << "Registration Number: " << registrationNumber << endl;
        cout << "Vehicle Type: " << getVehicleTypeDescription(vehicleType) << endl;
        cout << "Fine Amount: Rs." << fineAmount << endl;
        cout << "Payment Instructions: Pay the fine within 15 days to avoid additional charges." << endl;
        cout << "============================" << endl;
    }

    string getVehicleTypeDescription(int vehicleType)
    {
        switch (vehicleType)
        {
        case 1:
            return "Car";
        case 2:
            return "Motorcycle";
        case 3:
            return "Truck";
        // Add more descriptions for other vehicle types as needed
        default:
            return "Unknown Vehicle Type";
        }
    }
};
enum TrafficLightState
{
    RED_NORTH_SOUTH,
    GREEN_NORTH_SOUTH,
    RED_EAST_WEST,
    GREEN_EAST_WEST
};

// Function to display the current state of the traffic light
void displayTrafficLight(TrafficLightState state)
{
    cout << "Lights are at yellow" << endl;
    usleep(3000000); // Sleep for 1 second (1000000 microseconds)
    system("cls");   // Clear the console (for Windows)

    cout << "Traffic Light State: ";
    switch (state)
    {
    case RED_NORTH_SOUTH:
        cout << "INTERSECTION 1- RED (North/South) - GREEN (East/West)" << endl;
         cout << "INTERSECTION 2-GREEN (North/South) - RED (East/West)" << endl;
        break;
    case GREEN_NORTH_SOUTH:
        cout << "INTERSECTION 1- GREEN (North/South) - RED (East/West)" << endl;
          cout << "INTERSECTION 2- GREEN (North/South) - RED (East/West)" << endl;
        break;
    case RED_EAST_WEST:
        cout << "INTERSECTION 1- GREEN (North/South) - RED (East/West)" << endl;
         cout << "INTERSECTION 2- RED (North/South) - GREEN (East/West)" << endl;
        break;
    case GREEN_EAST_WEST:
        cout << "INTERSECTION 1- RED (North/South) - GREEN (East/West)" << endl;
        cout << "INTERSECTION 2- RED (North/South) - GREEN (East/West)" << endl;
        break;
    }
}

int main()
{
	
	cout <<"-----------------------------------------------------------------------------------"<<endl<<endl<<endl<<endl;
    srand(time(0)); // Seed for random number generation
    RoadNetwork roadNetwork(4);

    roadNetwork.addEdge(0, 1, 0);
    roadNetwork.addEdge(1, 0, 0);
    roadNetwork.addEdge(2, 3, 0);
    roadNetwork.addEdge(3, 2, 0);

    TrafficSimulator trafficSimulator(4);

    cout << "\t\t\t*' *' '* HEARTY WELCOME TO '*' '* *' '**" << endl;
    cout << "\t\t\t*' *'*' '*";
    cout << "*' *'*' '*";
    cout << "*' *'*'' *' '** " << endl;
    cout << "\t\t\t*' *'*' TRAFFIC STIMULATION SYSTEM '' *' '**" << endl;

    cout << "\t\t\t*' *'*' '*";
    cout << "*' *'*' '*";
    cout << "*' *'*'' *' '** ";

    cout << "\n";
    char choice;
    int chce;
    do
    {
        cout << "*' ENTER YOUR DESIRED OPTION: '*" << endl;
        cout << "*' Press 1 to enter manual traffic stimulation mode '*" << endl;
        cout << "*' Press 2 to get record of challan '*" << endl;
        cout << "*' Press 3 If you require HELP '*" << endl;
        cout << "*' Press 4 If there is an emergency '*" << endl;
        cout << "*' Press 5 for automatic timing for each traffic light '*" << endl
             << endl
             << endl;
        cin >> chce;

        if (chce == 1)
        {
            trafficSimulator.manualTrafficControl();
            trafficSimulator.simulateTrafficLights();
            trafficSimulator.showTrafficLightsStatus();
        }
        else if (chce == 2)
        {
            int vehicleType;
            string ownerName, registrationNumber;

            cout << "Enter vehicle type (1 for Car, 2 for Motorcycle, 3 for Truck, etc.): ";
            cin >> vehicleType;
            cout << "Enter owner name: ";
            cin.ignore();
            getline(cin, ownerName);
            cout << "Enter registration number: ";
            getline(cin, registrationNumber);

            trafficSimulator.generateChallan(vehicleType, ownerName, registrationNumber);
        }
        else if (chce == 3)
        {
            cout << "Contact Information:" << endl;
            cout << "Emergency Contact: 911" << endl;
            cout << "Traffic Booth Information:" << endl;
            cout << "Booth 1 - North: Contact - 925-5026-22-1" << endl;
            cout << "Booth 2 - East: Contact - 925-5030-22-2" << endl;
            cout << "Booth 3 - South: Contact - 925-5095-22-3" << endl;
            cout << "Booth 4 - West: Contact - 925-5026-22-1" << endl;
        }
        else if (chce == 4)
        {
            trafficSimulator.handleEmergency();
        }
        else if (chce == 5)
        {
            TrafficLightState currentState = RED_NORTH_SOUTH;
        c:

            for (int i = 0; i <= 3; ++i)
            {
                displayTrafficLight(currentState);

                // Sleep for a short duration to simulate time passing
               // usleep(2000000); // 5 seconds

                // Change the state of the traffic light
                switch (currentState)
                {
                case RED_NORTH_SOUTH:
                    currentState = GREEN_NORTH_SOUTH;
                    break;
                case GREEN_NORTH_SOUTH:
                    currentState = RED_EAST_WEST;
                    break;
                case RED_EAST_WEST:
                    currentState = GREEN_EAST_WEST;
                    break;
                case GREEN_EAST_WEST:
                    currentState = RED_NORTH_SOUTH;
                    break;
                }
            }
            int a, b;
            //
            cout << "Press 2 to continue... and Press 1 to go to Main menu";
            cin >> a;
            if (a == 1)
            {
                goto b; // to restart program
            }
            else if (a == 2)
            {

                goto c; // to again run stimulation
            }
            else
            {
                break;
            }
            // This will wait for the user to press any key before continuing
        }
    b:
        cout << "Do you want to enter traffic control again? (y/n): " << endl
             << endl
             << endl;
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Exiting the program. Thank you!" << endl;

    return 0;
}