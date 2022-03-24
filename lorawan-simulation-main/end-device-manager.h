#ifndef END_DEVICE_MANAGER_H
#define END_DEVICE_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lora-helper.h"
#include "hapnode.h"

using namespace ns3;
using namespace lorawan;

class EndDeviceManager {
    private:

    public:
    Ptr<HAPNode> CreateSingleEndDevice ();
    NodeContainer CreateEndDevices (uint32_t endDeviceN, MobilityHelper mobility);
};

#endif