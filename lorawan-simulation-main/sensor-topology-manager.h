#ifndef SENSOR_TOPOLOGY_MANAGER_H
#define SENSOR_TOPOLOGY_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "hapnode.h"
#include "haps-mobility-manager.h"
#include <random>
#include "end-device-manager.h"
#include "ns3/mobility-helper.h"
#include "global-variables.h"
#include "ns3/random-variable-stream.h"
#include <random>

using namespace ns3;
using namespace std;

class RingCoord
{
    public:
    double X;
    double Y;
    double Z;
    double distance;
    double angle;
};

class SensorTopologyManager {
    private:
    public:

    MobilityHelper mobility;
    CityCoordinates* map_reference_coord;
    HAPSMobilityManager hmm;
    list <CityCoordinates*> ccoList;

    void CreateSensors (NodeContainer& endDevices, string cityCoordPath);
    int CreateSensorsFromCityPoint (double x, double y, NodeContainer& endDevices, int crr_sensor_id);
    RingCoord GetNextRingLocation (double x, double y, double sphere_range);
};

#endif