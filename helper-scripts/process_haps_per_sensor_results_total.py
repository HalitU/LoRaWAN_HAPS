# https://stackoverflow.com/questions/3207219/how-do-i-list-all-files-of-a-directory
import glob
import numpy as np

import pandas as pd

import sys

# We need to separate each experiment into different sections according to their ED count and speed
results = {}
cum_results = {}
sum_results = {}

# Get all of the results
main_directory = "experiments/lorawan-haps-results/"
experiment_result_list = glob.glob(main_directory + "*/per_sensor_results.txt")

print ("Reading experiment files...")

# Read the experiment results
for ex_file in experiment_result_list:
    # Read input file
    fin = open (ex_file, "rt")

    data_f = pd.read_csv(ex_file, sep="\t")

    # Get the lines
    all_lines = fin.read().splitlines()

    # Read the first line for header
    header = all_lines [0]

    # Read the sensor data lines
    for line in all_lines [1:]:
        # Get the cluster parameters and the end-device name
        # key = distance_threshold	outlier_threshold	remove_intersections	sensor_name
        current_results = line.split ('\t')
                
        sensor_choice = int (current_results [1])
        duty_cycle_choice = float (current_results [2])
        haps_speed_choice = float (current_results [3])
        sigma_choice = float (current_results [4])
        
        crr_results = []
        for name, values in data_f.iloc [:, 5:].sum ().iteritems():
            crr_results.append (values)

        if (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice) in sum_results:
            sum_results [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice].append (crr_results)
        else:
            sum_results [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice] = []
            sum_results [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice].append (crr_results)

        break

def print_all_results (results):
    # Print the results into a new file
    results_file = open (main_directory + 'sensor_all_results_per_experiment.csv', "w")

    # Write single header to avg results file
    results_file.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        fixed_list = []
        for v in value:
            fixed_list.append ([sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice] + v)

        # print (np.array (fixed_list).shape)
        dframe = pd.DataFrame(fixed_list)
        dframe.to_csv (results_file, mode='a', header=False, index=False)

    results_file.close ()

def print_std_results (results):
    # Print the results into a new file
    results_file = open (main_directory + 'sensor_std_results_per_configuration.csv', "w")

    # Write single header to avg results file
    results_file.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        dframe = pd.DataFrame(value)  
        stds = [s for s in dframe.std ()]
        result = [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice] + stds

        # Calculate average and write it to a separate file
        current_average_string = ','.join ([str(num) for num in result])

        results_file.write (current_average_string + '\n')

    results_file.close ()

def print_mean_results (results):
    # Print the results into a new file
    results_file = open (main_directory + 'sensor_mean_results_per_configuration.csv', "w")

    # Write single header to avg results file
    results_file.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        dframe = pd.DataFrame(value)
        means = [s for s in dframe.mean ()]
        result = [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice] + means

        # Calculate average and write it to a separate file
        current_average_string = ','.join ([str(num) for num in result])

        results_file.write (current_average_string + '\n')

    results_file.close ()


print_all_results (sum_results)
print_mean_results (sum_results)
print_std_results (sum_results)

#################################################
### FIX dot to comma in state files
print ("Done.")