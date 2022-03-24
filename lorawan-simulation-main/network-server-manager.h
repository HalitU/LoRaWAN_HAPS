#ifndef NETWORK_SERVER_MANAGER_H
#define NETWORK_SERVER_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/network-server-helper.h"
#include "ns3/forwarder-helper.h"

using namespace ns3;
using namespace lorawan;
using namespace std;

class NetworkServerManager {
    private:
    public:
    NodeContainer InstallNetworkServer (uint32_t nNetworkservers, MobilityHelper mobility, NetworkServerHelper nsHelper
        , NodeContainer endDevices, NodeContainer gateways, ForwarderHelper forHelper);
};

#endif