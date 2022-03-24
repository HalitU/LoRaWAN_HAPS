#include "address-manager.h"


void AddressManager::InstallAddressToDevices (
        LoraPhyHelper phyHelper, LorawanMacHelper macHelper, LoraHelper helper, NodeContainer endDevices) {
    // Create the LoraNetDevices of the end devices
    uint8_t nwkId = GlobalVariables::nwkId;
    uint32_t nwkAddr = GlobalVariables::nwkAddr;
    Ptr<LoraDeviceAddressGenerator> addrGen =
        CreateObject<LoraDeviceAddressGenerator> (nwkId, nwkAddr);

    // Create the LoraNetDevices of the end devices
    macHelper.SetAddressGenerator (addrGen);
    macHelper.SetRegion (LorawanMacHelper::SingleChannel);
    phyHelper.SetDeviceType (LoraPhyHelper::ED);
    macHelper.SetDeviceType (LorawanMacHelper::ED_A);
    endDevicesNetDevices = helper.Install (phyHelper, macHelper, endDevices);      
    
    // Set message type (Default is unconfirmed)
    for (NodeContainer::Iterator i = endDevices.Begin (); i != endDevices.End (); ++i) {
        Ptr<HAPNode> node = (dynamic_cast<HAPNode *> (PeekPointer(*i)));
        Ptr<LorawanMac> edMac1 = node->GetDevice (0)->GetObject<LoraNetDevice> ()->GetMac ();
        // ClassAEndDeviceLorawanMac
        // EndDeviceLorawanMac
        Ptr<EndDeviceLorawanMac> edLorawanMac1 = edMac1->GetObject<EndDeviceLorawanMac> ();
        edLorawanMac1->SetMType (LorawanMacHeader::CONFIRMED_DATA_UP);
    };
};