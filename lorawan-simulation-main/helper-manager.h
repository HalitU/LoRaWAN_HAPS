#ifndef HELPER_MANAGER_H
#define HELPER_MANAGER_H

#include "ns3/lora-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/forwarder-helper.h"
#include "ns3/network-server-helper.h"

#include "hapnode.h"
#include "global-variables.h"

using namespace ns3;
using namespace std;
using namespace lorawan;

class HelperManager {
    private:
    public:
    LoraPhyHelper phyHelper;
    LorawanMacHelper macHelper;
    LoraHelper helper;
    NetworkServerHelper nsHelper;
    ForwarderHelper forHelper;
    MobilityHelper mobility;

    void InitializeHelpers (Ptr<LoraChannel> channel);
    void InitializeSimulationRandomness ();
    void SetupEndDeviceStateTrackers (NodeContainer endDevices, GlobalVariables* gv);
    void SetupGatewayMobilityTrackers (NodeContainer gateways, GlobalVariables* gv);
};

#endif