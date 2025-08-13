
import sys
import os
from collections import defaultdict
import re

#Makes a new path to cgi-bin in order to run Macro's
current_dir = os.path.dirname(__file__)
parent_dir = os.path.abspath(os.path.join(current_dir, '..'))
sys.path.append(parent_dir)

import query_database

import numpy as np
import subprocess

def dates1():


        # SQL query to fetch calibration timestamps (ensure the query is correct)
        query = """
        select t1.calib_timestamp from cms_tom.cms_pix_calibrations t1
        inner join cms_tom.cms_pix_calib_readbacker_converted t2 on t1.run_id=t2.run_id
        inner join cms_tom.cms_pix_names t3 on t2.part_id=t3.name_id
        inner join cms_tom.cms_pix_calib_readbacker t4 on t4.part_id=t2.part_id and t4.run_id=t2.run_id
        where t1.calib_type_id=1 and t3.name like 'BPix_BmO_SEC1_LYR1_LDR1H_MOD3_ROC4'	
        """

        # Fetching result from the database
        result = query_database.fetch(query, params={}, user="cms_tom_r@int2r")

        # Convert result to a set to ensure unique dates
        unique_dates = {item[0] for item in result}


        sorted_dates = sorted(unique_dates)

        # Format dates as strings
        string_dates = [date.strftime("%m/%d/%Y %H:%M:%S") for date in sorted_dates]

        # Write the formatted dates to the file
        with open("/cmsnfshome0/nfshome0/cgillesp/dates_lyr1.txt", "w") as file1:
                file1.write("\n".join(string_dates))
        print("dates1()")
        return 0;




def dates234():	
	

	# SQL query to fetch calibration timestamps (ensure the query is correct)
	query = """
	select t1.calib_timestamp from cms_tom.cms_pix_calibrations t1
        inner join cms_tom.cms_pix_calib_readbacker_converted t2 on t1.run_id=t2.run_id
        inner join cms_tom.cms_pix_names t3 on t2.part_id=t3.name_id
        inner join cms_tom.cms_pix_calib_readbacker t4 on t4.part_id=t2.part_id and t4.run_id=t2.run_id
        where t1.calib_type_id=1 and t3.name like 'BPix_BmO_SEC1_LYR1_LDR1H_MOD3_ROC4'
	"""
    
	# Fetching result from the database
	result = query_database.fetch(query, params={}, user="cms_tom_r@int2r")
    
	# Convert result to a set to ensure unique dates
	unique_dates = {item[0] for item in result}


	sorted_dates = sorted(unique_dates)

	# Format dates as strings
	string_dates = [date.strftime("%m/%d/%Y %H:%M:%S") for date in sorted_dates]

	# Write the formatted dates to the file
	with open("/cmsnfshome0/nfshome0/cgillesp/dates_lyr234.txt", "w") as file2:
		file2.write("\n".join(string_dates))
	print("dates234()")
	return 0;
