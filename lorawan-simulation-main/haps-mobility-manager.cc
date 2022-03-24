#include "haps-mobility-manager.h"

// lat => parallel, lon => shrinking
double HAPSMobilityManager::distanceBetweenCoords; // = 111000;// 111000;

void HAPSMobilityManager::ReadHAPSMobilityFile (std::string coordinate_path, Ptr<HAPNode> gateway, double simDuration) {
    if (GlobalVariables::DEBUG) {
        NS_LOG_UNCOND ("Reading the coordinate file...");
    }

    // Get the file
    std::ifstream in (coordinate_path);
    std::string line;

    if (!in){
      NS_LOG_UNCOND ("File can't be opened! " << coordinate_path);
      exit(1);
    }

    bool map_reference = true;
    gateway->map_reference = new CityCoordinates;
    while (getline(in, line))
    {
        istringstream ss (line);
        if (map_reference) {
            ss >> gateway->map_reference->lat >> gateway->map_reference->lon;
            map_reference = false;
        }
        else {
            CityCoordinates* cco = new CityCoordinates;
            ss >> cco->lat >> cco->lon;
            gateway->ccoList.push_back (cco);
        }
    }

    if (GlobalVariables::DEBUG) {
        NS_LOG_UNCOND ("Coordinations read successfully...");
    }

    gateway->ccoList.front ()->initial = true;
    gateway->ccoList.front ()->visited = true;

    // Set gateways location according to reference and HAPS initial location
    CoordDifference* cd = CalculateDistanceBetweenTwoCoords (*gateway->map_reference, *gateway->ccoList.front ());
    Ptr<MobilityModel> mobility = gateway->GetObject<MobilityModel> ();
    Vector position = mobility->GetPosition ();
    position.x = cd->lat;
    position.y = cd->lon;
    position.z = 20000.0;
    mobility->SetPosition (position);

    gateway->current_coords = gateway->ccoList.front ();
    gateway->target_coords = gateway->ccoList.front ();

    if (GlobalVariables::DEBUG) {
        NS_LOG_UNCOND ("Coordinations for GW: " << gateway->GetId () << " read successfully...");
        NS_LOG_UNCOND ("Initial locations are (x, y, z): " << position.x << " " << position.y << " " << position.z);
    }

    // Prepare the timers from the read list
    PrepareHAPSCoordTimers (gateway, simDuration);

    // Schedule the initial movement update
    Simulator::Schedule (Seconds (gateway->haps_delay), &HAPSMobilityManager::UpdateMovementDirection, this, gateway);
    Simulator::Schedule (Seconds (GlobalVariables::HAPSMovementCalculationSeconds), &HAPSMobilityManager::RecordHAPSMovement, this, gateway);
};

// https://www.nsnam.org/doxygen/classns3_1_1_constant_velocity_mobility_model.html
void HAPSMobilityManager::UpdateMovementDirection (Ptr<HAPNode> gateway) {
    bool allFinished = true;
    HAPSCoordTimers* nextTimer;
    for (HAPSCoordTimers* const& timer : gateway->hapsCoordTimers) {
        if (timer->finished == false) {
            nextTimer = timer;
            allFinished = false;
            break;
        }
    };

    // Reset the finished status if all timers are completed
    if (allFinished == true) {
        // Also reset the visited status for next cycle
        for (HAPSCoordTimers* const& timer : gateway->hapsCoordTimers) {        
            timer->finished = false;
        }
        nextTimer = gateway->hapsCoordTimers.front ();
    }

    // This timer will definitely be processed.
    nextTimer->finished = true;
    // NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " || Current speed and timer: " << nextTimer->lat_speed << " " << nextTimer->lon_speed << " " << nextTimer->timer);

    // Update the speed and set next scheduling
    Ptr <ConstantVelocityMobilityModel> mobility = gateway->GetObject<ConstantVelocityMobilityModel>();
    mobility->SetVelocity( Vector (nextTimer->lat_speed, nextTimer->lon_speed, 0));

    // Get current location and log it
    Vector position = mobility->GetPosition ();    
    gateway->experimentGatewayMobilityFile << Simulator::Now (). GetSeconds () 
        << "\t" << position.x << "\t" << position.y << "\t" << position.z << std::endl;

    // Schedule the movement update for the expected arrival on the next location
    // TODO: Since only one direction is required for the grid experiment, no need to update the speed after
    // TODO: the initial setting.
    // Simulator::Schedule (Seconds (nextTimer->timer), &HAPSMobilityManager::UpdateMovementDirection, this, gateway);
};

void HAPSMobilityManager::RecordHAPSMovement (Ptr<HAPNode> gateway) {
    // Record current location
    Ptr <ConstantVelocityMobilityModel> mobility = gateway->GetObject<ConstantVelocityMobilityModel>();
    Vector position = mobility->GetPosition ();    
    gateway->experimentGatewayMobilityFile << Simulator::Now (). GetSeconds () 
        << "\t" << position.x << "\t" << position.y << "\t" << position.z << std::endl;    

    // Schedule the next second
    Simulator::Schedule (Seconds (GlobalVariables::HAPSMovementCalculationSeconds), &HAPSMobilityManager::RecordHAPSMovement, this, gateway);
}

void HAPSMobilityManager::PrepareHAPSCoordTimers (Ptr<HAPNode> gateway, double simDuration) {
    double default_speed = GlobalVariables::default_speed;
    double elapsedTime = 0.0;
    
    if (GlobalVariables::DEBUG) {
        NS_LOG_UNCOND ("Preparing coordinates for haps with id: " << gateway->GetId ());
    }


    while (simDuration > elapsedTime) {
        gateway->target_coords->visited = true;

        // Read the next unvisited coordinates or if all visited initial coordinates and update x and y speed
        // according to distance and required speed
        // First 
        CityCoordinates* nextCoord;
        bool allVisited = true;
        for (CityCoordinates* const& iter : gateway->ccoList) {
            if (iter->visited == false) {
                nextCoord = iter;
                allVisited = false;
                break;
            }
        }    

        // Update current as target
        gateway->current_coords = gateway->target_coords;

        // if last city is visited target is maintenance location
        if (allVisited == true) {
            gateway->target_coords = gateway->ccoList.front ();
            // Also reset the visited status for next cycle
            for (CityCoordinates* const& iter : gateway->ccoList) {        
                iter->visited = false;
            }
        }
        // otherwise target is next city
        else {
            gateway->target_coords = nextCoord;
        }

        // Update speed according to new target coordinates
        CoordDifference* cd = CalculateRealDistanceBetweenTwoCoords (*gateway->current_coords, *gateway->target_coords);

        double newXVelocity = default_speed * cos (cd->angle);
        double newYVelocity = default_speed * sin (cd->angle);

        double expectedArrival_X = std::abs (cd->lat / newXVelocity);
        double expectedArrival_Y = std::abs (cd->lon / newYVelocity);

        if (GlobalVariables::DEBUG) {
            NS_LOG_UNCOND ("Velocity: [" << newXVelocity << ", " << newYVelocity << "]" << " "
                << "Expected arrival: [" << expectedArrival_X << ", " << expectedArrival_Y << "]");
        }

        if (expectedArrival_X + expectedArrival_Y < 1.0) {
            throw exception ();
        }

        Ptr <ConstantVelocityMobilityModel> mobility = gateway->GetObject<ConstantVelocityMobilityModel>();
        mobility->SetVelocity( Vector (newXVelocity, newYVelocity, 0));

        // Schedule the movement update for the expected arrival on the next location
        HAPSCoordTimers* newCoordTimer = new HAPSCoordTimers;
        newCoordTimer->lat_speed = newXVelocity;
        newCoordTimer->lon_speed = newYVelocity;
        newCoordTimer->finished = false;

        if (expectedArrival_X > 0) {
            newCoordTimer->timer = expectedArrival_X;
            elapsedTime += expectedArrival_X;
        }
        else {
            newCoordTimer->timer = expectedArrival_Y;
            elapsedTime += expectedArrival_Y;
        }

        gateway->hapsCoordTimers.push_back (newCoordTimer);
    };
};


CoordDifference* HAPSMobilityManager::CalculateDistanceBetweenTwoCoords (CityCoordinates begin, CityCoordinates end) {
    CoordDifference* cd = new CoordDifference;

    cd->lon = std::abs (begin.lon - end.lon) * distanceBetweenCoords;
    cd->lat = std::abs (begin.lat - end.lat) * distanceBetweenCoords;

    return cd;
}

CoordDifference* HAPSMobilityManager::CalculateRealDistanceBetweenTwoCoords (CityCoordinates begin, CityCoordinates end) {
    CoordDifference* cd = new CoordDifference;

    cd->lat = (end.lat - begin.lat) * distanceBetweenCoords;
    cd->lon = -1.0 * (end.lon - begin.lon) * distanceBetweenCoords;
    cd->angle = atan2 (cd->lon, cd->lat);

    return cd;
};

double HAPSMobilityManager::CalculateDistanceBetweenNodes (Ptr<HAPNode> gw, Ptr<HAPNode> ed)
{
    Ptr<ConstantVelocityMobilityModel> gatewayMobilityModel = gw->GetObject<ConstantVelocityMobilityModel>();
    Ptr<ConstantPositionMobilityModel> enddeviceMobilityModel = ed->GetObject<ConstantPositionMobilityModel>();

    return gatewayMobilityModel->GetDistanceFrom (enddeviceMobilityModel);
};
