#include "additionalcallbacks.h"

NS_LOG_COMPONENT_DEFINE ("AdditionalCallbacks");

void AdditionalCallbacks::SentNewPacket (Ptr<const Packet> packet)
{
    SentNewPacketDatac->Update ();
}

void AdditionalCallbacks::TxCallback (std::string path, Ptr<const Packet> packet) {
  TxCallbackDatac->Update ();
}

bool AdditionalCallbacks::CheckAckFromPacketHeader(PacketMetadata::ItemIterator packetMetaDataIterator)
{
    while (packetMetaDataIterator.HasNext ())
    {
        PacketMetadata::Item item = packetMetaDataIterator.Next ();
        if (item.type == PacketMetadata::Item::HEADER)
        {
            NS_ASSERT (item.tid.HasConstructor ());
            Callback<ObjectBase *> constructor = item.tid.GetConstructor ();
            NS_ASSERT (!constructor.IsNull ());
            ObjectBase *instance = constructor ();
            NS_ASSERT (instance != 0);
            Chunk *chunk = dynamic_cast<Chunk *> (instance);
            NS_ASSERT (chunk != 0);
            Buffer::Iterator end = item.current;
            end.Next (item.currentSize); // move from start 
            chunk->Deserialize (item.current, end);

            LoraFrameHeader *frameHeader = dynamic_cast<LoraFrameHeader *> (chunk);
            if (frameHeader != NULL)
            {
                if (frameHeader->GetAck() == true)
                {
                    return true;
                }
            }

            // Clear the buffer
            delete chunk;            
        }
    }    
    return false;
}

LoraDeviceAddress AdditionalCallbacks::GetLoraDeviceAdressFromPacket(PacketMetadata::ItemIterator packetMetaDataIterator)
{
    while (packetMetaDataIterator.HasNext ())
    {
        PacketMetadata::Item item = packetMetaDataIterator.Next ();
        if (item.type == PacketMetadata::Item::HEADER)
        {
            NS_ASSERT (item.tid.HasConstructor ());
            Callback<ObjectBase *> constructor = item.tid.GetConstructor ();
            NS_ASSERT (!constructor.IsNull ());
            ObjectBase *instance = constructor ();
            NS_ASSERT (instance != 0);
            Chunk *chunk = dynamic_cast<Chunk *> (instance);
            NS_ASSERT (chunk != 0);
            Buffer::Iterator end = item.current;
            end.Next (item.currentSize); // move from start 
            chunk->Deserialize (item.current, end);

            LoraFrameHeader *frameHeader = dynamic_cast<LoraFrameHeader *> (chunk);
            if (frameHeader != NULL)
            {
                return frameHeader->GetAddress ();
            }

            // Clear the buffer
            delete chunk;            
        }
    }    
    return LoraDeviceAddress();
}

void AdditionalCallbacks::ReceivedPacketCounter (Ptr<const Packet> packet)
{

    //PacketTagIterator packetTagIterator = packet->GetPacketTagIterator();
    PacketMetadata::ItemIterator packetMetaDataIterator = packet->BeginItem();
    receivedPacketOnEndDevices->Update ();
}

void AdditionalCallbacks::RequiredTransmissions (uint8_t requiredTransmissions, bool success, Time time, Ptr<Packet> packet)
{
    // int requiredTransmissionCount = requiredTransmissions;
    // NS_LOG_UNCOND ("[Packet][" << packet->GetUid () <<  "] Required transmission count for this packet: " << requiredTransmissionCount << " is it successful: " << success);
    requiredTransmissionsEndDevice->Update ();
}

void AdditionalCallbacks::ReceivedPacketOnNetworkServerCounter (Ptr<CounterCalculator<uint32_t> > datac, Ptr<const Packet> packet)
{
    datac->Update ();
}

void AdditionalCallbacks::LoraChannelPacketSentTraceCallback (Ptr<const Packet> packet) 
{
    *PacketSentStream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << packet->GetSize() << " bytes" << std::endl;
}

/**
 * 
 * 
 * 
 * Packet failures
 * 
 * 
 * 
 */
void AdditionalCallbacks::LostPacketBecauseInterferenceCounter (std::string path, Ptr<const Packet> packet, uint32_t interference)
{
    lostPacketBecauseInterference->Update ();
}

/*
    0: SLEEP
    1: STANDBY
    2: TX
    3: RX
*/
/***
 * Whenever a state changes we can calculate the expected consumed energy by other devices in the sensor node.
 */
void EndDeviceState (Ptr<Node> node, Ptr<OutputStreamWrapper> deviceStateStream, EndDeviceLoraPhy::State oldState, EndDeviceLoraPhy::State newState)
{
    NS_LOG_UNCOND ("EndDeviceState WRITING");
    *deviceStateStream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << " current PHY device state " << "\t" << oldState << "\t" << newState << std::endl;
}

/**
 * 
 * Callbacks with context
 * 
 */
void OccupiedReceptionPaths (Ptr<Node> node, Ptr<OutputStreamWrapper> gatewayreceptionstream, int oldOccupiedReceptionPaths, int newOccupiedReceptionPaths)
{
    NS_LOG_UNCOND ("OccupiedReceptionPaths WRITING");
    *gatewayreceptionstream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << "occupiedgateway" 
        << "\t" << oldOccupiedReceptionPaths << "\t" << newOccupiedReceptionPaths << std::endl;
}
