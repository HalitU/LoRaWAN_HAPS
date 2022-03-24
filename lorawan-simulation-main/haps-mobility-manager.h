#ifndef HAPS_MOBILITY_MANAGER_H
#define HAPS_MOBILITY_MANAGER_H

#include <chrono> 
#include <string>
#include <cmath>
#include "ns3/core-module.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/constant-position-mobility-model.h"

#include "hapnode.h"
#include "global-variables.h"

using namespace ns3;
using namespace lorawan;
using namespace std;


struct CoordDifference {
    public:
    double lat;
    double lon;
    double angle; // radian please
};


class HAPSMobilityManager {
    private:
    public:
    static double distanceBetweenCoords;

    void ReadHAPSMobilityFile (std::string coordinate_path, Ptr<HAPNode> gateway, double simDuration);
    void UpdateMovementDirection (Ptr<HAPNode> gateway);
    static CoordDifference* CalculateDistanceBetweenTwoCoords (CityCoordinates begin, CityCoordinates end); // with abs
    CoordDifference* CalculateRealDistanceBetweenTwoCoords (CityCoordinates begin, CityCoordinates end);
    void PrepareHAPSCoordTimers (Ptr<HAPNode> gateway, double simDuration);
    static double CalculateDistanceBetweenNodes (Ptr<HAPNode> gw, Ptr<HAPNode> ed);
    void RecordHAPSMovement (Ptr<HAPNode> gateway);
};

#endif