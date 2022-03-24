#ifndef GATEWAY_MANAGER_H
#define GATEWAY_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lora-helper.h"
#include "hapnode.h"

using namespace ns3;
using namespace lorawan;

class GatewayManager {
    private:
    public:
    NodeContainer CreateGateways (uint32_t nGateways, MobilityHelper mobility);
    void InstallGatewayNetDevices (LoraPhyHelper phyHelper, LorawanMacHelper macHelper, LoraHelper helper, NodeContainer gateways);
};

#endif