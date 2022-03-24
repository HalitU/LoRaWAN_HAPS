#include "helper-manager.h"


void HelperManager::InitializeHelpers (Ptr<LoraChannel> channel) {
    // Create the LoraPhyHelper
    phyHelper = LoraPhyHelper ();
    phyHelper.SetChannel (channel);

    // Create the LorawanMacHelper
    macHelper = LorawanMacHelper ();

    // Create the LoraHelper
    helper = LoraHelper ();
    helper.EnablePacketTracking (); // Output filename
    // helper.EnableSimulationTimePrinting ();

    //Create the NetworkServerHelper
    nsHelper = NetworkServerHelper ();

    //Create the ForwarderHelper
    forHelper = ForwarderHelper ();

    // Position helper for 3d grid
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");    
};

void HelperManager::InitializeSimulationRandomness () {
    // Random seed this is the proper way of setting the seed
    srand (time(NULL)); // TODO: dont forget to re-activate!
    int random_seed = (rand() % 100) + 1; 
    
    // srand (1337); // TODO: remove
    // int random_seed = 68; // TODO: dont forget to remove!
    
    NS_LOG_UNCOND ("Setting random seed as: " << random_seed);
    ns3::RngSeedManager::SetSeed(random_seed); // TODO: activate!    
};

void HelperManager::SetupEndDeviceStateTrackers (NodeContainer endDevices, GlobalVariables* gv)
{
    // Device states and lorawan states for each end-device
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i)
    {
        // Get the node
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 

        // Create the corresponding files and attach them to the node
        string experimentEnddeviceDeviceStatePath = gv->experimentEnddeviceDeviceStateFolder + "device_state_" + to_string (node->GetId ()) + ".txt";
        string experimentEnddeviceLorawanStatePath = gv->experimentEnddeviceLorawanStateFolder + "lorawan_state_" + to_string (node->GetId ()) + ".txt";

        node->enddeviceDeviceStateFile = ofstream (experimentEnddeviceDeviceStatePath);
        node->enddeviceLorawanStateFile = ofstream (experimentEnddeviceLorawanStatePath);

        // Write the initial states
        if (gv->DEBUG) {
            node->enddeviceDeviceStateFile << Simulator::Now ().GetSeconds () << "\t" << 0 << endl;
            node->enddeviceLorawanStateFile << Simulator::Now ().GetSeconds () << "\t" << 0 << endl;
        }
    }    
};

void HelperManager::SetupGatewayMobilityTrackers (NodeContainer gateways, GlobalVariables* gv)
{
    // Device states and lorawan states for each end-device
    for (NodeContainer::Iterator i = gateways.Begin (); i != gateways.End (); ++i)
    {
        // Get the node
        Ptr<Node> n = (*i);
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 

        // Create the corresponding files and attach them to the node
        string experimentGatewayMobilityPath = gv->experimentGatewayMobilityFolder + "gateway_mobility_" + to_string (node->GetId ()) + ".txt";

        node->experimentGatewayMobilityFile = ofstream (experimentGatewayMobilityPath);
    }    
};