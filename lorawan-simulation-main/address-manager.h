#ifndef ADDRESS_MANAGER_H
#define ADDRESS_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/lora-helper.h"

#include "hapnode.h"
#include "global-variables.h"

using namespace ns3;
using namespace lorawan;

class AddressManager {
    private:
    public:
    NetDeviceContainer endDevicesNetDevices;

    void InstallAddressToDevices (
        LoraPhyHelper phyHelper, LorawanMacHelper macHelper, LoraHelper helper, NodeContainer enddevices);
};

#endif