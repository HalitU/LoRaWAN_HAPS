#include "buildings-manager.h"

struct BuildingRecord {
  double x_min;
  double x_max;
  double y_min;
  double y_max;
  double z_min;
  double z_max;  
  int b_type;
  int ex_type;
  uint32_t n_floor;
  uint32_t n_room_x;
  uint32_t n_room_y;
} building_record;

void BuildingsManager::CreateBuildingsFromFile (string path) {
    ifstream in (path);
    string line;

    if (!in){
      NS_LOG_UNCOND ("File can't be opened!");
      exit(1);
    }

    bool skip = true;
    while (getline(in, line))
    {
      // skip first line
      if (skip) {
        skip = false;
        continue;
      }

      istringstream ss (line);

      ss >> building_record.x_min >> building_record.x_max >> building_record.y_min >> building_record.y_max
        >> building_record.z_min >> building_record.z_max >> building_record.b_type >> building_record.ex_type
        >> building_record.n_floor >> building_record.n_room_x >> building_record.n_room_y;

      Ptr <Building> b = CreateObject <Building> ();
      b->SetBoundaries (
          Box (building_record.x_min, 
            building_record.x_max, 
            building_record.y_min, 
            building_record.y_max, 
            building_record.z_min, 
            building_record.z_max));
      b->SetBuildingType ((Building::BuildingType_t) building_record.b_type);
      b->SetExtWallsType ((Building::ExtWallsType_t) building_record.ex_type);
      b->SetNFloors (building_record.n_floor);
      b->SetNRoomsX (building_record.n_room_x);
      b->SetNRoomsY (building_record.n_room_y);        
    }    
};

/**
 * For each city coordinate, a number of clouds with different sizes will be 
 * distributed on the air with different sizes to different angles.
 */
void BuildingsManager::DistributeRandomClouds (SensorTopologyManager stm)
{
  // TODO: NOT USED
// {  
//   // Random number generators
//   std::random_device rd;
//   std::mt19937 gen(rd());
//   std::uniform_int_distribution<> cloudCountDist (GlobalVariables::minCloudCount, GlobalVariables::maxCloudCount);
//   std::uniform_int_distribution<> cloudDistanceDist (GlobalVariables::cloudInnerCircleRadius, GlobalVariables::cloudOuterCircleRadius);
//   std::uniform_real_distribution<double> angleDist (0, GlobalVariables::fullCircle);
//   std::uniform_int_distribution<> cloudXSizeDist (GlobalVariables::minCloudXSize, GlobalVariables::maxCloudXSize);
//   std::uniform_int_distribution<> cloudYSizeDist (GlobalVariables::minCloudYSize, GlobalVariables::maxCloudYSize);

//   // For each city
//   for (CityCoordinates* const& iter : stm.ccoList) {
//     // How many clouds will be distributed?
//     int nextCloudCount = cloudCountDist (gen);
//     // Current city center coords
//     CoordDifference* cd = stm.hmm.CalculateDistanceBetweenTwoCoords (*stm.map_reference_coord, *iter);
//     double cityCenterX = cd->lat;
//     double cityCenterY = cd->lon;
//     // For each cloud
//     int ncc;
//     for (ncc = 0; ncc < nextCloudCount; ncc++) {
//       // Which angle this cloud will be placed on?
//       double nextCloudAngle = angleDist (gen);
//       // How far its corner will be to the city center?
//       int nextCloudDistance = cloudDistanceDist (gen);
//       // What will be the width (x, y) of it? (default z value assumption is made)
//       int nextCloudXSize = cloudXSizeDist (gen);
//       int nextCloudYSize = cloudYSizeDist (gen);
//       // Calculate the location to place cloud in
//       double trueCloudX = cityCenterX + cos (nextCloudAngle) * nextCloudDistance;
//       double trueCloudY = cityCenterY + sin (nextCloudAngle) * nextCloudDistance;
//       // Create the cloud building
//       Ptr <Building> b = CreateObject <Building> ();
//       b->SetBoundaries (
//           Box (trueCloudX, 
//             trueCloudX + nextCloudXSize, 
//             trueCloudY, 
//             trueCloudY + nextCloudYSize, 
//             GlobalVariables::cloudAltitude, 
//             GlobalVariables::cloudAltitude + GlobalVariables::cloudZSize));
//       b->SetBuildingType (Building::BuildingType_t::Cloud);
//       b->SetExtWallsType (Building::ExtWallsType_t::CloudExt);
//       b->SetNFloors (1);
//       b->SetNRoomsX (1);
//       b->SetNRoomsY (1);
//     }
//   }
};

void BuildingsManager::RegisterBuildingsToNodes (NodeContainer enddevices, NodeContainer gateways) {
    BuildingsHelper::Install (enddevices);
    BuildingsHelper::Install (gateways);    
};
