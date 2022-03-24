#include "radio-energy-helper.h"

void RadioEnergyHelper::InstallLorawanRadioEnergy (NodeContainer endDevices, NetDeviceContainer endDevicesNetDevices) {
    /************************
     * Install Energy Model on Edge Devices
     ************************/
    BasicEnergySourceHelper basicSourceHelper;
    LoraRadioEnergyModelHelper radioEnergyHelper;

    double BasicEnergySourceInitialEnergyJ = GlobalVariables::BasicEnergySourceInitialEnergyJ;
    double BasicEnergySupplyVoltageV = GlobalVariables::BasicEnergySupplyVoltageV;
    double StandbyCurrentA = GlobalVariables::StandbyCurrentA;
    double TxCurrentA = GlobalVariables::TxCurrentA;
    double SleepCurrentA = GlobalVariables::SleepCurrentA;
    double RxCurrentA = GlobalVariables::RxCurrentA;
   
    // configure energy source
    basicSourceHelper.Set ("BasicEnergySourceInitialEnergyJ", DoubleValue (BasicEnergySourceInitialEnergyJ)); // Energy in J
    basicSourceHelper.Set ("BasicEnergySupplyVoltageV", DoubleValue (BasicEnergySupplyVoltageV));

    radioEnergyHelper.Set ("StandbyCurrentA", DoubleValue (StandbyCurrentA));
    radioEnergyHelper.Set ("TxCurrentA", DoubleValue (TxCurrentA));
    radioEnergyHelper.Set ("SleepCurrentA", DoubleValue (SleepCurrentA));
    radioEnergyHelper.Set ("RxCurrentA", DoubleValue (RxCurrentA));

    radioEnergyHelper.SetTxCurrentModel ("ns3::ConstantLoraTxCurrentModel", "TxCurrent", DoubleValue (TxCurrentA));

    // install source on EDs' nodes
    EnergySourceContainer sources = basicSourceHelper.Install (endDevices);

    // install device model
    DeviceEnergyModelContainer clientDeviceEnergyModels = radioEnergyHelper.Install (endDevicesNetDevices, sources);

    // Get the energy pointers for nodes
    for (EnergySourceContainer::Iterator iter = sources.Begin (); iter != sources.End (); iter ++)
    {
        DeviceEnergyModelContainer demc = (*iter)->FindDeviceEnergyModels ("ns3::LoraRadioEnergyModel");
        // Get the node
        Ptr<Node> n = (*iter)->GetNode ();
        Ptr<HAPNode> node = dynamic_cast<HAPNode*>(&*n); 
        // Append the lorawan energy model to the node for getting more precise energy consuming results
        node->demc = demc;   
    }    
}