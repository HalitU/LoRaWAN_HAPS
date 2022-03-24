#ifndef HAPNODE_H
#define HAPNODE_H

#include "ns3/core-module.h"
#include "ns3/node.h"
#include "CustomEnergyModel.h"

#include "ns3/energy-module.h"
#include "ns3/wifi-radio-energy-model-helper.h"
#include "ns3/basic-energy-source-helper.h"
#include "ns3/lora-radio-energy-model-helper.h"

#include "global-variables.h"

#include <iostream>
#include <fstream>

using namespace ns3;
using namespace std;

class TxInterval
{
    public:

    double TxStartTime = -1;
    double TxEndTime = -1;
    double TxPeriod = -1;
};

class CityCoordinates {
    public:
    double lat;
    double lon;
    double visited = false;
    double initial = false;
};

class TxPoint {
    public:
    double TxTime; 
    bool used;
};

class TxLog {
    public:
    double interval;
    uint32_t tx_count;
    uint32_t tx_data;
    uint32_t left_data;
};

struct HAPSCoordTimers {
    public:
    double lat_speed;
    double lon_speed;
    double timer;
    bool finished;
};

// Main Node
class HAPNode : public Node
{
private:
    double PI;
    double angleChange;
    double X;
    double Y;

    // Data storage related values
    double data_max_size;
    double data_generation_rate;
    double data_generation_period;
    double data_current_size = 0;
    double data_wasted_size = 0;
    double data_collection_time;

    std::string NodeType = "";



public:

    // Gateway coordinate information
    list <CityCoordinates*> ccoList;
    list <HAPSCoordTimers*> hapsCoordTimers;
    CityCoordinates *map_reference;
    CityCoordinates *current_coords;
    CityCoordinates *target_coords;
    //

    list <TxLog*> txLogList;

    bool visitedOnce = false;
    
    int sensor_id;

    // Indicates how many messages should be tried to sent during the 
    // expected good signal window
    uint32_t expectedNumberOfMessages = 0;
    uint32_t currentMessageCount = 0;

    int last_collection_time = 0;

    // Data transmission records
    double last_in_range_time = -1.0;
    double last_out_range_time = -1.0;
    double current_range_transmitted_data = -1.0;
    int current_transmitted_count = 0;


    // Haps information
    double HAPS_CENTER_X;
    double HAPS_CENTER_Y;
    double HAPS_ANGLE_PER_SEC;
    double HAPS_RADIUS;
    double haps_delay;

    double last_transmission_end_time = -1;
    double last_transmission_start_time = -1;
    double last_transmission_expected_start_time = -1;
    double total_transmission_time_spent = 0;
    uint32_t total_tx_ack_packets = 0;
    //
    double HAPS_ORIGIN_X_SPEED;
    double HAPS_ORIGIN_Y_SPEED;

    double distance_from_haps;
    double distance_angle;

    NodeContainer gateways;

    // Timer for TX scheduling
    list <TxInterval> txWindowList;
    list <int> txPointListt;
    list <TxPoint> txPointList;

    //
    CustomEnergyModel _customDeviceEnergyModel;
    DeviceEnergyModelContainer demc;

    // Loggers
    ofstream enddeviceDeviceStateFile;
    ofstream enddeviceLorawanStateFile;
    ofstream experimentGatewayMobilityFile;

    // Constructor
    HAPNode();

    // Data storage related functions
    double GetDataMaxSize();
    double GetDataCurrentSize();
    double GetDataGenerationRate();
    double GetDataGenerationPeriod();
    double GetDataWastedSize();
    double GetDataCollectionTime ();

    std::string GetNodeType ();

    void SetDataMaxSize(double newMaxSize);
    void SetDataCurrentSize(double newCurrentSize);
    void SetDataGenerationRate(double newGenerationRate);
    void SetDataGenerationPeriod(double newGenerationPeriod);
    void SetDataCollectionTime (double newDataCollectionTime);

    void SetNodeType (std::string nodeType);

    void IncrementCurrentSize();
    void ResetSize();
    void DecrementCurrentSize(double sizeToRemove);

    // Data transmission records
    void update_last_in_range_time (double current_time);
    bool update_last_out_range_time (double current_time);
    void update_current_range_transmitted_data (double transmitted_data);
    void reset_last_in_range_time ();
    void reset_last_out_range_time ();
    void reset_current_range_transmitted_data ();
    void write_latest_data_tx_record ();

    // Getters
    double GetXVelocity();
    double GetYVelocity();

    // Setters
    void SetXVelocity(double X_new);
    void SetYVelocity(double Y_new);
};

#endif