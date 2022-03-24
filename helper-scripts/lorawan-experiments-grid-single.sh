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

# example script for clearing existing experiment records
# sudo rm -rf experiments/lorawan-haps-results && mkdir experiments/lorawan-haps-results

echo "-----------------------------------"
echo "Starting the lorawan experiments"
echo "-----------------------------------"

./waf --run "lorawan-simulation-main --sensor_choice=0 --duty_cycle_choice=0 --haps_speed_choice=0 --sigma_choice=0 --use_custom_test=1"

# Logging
echo "---------------------------------------------------------------------"
ELAPSED="Current elapsed time: $(($SECONDS / 3600))hrs $((($SECONDS / 60) % 60))min $(($SECONDS % 60))sec"
echo $ELAPSED      
echo "---------------------------------------------------------------------"


# Run a python script for processing per sensor results
python process_haps_per_sensor_results.py && python process_haps_per_sensor_results_total.py

echo "Simulations are completed"
ELAPSED="Elapsed: $(($SECONDS / 3600))hrs $((($SECONDS / 60) % 60))min $(($SECONDS % 60))sec"
echo $ELAPSED