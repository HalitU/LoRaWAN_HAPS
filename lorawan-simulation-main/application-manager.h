#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/periodic-sender-helper.h"
#include "hapnode.h"
#include "global-variables.h"
#include "txwindowscheduler.h"
#include "logicalstatemachine.h"
#include "statistics-helper.h"

using namespace ns3;
using namespace lorawan;

class ApplicationManager {
    private:
    public:
    void CreateEndDeviceApplications (NodeContainer gateways, NodeContainer& endDevices, StatisticsHelper *sh);
    void SetupTransmissionSchedule (NodeContainer& endDevices, Ptr<TXWindowScheduler> txws, NodeContainer gateways);
    void SetupCollectionSchedule (NodeContainer& endDevices, Ptr<TXWindowScheduler> txws);
};

#endif