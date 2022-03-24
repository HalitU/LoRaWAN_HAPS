#ifndef CUSTOMENERGYMODEL_H
#define CUSTOMENERGYMODE_H

#include <chrono> 
#include "ns3/core-module.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/gateway-lorawan-mac.h"
#include "ns3/end-device-lora-phy.h"

using namespace ns3;
using namespace std::chrono; 

// Do we need the awakening and shutting down states?
enum CustomDeviceState { SLEEPING, ACTIVE };
enum CustomSimulatingState { WAKINGUP, COLLECTING, SHUTTINGDOWN, NORMAL };
enum CustomDeviceEvent { WAKEUP, SHUTDOWN };

class CustomEnergyModel
{
    private:
        bool _isTransmitting = false;
        bool _isCollecting = false;

        uint32_t _trackedLorawanState;


        // Currents related to the other device parts other than the lorawan hat
        double _supplyVoltage;
        double _sleepCurrent;
        double _activeCurrent;
        double _awakeningCurrent;
        double _shuttingDownCurrent;    
        double _collectingCurrent;

        // Time which should be spent at each state
        Time _sleepTime;
        Time _activeTime;
        Time _awakeningTime;
        Time _shuttingDownTime;
        Time _collectingTime;

        // @Unused
        uint32_t _maxNumberOfCollection;
        uint32_t _maxNumberOfTransmission;


    public:

        // Keeps track of total consumed energy
        /**
         * SLEEP=0
         * STANDBY=1
         * TX=2
         * RX=3
         */
        CustomDeviceState _currentState;
        double _consumedEnergy;
        uint32_t _totalTransmitted;
        uint32_t _totalAttempted;
        uint32_t _totalPacketAttempted;
        double _totalSentBytes;
        uint32_t _totalCollected;
        uint32_t _someUpdate;
        double _lastStateUpdateTime;
        double _lastEnergyControlTime;
        CustomSimulatingState _currentSimulatingState = CustomSimulatingState::NORMAL;
        double _totalProcessedEnergyTime = 0.0;

        double _sleepConsumedEnergy;
        double _awakeningConsumedEnergy;
        double _activeModeConsumedEnergy;
        double _collectionModeConsumedEnergy;
        double _shutDownModeConsumedEnergy;

        CustomEnergyModel ();

        // Changes state and returns the scheduling time for next state change
        void UpdateConsumedEnergy (double energyConsumed);
        bool GetIsTransmitting ();
        bool GetIsCollecting ();

        // Getters and Setters
        CustomDeviceState GetCurrentState ();
        double GetLastUpdateTime ();
        double GetLastEnergyControlTime ();
        double GetConsumedEnergy ();
        uint32_t GetLorawanDeviceState ();

        double GetSupplyVoltage ();
        double GetSleepCurrent ();
        double GetActiveCurrent ();
        double GetAwakeningCurrent ();
        double GetShuttingDownCurrent ();
        double GetCollectingCurrent ();

        Time GetSleepTime ();
        Time GetActiveTime ();
        Time GetAwakeningTime ();
        Time GetShuttingDownTime ();
        Time GetCollectingTime ();

        void SetLastEnergyControlTime (double seconds);

        void SetIsTransmitting (bool isTransmitting);
        void SetIsCollecting (bool isCollecting);
        void SetCurrentState (CustomDeviceState newState);

        void SetSupplyVoltage (double supplyVoltage);
        void SetSleepCurrent (double sleepCurrent);
        void SetActiveCurrent (double activeCurrent);
        void SetAwakeningCurrent (double awakeningCurrent);
        void SetShuttingDownCurrent (double shuttingDownCurrent);
        void SetCollectingCurrent (double collectingCurrent);

        void SetSleepTime (Time sleepTime);
        void SetActiveTime (Time activeTime);
        void SetAwakeningTime (Time awakeningTime); 
        void SetShuttingDownTime (Time shuttingDownTime);
        void SetCollectingTime (Time collectingTime);

        void SetTrackedLorawanDeviceState (uint32_t newState);

        // Helpers
        std::string GetStateName (CustomDeviceState state);
        uint32_t GetTotalCollected ();
        uint32_t GetTotalTransmitted ();
        uint32_t GetTotalAttempted ();
};

#endif
