#ifndef CHANNEL_MANAGER_H
#define CHANNEL_MANAGER_H

#include "ns3/lora-helper.h"
#include "ns3/core-module.h"
#include "ns3/correlated-shadowing-propagation-loss-model.h"
#include "ns3/building-penetration-loss.h"

using namespace ns3;
using namespace lorawan;

class ChannelManager {
    private:
    public:
    Ptr<LoraChannel> InitializeChannelWithLoss (double PathLossExponent, double ReferenceDistance, double ReferenceLoss);
};

#endif