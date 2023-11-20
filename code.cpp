#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

struct Flight {
    string flightNumber;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int completionTime;
    vector<string> boardedPassengers;
};

struct Passenger {
    string flightNumber;
    string passengerName;
    int arrivalTime;
    int burstTime;
};

void fcfs(vector<Flight>& flights) {
    int currentTime = 0;

    sort(flights.begin(), flights.end(), [currentTime](const Flight& a, const Flight& b) {
        if (a.arrivalTime <= currentTime && b.arrivalTime <= currentTime) {
            return a.arrivalTime < b.arrivalTime;
        } else if (a.completionTime > currentTime && b.completionTime > currentTime) {
            return a.waitingTime > b.waitingTime;
        } else {
            return a.completionTime < b.completionTime;
        }
    });

    for (Flight& flight : flights) {
        while (currentTime < flight.arrivalTime) {
            currentTime++;
        }

        for (Flight& otherFlight : flights) {
            if (otherFlight.completionTime == currentTime && otherFlight.arrivalTime < currentTime) {
                otherFlight.completionTime += 2;
            }
        }

        flight.completionTime = currentTime + flight.burstTime;

        cout << "Flight " << flight.flightNumber << " has arrived at time " << currentTime
             << ". Will depart at time " << flight.completionTime << ".\n";

        for (Flight& otherFlight : flights) {
            if (otherFlight.arrivalTime < currentTime && otherFlight.completionTime > currentTime) {
                otherFlight.waitingTime++;
            }
        }

        currentTime = flight.completionTime;

        cout << "Flight " << flight.flightNumber << " has departed at time " << currentTime << ".\n";
    }
}

void sjf(vector<Passenger>& passengers, vector<Flight>& flights) {
    int currentTime = 0;

    sort(passengers.begin(), passengers.end(), [](const Passenger& a, const Passenger& b) {
        return make_tuple(a.arrivalTime, a.burstTime) < make_tuple(b.arrivalTime, b.burstTime);
    });

    for (const Passenger& passenger : passengers) {
        currentTime = 0;

        while (currentTime < passenger.arrivalTime) {
            currentTime++;
        }

        cout << "Passenger " << passenger.passengerName << " for Flight " << passenger.flightNumber
             << " has arrived at time " << currentTime << ".\n";

        for (Flight& flight : flights) {
            if (flight.flightNumber == passenger.flightNumber) {
                flight.boardedPassengers.push_back(passenger.passengerName);
            }
        }

        currentTime += 1;

        cout << "Passenger " << passenger.passengerName << " for Flight " << passenger.flightNumber
             << " has left at time " << currentTime << ".\n";
    }
}

void printBoardedPassengers(const vector<Flight>& flights) {
    cout << "Final List of Boarded Passengers:\n";
    for (const Flight& flight : flights) {
        cout << "Flight " << flight.flightNumber << " - ";
        if (flight.boardedPassengers.empty()) {
            cout << "No passengers boarded.\n";
        } else {
            cout << "Passengers: ";
            for (const string& passenger : flight.boardedPassengers) {
                cout << passenger << " ";
            }
            cout << "\n";
        }
    }
}

tuple<vector<Flight>, vector<Passenger>> generateRandomDataset(int numFlights, int numPassengersPerFlight) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> arrivalTimeDist(0, 100);
    uniform_int_distribution<> burstTimeDist(1, 200);

    vector<Flight> flights;
    vector<Passenger> passengers;

    for (int i = 0; i < numFlights; ++i) {
        Flight flight;
        flight.flightNumber = "F" + to_string(i + 1);
        flight.arrivalTime = arrivalTimeDist(gen);
        flight.burstTime = burstTimeDist(gen);
        flight.waitingTime = 0;
        flight.completionTime = 0;
        flights.push_back(flight);

        for (int j = 0; j < numPassengersPerFlight; ++j) {
            Passenger passenger;
            passenger.flightNumber = flight.flightNumber;
            passenger.passengerName = "P" + to_string(j + 1);
            passenger.arrivalTime = arrivalTimeDist(gen);
            passenger.burstTime = 0;
            passengers.push_back(passenger);
        }
    }

    return make_tuple(flights, passengers);
}

int main() {
    int numFlights = 50;
    int numPassengersPerFlight = 100;
    auto dataset = generateRandomDataset(numFlights, numPassengersPerFlight);

    vector<Flight> flights = get<0>(dataset);
    vector<Passenger> passengers = get<1>(dataset);

    fcfs(flights);

    cout << "\nFlight Arrival and Departure Times:\n";
    for (const Flight& flight : flights) {
        cout << "Flight " << flight.flightNumber << " - Arrival: " << flight.arrivalTime
             << ", Departure: " << flight.completionTime << "\n";
    }

    for (Passenger& passenger : passengers) {
        for (const Flight& flight : flights) {
            if (passenger.flightNumber == flight.flightNumber) {
                passenger.burstTime = flight.completionTime - passenger.arrivalTime;
            }
        }
    }

    sjf(passengers, flights);

    printBoardedPassengers(flights);
    return 0;
}
