
import sys
import os
from collections import defaultdict
import re
from datetime import datetime

#Makes a new path to cgi-bin in order to run Macro's
current_dir = os.path.dirname(__file__)
parent_dir = os.path.abspath(os.path.join(current_dir, '..'))
sys.path.append(parent_dir)

from query_database import fetch

import numpy as np
import subprocess

def dates():


        # SQL query to fetch calibration timestamps (ensure the query is correct)
        
        
        query = """
        select t1.calib_timestamp from cms_tom.cms_pix_calibrations t1
        inner join cms_tom.cms_pix_calib_readbacker_converted t2 on t1.run_id=t2.run_id
        inner join cms_tom.cms_pix_names t3 on t2.part_id=t3.name_id
        inner join cms_tom.cms_pix_calib_readbacker t4 on t4.part_id=t2.part_id and t4.run_id=t2.run_id
        where t1.calib_type_id=1 and t3.name like 'BPix_BmO_SEC1_LYR1_LDR1H_MOD3_ROC4'
        """

        # Fetching result from the database
        result=fetch(query, params={}, user="cms_tom_r@int2r")

        # Convert result to a set to ensure unique dates
        unique_dates = {item[0] for item in result}


        sorted_dates = sorted(unique_dates)

        # Format dates as strings
        string_dates = [date.strftime("%m/%d/%Y %H:%M:%S") for date in sorted_dates]

        # Write the formatted dates to the file
        with open("/cmsnfshome0/nfshome0/cgillesp/dates_lyr1.txt", "w") as file1:
                file1.write("\n".join(string_dates))
        print("dates()")
        return 0;


def run_brilcalc_from_dates(file_path, output_path):
    """
    Reads dates from a text file, runs brilcalc for each start-end pair,
    and writes the results to an output file.
    
    Parameters:
    file_path (str): Path to the text file containing the dates.
    output_path (str): Path to the output text file to store brilcalc results.
    """
    # Read dates from the input file
    with open(file_path, 'r') as file:
        dates = file.readlines()

    # Strip newline characters
    dates = [date.strip() for date in dates]

    # First date is the start date, others are end dates
    start_date = dates[0]

    # Function to run brilcalc with given start and end dates
    def run_brilcalc(start_date, end_date):
        command = ['brilcalc', 'lumi', '-b', 'STABLE BEAMS', '--begin', start_date, '--end', end_date]
        try:
            process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout, stderr = process.communicate()

            if process.returncode == 0:
                return stdout.decode('utf-8')
            else:
                return "Error: " + stderr.decode('utf-8')
        except Exception as e:
            return "Exception: " + str(e)

    # Create the output file to store brilcalc results
    with open(output_path, 'w') as out_file:
        for end_date in dates[1:]:
            out_file.write("Start: " + start_date + ", End: " + end_date + "\n")
            result = run_brilcalc(start_date, end_date)
            out_file.write(result + "\n")


def reformat_date(date_str):

	dt = datetime.strptime(date_str, '%m/%d/%y %H:%M:%S')
	formatted_date = dt.strftime('%Y-%m-%d %H:%M:%S')
	return formatted_date.upper()



def parse_lumi_file(file_path):
    """
    Parses the lumi_date.txt file and extracts the start and end dates,
    as well as the total recorded luminosity from the summary table.

    Parameters:
    file_path (str): Path to the lumi date text file.

    Returns:
    List of tuples containing (start_date, recorded_luminosity) and (end_date, recorded_luminosity).
    """
    tuple_list = []

    with open(file_path, 'r') as file:
        lines = file.readlines()

    start_date = None
    end_date = None
    condition = False
    skip_next_line = False

    for line in lines:
        line = line.strip()

        # Detect the start and end dates
        if line.startswith("Start:"):
            parts = line.split(", End: ")
            start_date = parts[0].replace("Start: ", "").strip()  # Extract start date
            end_date = parts[1].strip()  # Extract end date
            if not condition:
                condition = True
                tuple_list.append((start_date, "0"))  # First tuple with zero recorded luminosity

        # Detect the summary header line
        if "| nfill | nrun | nls  | ncms | totdelivered(/ub) | totrecorded(/ub) |" in line:
            skip_next_line = True  # Set a flag to skip the next line

        # Skip the next line after the summary header
        elif skip_next_line:
            skip_next_line = False  # Reset the flag

        # If not skipping, check for the summary line with recorded luminosity
        elif re.match(r"\|\s*\d+\s*\|\s*\d+\s*\|\s*\d+\s*\|\s*\d+\s*\|\s*[\d.]+\s*\|\s*[\d.]+\s*\|", line):
            # Remove the pipe characters and split the columns
            columns = line.replace('|', '').strip().split()
            if len(columns) >= 6:
                totrecorded_lumi = columns[5]  # The 6th number is the total recorded luminosity

                # Check if we have both start and end dates before adding the entry
                if end_date and totrecorded_lumi:
                    tuple_list.append((end_date, totrecorded_lumi))  # Add the end date with the recorded luminosity
                    # Reset for next entry
                    start_date = None
                    end_date = None

    
    for i in range(len(tuple_list)):
       tuple_list[i] = (reformat_date(tuple_list[i][0]), tuple_list[i][1])
    
            

    return tuple_list

def vbg_get():


        # SQL query to fetch ROC and its initial vana

        
        query = """
        select t3.name, t1.vbg from cms_tom.cms_pix_calib_readbacker_converted t1
	inner join cms_tom.cms_pix_calibrations t2 on t1.run_id=t2.run_id
	inner join cms_tom.cms_pix_names t3 on t1.part_id = t3.name_id
	where t2.calib_timestamp = '19-NOV-21 03.13.58.000000 PM'
        """
        

        # Fetching result from the database
        result=fetch(query, params={}, user="cms_tom_r@int2r")

        # Convert result to a set to ensure unique dates
        data_list = {(item[0],item[1]) for item in result}

        
        # Output VANA tuple
        return data_list;


def test():
	#decode
	file_path = "/nfshome0/cgillesp/pom/cgi-bin/endpoints/lumi_date.txt"
	lumi_data = parse_lumi_file(file_path)

	#Display the parsed tuples
	for date, lumi in lumi_data:
		print("Date: " + date +" , Recorded Luminosity: "+ lumi)
        """
	vbg_list = vbg_get()

	for ROC, vbg in vbg_list:
		#print("ROC: " + ROC +" ,Vbg : "+ str(vbg))
                continue
        """
##test()
