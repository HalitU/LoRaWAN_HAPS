#include "statistics-helper.h"

void StatisticsHelper::FillImprovedExperimentFile () {
    // All expected results by the end of the experiment
    gv->improvedExperimentFile << 
        "ED Count" << "\t" << "HAPS Speed" << "\t" <<
        "Attempts" << "\t" << "Attempts Per ED" << "\t" << "Total Packet Attempts" << "\t" << "Total Packet Attempts Per ED" << "\t" <<  "Number of Unique ED visited" << "\t" <<
        "Collects" << "\t" << "Collects Per ED" << "\t" << "Transmits" << "\t" << "Transmits Per ED" << "\t" << "Transmitted Bytes" << "\t" <<
        "Transmitted Byte Per ED" << "\t" << "Failed Packets" << "\t" << "Failed Packets Per ED" << "\t" << 
        "Not collected" << "\t" << "Not collected Per ED" << "\t" << "Consumed Energy" << "\t" << "Consumed Energy Per ED" << "\t" <<
        "HAPS Total" << "\t" << "HAPS Received" << "\t" << "HAPS Interfered" << "\t" << "HAPS NO MORE RECEIVERS" << "\t" <<
        "HAPS Under sensitivity" << "\t" << "HAPS LOST BECAUSE TX" << "\t" << "Sleep Energy Per ED" << "\t" << "Awakening Energy Per ED" << "\t" <<
        "Active Mode Energy Per ED" << "\t" << "Collection Energy Per ED" << "\t" << "Shutdown Energy Per ED" << "\t" << "Lorawan Energy Per ED" << "\t" <<

        "Gateway Count" << "\t" << "Gateway Range" << "\t" << "Simulation Duration (seconds)" << "\t" << "Duty Cycle Duration (seconds)" << "\t" <<
        "Time-slot duration (seconds)" << "\t" << "Average TX time per packet (seconds)" << "\t" << "Average TX time per ACK (seconds)" << endl;

    gv->improvedExperimentFile << endDeviceN << "\t";
    gv->improvedExperimentFile << "NaN" << "\t";  

    gv->improvedExperimentFile << totalNumberOfAttempts << "\t";
    gv->improvedExperimentFile << totalNumberOfAttempts / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << totalNumberofCompleteAttempts << "\t";
    gv->improvedExperimentFile << totalNumberofCompleteAttempts / (double)endDeviceN << "\t";

    gv->improvedExperimentFile << visitedOnceCount << "\t";
    gv->improvedExperimentFile << totalNumberOfCollects << "\t";
    gv->improvedExperimentFile << totalNumberOfCollects / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << totalNumberofTransmits << "\t";
    gv->improvedExperimentFile << totalNumberofTransmits / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << totalNumberOfTransmittedBytes << "\t";
    gv->improvedExperimentFile << totalNumberOfTransmittedBytes / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << (totalNumberofCompleteAttempts - totalNumberofTransmits) << "\t";
    gv->improvedExperimentFile << (totalNumberofCompleteAttempts - totalNumberofTransmits) / (double)endDeviceN << "\t";

    gv->improvedExperimentFile << not_collected_wasted_data << "\t";
    gv->improvedExperimentFile << not_collected_wasted_data / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << allEnergySpent << "\t";
    gv->improvedExperimentFile << allEnergySpent / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << tracker.PrintPhyPacketsPerGw (Seconds (0), Seconds (gv->simulationTime) + Hours (1), endDeviceN);

    gv->improvedExperimentFile << sleepEnergySpent / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << awakeningEnergySpent / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << activeEnergySpent / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << collectionEnergySpent / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << shutdownEnergySpent / (double)endDeviceN << "\t";
    gv->improvedExperimentFile << lorawanEnergySpent / (double)endDeviceN << "\t";
    
    //
    gv->improvedExperimentFile << gatewayN << "\t";
    gv->improvedExperimentFile << gv->HAPSRange << "\t";
    gv->improvedExperimentFile << gv->simulationTime << "\t";
    gv->improvedExperimentFile << gv->txDutyCycleBackoff << "\t";
    gv->improvedExperimentFile << gv->txDutyDuration << "\t";
    gv->improvedExperimentFile << (totalTxTime / totalPacketCount) << "\t";
    gv->improvedExperimentFile << (totalTxTime / totalACKCount);

    gv->improvedExperimentFile.close ();
};

void StatisticsHelper::EDCollectionTransmissionSummary (NodeContainer endDevices) {
    gv->enddeviceDataCollectionSummaryFile << "ID" << "\t" << "Max Size" << "\t" << "Current Size" << "\t" << "Wasted Size" << endl;

    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i){
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 
        gv->enddeviceDataCollectionSummaryFile << node->GetId() << "\t" << node->GetDataMaxSize() << "\t" 
            << node->GetDataCurrentSize() << "\t" << node->GetDataWastedSize() << endl;
        
        // tx times
        totalTxTime += node->total_transmission_time_spent;
        totalPacketCount += node->total_tx_ack_packets;
        totalACKCount += node->_customDeviceEnergyModel._totalTransmitted;
    }

    gv->enddeviceColletionTransmissionFile 
        << "ID" << "\t" 
        << "Total Collection" << "\t" 
        << "Total Attempted" << "\t" 
        << "Total Packet Attempts" << "\t" 
        << "Total Transmission (ACK successful)" << "\t"
        << "Distance From HAPS Line" << "\t"
        << "Angle According to Center" << endl;

    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i){
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 
        gv->enddeviceColletionTransmissionFile 
            << node->GetId() << "\t" 
            << node->_customDeviceEnergyModel.GetTotalCollected () << "\t" 
            << node->_customDeviceEnergyModel.GetTotalAttempted () << "\t"
            << node->_customDeviceEnergyModel._totalPacketAttempted << "\t"
            << node->_customDeviceEnergyModel.GetTotalTransmitted () << "\t"
            << node->distance_from_haps << "\t"
            << node->distance_angle << endl;

        //
        totalNumberOfAttempts += node->_customDeviceEnergyModel.GetTotalAttempted ();
        totalNumberOfCollects += node->_customDeviceEnergyModel.GetTotalCollected ();
        totalNumberofTransmits += node->_customDeviceEnergyModel.GetTotalTransmitted ();
        totalNumberofCompleteAttempts += node->_customDeviceEnergyModel._totalPacketAttempted;
        totalNumberOfTransmittedBytes += node->_customDeviceEnergyModel._totalSentBytes;
        totalNumberOfPacketSeries += node->_customDeviceEnergyModel._totalAttempted;
    }

    gv->experimentFile << "-------- collection - transmissions - acks --------" << endl;
    gv->experimentFile << "Total number of attempts: " << totalNumberOfAttempts << endl;
    gv->experimentFile << "Total number of collects: " << totalNumberOfCollects << endl;
    gv->experimentFile << "Total number of transmits: " << totalNumberofTransmits << endl;
    gv->experimentFile << "---------------------------------------------------" << endl;

    gv->energySpentOutsideLorawanFile << "ID" << "\t" << "Total Energy Spent" << "\t" << "Last time saved for energy calculation (seconds)" << endl;
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i){
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 
        gv->energySpentOutsideLorawanFile << node->GetId() << "\t" << node->_customDeviceEnergyModel._consumedEnergy << "\t" << node->_customDeviceEnergyModel._lastEnergyControlTime << endl;
    }


    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 

        not_collected_wasted_data += node->GetDataCurrentSize ();
    }    
};

void StatisticsHelper::EnergyCalculations (NodeContainer endDevices) {

    // Calculate total energy spent before destroying the simulation
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 
        if (node->visitedOnce == true) {
                
            double deviceEnergy = node->_customDeviceEnergyModel.GetConsumedEnergy ();

            DeviceEnergyModelContainer demc = node->demc;
            double lorawanEnergy = 0.0;
            for (DeviceEnergyModelContainer::Iterator demcIter = demc.Begin (); demcIter != demc.End (); demcIter ++)
            {
            lorawanEnergy += (*demcIter)->GetTotalEnergyConsumption ();        
            }  

            totalEnergySpent += (deviceEnergy + lorawanEnergy);

            allEnergySpent += (deviceEnergy + lorawanEnergy);

            //
            sleepEnergySpent += node->_customDeviceEnergyModel._sleepConsumedEnergy;
            awakeningEnergySpent += node->_customDeviceEnergyModel._awakeningConsumedEnergy;
            activeEnergySpent += node->_customDeviceEnergyModel._activeModeConsumedEnergy;
            collectionEnergySpent += node->_customDeviceEnergyModel._collectionModeConsumedEnergy;
            shutdownEnergySpent += node->_customDeviceEnergyModel._shutDownModeConsumedEnergy;
            lorawanEnergySpent += lorawanEnergy;
        }
    }
    totalEnergySpent = totalEnergySpent / endDeviceN;    
};

void StatisticsHelper::VisitedNodes (NodeContainer endDevices) {
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));

        if (node->visitedOnce == true)
            visitedOnceCount++;
    }
    NS_LOG_UNCOND ("Number of EDs visited at least once is: " << visitedOnceCount);

    // All sensors should be visited at least once!
    // NS_ASSERT (visitedOnceCount == endDeviceN);
    endDeviceN = visitedOnceCount;    
};

void StatisticsHelper::VisualizeEnvironment (NodeContainer endDevices, NodeContainer gateways) {
    /*****************
     * Print simulation environment so that we can visualize it as image in python later
     *****************/
    gv->experimentEnvironmentFile << "ED" << endl;

    // Putting a pointer on locked position to compare distributions
    for (NodeContainer::Iterator i = gateways.Begin (); i != gateways.End (); ++i)
    {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
        CoordDifference* cd = HAPSMobilityManager::CalculateDistanceBetweenTwoCoords (*node->map_reference, *node->current_coords);
        // NS_LOG_UNCOND (cd->lat << " " << cd->lon);
        gv->experimentEnvironmentFile << cd->lat - GlobalVariables::range_margin << "\t" << cd->lon - GlobalVariables::range_margin << endl;
        // NS_ASSERT (false);
        break;
    }    

    // End-devices
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
        Ptr<ConstantPositionMobilityModel> mobilityModel = node->GetObject<ConstantPositionMobilityModel>();
        Vector currrentPosition = mobilityModel->GetPosition();

        gv->experimentEnvironmentFile << currrentPosition.x << "\t" << currrentPosition.y << endl;
    }
    gv->experimentEnvironmentFile << "GW" << endl;
    // HAPS
    for (NodeContainer::Iterator i = gateways.Begin (); i != gateways.End (); ++i)
    {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
        Ptr <ConstantVelocityMobilityModel> mobility = node -> GetObject<ConstantVelocityMobilityModel>();

        Vector currentPosition = mobility->GetPosition ();
        gv->experimentEnvironmentFile << currentPosition.x << "\t" << currentPosition.y << endl;
    }        
};

void StatisticsHelper::PacketTracker (LoraHelper helper, NodeContainer gateways) {
    tracker = helper.GetPacketTracker ();
    
    gv->phypacketgwFile << "Global MAC packets (sent - received): " 
        << tracker.CountMacPacketsGlobally (Seconds (0), Seconds (gv->simulationTime) + Hours (1)) << std::endl;
    gv->phypacketgwFile << "PrintPhyPacketsPerGw: " << "TOTAL" << "\t" << "RECEIVED" << "\t" << "INTERFERED" << "\t" << "NO_MORE_RECEIVERS"
        << "\t" << "UNDER_SENSITIVITY" << "\t" << "LOST_BECAUSE_TX" << std::endl;
    // Per gateway
    for (NodeContainer::Iterator i = gateways.Begin (); i != gateways.End (); ++i)
    {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
        gv->phypacketgwFile << "Node [" + std::to_string (node->GetId ()) << "]: " <<
            tracker.PrintPhyPacketsPerGw (Seconds (0), Seconds (gv->simulationTime) + Hours (1), node->GetId ())
            << std::endl;
    };
};

void StatisticsHelper::FillAverageTXInformationFile (
    GlobalVariables *gv, NodeContainer gateways, NodeContainer endDevices, LoraPacketTracker &tracker) {
    // Tx data per end-device 
    // [
    //  Number of ED, 
    //  Number of HAPS,
    //  HAPS interval in seconds,
    //  average range time in seconds, 
    //  average packet attempt count, 
    //  average succesfuly packet count, 
    //  average total sent byte size, 
    //  average left-over byte size
    // ]
    // File header
    gv->averageTxInformationFile <<
        "Number of ED" << "\t" <<
        "Number of HAPS" << "\t" <<
        "HAPS interval in seconds" << "\t" <<
        "Time-slot duration (seconds)" << "\t" <<
        "Duty-cycle duration (seconds)" << "\t" <<
        "Average range time in seconds" << "\t" <<

        // packets
        "Total Packet Attempt" << "\t" <<
        "Total Successful Packet Attempts" << "\t" <<
        "Average packet attempt count" << "\t" <<
        "Average succesfuly packet count" << "\t" <<
        "Total Packet TX Series" << "\t" <<
        "Failed Packets" << "\t" << 
        "Failed Packets Per ED" << "\t" <<
        "Average total transmitted" << "\t" <<
        "Average time it took to successfuly transmit packet (seconds)" << "\t" <<

        // Data
        "Total Sent Data Size (byte)" << "\t" <<
        "Average total sent byte size" << "\t" <<
        "Total Left-over Data Size (byte)" << "\t" <<
        "Average left-over byte size" << "\t" <<
        "Average last left-over byte size" << "\t" <<
        "Average real left-over byte size" << "\t" <<

        // HAPS part
        "HAPS Phy Total" << "\t" <<
        "HAPS Phy Received" << "\t" <<
        "HAPS Phy Interfered" << "\t" <<
        "HAPS Phy No More Receivers" << "\t" <<
        "HAPS Phy Under Sensitivity" << "\t" <<
        "HAPS Phy Lost Because TX" << "\t" <<

        // Energy consumptions
        "Total Energy Spent" << "\t" <<
        "Sleep Energy Per ED" << "\t" << 
        "Awakening Energy Per ED" << "\t" <<
        "Active Mode Energy Per ED" << "\t" << 
        "Collection Energy Per ED" << "\t" << 
        "Shutdown Energy Per ED" << "\t" << 
        "Lorawan Energy Per ED" << "\t" <<

        // Expected plans
        "Number of total TX points to be scheduled" << "\t" <<
        "Total expected time to be spent under HAPS by EDs" << "\t" <<
        "Total expected TX count for EDs according to time spent under HAPS";

    gv->averageTxInformationFile  << std::endl;

    // Gateway packet tracker stuff
    int gwPhyTotal = 0;
    int gwPhyReceived = 0;
    int gwPhyInterfered = 0;
    int gwPhyNoMoreReceivers = 0;
    int gwPhyUnderSensitivity = 0;
    int gwPhyLostBecauseTx = 0;
    for (NodeContainer::Iterator i = gateways.Begin (); i != gateways.End (); ++i) {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
        
        vector<int> gwPhyDetails = tracker.CountPhyPacketsPerGw (Seconds (0), Seconds (GlobalVariables::simulationTime) + Hours (1), node->GetId ());

        gwPhyTotal += gwPhyDetails [0];
        gwPhyReceived += gwPhyDetails [1];
        gwPhyInterfered += gwPhyDetails [2];
        gwPhyNoMoreReceivers += gwPhyDetails [3];
        gwPhyUnderSensitivity += gwPhyDetails [4];
        gwPhyLostBecauseTx += gwPhyDetails [5];
    }    

    double average_range_time = 0.0;
    double average_packet_attempt = 0.0;
    double average_succ_packets = 0.0;
    double average_sent_bytes = 0.0;
    double average_leftover_bytes = 0.0;
    double average_last_leftover_bytes = 0.0;
    double average_real_leftover_bytes = 0.0;
    double average_total_transmitted = 0.0;
    double average_total_transmission_time_spent = 0.0;

    uint32_t node_count = 0;
    uint32_t gateway_count = gateways.GetN ();

    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        // Get the node
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));

        if (node->visitedOnce == true) {
            node_count++;

            double node_range_time = 0.0;
            double node_succ_packets = 0.0;
            double node_sent_bytes = 0.0;
            double node_leftover_bytes = 0.0;        
            double node_last_leftover_bytes = 0.0;

            // Iterate through the tx logs of this node
            for (const TxLog* log : node->txLogList) {
                node_range_time += log->interval;
                node_succ_packets += log->tx_count;
                // NS_LOG_UNCOND (log->tx_data);
                node_sent_bytes += log->tx_data;
                node_leftover_bytes += log->left_data;
                node_last_leftover_bytes = log->left_data;
            }

            // Add them to cumulative data
            // Leftover is the current max, so you should take the latest directly from the node
            // instead of summing them up!
            average_range_time += node_range_time;
            average_packet_attempt += node->_customDeviceEnergyModel._totalPacketAttempted;
            average_succ_packets += node_succ_packets;
            average_sent_bytes += node_sent_bytes;
            average_leftover_bytes += node_leftover_bytes;
            average_last_leftover_bytes += node_last_leftover_bytes;
            average_real_leftover_bytes += node->GetDataCurrentSize ();
            average_total_transmitted += node->_customDeviceEnergyModel._totalTransmitted;
            average_total_transmission_time_spent += node->total_transmission_time_spent;
        }
    }
    average_total_transmission_time_spent = average_total_transmission_time_spent / average_total_transmitted;
    // NS_LOG_UNCOND (average_total_transmitted << " " << average_total_transmission_time_spent);
    // NS_LOG_UNCOND ("sent bytes: " << average_sent_bytes);

    // NS_LOG_UNCOND ("sad: " << totalNumberofTransmits);
    // NS_LOG_UNCOND (" " << totalNumberOfTransmittedBytes);
    // NS_LOG_UNCOND (" " << average_sent_bytes);

    // Write to the file
    gv->averageTxInformationFile << 
        node_count << "\t" << 
        gateway_count << "\t" <<
        GlobalVariables::haps_delay << "\t" <<
        GlobalVariables::txDutyDuration << "\t" <<
        GlobalVariables::txDutyCycleBackoff << "\t" <<
        (average_range_time / node_count) << "\t" <<
        // Packets
        average_packet_attempt << "\t" <<
        average_succ_packets << "\t" <<
        (average_packet_attempt / node_count) << "\t" <<
        (average_succ_packets / node_count) << "\t" <<
        totalNumberOfPacketSeries << "\t" <<
        (totalNumberofCompleteAttempts - totalNumberofTransmits) << "\t" <<
        (totalNumberofCompleteAttempts - totalNumberofTransmits) / (double)endDeviceN << "\t" <<
        average_total_transmitted << "\t" <<
        average_total_transmission_time_spent << "\t" <<
        // Data
        average_sent_bytes << "\t" <<
        (average_sent_bytes / node_count) << "\t" <<
        average_leftover_bytes << "\t" <<
        (average_leftover_bytes / node_count) << "\t" <<
        (average_last_leftover_bytes / node_count) << "\t" <<
        (average_real_leftover_bytes / node_count) << "\t";

    // haps
    gv->averageTxInformationFile <<
        gwPhyTotal << "\t" <<
        gwPhyReceived << "\t" <<
        gwPhyInterfered << "\t" <<
        gwPhyNoMoreReceivers << "\t" <<
        gwPhyUnderSensitivity << "\t" <<
        gwPhyLostBecauseTx << "\t";

    // Energy
    gv->averageTxInformationFile << totalEnergySpent << "\t";
    gv->averageTxInformationFile << sleepEnergySpent / (double)node_count << "\t";
    gv->averageTxInformationFile << awakeningEnergySpent / (double)node_count << "\t";
    gv->averageTxInformationFile << activeEnergySpent / (double)node_count << "\t";
    gv->averageTxInformationFile << collectionEnergySpent / (double)node_count << "\t";
    gv->averageTxInformationFile << shutdownEnergySpent / (double)node_count << "\t";
    gv->averageTxInformationFile << lorawanEnergySpent / (double)node_count << "\t";

    // Expectations
    gv->averageTxInformationFile << numberOfTXPoints << "\t";
    gv->averageTxInformationFile << totalTimeUnderHAPS << "\t";
    gv->averageTxInformationFile << expectedPackageCount;

    // Don't forget to close it
    gv->averageTxInformationFile.close ();    
}

void StatisticsHelper::FillSensorOutputFile (
    GlobalVariables *gv, NodeContainer gateways, NodeContainer endDevices, LoraPacketTracker &tracker)
{
    // File header
    gv->perSensorResultsFile <<
        // Sensor identifier
        "sensor_id" << "\t" <<

        // Clustering parameters
        "sensor_choice" << "\t" <<
        "duty_cycle_choice" << "\t" <<
        "haps_speed_choice" << "\t" <<
        "sigma_choice" << "\t" <<

        // Range time in seconds
        "Simulation Duration (seconds)" << "\t" <<

        // packets
        "Packet Attempts (Retransmission Included)" << "\t" <<
        "Successful Transmission Count" << "\t" <<
        "Transmission Count" << "\t" <<
        "Failed Packet Attempts" << "\t" << 
        "Time Spent for Successful Transmissions (seconds)" << "\t" <<

        // Data
        "Sent Data Size (byte)" << "\t" <<
        // "Total Left-over Data Size (byte)" << "\t" <<

        // Energy consumptions
        "Total Energy Consumption (J)" << "\t" <<
        "Hardware Sleep Mode Energy Consumption (J)" << "\t" << 
        "Hardware Awakening Mode Energy Consumption (J)" << "\t" <<
        "Hardware Active Mode Energy Consumption (J)" << "\t" << 
        "Hardware Collection Mode Energy Consumption (J)" << "\t" << 
        "Hardware Shutdown Mode Energy Consumption (J)" << "\t" << 
        "Lorawan Module Energy Consumption (J)";

    gv->perSensorResultsFile  << std::endl;

    // Filling the data per sensor
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        // Get the node
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));

        if (node->visitedOnce == true) {
            // Sensor identifier
            gv->perSensorResultsFile << node->sensor_id << "\t";

            // Clustering parameters
            gv->perSensorResultsFile << GlobalVariables::minCityEDCount << "\t";
            gv->perSensorResultsFile << GlobalVariables::txDutyCycleBackoff << "\t";
            gv->perSensorResultsFile << GlobalVariables::default_speed << "\t";
            gv->perSensorResultsFile << GlobalVariables::dist_sigma << "\t";

            // Generate Node Data
            double node_range_time = 0.0;
            double node_succ_packets = 0.0;
            double node_sent_bytes = 0.0;
            double node_leftover_bytes = 0.0;        

            // Iterate through the tx logs of this node
            for (const TxLog* log : node->txLogList) {
                node_range_time += log->interval;
                node_succ_packets += log->tx_count;
                // NS_LOG_UNCOND (log->tx_data);
                node_sent_bytes += log->tx_data;
                node_leftover_bytes += log->left_data;
            }

            double deviceEnergy = node->_customDeviceEnergyModel.GetConsumedEnergy ();

            DeviceEnergyModelContainer demc = node->demc;
            double lorawanEnergy = 0.0;
            for (DeviceEnergyModelContainer::Iterator demcIter = demc.Begin (); demcIter != demc.End (); demcIter ++)
            {
                lorawanEnergy += (*demcIter)->GetTotalEnergyConsumption ();        
            }  

            // Range time in seconds
            gv->perSensorResultsFile << node_range_time << "\t";
            
            // Packets
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._totalPacketAttempted << "\t";
            gv->perSensorResultsFile << node_succ_packets << "\t";
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._totalAttempted << "\t";
            gv->perSensorResultsFile << (node->_customDeviceEnergyModel._totalPacketAttempted - node->_customDeviceEnergyModel.GetTotalTransmitted ()) << "\t";
            gv->perSensorResultsFile << node->total_transmission_time_spent << "\t";

            // Data
            gv->perSensorResultsFile << node_sent_bytes << "\t";
            // gv->perSensorResultsFile << node_leftover_bytes << "\t";


            // Energy consumptions
            gv->perSensorResultsFile << (deviceEnergy + lorawanEnergy) << "\t";
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._sleepConsumedEnergy << "\t";
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._awakeningConsumedEnergy << "\t";
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._activeModeConsumedEnergy << "\t";
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._collectionModeConsumedEnergy << "\t";
            gv->perSensorResultsFile << node->_customDeviceEnergyModel._shutDownModeConsumedEnergy << "\t";
            gv->perSensorResultsFile << lorawanEnergy;
            
            // EOL
            gv->perSensorResultsFile << std::endl;
        }    
        else {
            NS_LOG_UNCOND ("A SENSOR DID NOT SEND A PACKET AT ALL!");
            // Check the minimum distance between this sensor and gateways
            for (NodeContainer::Iterator i = gateways.Begin (); i != gateways.End (); ++i) {
                Ptr<HAPNode> current_gateway = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
                // Get the location of both devices
                double currentDistance = HAPSMobilityManager::CalculateDistanceBetweenNodes (current_gateway, node);
                NS_LOG_UNCOND (currentDistance);
            }
        }    
    }    

    // Don't forget to close it
    gv->perSensorResultsFile.close ();   
};
