#include "logicalstatemachine.h"

NS_LOG_COMPONENT_DEFINE ("LogicalStateMachine");

GlobalVariables logical_gv;

/*******************************************
 * END-DEVICE COLLECTION AND TRANSMISSION CALLBACKS
 *******************************************/

bool sendingType = true;
bool collectionType = false;

// Distance calculator from end-device to HAPS
// Since we can have multiple HAPS get the closest distance!
double GetDistance (Ptr<HAPNode> node)
{
    double distance = 5000000;
    // Get ED Mobility model
    Ptr<ConstantPositionMobilityModel> mobilityModel = node->GetObject<ConstantPositionMobilityModel>();

    // Get GW mobility model
    Ptr<ConstantVelocityMobilityModel> gatewayMobilityModel;
    for (NodeContainer::Iterator i = node->gateways.Begin (); i != node->gateways.End (); ++i)
    {
        Ptr<Node> n = (*i);
        Ptr<HAPNode> gw = dynamic_cast<HAPNode*>(&*n); 

        gatewayMobilityModel = gw->GetObject<ConstantVelocityMobilityModel>();

        // Calculate distance using the ns3 module
        double currentDistance = gatewayMobilityModel->GetDistanceFrom (mobilityModel);

        if (currentDistance < distance) {
            distance = currentDistance;
        }
    }

    return distance;
};

/**
 * Updates the state of the energy device (not lorawan device)
 * While changing states, energy device needs to pay attention to whether the collecting or transmitting booleans are 
 * true or false, if these are active, machine should not be able to go back to sleep mode.
 * 
 * TODO: Do not forget just because the device is in the range, doesnt mean it has to be awake all the time
 * TODO: at least for the initial scheduling implementation!
 */
void ChangeDeviceEnergyState_Latest (Ptr<HAPNode> node, CustomDeviceEvent energyDeviceEvent, ApplicationTrigger* trigger, Ptr<TXWindowScheduler> txws)
{
    // Manage the states
    switch (energyDeviceEvent)
    {
    case CustomDeviceEvent::WAKEUP:
        if (GlobalVariables::SM_DEBUG == true) {
            NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " device is waking up");
        }
        // After waking up device will be active, if it is already active no action needed.
        if (node->_customDeviceEnergyModel.GetCurrentState () != CustomDeviceState::ACTIVE)
        {
            // Sleeping time is before X seconds awakening so we can just take that value from assigned energy model
            double consumedEnergyDuringSleeping = node->_customDeviceEnergyModel.GetSleepCurrent()
                * node->_customDeviceEnergyModel.GetSupplyVoltage()
                * (Simulator::Now().GetSeconds() - node->_customDeviceEnergyModel.GetAwakeningTime().GetSeconds() - node->_customDeviceEnergyModel.GetLastEnergyControlTime());
            
            // Energy spent during waking up process
            double consumedEnergyDuringAwakening = node->_customDeviceEnergyModel.GetAwakeningCurrent() 
                * node->_customDeviceEnergyModel.GetSupplyVoltage()
                * node->_customDeviceEnergyModel.GetAwakeningTime().GetSeconds();
                // (Simulator::Now().GetSeconds() - node->_customDeviceEnergyModel.GetLastEnergyControlTime());
        

            node->_customDeviceEnergyModel._lastEnergyControlTime = Simulator::Now().GetSeconds();
            node->_customDeviceEnergyModel.UpdateConsumedEnergy (consumedEnergyDuringSleeping);
            node->_customDeviceEnergyModel.UpdateConsumedEnergy (consumedEnergyDuringAwakening);


            node->_customDeviceEnergyModel._sleepConsumedEnergy += consumedEnergyDuringSleeping;
            node->_customDeviceEnergyModel._awakeningConsumedEnergy += consumedEnergyDuringAwakening;

            node->_customDeviceEnergyModel.SetCurrentState (CustomDeviceState::ACTIVE);

            node->_customDeviceEnergyModel._totalProcessedEnergyTime += (Simulator::Now().GetSeconds() - node->_customDeviceEnergyModel.GetLastEnergyControlTime());

            if (GlobalVariables::DEBUG) {
                node->enddeviceDeviceStateFile << Simulator::Now ().GetSeconds () << "\t" << CustomDeviceState::ACTIVE << endl;
            }
        }
        break;
    case CustomDeviceEvent::SHUTDOWN:
        if (node->_customDeviceEnergyModel.GetIsCollecting() == false && 
            node->_customDeviceEnergyModel.GetIsTransmitting() == false &&
            node->_customDeviceEnergyModel.GetCurrentState () != CustomDeviceState::SLEEPING)
            {
                if (GlobalVariables::SM_DEBUG == true) {
                    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " device is shutting down...");
                }
                // TODO: activate assertions after debugging
                double distance = GetDistance (node);
                TxPoint nextSchedulingPoint = txws->GetNextTimerForED (node, Simulator::Now ().GetSeconds ());
                int nextSchedulingTime = nextSchedulingPoint.TxTime;

                // Device could be within range but exhausted current ranges time-slots, and it can still have future
                // transmissions available!
                NS_ASSERT_MSG (distance > GlobalVariables::HAPSRange 
                    || (distance <= GlobalVariables::HAPSRange && nextSchedulingTime == -1)
                    || (
                        (distance <= GlobalVariables::HAPSRange) && 
                        (nextSchedulingTime >= (node->_customDeviceEnergyModel.GetAwakeningTime().GetSeconds()))
                        ), 
                    Simulator::Now ().GetSeconds () << " " << node->GetId () 
                    << " distance and range: " << distance << " " << GlobalVariables::HAPSRange << " " 
                    << txws->PrintEDWindows (node) << " scheduling time: " << nextSchedulingTime << " duty cycle backoff: " << GlobalVariables::txDutyCycleBackoff
                    << " event is: " << trigger->event);

                // Since lorawan can make the items stay idle, in case the device stayed idle for a while we need to take that into consideration as well
                double activeIdleTime = Simulator::Now().GetSeconds() 
                    - node->_customDeviceEnergyModel.GetShuttingDownTime().GetSeconds()
                    - node->_customDeviceEnergyModel.GetLastEnergyControlTime();
                double shuttingDownTime = node->_customDeviceEnergyModel.GetShuttingDownTime().GetSeconds();

                double consumedEnergyDuringActiveIdle = node->_customDeviceEnergyModel.GetActiveCurrent()
                    * node->_customDeviceEnergyModel.GetSupplyVoltage()
                    * activeIdleTime;
                
                double consumedEnergyDuringShuttingDown = node->_customDeviceEnergyModel.GetShuttingDownCurrent()
                    * node->_customDeviceEnergyModel.GetSupplyVoltage()
                    * shuttingDownTime;
                
                node->_customDeviceEnergyModel._lastEnergyControlTime = Simulator::Now().GetSeconds();
                node->_customDeviceEnergyModel.UpdateConsumedEnergy (consumedEnergyDuringActiveIdle);
                node->_customDeviceEnergyModel.UpdateConsumedEnergy (consumedEnergyDuringShuttingDown);

                node->_customDeviceEnergyModel._activeModeConsumedEnergy += consumedEnergyDuringActiveIdle;
                node->_customDeviceEnergyModel._shutDownModeConsumedEnergy += consumedEnergyDuringShuttingDown;

                node->_customDeviceEnergyModel.SetCurrentState (CustomDeviceState::SLEEPING);

                node->_customDeviceEnergyModel._totalProcessedEnergyTime += (activeIdleTime + shuttingDownTime);

                if (GlobalVariables::DEBUG) {
                    node->enddeviceDeviceStateFile << Simulator::Now ().GetSeconds () << "\t" << CustomDeviceState::SLEEPING << endl;
                }
            }
        break;
    default:
        NS_LOG_UNCOND ("Invalid energy device state.");
        break;
    }

    // Manage the callback
    Time awakeningTime = node->_customDeviceEnergyModel.GetAwakeningTime ();
    Time shuttingDownTime = node->_customDeviceEnergyModel.GetShuttingDownTime ();      
    switch (energyDeviceEvent)
    {
    case CustomDeviceEvent::WAKEUP:
        Simulator::Schedule (awakeningTime, &ApplicationScheduler, node, trigger, txws);
        break;
    case CustomDeviceEvent::SHUTDOWN:
        Simulator::Schedule (shuttingDownTime, &ApplicationScheduler, node, trigger, txws);
        break;
    default:
        NS_LOG_UNCOND ("Invalid energy device state.");
        break;
    }
}

/**
 * Single management location for both transmissions and collections
 */
void ApplicationScheduler (Ptr<HAPNode> node, const ApplicationTrigger* trigger, Ptr<TXWindowScheduler> txws) {
    double debugSimTime = Simulator::Now ().GetSeconds ();
    if (GlobalVariables::SM_DEBUG == true) {
        NS_LOG_UNCOND (debugSimTime << " Calling application scheduler...");
    }
    // Whenever the application is triggered 
    // we check both next transmission, as well as next collection scheduler
    // and schedule both events
    // COLLECTION timer
    int nextCollectionTime = node->last_collection_time + 60;
    // TX timer
    double currentSimTime = Simulator::Now ().GetSeconds ();
    TxPoint nextSchedulingPoint = txws->GetNextTimerForED (node, currentSimTime);
    // nextTransmissionTime means next tx should happen in given seconds
    int nextTransmissionTime = nextSchedulingPoint.TxTime;
    Time awakeningTime = node->_customDeviceEnergyModel.GetAwakeningTime ();
    Time shuttingDownTime = node->_customDeviceEnergyModel.GetShuttingDownTime ();    
    // If initial call both events with their wakeup trigger
    // after waking up, the energy manager will trigger this function back
    // in the time it takes to wakeup after the scheduler is triggered.
    // So wake up triggered at 48 secs, after 11 secs, at 59 this function will
    // be scheduled to work again.
    // If it is not initial then we generalize how the function works according to
    // which purpose it is called for
    ApplicationTrigger* newTrigger = new ApplicationTrigger;

    Time collectionAwakeningTime = Seconds (nextCollectionTime) - awakeningTime;
    Time txAwakeningTime = Seconds (nextTransmissionTime) - awakeningTime;

    switch (trigger->event) {
        case ApplicationTriggerEvent::COLLECTION_IDLE:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling COLLECTION_IDLE...");
            }
            // COLLECTION
            newTrigger->event = ApplicationTriggerEvent::COLLECTION_WAKEUP;
            Simulator::Schedule (collectionAwakeningTime, ApplicationScheduler, node, newTrigger, txws);
            break;
        case ApplicationTriggerEvent::DEVICE_TX_IDLE:
            if (GlobalVariables::SM_DEBUG == true) {        
                NS_LOG_UNCOND (debugSimTime << " Calling DEVICE_TX_IDLE...");
            }
            // TX -- not all end-devices might be able to get a time-slot
            if (nextTransmissionTime != -1) {
                newTrigger->event = ApplicationTriggerEvent::DEVICE_TX_WAKEUP;
                Simulator::Schedule (txAwakeningTime, ApplicationScheduler, node, newTrigger, txws);
            }
            break;
        // Device should trigger wake up for COLLECTION
        case ApplicationTriggerEvent::COLLECTION_WAKEUP:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling COLLECTION_WAKEUP...");
            }
            // COLLECTION
            node->_customDeviceEnergyModel.SetIsCollecting (true);
            newTrigger->event = ApplicationTriggerEvent::COLLECTION;
            Simulator::Schedule (
                Seconds (0), ChangeDeviceEnergyState_Latest, node, CustomDeviceEvent::WAKEUP, newTrigger, txws);
            break;
        // Device should trigger wake up for TX
        case ApplicationTriggerEvent::DEVICE_TX_WAKEUP:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling DEVICE_TX_WAKEUP...");
            }
            // Set transmitting to true so that other events will not shutdown the machine
            node->_customDeviceEnergyModel.SetIsTransmitting (true);
            node->visitedOnce = true;
            newTrigger->event = ApplicationTriggerEvent::DEVICE_TX;
            Simulator::Schedule (
                Seconds (0), ChangeDeviceEnergyState_Latest, node, CustomDeviceEvent::WAKEUP, newTrigger, txws);
            break;  
        case ApplicationTriggerEvent::COLLECTION:
            // Collect data
            node->IncrementCurrentSize();
            newTrigger->event = ApplicationTriggerEvent::COLLECTION_SHUTDOWN;
            // Calculate energy consumption during collection
            UpdateCollectionEnergyConsumption (node);
            // Trigger the sleep
            Simulator::Schedule (Seconds (0), ApplicationScheduler, node, newTrigger, txws);            
            break;
        case ApplicationTriggerEvent::DEVICE_TX:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling DEVICE_TX...");
            }
            // This is called after a waking up call or when the device is awake and requires TX soon, 
            // after a packet process is completed this function will be called with DEVICE_TX_REQ request, 
            // so remaining will be handled at that case.
            // TODO: what about no data case?
            Simulator::Schedule (Seconds (0), OneShotSenderScheduler, node);
            break;
        case ApplicationTriggerEvent::DEVICE_TX_REQ:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling DEVICE_TX_REQ...");
            }
            // Called after a packet is completed either successfuly or failed
            // Depending on the time it will take for next transmission time to arrive we can either schedule
            // a TX directly, or trigger a shutdown so that shutdown will trigger the future TX event.
            if (nextTransmissionTime == -1 || nextTransmissionTime > GlobalVariables::txDutyCycleBackoff) {
                // Just sleep
                newTrigger->event = ApplicationTriggerEvent::DEVICE_TX_SHUTDOWN;
                Simulator::Schedule (Seconds (0), ApplicationScheduler, node, newTrigger, txws);
            }
            else {
                // A soon to be available TX time-slot exists, so schedule for another transmission
                newTrigger->event = ApplicationTriggerEvent::DEVICE_TX;
                Simulator::Schedule (Seconds (nextTransmissionTime), ApplicationScheduler, node, newTrigger, txws);
            }
            break;
        case ApplicationTriggerEvent::COLLECTION_SHUTDOWN:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling COLLECTION_SHUTDOWN...");
            }
            node->_customDeviceEnergyModel.SetIsCollecting (false);
            newTrigger->event = ApplicationTriggerEvent::COLLECTION_IDLE;
            Simulator::Schedule (
                Seconds (0), ChangeDeviceEnergyState_Latest, node, CustomDeviceEvent::SHUTDOWN, newTrigger, txws);  
            break;
        case ApplicationTriggerEvent::DEVICE_TX_SHUTDOWN:
            if (GlobalVariables::SM_DEBUG == true) {
                NS_LOG_UNCOND (debugSimTime << " Calling DEVICE_TX_SHUTDOWN...");
            }
            // Trigger the shutdown process and return back to TX IDLE mode for next trigger
            node->_customDeviceEnergyModel.SetIsTransmitting (false);
            newTrigger->event = ApplicationTriggerEvent::DEVICE_TX_IDLE;
            Simulator::Schedule (
                Seconds (0), ChangeDeviceEnergyState_Latest, node, CustomDeviceEvent::SHUTDOWN, newTrigger, txws);            
            break;
        default:
            NS_LOG_UNCOND ("INVALID EVENT AT APPLICATION TRIGGER " << trigger->event);
            break;
    }
};

void UpdateCollectionEnergyConsumption (Ptr<HAPNode> node) {
    // Collection
    double collectingTime = node->_customDeviceEnergyModel.GetCollectingTime().GetSeconds();
    double consumedEnergyDuringCollecting = 
          node->_customDeviceEnergyModel.GetCollectingCurrent()
        * node->_customDeviceEnergyModel.GetSupplyVoltage()
        * collectingTime;

    // Idle without collection up until this point
    double activeIdleTime = Simulator::Now().GetSeconds() 
        - collectingTime
        - node->_customDeviceEnergyModel.GetLastEnergyControlTime();
    double consumedEnergyDuringActiveIdle = 
          node->_customDeviceEnergyModel.GetActiveCurrent()
        * node->_customDeviceEnergyModel.GetSupplyVoltage()
        * activeIdleTime;    


    node->_customDeviceEnergyModel.UpdateConsumedEnergy (consumedEnergyDuringActiveIdle);
    node->_customDeviceEnergyModel._activeModeConsumedEnergy += consumedEnergyDuringActiveIdle;

    node->_customDeviceEnergyModel.UpdateConsumedEnergy (consumedEnergyDuringCollecting);
    node->_customDeviceEnergyModel._collectionModeConsumedEnergy += consumedEnergyDuringCollecting;

    node->_customDeviceEnergyModel._totalProcessedEnergyTime += activeIdleTime;
    node->_customDeviceEnergyModel._totalProcessedEnergyTime += collectingTime;
    node->_customDeviceEnergyModel._lastEnergyControlTime = Simulator::Now().GetSeconds();
}

/**
 * Triggers the original one shot sender from end-device to gateway
 * Sleeping should be triggered at the end of transmission where lorawan goes into sleep mode.
 * Next scheduling should be done where the device starts sleeping.   
 */
void OneShotSenderScheduler (Ptr<HAPNode> node)
{    
    if (GlobalVariables::DEBUG)
        NS_LOG_UNCOND ("OneShotSenderScheduler START");
    double distance = GetDistance (node);
    if (GlobalVariables::DEBUG)
        NS_LOG_UNCOND ("Distance of the current node which is sending packet: " << distance);
    if (GlobalVariables::DEBUG)
        NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " " << node->GetId () << " starting message process.");

    node->last_transmission_start_time = ceil (Simulator::Now ().GetSeconds ());

    OneShotSenderHelper oneShotSenderHelper;
    oneShotSenderHelper.SetSendTime (Seconds (0));
    oneShotSenderHelper.Install (node);
    if (GlobalVariables::DEBUG)
        NS_LOG_UNCOND ("OneShotSenderScheduler END");
};

/**
 * Records the information regarding to data transmissions for the end-device sensor
 */
void DataTxRecorderScheduler(Ptr<HAPNode> node)
{
    double distance = GetDistance (node);
    double currentSimTime = Simulator::Now ().GetSeconds ();

    if (distance <= GlobalVariables::HAPSRange) {
        node->update_last_in_range_time (currentSimTime);
    }
    else {
        bool recordProcess = node->update_last_out_range_time (currentSimTime);
        if (recordProcess == true) {
            // Save the records to a file for this end-device
            node->write_latest_data_tx_record ();
            // Reset data
            node->reset_last_in_range_time ();
            node->reset_last_out_range_time ();
            node->reset_current_range_transmitted_data ();
        }
    }

    Simulator::Schedule (Seconds (1), DataTxRecorderScheduler, node);
};


/**
 * Manages the state changes on the lorawan device model
 */
void EndDeviceStatePerNode (Ptr<HAPNode> node, EndDeviceLoraPhy::State oldState, EndDeviceLoraPhy::State newState)
{
    // Only check if the devices purpose is to be end-device (so not netserver)
    if (node->GetNodeType() != "ED") {
        return;
    }

    // Log the state
    if (GlobalVariables::DEBUG) {
        node->enddeviceLorawanStateFile << Simulator::Now ().GetSeconds () << "\t" << newState << endl;
    }
}

/**
 * THE MOST IMPORTANT FUNCTION OUT THERE
 * Utility which check the success for required transmissions and how many transmissions it required to succeed or fail
 */
void RequiredTransmissions (Ptr<HAPNode> node, Ptr<TXWindowScheduler> txws, uint8_t reqTx, bool success, Time firstAttempt, Ptr<Packet> packet)
{
    if (GlobalVariables::DEBUG)
        NS_LOG_UNCOND ("RequiredTransmissions START");
    // VERY IMPORTANT NOT ALL PACKETS ARE INTENDED TO BE USED BY THE NODE 
    // WHICH IS WHY WE CHECK THE ADDRESS
    // Work on a copy of the packet

    AdditionalCallbacks ac;

    // if (success)
    //     NS_LOG_UNCOND ("KEK: " << node_address << " " << packet_address);

    // Safety checks
    NS_ASSERT (node->_customDeviceEnergyModel.GetCurrentState () == CustomDeviceState::ACTIVE);
    NS_ASSERT (node->_customDeviceEnergyModel.GetIsTransmitting () == true);

    // NS_LOG_UNCOND ("Node: " << node->GetId () << " packet guid: " << packet->GetUid () << " success: " << success << " requred tx count: " << unsigned (reqTx));

    // Update success stats
    if (success == true)
    {
        // NS_LOG_UNCOND (packet->GetSize ());
        node->DecrementCurrentSize(packet->GetSize());
        node->_customDeviceEnergyModel._totalSentBytes += packet->GetSize ();
        node->_customDeviceEnergyModel._totalTransmitted++;
        node->update_current_range_transmitted_data (packet->GetSize ());
    }
    // NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " " << node->GetId () << " done with current message process. It took " 
    //     << unsigned (reqTx) << " attempts. Did it succeed: " << success);
    // Update global stats
    node->_customDeviceEnergyModel._totalPacketAttempted += unsigned (reqTx);
    node->_customDeviceEnergyModel._totalAttempted++;
    node->currentMessageCount++;

    node->last_transmission_end_time = Simulator::Now ().GetSeconds ();

    if (success == true) {
        // NS_LOG_UNCOND ("Timers for this packet: " << firstAttempt.GetSeconds () << " " << node->last_transmission_start_time << " " << node->last_transmission_end_time);
        node->total_transmission_time_spent += (node->last_transmission_end_time - firstAttempt.GetSeconds ());
        node->total_tx_ack_packets += unsigned (reqTx);
    }    
    // Re-Scheduling for the next packet with repetition in mind
    // device does not have to repeat but it will still stay idle
    // before checking for the next event.
    if (GlobalVariables::DEBUG)
        NS_LOG_UNCOND ("RequiredTransmissions END");
    
    ApplicationTrigger* rqTrigger = new ApplicationTrigger;
    rqTrigger->event = ApplicationTriggerEvent::DEVICE_TX_REQ;
    ApplicationScheduler (node, rqTrigger, txws);
    // CustomOneShotSender (node, false, txws);
}


void CannotSendBecauseDutyCycle (Ptr<HAPNode> node, Ptr<const Packet> packet) {
    NS_LOG_UNCOND ("cannot send the message because of duty cycle.");
}
