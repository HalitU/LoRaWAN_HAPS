# https://stackoverflow.com/questions/3207219/how-do-i-list-all-files-of-a-directory
import glob
import numpy as np

import pandas as pd

import sys

# We need to separate each experiment into different sections according to their ED count and speed
results = {}
cum_results = {}

# Get all of the results
main_directory = "experiments/lorawan-haps-results/"
experiment_result_list = glob.glob(main_directory + "*/per_sensor_results.txt")

print ("Reading experiment files...")

# Read the experiment results
for ex_file in experiment_result_list:
    # Read input file
    fin = open (ex_file, "rt")

    # Get the lines
    all_lines = fin.read().splitlines()

    # Read the first line for header
    header = all_lines [0]

    # Read the sensor data lines
    for line in all_lines [1:]:
        # Get the cluster parameters and the end-device name
        # key = distance_threshold	outlier_threshold	remove_intersections	sensor_name
        current_results = line.split ('\t')
        
        sensor_id = current_results [0]
        
        sensor_choice = int (current_results [1])
        duty_cycle_choice = float (current_results [2])
        haps_speed_choice = float (current_results [3])
        sigma_choice = float (current_results [4])
        
        if (sensor_id, sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice) in results:
            results [sensor_id, sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice].append (line)
        else:
            results [sensor_id, sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice] = []
            results [sensor_id, sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice].append (line)

        if (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice) in cum_results:
            cum_results [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice].append (line)
        else:
            cum_results [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice] = []
            cum_results [sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice].append (line)


def print_all_results (results):
    # Print the results into a new file
    results_file = open (main_directory + 'sensor_all_results.csv', "w")

    # Write single header to avg results file
    results_file.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        # Fix the values
        fixed_list = []
        for ix in range (len (value)):
            val_str = value [ix].split ('\t')
            skip_id = True
            fixed_vals = []
            for val in val_str:
                if not skip_id:
                    fixed_vals.append (float (val))
                else:
                    skip_id = False
            fixed_list.append (fixed_vals)

        # print (np.array (fixed_list).shape)
        dframe = pd.DataFrame(fixed_list)
        dframe.to_csv (results_file, mode='a', header=False, index=False)

    results_file.close ()

def print_std_results (results):
    # Print the results into a new file
    results_file = open (main_directory + 'sensor_std_results.csv', "w")

    # Write single header to avg results file
    results_file.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        # Fix the values
        fixed_list = []
        for ix in range (len (value)):
            val_str = value [ix].split ('\t')
            skip_id = True
            fixed_vals = []
            for val in val_str:
                if not skip_id:
                    fixed_vals.append (float (val))
                else:
                    skip_id = False
            fixed_list.append (fixed_vals)

        # print (np.array (fixed_list).shape)
        dframe = pd.DataFrame(fixed_list)  
        stds = [s for s in dframe.std ()]
        result = []
        for ix, s in enumerate (stds):
            if ix == 0:
                result.append (sensor_choice)
            elif ix == 1:
                result.append (duty_cycle_choice)
            elif ix == 2:
                result.append (haps_speed_choice)
            elif ix == 3:
                result.append (sigma_choice)
            else:
                result.append (s)

        # Calculate average and write it to a separate file
        current_average_string = ','.join ([str(num) for num in result])

        results_file.write (current_average_string + '\n')

    results_file.close ()

def print_mean_results (results):
    # Print the results into a new file
    results_file = open (main_directory + 'sensor_mean_results.csv', "w")

    # Write single header to avg results file
    results_file.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        # Fix the values
        fixed_list = []
        for ix in range (len (value)):
            val_str = value [ix].split ('\t')
            skip_id = True
            fixed_vals = []
            for val in val_str:
                if not skip_id:
                    fixed_vals.append (float (val))
                else:
                    skip_id = False
            fixed_list.append (fixed_vals)

        # print (np.array (fixed_list).shape)
        dframe = pd.DataFrame(fixed_list)  
        means = [s for s in dframe.mean ()]
        result = []
        for ix, s in enumerate (means):
            if ix == 0:
                result.append (sensor_choice)
            elif ix == 1:
                result.append (duty_cycle_choice)
            elif ix == 2:
                result.append (haps_speed_choice)
            elif ix == 3:
                result.append (sigma_choice)
            else:
                result.append (s)

        # Calculate average and write it to a separate file
        current_average_string = ','.join ([str(num) for num in result])

        results_file.write (current_average_string + '\n')

    results_file.close ()

def print_sum_results (results):
    # Print the results into a new file
    sensor_sum_results = open (main_directory + 'sensor_sum_results.csv', "w")

    # Write single header to avg results file
    sensor_sum_results.write (','.join (head for head in header.split ('\t')[1:]) + '\n')

    for (sensor_choice, duty_cycle_choice, haps_speed_choice, sigma_choice), value in results.items():

        # Fix the values
        fixed_list = []
        for ix in range (len (value)):
            val_str = value [ix].split ('\t')
            skip_id = True
            fixed_vals = []
            for val in val_str:
                if not skip_id:
                    fixed_vals.append (float (val))
                else:
                    skip_id = False
            fixed_list.append (fixed_vals)

        # print (len (fixed_list), " ", sensor_choice)
        experiment_count = len (fixed_list) / sensor_choice
        # quit ()

        # print (np.array (fixed_list).shape)
        dframe = pd.DataFrame(fixed_list)  
        dframe = dframe / experiment_count # Results are average of different experiments
        sums = [s for s in dframe.sum ()]
        result = []
        for ix, s in enumerate (sums):
            if ix == 0:
                result.append (sensor_choice)
            elif ix == 1:
                result.append (duty_cycle_choice)
            elif ix == 2:
                result.append (haps_speed_choice)
            elif ix == 3:
                result.append (sigma_choice)
            else:
                result.append (s)

        # Calculate average and write it to a separate file
        current_average_string = ','.join ([str(num) for num in result])

        sensor_sum_results.write (current_average_string + '\n')

    sensor_sum_results.close ()

print_all_results (cum_results)
print_mean_results (cum_results)
print_std_results (cum_results)
print_sum_results (cum_results)

#################################################
### FIX dot to comma in state files
print ("Done.")