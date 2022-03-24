#ifndef TXWINDOWSCHEDULER_H
#define TXWINDOWSCHEDULER_H

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "hapnode.h"
#include "haps-mobility-manager.h"
#include "global-variables.h"

using namespace ns3;

class TimeBlock
{
    public:
    double StartTime;
    double EndTime;
    double IntervalLength;
    int nConflicts;
    list<int> EDIDs;
};

class TxConflictingWindow
{
    public:
    double StartTime;
    double EndTime;
    double NConflicts;
    double SelfID;
};

class TXWindowScheduler : public Object
{
    private:
    
    public:
    // Debugging

    // Variables
    list<TimeBlock> sharedTimeBlocks;

    // Methods
    void IdentifyRangeTimers (NodeContainer gateways, NodeContainer& enddevices, double circlePeriod);    
    bool CanEDScheduleTX (Ptr<HAPNode> node);
    int GetEDNextSchedulingTime (Ptr<HAPNode> node, double currentTime);
    string PrintEDWindows (Ptr<HAPNode> node);
    int GetEDNextAwakeningTime (Ptr<HAPNode> node, double currentTime);
    void IdentifyEntireSchedule (NodeContainer gateways, NodeContainer& enddevices, double circlePeriod);
    void PrintEntireSchedule (NodeContainer& enddevices);
    TxPoint GetNextTimerForED (Ptr<HAPNode> node, double currentTime);
    list<int> CheckHAPSInRange (NodeContainer gateways, Ptr<HAPNode> enddevice, double current_time);
    void UpdateHAPSPositions (NodeContainer gateways, double current_time, double max_allowed_counter_per_iteration);
    void ResetHAPSPositions (NodeContainer gateways);
};

#endif