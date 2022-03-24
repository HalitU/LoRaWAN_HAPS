#include "sensor-topology-manager.h"

void SensorTopologyManager::CreateSensors (NodeContainer& endDevices, string cityCoordPath) {
    NS_LOG_UNCOND ("Reading the city coordinate file for creating sensors...");

    // Get the file
    std::ifstream in (cityCoordPath);
    std::string line;

    if (!in){
      NS_LOG_UNCOND ("File can't be opened!");
      exit(1);
    }

    // Read the file contents and get the coordinates
    bool map_reference = true;
    map_reference_coord = new CityCoordinates;
    while (getline(in, line))
    {
        istringstream ss (line);
        if (map_reference) {
            ss >> map_reference_coord->lat >> map_reference_coord->lon;
            map_reference = false;
        }
        else {
            CityCoordinates* cco = new CityCoordinates;
            ss >> cco->lat >> cco->lon;
            ccoList.push_back (cco);
        }
    }   

    // Create the sensors for each coordinate aside from the map reference point
    // in a sphere distribution format
    int crr_sensor_id = 1;

    for (CityCoordinates* const& iter : ccoList) {
        if (GlobalVariables::DEBUG) {
            NS_LOG_UNCOND (iter->lat << " " << iter->lon << " " << iter->visited);
        }
        // Get the location of the coordinate according to map reference
        CoordDifference* cd = hmm.CalculateDistanceBetweenTwoCoords (*map_reference_coord, *iter);
        // Create the sensors from this location
        int createdEDCount = CreateSensorsFromCityPoint (cd->lat, cd->lon, endDevices, crr_sensor_id);

        // increment sensor count
        crr_sensor_id += createdEDCount;
    }        
};


int SensorTopologyManager::CreateSensorsFromCityPoint (double x, double y, NodeContainer& endDevices, int crr_sensor_id) {
    // Custom device manager for creating basic sensor node
    EndDeviceManager edm;

    // Set mobility helper model to constant position
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

    // Normal distribution settings
    // https://www.nsnam.org/doxygen/classns3_1_1_normal_random_variable.html
    // double bound = 3000.0;
    double shared_sigma = GlobalVariables::dist_sigma;

    std::random_device mch;
    // X position
    double mean_x = x;
    std::default_random_engine generator (mch());
    std::normal_distribution<double> distribution_x (mean_x, shared_sigma);

    // Y position
    double mean_y = y;
    std::normal_distribution<double> distribution_y (mean_y, shared_sigma);

    

    int i;
    for (i=0;i<GlobalVariables::minCityEDCount;i++) {

        RingCoord nextCoord;
        nextCoord.X = distribution_x (generator);
        nextCoord.Y = distribution_y (generator);
        nextCoord.Z = 0;

        // Fixing locations
        int tries = 0;
        while (nextCoord.X > x + GlobalVariables::range_margin or nextCoord.X < x - GlobalVariables::range_margin) {
            nextCoord.X = distribution_x (generator);
            tries++;
            // if (tries > 20) {
            //     NS_ASSERT (false);
            // }
        }
        tries = 0;
        while (nextCoord.Y > y + GlobalVariables::range_margin or nextCoord.Y < y - GlobalVariables::range_margin) {
            nextCoord.Y = distribution_y (generator);
            tries++;
            // if (tries > 20) {
            //     NS_ASSERT (false);
            // }
        }

        // Create the sensor
        Ptr<HAPNode> newNode = edm.CreateSingleEndDevice ();
        
        newNode->sensor_id = crr_sensor_id;

        // Install its mobility model
        mobility.Install (newNode);

        // Update its coordinates
        Ptr<ConstantPositionMobilityModel> mobilityModel = newNode->GetObject<ConstantPositionMobilityModel>();
        Vector currrentPosition = mobilityModel->GetPosition();

        currrentPosition.x = nextCoord.X;
        currrentPosition.y = nextCoord.Y;
        currrentPosition.z = nextCoord.Z;
        mobilityModel->SetPosition (currrentPosition);

        // Energy
        newNode->expectedNumberOfMessages = GlobalVariables::expectedNumberOfMessages;
        newNode->currentMessageCount = 0;

        newNode->SetNodeType ("ED");
        newNode->_customDeviceEnergyModel._consumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._sleepConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._awakeningConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._activeModeConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._collectionModeConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._shutDownModeConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._totalTransmitted = 0;
        newNode->_customDeviceEnergyModel._totalCollected = 0;
        newNode->_customDeviceEnergyModel._totalAttempted = 0;
        newNode->_customDeviceEnergyModel._totalPacketAttempted = 0;
        newNode->_customDeviceEnergyModel._totalSentBytes = 0.0;
        newNode->_customDeviceEnergyModel._lastStateUpdateTime = 0.0;
        newNode->_customDeviceEnergyModel._lastEnergyControlTime = 0.0;
        newNode->_customDeviceEnergyModel._someUpdate = 0;
        newNode->_customDeviceEnergyModel._currentState = CustomDeviceState::SLEEPING;

        // Setup the device energy model for the node
        // A scheduling is required to update this devices state (not the lorawan device state!)
        // lorawan state changes and the external device changes can be used together to
        // calculate the total consumed energy
        newNode->_customDeviceEnergyModel.SetSupplyVoltage(GlobalVariables::EDsupplyVoltage);
        newNode->_customDeviceEnergyModel.SetActiveCurrent (GlobalVariables::EDactiveCurrent);
        newNode->_customDeviceEnergyModel.SetAwakeningCurrent (GlobalVariables::EDawakeningCurrent);
        newNode->_customDeviceEnergyModel.SetCollectingCurrent (GlobalVariables::EDcollectionCurrent);
        newNode->_customDeviceEnergyModel.SetShuttingDownCurrent (GlobalVariables::EDshuttingdownCurrent);
        newNode->_customDeviceEnergyModel.SetSleepCurrent (GlobalVariables::EDsleepCurrent);
        
        newNode->_customDeviceEnergyModel.SetActiveTime (GlobalVariables::EDactiveDuration);
        newNode->_customDeviceEnergyModel.SetAwakeningTime (GlobalVariables::EDawakeningDuration);
        newNode->_customDeviceEnergyModel.SetCollectingTime (GlobalVariables::EDcollectionDuration);
        newNode->_customDeviceEnergyModel.SetShuttingDownTime (GlobalVariables::EDshuttingdownDuration);
        newNode->_customDeviceEnergyModel.SetSleepTime (GlobalVariables::EDsleepingDuration);     

        // Initialize the counters for tx logs
        newNode->reset_last_in_range_time ();
        newNode->reset_last_out_range_time ();
        newNode->reset_current_range_transmitted_data ();

        // Add it to the container
        endDevices.Add (newNode);

        // Increment identifier
        crr_sensor_id++;        
    }

    return GlobalVariables::minCityEDCount;
};

/**
 * Use this to generate random variables within 360 degrees without any specific rules
 * Ring: https://stackoverflow.com/questions/16153589/generating-a-uniform-random-integer-in-c
 * Random number generation: https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
 */
RingCoord SensorTopologyManager::GetNextRingLocation (double x, double y, double sphere_range)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Generate angle with radians
    std::uniform_real_distribution<double> angleDist (0, GlobalVariables::fullCircle);

    double angle = angleDist (gen);

    // double angle = (1.0 * rand() / RAND_MAX) * ( gv.fullCircle + 1 );

    // Calculate the X and Y positions and give default Z
    // DONT forget that Y is reversed in ns3
    RingCoord coord;
    coord.X = x + 1.0 * sphere_range * cos (angle);
    coord.Y = y - 1.0 * sphere_range * sin (angle);
    coord.Z = 0;
    coord.distance = sphere_range;
    coord.angle = angle;

    // Return the result
    return coord;
};
