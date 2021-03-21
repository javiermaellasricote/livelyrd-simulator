#include <iostream>
#include "MongoDb/MongoDb.h"
#include "SimulationCreator.h"

int main() {
    std::string environmentName, customerName;
    std::cout << "Username:\t";
    std::cin >> customerName;
    std::cout << "Environment to simulate:\t";
    std::cin >> environmentName;
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{}};

    mongocxx::database customerDb = client[customerName];
    livelyRD::SimulationCreator simulation(environmentName, customerDb[environmentName], instance);

    const auto& environment = simulation.getSimulationEnvironment();
    environment->solve();
    std::cout << "Environment has been simulated." << std::endl;
    return 0;
}
