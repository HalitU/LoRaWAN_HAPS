#ifndef ADDITIONALCALLBACKS_H
#define ADDITIONALCALLBACKS_H

#include <chrono> 
#include "ns3/core-module.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/gateway-lorawan-mac.h"
#include "ns3/end-device-lora-phy.h"
#include "ns3/stats-module.h"
#include "hapnode.h"
#include "ns3/output-stream-wrapper.h"

using namespace ns3;
using namespace lorawan;
using namespace std::chrono; 

class AdditionalCallbacks
{
    private:

    public:

    Ptr<CounterCalculator<uint32_t> > SentNewPacketDatac;
    Ptr<CounterCalculator<uint32_t> > TxCallbackDatac;
    Ptr<OutputStreamWrapper> PacketSentStream;
    Ptr<OutputStreamWrapper> deviceStateStream;
    Ptr<CounterCalculator<uint32_t> > requiredTransmissionsEndDevice;
    Ptr<CounterCalculator<uint32_t> > receivedPacketOnEndDevices;
    Ptr<CounterCalculator<uint32_t> > lostPacketBecauseInterference;

    void OnDataRateChange (std::string context, uint8_t oldDr, uint8_t newDr);
    void OnTxPowerChange (double oldTxPower, double newTxPower);
    void LastKnownGatewayCount (int oldLastKnownGatewayCount, int newLastKnownGatewayCount);
    void LastKnownLinkMargin (double oldLastKnownLinkMargin, double newLastKnownLinkMargin);
    void SentNewPacket (Ptr<const Packet> packet);
    void TxCallback (std::string path, Ptr<const Packet> packet);

    bool CheckAckFromPacketHeader(PacketMetadata::ItemIterator packetMetaDataIterator);
    LoraDeviceAddress GetLoraDeviceAdressFromPacket(PacketMetadata::ItemIterator packetMetaDataIterator);
    void ReceivedPacketCounter (Ptr<const Packet> packet);
    void RequiredTransmissions (uint8_t requiredTransmissions, bool success, Time time, Ptr<Packet> packet);
    void ReceivedPacketOnNetworkServerCounter (Ptr<CounterCalculator<uint32_t> > datac, Ptr<const Packet> packet);
    void LoraChannelPacketSentTraceCallback (Ptr<const Packet> packet);
    void LostPacketBecauseWrongSpreadingFactor (Ptr<const Packet> packet, uint32_t wrongSF);
    void LostPacketBecauseWrongFrequency (Ptr<const Packet> packet, uint32_t wrongFrequency);
    void PhyRxEnd (Ptr<const Packet> packet);
    void LostPacketBecauseInterference (Ptr<const Packet> packet, uint32_t interference);
    void LostPacketBecauseInterferenceCounter (std::string path, Ptr<const Packet> packet, uint32_t interference);
    void LostPacketBecauseUnderSensitivity (Ptr<const Packet> packet, uint32_t sensitivity);
    void ReceivedPacket (Ptr<const Packet> packet, uint32_t received);
    void StartSending (Ptr<const Packet> packet, uint32_t sent);

};

// Callbacks which require context
void EndDeviceState (Ptr<Node> node, Ptr<OutputStreamWrapper> deviceStateStream, EndDeviceLoraPhy::State oldState, EndDeviceLoraPhy::State newState);
void OccupiedReceptionPaths (Ptr<Node> node, Ptr<OutputStreamWrapper> gatewayreceptionstream, int oldOccupiedReceptionPaths, int newOccupiedReceptionPaths);

#endif