import sys
import os
import argparse


#Makes a new path to cgi-bin in order to run Macro's
current_dir = os.path.dirname(__file__)
parent_dir = os.path.abspath(os.path.join(current_dir, '..'))
sys.path.append(parent_dir)

import dashboard
from plot_update import update_plot
from compare_update import compare_update
import csv
import plotly.graph_objects as go
import plotly.io as pio
from date_to_lumi import run_brilcalc_from_dates
from dates import dates1, dates234

def main():
	#Define Variables
	#This is to make the functions readable on a half page app
	rn = 20
	sst = 30
	fr = 10
	op = 0

	print("hello, this is test.py")

	table1 = """
	select t1.calib_timestamp, t2.vana, t2.vbg, t3.name from cms_tom.cms_pix_calibrations t1       
	inner join cms_tom.cms_pix_calib_readbacker_converted t2 on t1.run_id=t2.run_id        
	inner join cms_tom.cms_pix_names t3 on t2.part_id=t3.name_id
	inner join cms_tom.cms_pix_calib_readbacker t4 on t4.part_id=t2.part_id and t4.run_id=t2.run_id 
	where t1.calib_type_id=1 and t3.name like 'FPix_%%%_%%_%%%%_%%%%_RNG1_%%%%' order by t1.calib_timestamp
        """
        
                
	new_plot=dashboard.plot()
	mode = "median"
	variable = "vana"
	graph_type = "normal"
	xaxis = "dates"
	cat1 = "mod1"
	cat2 = "mod2"
	cat3 = "mod3"
	cat4 = "mod4"
	compare = 0
	#mode can be median, mode, mean

	#new_plot = compare_update(new_plot,mode,variable,graph_type,cat1,xaxis,table1,table2,compare, rn, sst, fr,op)
	new_plot = update_plot(new_plot,mode,variable,graph_type,cat1,xaxis,table1, rn, sst, fr,op)

if __name__ == "__main__":
	main()
