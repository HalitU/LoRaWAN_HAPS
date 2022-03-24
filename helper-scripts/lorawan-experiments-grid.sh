#!/bin/bash
set -eo pipefail
# Keeping old results
DELETE_EXISTNG_EXPERIMENTS=0

# Clear existing files and recreate the subdirectory
if (($DELETE_EXISTNG_EXPERIMENTS == 1))
then
    # Logging
    echo "-----------------------------------"
    echo "Clearing existing experiment results for consistency"
    echo "-----------------------------------"

    rm -rf experiments/lorawan-haps-results && mkdir experiments/lorawan-haps-results
else
    # Logging
    echo "-----------------------------------"
    echo "Not clearing existing files could mean any existing results will be taken in account"
    echo "-----------------------------------"
fi

# Remove the files used to store overall summaries
rm experiments/lorawan-haps-results/sensor_*

# Reset BASH time counter
SECONDS=0

# Whether to use the customly prepared parameters for test purposes or not
use_custom_test=0

# example script for clearing existing experiment records
# sudo rm -rf experiments/lorawan-haps-results && mkdir experiments/lorawan-haps-results

echo "-----------------------------------"
echo "Starting the lorawan experiments"
echo "-----------------------------------"
EXPERIMENT_COUNT=1

for sensor_choice in {0..6} # {0..5}
do
  for duty_cycle_choice in {0..4}
  do
    for haps_speed_choice in {0..3}
    do
      for sigma_choice in {0..2} # {0..4}
      do
        for ((repeat_count=1; repeat_count<=$EXPERIMENT_COUNT; repeat_count++))
        do
          echo "Running experiment: iteration=$repeat_count, sensor_choice=$sensor_choice, duty_cycle_choice=$duty_cycle_choice, haps_speed_choice=$haps_speed_choice, sigma_choice=$sigma_choice, use_custom_test=$use_custom_test"
          ./waf --run "lorawan-simulation-main --sensor_choice=$sensor_choice --duty_cycle_choice=$duty_cycle_choice --haps_speed_choice=$haps_speed_choice --sigma_choice=$sigma_choice --use_custom_test=$use_custom_test"

          # Logging
          echo "---------------------------------------------------------------------"
          ELAPSED="Current elapsed time: $(($SECONDS / 3600))hrs $((($SECONDS / 60) % 60))min $(($SECONDS % 60))sec"
          echo $ELAPSED      
          echo "---------------------------------------------------------------------"
        done
      done
    done
  done
done

# Run a python script for processing per sensor results
python process_haps_per_sensor_results.py && python process_haps_per_sensor_results_total.py

echo "Simulations are completed"
ELAPSED="Elapsed: $(($SECONDS / 3600))hrs $((($SECONDS / 60) % 60))min $(($SECONDS % 60))sec"
echo $ELAPSED