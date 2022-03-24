#include <chrono> 
#include "ns3/core-module.h"
#include "CustomEnergyModel.h"
#include "ns3/end-device-lora-phy.h"

using namespace ns3;
using namespace std::chrono; 

CustomEnergyModel::CustomEnergyModel(){};

void CustomEnergyModel::UpdateConsumedEnergy (double energyConsumed){
    if (_consumedEnergy <= 0 && _consumedEnergy > -0.5) {
        _consumedEnergy = 0.0;
    }
    NS_ASSERT_MSG (_consumedEnergy >= 0.0, "Energy consumption below 0, value is: " << _consumedEnergy);
    _consumedEnergy = _consumedEnergy + energyConsumed;
};

bool CustomEnergyModel::GetIsTransmitting ()
{
    return _isTransmitting;
}

bool CustomEnergyModel::GetIsCollecting ()
{
    return _isCollecting;
}

// Getters and Setters
CustomDeviceState CustomEnergyModel::GetCurrentState ()
{
    return _currentState;
};
double CustomEnergyModel::GetLastUpdateTime ()
{
    return _lastStateUpdateTime;
};
// Update and return the last controlled time
double CustomEnergyModel::GetLastEnergyControlTime ()
{
    return _lastEnergyControlTime;
};
double CustomEnergyModel::GetConsumedEnergy ()
{
    return _consumedEnergy;
};


uint32_t CustomEnergyModel::GetLorawanDeviceState ()
{
    return _trackedLorawanState;
}

double CustomEnergyModel::GetSupplyVoltage () {
    return _supplyVoltage;
}
double CustomEnergyModel::GetSleepCurrent () {
    return _sleepCurrent;
}
double CustomEnergyModel::GetActiveCurrent () {
    return _activeCurrent;
}
double CustomEnergyModel::GetAwakeningCurrent () {
    return _awakeningCurrent;
}
double CustomEnergyModel::GetShuttingDownCurrent () {
    return _shuttingDownCurrent;
}
double CustomEnergyModel::GetCollectingCurrent () {
    return _collectingCurrent;
}

Time CustomEnergyModel::GetSleepTime ()
{
    return _sleepTime;
}
Time CustomEnergyModel::GetActiveTime ()
{
    return _activeTime;
}
Time CustomEnergyModel::GetAwakeningTime ()
{
    return _awakeningTime;
}
Time CustomEnergyModel::GetShuttingDownTime ()
{
    return _shuttingDownTime;
}
Time CustomEnergyModel::GetCollectingTime ()
{
    return _collectingTime;
}

void CustomEnergyModel::SetLastEnergyControlTime (double seconds) {
    NS_ASSERT (seconds != 0.0);
    _lastEnergyControlTime = seconds;
}

void CustomEnergyModel::SetIsTransmitting (bool isTransmitting)
{
    _isTransmitting = isTransmitting;
}

void CustomEnergyModel::SetIsCollecting (bool isCollecting)
{
    _totalCollected++;
    _isCollecting = isCollecting;
}

/**
 * We can calculate the consumed energy during state transitions.
 * TODO: How about the consumption for the final state where no transition exists?
 */
void CustomEnergyModel::SetCurrentState (CustomDeviceState newState)
{
    _currentState = newState;
}

void CustomEnergyModel::SetSupplyVoltage (double supplyVoltage){
    _supplyVoltage = supplyVoltage;
};

void CustomEnergyModel::SetSleepCurrent (double sleepCurrent){
    _sleepCurrent = sleepCurrent;
};
void CustomEnergyModel::SetActiveCurrent (double activeCurrent){
    _activeCurrent = activeCurrent;
};
void CustomEnergyModel::SetAwakeningCurrent (double awakeningCurrent){
    _awakeningCurrent = awakeningCurrent;
};
void CustomEnergyModel::SetShuttingDownCurrent (double shuttingDownCurrent){
    _shuttingDownCurrent = shuttingDownCurrent;
};
void CustomEnergyModel::SetCollectingCurrent (double collectingCurrent){
    _collectingCurrent = collectingCurrent;
};

void CustomEnergyModel::SetSleepTime (Time sleepTime){
    _sleepTime = sleepTime;
};
void CustomEnergyModel::SetActiveTime (Time activeTime){
    _activeTime = activeTime;
};
void CustomEnergyModel::SetAwakeningTime (Time awakeningTime){
    _awakeningTime = awakeningTime;
}; 
void CustomEnergyModel::SetShuttingDownTime (Time shuttingDownTime){
    _shuttingDownTime = shuttingDownTime;
};
void CustomEnergyModel::SetCollectingTime (Time collectingTime){
    _collectingTime = collectingTime;
};


void CustomEnergyModel::SetTrackedLorawanDeviceState (uint32_t newState)
{
    _trackedLorawanState = newState;
}

/**
 * Helpers
 */
std::string CustomEnergyModel::GetStateName (CustomDeviceState state)
{
    std::string stateName;
    switch (state)
    {
    case CustomDeviceState::ACTIVE:
        stateName = "ACTIVE";
        break;
    case CustomDeviceState::SLEEPING:
        stateName = "SLEEPING";
        break;
    default:
        break;
    }
    return stateName;
}

uint32_t CustomEnergyModel::GetTotalCollected ()
{
    return _totalCollected;
}
uint32_t CustomEnergyModel::GetTotalTransmitted ()
{
    return _totalTransmitted;
}
uint32_t CustomEnergyModel::GetTotalAttempted ()
{
    return _totalAttempted;
}


