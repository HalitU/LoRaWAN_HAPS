#include "application-manager.h"

void ApplicationManager::CreateEndDeviceApplications (NodeContainer gateways, NodeContainer& endDevices, StatisticsHelper *sh) {
    Ptr<TXWindowScheduler> txws = CreateObject<TXWindowScheduler> ();
    
    // Find all of the packet sending timers
    txws->IdentifyRangeTimers (gateways, endDevices, GlobalVariables::simulationTime);

    /***************************************************************/
    // After finding all the timers we need to validate some numbers.
    // Check the total time end-devices will spend under HAPS range
    uint32_t numberOfTXPoints = 0;
    double totalTimeUnderHAPS = 0;
    double expectedPackageCount = 0;
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i) {
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n);  
        // these are exact expected tx points
        numberOfTXPoints += node->txPointList.size ();

        // these are available window intervals when ED was in HAPS range
        for(TxInterval window : node->txWindowList) {
            // NS_LOG_UNCOND ("Window: " << window.TxStartTime << " " << window.TxEndTime);
            if (window.TxStartTime >= 0) {
                NS_ASSERT_MSG (window.TxEndTime >= 0, "Node: " << node->GetId () << ", Message: " << window.TxStartTime << " " << window.TxEndTime);
                double nodeTimeUnderHAPS = (window.TxEndTime  - window.TxStartTime);
                // Check the number of packages to be scheduled
                expectedPackageCount += (nodeTimeUnderHAPS / GlobalVariables::txDutyCycleBackoff);
                totalTimeUnderHAPS += nodeTimeUnderHAPS;
            }
        }
    }
    // NS_ASSERT (false);

    // NS_LOG_UNCOND ("Printing packet timers for debugging...");
    // for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i) {
    //     Ptr<Node> n = (*i);
    //     Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n);  
    //     // these are exact expected tx points
    //     if (node->GetId () == 5) {
    //         for (TxPoint point: node->txPointList) {
    //             NS_LOG_UNCOND (point.TxTime);
    //         }
    //     }
    // }

    // Logs for validation
    NS_LOG_UNCOND ("Number of total TX points to be scheduled is: " << numberOfTXPoints);
    NS_LOG_UNCOND ("Total expected time to be spent under HAPS by EDs is: " << totalTimeUnderHAPS << " seconds");
    NS_LOG_UNCOND ("Total expected TX count for EDs according to time spent under HAPS is: " << expectedPackageCount);

    sh->numberOfTXPoints = numberOfTXPoints;
    sh->totalTimeUnderHAPS = totalTimeUnderHAPS;
    sh->expectedPackageCount = expectedPackageCount;

    /***************************************************************/
    // Start the programs
    SetupTransmissionSchedule (endDevices, txws, gateways);

    // Sensor data collection from environment
    // SetupCollectionSchedule (endDevices, txws);
};

void ApplicationManager::SetupTransmissionSchedule (NodeContainer& endDevices, Ptr<TXWindowScheduler> txws, NodeContainer gateways) {
    NS_LOG_UNCOND("Setting up data sending applications...");
    
    uint32_t dataGenerationPeriod = GlobalVariables::dataGenerationPeriod;
    uint32_t dataMaxSize = GlobalVariables::dataMaxSize;
    uint32_t dataGenerationRate = GlobalVariables::dataGenerationRate;
    uint32_t dataCollectionTime = GlobalVariables::dataCollectionTime;

    // Install the oneshotsender for all nodes according to predicted arrival time, this might actually work
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 
        node->gateways = gateways; // For the sake of precise simulation, ED needs to know some info about GW
        // this is the assumption mentioned in the paper.

        // This is the applications which enables end-devices to send messages to gateways and GW forwards them to NS
        // First message should be send around the first expected HAPS arrival time for that node
        // which was calculated with the window scheduler. This might also improve performance too.
        int nextSchedulingTime = txws->GetEDNextAwakeningTime (node, 0.0);
        // Device needs some time to awaken so that it can fit within the message sending window...
        int awakeningTime = node->_customDeviceEnergyModel.GetAwakeningTime ().GetSeconds ();

        // Initially all end-devices should have some proper scheduling except if the duty cycle does not allow them
        // if (nextSchedulingTime == -1) throw exception ();

        if (nextSchedulingTime - awakeningTime < 0) nextSchedulingTime = 0;
        else nextSchedulingTime  = nextSchedulingTime - awakeningTime;

        // NS_LOG_UNCOND (node->GetId () << " will start awakening at: " << nextSchedulingTime);
        
        // Collectiong settings
        node->SetDataMaxSize (dataMaxSize);
        node->SetDataCurrentSize (0);
        node->SetDataGenerationRate (dataGenerationRate);
        node->SetDataGenerationPeriod (dataGenerationPeriod);
        node->SetDataCollectionTime (dataCollectionTime);

        // Simulator::Schedule (Seconds (nextSchedulingTime), &CustomOneShotSender, node, true, txws);
        ApplicationTrigger* collectionTrigger = new ApplicationTrigger;
        collectionTrigger->initial = true;
        collectionTrigger->event = ApplicationTriggerEvent::COLLECTION_IDLE;
        Simulator::Schedule (Seconds (0), &ApplicationScheduler, node, collectionTrigger, txws);

        ApplicationTrigger* txTrigger = new ApplicationTrigger;
        txTrigger->initial = true;
        txTrigger->event = ApplicationTriggerEvent::DEVICE_TX_IDLE;
        Simulator::Schedule (Seconds (0), &ApplicationScheduler, node, txTrigger, txws);

        // Recording data transmission values and timers for range stuff
        Simulator::Schedule (Seconds (0), &DataTxRecorderScheduler, node);

        std::string endDeviceStateConfig = "/NodeList/" + std::to_string (node->GetId ()) + "/DeviceList/*/$ns3::LoraNetDevice/Phy/$ns3::EndDeviceLoraPhy/EndDeviceState";
        Config::ConnectWithoutContext (endDeviceStateConfig, MakeBoundCallback (&EndDeviceStatePerNode, node));

        // Required packet utility
        Config::ConnectWithoutContext (
            "/NodeList/" + std::to_string (node->GetId ()) + "/DeviceList/*/$ns3::LoraNetDevice/Mac/$ns3::EndDeviceLorawanMac/RequiredTransmissions",
            MakeBoundCallback (&RequiredTransmissions, node, txws));

        // duty cycle tracker
        Config::ConnectWithoutContext (
            "/NodeList/" + std::to_string (node->GetId ()) + "/DeviceList/*/$ns3::LoraNetDevice/Mac/$ns3::EndDeviceLorawanMac/CannotSendBecauseDutyCycle",
            MakeBoundCallback (&CannotSendBecauseDutyCycle, node));
    }
};