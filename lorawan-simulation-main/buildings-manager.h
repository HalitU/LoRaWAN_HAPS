#ifndef BUILDINGS_MANAGER_H
#define BUILDINGS_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/buildings-helper.h"
#include "ns3/building-allocator.h"
#include "ns3/building-penetration-loss.h"
#include "ns3/correlated-shadowing-propagation-loss-model.h"

#include "math.h"

#include "sensor-topology-manager.h"
#include "global-variables.h"

using namespace std;
using namespace ns3;

class BuildingsManager {
    private:
    
    public:
    void CreateBuildingsFromFile (string path);
    void RegisterBuildingsToNodes (NodeContainer enddevices, NodeContainer gateways);
    void DistributeRandomClouds (SensorTopologyManager stm);
};

#endif