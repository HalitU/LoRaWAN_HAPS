#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "ns3/lora-helper.h"
#include "ns3/core-module.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/trace-helper.h"
#include <chrono> 
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

using namespace ns3;
using namespace lorawan;
using namespace std;
using namespace std::chrono;

class GlobalVariables {
    private:

    public:

    static string PathLossChoice;
    static double PathLossExponent; //TODO: fixxxxxxxx 3.76, 3.3 seems the corner case for safely sending;
    static double ReferenceDistance; // 1
    static double ReferenceLoss; // 7.7

    static uint32_t nGateways;
    static uint32_t nNetworkservers;
    static double haps_delay;//10000;

    static double simulationTime;// 15000;//72000.0;
    
    static string animFile;
    static string haps_coordinate_file_header;
    static string city_coordinate_file;

    static uint32_t dataGenerationPeriod;//320; // in seconds
    static uint32_t dataMaxSize;
    static uint32_t dataGenerationRate;//52;
    static uint32_t dataCollectionTime;    

    static double distanceBetweenCoords; // in meters
    static double default_speed; // in meters

    static double dist_sigma;

    // HAPS Movement
    static double HAPSMovementCalculationSeconds;
    static double HAPSRange; // basically 20k * sqrt (2)
    static uint32_t EDIdleDurationBeforeNextRangeControl;

    // Common circle variables
    static bool backoff; // ALWAYS SET IT HERE BECAUSE THESE ARE GLOBAL VALUES!!!
    static double fullCircle;
    static double degToRad;
    static Ptr<OutputStreamWrapper> experimentOverlappingInterferenceDurationsFile;
    static double ED_location_inner; // TODO: tests for both 1000 and 5000

    // Duty cycle parameters
    static double txDutyDuration; // seconds 5
    static double txDutyCycleBackoff; // seconds 135

    static double range_margin;

    // End-device sensor distributions
    static double InnerCircleRadius;
    static double OuterCircleRadius;
    static int minCityEDCount;
    static int maxCityEDCount;

    static double main_distance;
    static double location_range;

    static int expectedNumberOfMessages;
    static double EDsupplyVoltage;
    static double EDactiveCurrent;
    static double EDawakeningCurrent;
    static double EDcollectionCurrent;
    static double EDshuttingdownCurrent;
    static double EDsleepCurrent;

    static Time EDactiveDuration;
    static Time EDawakeningDuration;
    static Time EDcollectionDuration;
    static Time EDshuttingdownDuration;
    static Time EDsleepingDuration;

    // Cloud buildings
    static int minCloudCount; // 5000;
    static int maxCloudCount; // 10000;
    static int cloudInnerCircleRadius;
    static int cloudOuterCircleRadius;// 1000;
    static int minCloudXSize;// 100;
    static int maxCloudXSize;// 500;
    static int minCloudYSize;// 100;
    static int maxCloudYSize;// 500;
    static int cloudZSize;// 10;
    static int cloudAltitude;

    //
    static bool DEBUG;
    static bool SM_DEBUG;

    // network address
    static uint8_t nwkId;
    static uint32_t nwkAddr;

    // Lorawan radio energy parameters
    static double BasicEnergySourceInitialEnergyJ;
    static double BasicEnergySupplyVoltageV;
    static double StandbyCurrentA;
    static double TxCurrentA;
    static double SleepCurrentA;
    static double RxCurrentA;

    // Tracker paths
    // void PrepareTrackerPaths ();

    GlobalVariables (void);

    AsciiTraceHelper customStaticsOutputAsciiHelper;
    string hapsMovementTrackerPath = experimentFolder + "haps_movement_tracker_for_windows.txt";;
    Ptr<OutputStreamWrapper> hapsMovementTrackerFile = customStaticsOutputAsciiHelper.CreateFileStream (hapsMovementTrackerPath);;

    // TODO: Statistical outputs
    static void CreateEssentialFolders ();

    static string experimentFileName;
    static string experimentFolder;
    static string experimentEnddeviceFolder;
    static string experimentEnddeviceTimedFolder;
    static string experimentEnddeviceDeviceStateFolder;
    static string experimentEnddeviceLorawanStateFolder;
    static string experimentEnddeviceDataRecordFolder;
    static string experimentGatewayMobilityFolder;

    string experimentPath = experimentFolder + "main_summary.txt";
    string improvedExperimentPath = experimentFolder + "improved_main_summary.txt";
    string phypacketgwPath = experimentFolder + "phy_packet_gw.txt";
    string energySpentOutsideLorawanPath = experimentFolder + "energy_spent_per_enddevice_except_lorawan_module.txt";
    string enddeviceColletionTransmissionPath = experimentFolder + "enddevice_collection_transmission_counts.txt";
    string enddeviceDataCollectionSummaryPath = experimentFolder + "enddevice_collection_summary.txt";
    string enddeviceLorawanModuleEnergyConsumptionPath = experimentFolder + "enddevice_lorawan_module_energy_consumption.txt";
    string experimentEnvironmentPath = experimentFolder + "experiment_environment_locs.txt";
    string hapsMovementTrackerRealPath = experimentFolder + "haps_movement_tracker_for_real.txt";
    string hapsMovementTrackerNetworkPath = experimentFolder + "haps_movement_tracker_for_network.txt";
    string averageTxInformationPath = experimentFolder + "average_tx_information.txt";
    string perSensorResultsPath = experimentFolder + "per_sensor_results.txt";

    string pythonExperimentEnvVisualizationPath = "python visualize_experiment_environment_colored.py " + experimentEnvironmentPath;

    ofstream experimentFile;
    ofstream improvedExperimentFile;  
    ofstream phypacketgwFile;
    ofstream energySpentOutsideLorawanFile;
    ofstream enddeviceColletionTransmissionFile;
    ofstream enddeviceDataCollectionSummaryFile;
    ofstream enddeviceLorawanModuleEnergyConsumptionFile;
    ofstream experimentEnvironmentFile;
    ofstream averageTxInformationFile;
    ofstream perSensorResultsFile;

    static void ParseArguments (int argc, char *argv[]);
};

#endif