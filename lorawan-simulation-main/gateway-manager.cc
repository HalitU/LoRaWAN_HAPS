#include "gateway-manager.h"


NodeContainer GatewayManager::CreateGateways (uint32_t nGateways, MobilityHelper mobility) {
    // Create the gateway nodes
    NodeContainer gateways;
    for (uint32_t i = 0; i < nGateways; i++)
    {
        Ptr<HAPNode> newNode = CreateObject<HAPNode> ();
        gateways.Add (newNode);
    }    
  
    // Assign a mobility model to the node
    mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");    
    mobility.Install (gateways);

    return gateways;
};


void GatewayManager::InstallGatewayNetDevices (
    LoraPhyHelper phyHelper, LorawanMacHelper macHelper, LoraHelper helper, NodeContainer gateways) {
    // Create a netdevice for each gateway
    phyHelper.SetDeviceType (LoraPhyHelper::GW);
    macHelper.SetDeviceType (LorawanMacHelper::GW);
    helper.Install (phyHelper, macHelper, gateways);  
};