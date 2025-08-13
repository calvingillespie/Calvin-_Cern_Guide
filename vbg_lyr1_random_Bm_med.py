
import sys
import os
import argparse


#Makes a new path to cgi-bin in order to run Macro's
current_dir = os.path.dirname(__file__)
parent_dir = os.path.abspath(os.path.join(current_dir, '..'))
sys.path.append(parent_dir)

import dashboard
#from plot_fabricator import fabricate_plot
from plot_update import update_plot
import csv
import plotly.graph_objects as go
import plotly.io as pio



def create_plot(run_numbers, start_stop_time, fill_range, options={}):


	#Define Variables
	#This is to make the functions readable on a half page app
	rn = run_numbers
	sst = start_stop_time
	fr = fill_range
	op = options


	table1 = """
	select t1.calib_timestamp, t2.vana, t2.vbg, t3.name from cms_tom.cms_pix_calibrations t1
	inner join cms_tom.cms_pix_calib_readbacker_converted t2 on t1.run_id=t2.run_id 
	inner join cms_tom.cms_pix_names t3 on t2.part_id=t3.name_id 
	inner join cms_tom.cms_pix_calib_readbacker t4 on t4.part_id=t2.part_id 
	and t4.run_id=t2.run_id where t1.calib_type_id=1 and t3.name 
	like 'BPix_BpO_SEC4_LYR1_LDR3H_MOD1_ROC6' order by t1.calib_timestamp

	"""
        
	new_plot=dashboard.plot()
	mode = "median"
	variable = "vbg"
	graph_type = "normal"
	xaxis = "dates"
	cat1 = "mod1"
	cat2 = "mod2"
	cat3 = "mod3"
	cat4 = "mod4"
	#mode can be median, mode, mean	
	
	new_plot = update_plot(new_plot,mode,variable,graph_type,cat1,xaxis,table1, rn, sst, fr, op)
	if graph_type == "normal":

		new_plot.xaxis_title="Time"
		new_plot.yaxis_title="Vbg(V) for Random ROC"
#		new_plot.update_yaxes(range=[0.7, 1.2])





	return new_plot

def get_description():
	return "description"

