#include "network-server-manager.h"

NodeContainer NetworkServerManager::InstallNetworkServer (uint32_t nNetworkservers, MobilityHelper mobility,
    NetworkServerHelper nsHelper, NodeContainer endDevices, NodeContainer gateways, ForwarderHelper forHelper) {
    // Create the NS node
    NodeContainer networkServer;
    networkServer.Create (nNetworkservers);

    // Assign a mobility model to the node
    mobility.Install (networkServer);

    // Create a NS for the network
    nsHelper.SetEndDevices (endDevices);
    nsHelper.SetGateways (gateways);
    nsHelper.Install (networkServer);

    //Create a forwarder for each gateway
    forHelper.Install (gateways);

    // Assigning specific locations for the network server
    for (NodeContainer::Iterator j = networkServer.Begin (); j != networkServer.End (); ++j)
    {
      Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
      Vector position = mobility->GetPosition ();
      position.x = 5.0;
      position.y = 50.0;
      position.z = 0.0;
      mobility->SetPosition (position);
    };  

    return networkServer;
};