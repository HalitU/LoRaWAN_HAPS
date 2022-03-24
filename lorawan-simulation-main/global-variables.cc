#include "global-variables.h"

string GlobalVariables::PathLossChoice = "boat";
double GlobalVariables::PathLossExponent = 2; // 1.76; // 2; //TODO: fixxxxxxxx 3.76, 3.3 for corner 28 km case; paper mentions 2.65, 3.1 is nice
double GlobalVariables::ReferenceDistance = 1000; // 1 in meters
double GlobalVariables::ReferenceLoss = 91.22; // 126.43; // 91.22; // 7.7 13.225 mentioned in the paper

uint32_t GlobalVariables::nGateways = 1;
uint32_t GlobalVariables::nNetworkservers = 1;
double GlobalVariables::haps_delay = 0;//10000;

double GlobalVariables::main_distance = 6000;
double GlobalVariables::location_range;

// int appPeriodSeconds = 600;

string GlobalVariables::animFile = "lorawan-animation.xml";
string GlobalVariables::haps_coordinate_file_header = "scratch/lorawan-simulation-main/data/haps_coordinates/haps_coordinates_";
string GlobalVariables::city_coordinate_file = "scratch/lorawan-simulation-main/data/city_coords.txt";

uint32_t GlobalVariables::dataGenerationPeriod = 180;//3600;//320; // in seconds
uint32_t GlobalVariables::dataMaxSize = 6000;
uint32_t GlobalVariables::dataGenerationRate = 48;//52;
uint32_t GlobalVariables::dataCollectionTime = 3;    

// TODO: also changed coords for static etc.
double GlobalVariables::distanceBetweenCoords = 111000; // in meters
double GlobalVariables::default_speed = 20.27; // 2.53375; // 5.0675; // 10.135; // 20.27; // in meters 75 km into seconds with 75km/h speed
double GlobalVariables::simulationTime = 3600; // 28800 // 14400; // 7200; // 3600; // TODO: 86400;// 15000;//72000.0; 86400 seconds is one day

double GlobalVariables::dist_sigma;

// HAPS Movement
double GlobalVariables::HAPSMovementCalculationSeconds = 1;
double GlobalVariables::HAPSRange = 28284.2712475; // basically 20k * sqrt (2)

// Common circle variables
bool GlobalVariables::backoff = false; // ALWAYS SET IT HERE BECAUSE THESE ARE GLOBAL VALUES!!!
double GlobalVariables::fullCircle = 2 * M_PI;
double GlobalVariables::degToRad = M_PI / 180.0;
double GlobalVariables::ED_location_inner = 0.0; // TODO: tests for both 1000 and 5000

// Duty cycle parameters
double GlobalVariables::txDutyDuration = 4; // seconds 5
double GlobalVariables::txDutyCycleBackoff = 132; // seconds 135

double GlobalVariables::range_margin = 19000.0;

// End-device sensor distributions
double GlobalVariables::InnerCircleRadius = 1000;
double GlobalVariables::OuterCircleRadius = 1000;
int GlobalVariables::minCityEDCount = 5;
int GlobalVariables::maxCityEDCount = 7;

int GlobalVariables::expectedNumberOfMessages = 10;
double GlobalVariables::EDsupplyVoltage = 5.5;
double GlobalVariables::EDactiveCurrent = 0.1;
double GlobalVariables::EDawakeningCurrent = 0.15;
double GlobalVariables::EDcollectionCurrent = 0.12;
double GlobalVariables::EDshuttingdownCurrent = 0.08;
double GlobalVariables::EDsleepCurrent = 0.05;

Time GlobalVariables::EDactiveDuration = Seconds (10);
Time GlobalVariables::EDawakeningDuration = Seconds (11);
Time GlobalVariables::EDcollectionDuration = Seconds (3);
Time GlobalVariables::EDshuttingdownDuration = Seconds (4);
Time GlobalVariables::EDsleepingDuration = Seconds (5);

// Cloud buildings
int GlobalVariables::minCloudCount = 1; // 5000;
int GlobalVariables::maxCloudCount = 1; // 10000;
int GlobalVariables::cloudInnerCircleRadius = 1;
int GlobalVariables::cloudOuterCircleRadius = 1;// 1000;
int GlobalVariables::minCloudXSize = 1;// 100;
int GlobalVariables::maxCloudXSize = 1;// 500;
int GlobalVariables::minCloudYSize = 1;// 100;
int GlobalVariables::maxCloudYSize = 1;// 500;
int GlobalVariables::cloudZSize = 1;// 10;
int GlobalVariables::cloudAltitude = 7600;

//
bool GlobalVariables::DEBUG = false;
bool GlobalVariables::SM_DEBUG = false;

// network address
uint8_t GlobalVariables::nwkId = 54;
uint32_t GlobalVariables::nwkAddr = 1864;    

// Lorawan radio energy parameters
double GlobalVariables::BasicEnergySourceInitialEnergyJ = 10000;
double GlobalVariables::BasicEnergySupplyVoltageV = 5;
double GlobalVariables::StandbyCurrentA = 0.0014;
double GlobalVariables::TxCurrentA = 0.028;
double GlobalVariables::SleepCurrentA = 0.0000015;
double GlobalVariables::RxCurrentA = 0.0112;

Ptr<OutputStreamWrapper> GlobalVariables::experimentOverlappingInterferenceDurationsFile;

string GlobalVariables::experimentFileName;
string GlobalVariables::experimentFolder;
string GlobalVariables::experimentEnddeviceFolder;
string GlobalVariables::experimentEnddeviceTimedFolder;
string GlobalVariables::experimentEnddeviceDeviceStateFolder;
string GlobalVariables::experimentEnddeviceLorawanStateFolder;
string GlobalVariables::experimentEnddeviceDataRecordFolder;
string GlobalVariables::experimentGatewayMobilityFolder;

GlobalVariables::GlobalVariables (void) 
: experimentFile (experimentPath)
, improvedExperimentFile (improvedExperimentPath)
, phypacketgwFile (phypacketgwPath)
, energySpentOutsideLorawanFile (energySpentOutsideLorawanPath)
, enddeviceColletionTransmissionFile (enddeviceColletionTransmissionPath)
, enddeviceDataCollectionSummaryFile (enddeviceDataCollectionSummaryPath)
, enddeviceLorawanModuleEnergyConsumptionFile (enddeviceLorawanModuleEnergyConsumptionPath)
, experimentEnvironmentFile (experimentEnvironmentPath)
, averageTxInformationFile (averageTxInformationPath)
, perSensorResultsFile (perSensorResultsPath)
{
};

void GlobalVariables::CreateEssentialFolders () {
    mkdir (experimentFolder.c_str (), 0777);
    mkdir (experimentEnddeviceFolder.c_str (), 0777);
    mkdir (experimentEnddeviceTimedFolder.c_str (), 0777);
    mkdir (experimentEnddeviceDeviceStateFolder.c_str (), 0777);
    mkdir (experimentEnddeviceLorawanStateFolder.c_str (), 0777);
    mkdir (experimentEnddeviceDataRecordFolder.c_str (), 0777);
    mkdir (experimentGatewayMobilityFolder.c_str (), 0777);
};

// https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/
double round_double (double var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    double value = (int)(var * 10 + .5);
    return (double)value / 10;
}

void GlobalVariables::ParseArguments (int argc, char *argv[])
{
    // Setting path loss according to choice in this file
    if (GlobalVariables::PathLossChoice == "boat") {
        GlobalVariables::PathLossExponent = 1.76; // 2; //TODO: fixxxxxxxx 3.76, 3.3 for corner 28 km case; paper mentions 2.65, 3.1 is nice
        GlobalVariables::ReferenceDistance = 1000; // 1 in meters
        GlobalVariables::ReferenceLoss = 126.43; // 91.22; // 7.7 13.225 mentioned in the paper
        NS_LOG_UNCOND ("Using boat path-loss settings...");
    }
    else if (GlobalVariables::PathLossChoice == "free") {
        GlobalVariables::PathLossExponent = 2; //TODO: fixxxxxxxx 3.76, 3.3 for corner 28 km case; paper mentions 2.65, 3.1 is nice
        GlobalVariables::ReferenceDistance = 1000; // 1 in meters
        GlobalVariables::ReferenceLoss = 91.22; // 7.7 13.225 mentioned in the paper
        NS_LOG_UNCOND ("Using free-space path-loss settings...");
    }
    else {
        // Raise error because of invalid string
        NS_ASSERT (false);
    }

    // Env opts
    uint32_t sensor_choice = 0;
    uint32_t duty_cycle_choice = 1;
    uint32_t haps_speed_choice = 0;
    uint32_t sigma_choice = 0;
    uint32_t use_custom_test = 0;

    uint32_t sensor_list [7] = { 15, 25, 50, 75, 100, 125, 150 };
    double duty_cycle_list [5] = { 75.0, 105.0, 135.0, 165.0, 200.0 };
    double haps_speed_list [4] = { 6.25, 12.5, 18.75, 25 }; // { 3.125, 6.25, 12.5, 25 };
    double simulation_timer_list [4] = { 16000, 8000, 5333, 4000 }; // 100k meter to take
    double sigma_list [3] = { 15000.0, 20000.0, 25000.0 }; // { 250.0, 2000.0, 5000.0, 10000.0, 30000.0 };

    // Parsing args
    CommandLine cmd;
    cmd.AddValue("sensor_choice", "HAPS sensor choice list: { 4, 9, 16, 25, 36, 49, 64 }, default = 0", sensor_choice);
    cmd.AddValue("duty_cycle_choice", "Duty cycle choice list: { 120.0, 135.0, 150.0 }, default = 1", duty_cycle_choice);
    cmd.AddValue("haps_speed_choice", "HAPS speed choice list: { 2.53375, 5.0675, 10.135, 20.27 }, default = 0", haps_speed_choice);
    cmd.AddValue("sigma_choice", "Sigma choice list: { 250.0, 500.0, 1000.0, 1500.0 }, default = 0", sigma_choice);
    cmd.AddValue("use_custom_test", "use_custom_test choice: { 0, 1 }, default = 0", use_custom_test);
    
    cmd.Parse (argc, argv);

    if (use_custom_test == 0) {
        GlobalVariables::minCityEDCount = sensor_list [sensor_choice];
        GlobalVariables::maxCityEDCount = sensor_list [sensor_choice];

        GlobalVariables::default_speed = haps_speed_list [haps_speed_choice];
        GlobalVariables::simulationTime = simulation_timer_list [haps_speed_choice];

        GlobalVariables::txDutyCycleBackoff = duty_cycle_list [duty_cycle_choice];

        GlobalVariables::dist_sigma = sigma_list [sigma_choice];

        // GlobalVariables::txDutyDuration = floor (GlobalVariables::txDutyCycleBackoff / GlobalVariables::minCityEDCount);
        GlobalVariables::txDutyDuration = GlobalVariables::txDutyCycleBackoff / GlobalVariables::minCityEDCount;
    }
    // GlobalVariables::txDutyDuration = round_double (GlobalVariables::txDutyDuration);
    else {
        GlobalVariables::minCityEDCount = 17;
        GlobalVariables::maxCityEDCount = 17;

        GlobalVariables::default_speed = 6.25;
        GlobalVariables::simulationTime = 16000;

        GlobalVariables::txDutyCycleBackoff = 135.0;

        GlobalVariables::dist_sigma = 5000.0;

        // GlobalVariables::txDutyDuration = floor (GlobalVariables::txDutyCycleBackoff / GlobalVariables::minCityEDCount);
        GlobalVariables::txDutyDuration = GlobalVariables::txDutyCycleBackoff / GlobalVariables::minCityEDCount;
    }

    NS_LOG_UNCOND ("Time-slot is adjusted as: " << GlobalVariables::txDutyDuration << " , with duty-cycle: " << GlobalVariables::txDutyCycleBackoff);
    NS_LOG_UNCOND ("Sensor count is chosen as: " << GlobalVariables::minCityEDCount << " , sigma range std chosen as: " << GlobalVariables::dist_sigma);
};


