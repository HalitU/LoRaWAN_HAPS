#ifndef STATISTICS_HELPER_H
#define STATISTICS_HELPER_H

#include "global-variables.h"
#include "hapnode.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lora-helper.h"
#include "sensor-topology-manager.h"

class StatisticsHelper {
    private:
    public:
    //
    GlobalVariables* gv;

    uint32_t totalNumberOfAttempts = 0;
    uint32_t totalNumberOfCollects = 0;
    uint32_t totalNumberofTransmits = 0;
    uint32_t totalNumberofCompleteAttempts = 0;
    double totalNumberOfTransmittedBytes = 0.0;
    uint32_t not_collected_wasted_data = 0;
    uint32_t totalNumberOfPacketSeries = 0;

    double totalEnergySpent = 0.0;
    double sleepEnergySpent = 0.0;
    double awakeningEnergySpent = 0.0;
    double activeEnergySpent = 0.0;
    double collectionEnergySpent = 0.0;
    double shutdownEnergySpent = 0.0;
    double lorawanEnergySpent = 0.0;
    double allEnergySpent = 0.0;

    uint32_t endDeviceN = 0;
    uint32_t visitedOnceCount = 0;

    uint32_t gatewayN = 0;
    double gatewayRange = 0;

    double totalTxTime = 0;
    double totalPacketCount = 0;
    double totalACKCount = 0;

    LoraPacketTracker tracker;

    double numberOfTXPoints = 0;
    double totalTimeUnderHAPS = 0;
    double expectedPackageCount = 0;

    //
    void FillImprovedExperimentFile ();
    void EDCollectionTransmissionSummary (NodeContainer endDevices);
    void EnergyCalculations (NodeContainer endDevices);
    void VisitedNodes (NodeContainer endDevices);
    void VisualizeEnvironment (NodeContainer endDevices, NodeContainer gateways);
    void PacketTracker (LoraHelper helper, NodeContainer gateways);
    void FillAverageTXInformationFile (GlobalVariables *gv, NodeContainer gateways, NodeContainer endDevices, LoraPacketTracker &tracker);
    void FillSensorOutputFile (GlobalVariables *gv, NodeContainer gateways, NodeContainer endDevices, LoraPacketTracker &tracker);
};

#endif