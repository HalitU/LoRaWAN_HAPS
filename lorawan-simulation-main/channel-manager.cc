#include "channel-manager.h"

Ptr<LoraChannel> ChannelManager::InitializeChannelWithLoss (double PathLossExponent, double ReferenceDistance, double ReferenceLoss) {
    Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel> ();
    loss->SetPathLossExponent (PathLossExponent);
    loss->SetReference (ReferenceDistance, ReferenceLoss);

    // // Create the correlated shadowing component
    // Ptr<CorrelatedShadowingPropagationLossModel> shadowing =
    //     CreateObject<CorrelatedShadowingPropagationLossModel> ();

    // // Aggregate shadowing to the logdistance loss
    // loss->SetNext (shadowing);

    // // Add the effect to the channel propagation loss
    // Ptr<BuildingPenetrationLoss> buildingLoss = CreateObject<BuildingPenetrationLoss> ();
    // shadowing->SetNext (buildingLoss);

    Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();

    Ptr<LoraChannel> channel = CreateObject<LoraChannel> (loss, delay);       

    return channel;
};