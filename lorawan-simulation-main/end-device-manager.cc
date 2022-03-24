#include "end-device-manager.h"

Ptr<HAPNode> EndDeviceManager::CreateSingleEndDevice () {
  Ptr<HAPNode> newNode = CreateObject<HAPNode> ();

  return newNode;
};

// NOT USED
NodeContainer EndDeviceManager::CreateEndDevices (uint32_t endDeviceN, MobilityHelper mobility) {
    // Create a set of nodes
    NodeContainer endDevices;
    endDevices.Create (endDeviceN);

    // Assign a mobility model to the node
    mobility.Install (endDevices);

    int expectedNumberOfMessages = 10;
    double EDsupplyVoltage = 5.5;
    double EDactiveCurrent = 0.1;
    double EDawakeningCurrent = 0.15;
    double EDcollectionCurrent = 0.12;
    double EDshuttingdownCurrent = 0.08;
    double EDsleepCurrent = 0.05;

    Time EDactiveDuration = Seconds (10);
    Time EDawakeningDuration = Seconds (11);
    Time EDcollectionDuration = Seconds (2);
    Time EDshuttingdownDuration = Seconds (4);
    Time EDsleepingDuration = Seconds (5);

    // Assigning specific locations for the nodes
    for (NodeContainer::Iterator j = endDevices.Begin (); j != endDevices.End (); ++j)
    {
        Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
        Vector position = mobility->GetPosition ();
        position.x = 7000.0;
        position.y = 50.0;
        position.z = 0.0;
        mobility->SetPosition (position);

        Ptr<HAPNode> newNode = (dynamic_cast<HAPNode *> (PeekPointer(*j)));

        newNode->expectedNumberOfMessages = expectedNumberOfMessages;
        newNode->currentMessageCount = 0;

        newNode->SetNodeType ("ED");
        newNode->_customDeviceEnergyModel._consumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._sleepConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._awakeningConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._activeModeConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._collectionModeConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._shutDownModeConsumedEnergy = 0.0;
        newNode->_customDeviceEnergyModel._totalTransmitted = 0;
        newNode->_customDeviceEnergyModel._totalCollected = 0;
        newNode->_customDeviceEnergyModel._totalAttempted = 0;
        newNode->_customDeviceEnergyModel._totalPacketAttempted = 0;
        newNode->_customDeviceEnergyModel._totalSentBytes = 0.0;
        newNode->_customDeviceEnergyModel._lastStateUpdateTime = 0.0;
        newNode->_customDeviceEnergyModel._lastEnergyControlTime = 0.0;
        newNode->_customDeviceEnergyModel._someUpdate = 0;
        newNode->_customDeviceEnergyModel._currentState = CustomDeviceState::SLEEPING;

        // Setup the device energy model for the node
        // A scheduling is required to update this devices state (not the lorawan device state!)
        // lorawan state changes and the external device changes can be used together to
        // calculate the total consumed energy
        newNode->_customDeviceEnergyModel.SetSupplyVoltage(EDsupplyVoltage);
        newNode->_customDeviceEnergyModel.SetActiveCurrent (EDactiveCurrent);
        newNode->_customDeviceEnergyModel.SetAwakeningCurrent (EDawakeningCurrent);
        newNode->_customDeviceEnergyModel.SetCollectingCurrent (EDcollectionCurrent);
        newNode->_customDeviceEnergyModel.SetShuttingDownCurrent (EDshuttingdownCurrent);
        newNode->_customDeviceEnergyModel.SetSleepCurrent (EDsleepCurrent);
        
        newNode->_customDeviceEnergyModel.SetActiveTime (EDactiveDuration);
        newNode->_customDeviceEnergyModel.SetAwakeningTime (EDawakeningDuration);
        newNode->_customDeviceEnergyModel.SetCollectingTime (EDcollectionDuration);
        newNode->_customDeviceEnergyModel.SetShuttingDownTime (EDshuttingdownDuration);
        newNode->_customDeviceEnergyModel.SetSleepTime (EDsleepingDuration);      
    }    

    return endDevices;
};

