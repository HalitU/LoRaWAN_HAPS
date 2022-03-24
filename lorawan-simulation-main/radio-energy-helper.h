#ifndef RADIO_ENERGY_HELPER
#define RADIO_ENERGY_HELPER

#include "ns3/basic-energy-source-helper.h"
#include "ns3/lora-radio-energy-model-helper.h"
#include "ns3/wifi-radio-energy-model-helper.h"

#include "hapnode.h"
#include "global-variables.h"

using namespace ns3;
using namespace lorawan;

class RadioEnergyHelper {
    private:
    public:
    void InstallLorawanRadioEnergy (NodeContainer endDevices, NetDeviceContainer endDevicesNetDevices);
};

#endif