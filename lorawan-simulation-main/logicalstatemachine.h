#ifndef LOGICALSTATEMACHINE_H
#define LOGICALSTATEMACHINE_H

#include <chrono> 
#include "ns3/core-module.h"
#include "hapnode.h"
#include "ns3/end-device-lora-phy.h"
#include "additionalcallbacks.h"
#include "ns3/one-shot-sender-helper.h"

#include "ns3/gateway-lora-phy.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/gateway-lorawan-mac.h"
#include "ns3/lora-helper.h"
#include "ns3/mobility-module.h"
#include "math.h"
#include "txwindowscheduler.h"
#include "global-variables.h"

using namespace ns3;
using namespace lorawan;
using namespace std::chrono; 

enum ApplicationTriggerEvent { 
    COLLECTION_WAKEUP, 
    COLLECTION_SHUTDOWN,
    COLLECTION,
    COLLECTION_IDLE,
    DEVICE_TX_WAKEUP,
    DEVICE_TX_SHUTDOWN,
    DEVICE_TX,
    DEVICE_TX_REQ,
    DEVICE_TX_IDLE
};

class ApplicationTrigger {
    public:
    bool initial = false;
    ApplicationTriggerEvent event;
};

void ChangeDeviceEnergyState (
    Ptr<HAPNode> node, 
    CustomDeviceEvent energyDeviceEvent, 
    void (*f) (Ptr<HAPNode>, bool, Ptr<TXWindowScheduler>), 
    bool triggerCallBack, 
    bool repeat, 
    Ptr<TXWindowScheduler> txws);
void OneShotSenderScheduler (Ptr<HAPNode> node);
void CustomOneShotSender(Ptr<HAPNode> node, bool awaken, Ptr<TXWindowScheduler> txws);
void EndDeviceStatePerNode (Ptr<HAPNode> node, EndDeviceLoraPhy::State oldState, EndDeviceLoraPhy::State newState);
void TriggerDataCollectionSchedule (Ptr<HAPNode> node, void (*f) (Ptr<HAPNode>, bool, Ptr<TXWindowScheduler>), Ptr<TXWindowScheduler> txws);
void TriggerDataCollection(Ptr<HAPNode> node, bool repeat, Ptr<TXWindowScheduler> txws);
void ReceivedPacketBounded (Ptr<HAPNode> node, Ptr<const Packet> packet, uint32_t received);
void LostPacketBecauseInteferenceBounded (Ptr<HAPNode> node, Ptr<const Packet> packet, uint32_t received);
void LostPacketBecauseUnderSensitivity (Ptr<HAPNode> node, Ptr<const Packet> packet, uint32_t received);
void LostPacketBecauseWrongSpreadingFactor (Ptr<HAPNode> node, Ptr<const Packet> packet, uint32_t received);
void LostPacketBecauseWrongFrequency (Ptr<HAPNode> node, Ptr<const Packet> packet, uint32_t received);
void RequiredTransmissions (Ptr<HAPNode> node, Ptr<TXWindowScheduler> txws, uint8_t reqTx, bool success, Time firstAttempt, Ptr<Packet> packet);
void CannotSendBecauseDutyCycle (Ptr<HAPNode> node, Ptr<const Packet> packet);
void DataTxRecorderScheduler(Ptr<HAPNode> node);

void ChangeDeviceEnergyState_Latest (Ptr<HAPNode> node, CustomDeviceEvent energyDeviceEvent, ApplicationTrigger* trigger, Ptr<TXWindowScheduler> txws);
void ApplicationScheduler (Ptr<HAPNode> node, const ApplicationTrigger* trigger, Ptr<TXWindowScheduler> txws);
void UpdateCollectionEnergyConsumption (Ptr<HAPNode> node);

#endif