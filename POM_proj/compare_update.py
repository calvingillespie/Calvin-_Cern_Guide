
#This is supposed to be the update function for plots



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
import dashboard
import numpy as np
from decimal import Decimal
#import check_lumi
import csv
import plotly.graph_objects as go
import plotly.io as pio

from set_up import parse_lumi_file
from set_up import vbg_get

#NOTE I did not complete this. I still need to account for comparisons between Vana Corrected Layers. Only Vana Raw. Also needs to use the new update files and add the lumi to dose corrections for FPIX. Just steal from my 
#section
#T









#############################################################################################################################################
 
def compare_update(plot_name,mode,variable,graph_type,category,xaxis,query1,query2,comparison, run_numbers, start_stop_time, fill_range,options={}):


        print("This is the testing for the code of Plot update")
        #Checks Luminosity Macro
        #check_lumi.check_lumi()


        #Define dosage due to luminoisty
        lumi_to_dose=0.380

        #Run parse_lumi_file for double tuple of lumi with date and vbg with ROC name
        recorded_lumis = parse_lumi_file("/nfshome0/cgillesp/pom/cgi-bin/endpoints/lumi_date.txt")
        
        vbg_inital = vbg_get()

        lumi_dict = {thing[0]: thing[1] for thing in recorded_lumis}
        
        
        # Create a dictionary for quick look-up of initial VBG values by ROC
        vbg_init_dict = {thing[0]: thing[1] for thing in vbg_inital}

        #print(recorded_lumis) #Debug line

        result=fetch(query1, params={}, user="cms_tom_r@int2r")

        #ROCs = set()
        #dates = set()
        
		
        data1 = defaultdict(lambda: defaultdict(list))
	
        for item in result:
        	date = item[0]
        	vana = Decimal(item[1])
        	vbg = Decimal(item[2])
        	roc = item[3]
	
		# add to sets. Will automatically deal with repeats
        	#ROCs.add(roc)
        	#dates.add(date)
        	

        	 
        	# Use dictionary look-ups for luminosity and initial VBG
        	lumi = lumi_dict.get(str(date), 0.00000)  # Default to 0.00000 if not found
        	#lumi = 0  #turn on to turn lumi corrections off
        	vbg_init = vbg_init_dict.get(roc, 1.00000)  # Default to 0.00000 if not found
        	
        	data1[date][roc].append((vana, vbg, lumi, vbg_init))
        
        result=fetch(query1, params={}, user="cms_tom_r@int2r")
        
        #ROCs1 = set()
        #dates1 = set()
                
                
        data2 = defaultdict(lambda: defaultdict(list))
        
        for item in result:
                date = item[0]
                vana = Decimal(item[1])
                vbg = Decimal(item[2])
                roc = item[3]
                #ROCs1.add(roc)
                #dates1.add(date)
                lumi = lumi_dict.get(str(date), 0.00000)
                vbg_init = vbg_init_dict.get(roc, 1.00000)
                data2[date][roc].append((vana, vbg, lumi, vbg_init))
        
        #####################################################################################################333	
        
        
        #Debugging Line
        """
        for date, rocs in sorted(1data.items()):
                print("Date: " + str(date))
                for roc, values in rocs.items():
                        for vana_val, vbg, luminosity, vbg_initial in values:
                               print("  ROC: " + str(roc)  +" , Vana: "+ str(vana_val) +" , VBG: " + str(vbg)  +" , Luminosity: "+ str(luminosity)  + ", VBG Initial: " + str(vbg_initial))
        """
        
        
        ########################################################################################################################################
     
        #Calculate the Vana_cor and the dosage. The function provided was from Keegan
        #Define Dict for vana
        vana_cor = defaultdict(list)
        temp = 0;

        #function to calculate the vbg shift from the dosage received
        f=lambda x:Decimal(-384)*x**5 + Decimal(757.5)*x**4 - Decimal(500)*x**3 + Decimal(102.7)*x**2 + Decimal(19.01)*x + Decimal(3.484*10**-14)  #Keegans Approixmations
        g=lambda x:Decimal(-17.42)*x**2 + Decimal(25.29)*x  #Calvins Approximation	
	for date, rocs in sorted(data1.items()):
	              
	         for roc, values in rocs.items():
	                 for vana_val, vbg, luminosity, vbg_initial in values:
	                        dose = Decimal(luminosity) * Decimal(lumi_to_dose) / Decimal(10e9) /Decimal(100) #From microbarns lumi to fentobarns and Mrads to MGys
	                        if dose > float(.75053):
	                            dose = Decimal(.75053)
	                        #print("Date: " + str(date) + "VBG correction: " + str(dose)) #Debug
	                        temp = Decimal(vana_val) * Decimal(vbg_initial) / Decimal(vbg) * Decimal(1 + f(dose)/Decimal(100))
	                        if temp < 3:
                                    vana_cor[date].append(temp)
        

        print("Processing done")
        #print(vana_cor)  # Debugging line
        
        def convert_vana_cor_to_floats(vana_cor):
            for date, temps in vana_cor.items():
                    for i in range(len(temps)):
                                try:
                                    # Convert each Decimal to float
                                    temps[i] = float(temps[i])
                                except (ValueError, TypeError) as e:
                                    # Handle errors during conversion using the format method
                                    error_message = "Error converting value {} for date {}: {}".format(temps[i], date, e)
                                    print(error_message)
                                                                                                                                
            return vana_cor
                                                                                                                                    
        # Example usage
        vana_cor_float = convert_vana_cor_to_floats(vana_cor)
        
        #print(vana_cor_float) # Debugging Line
        
        ##################################################################################################################################
        
        #GRAPHING
        
        
        print("Converting to Floats Finished")
        if mode == "median":
            cor_medians = []
            medians1 = []
            medians2 = []
            graph_dates = []
            graph_dates_two = []
            vana_median_temp1 = 0
            vana_median_temp2 = 0
            
        #########################################################################################################################3
                    
            if variable == "vana":
            
                """
                # Calculate medians for corrected value of vana
                for date, values in vana_cor_float.items():
                    if values:  # Check if the list is not empty
                        cor_median = np.median(values)
                        std = np.std(values)
                        #print(str(date)) #Debug line
                        
                        if str(date) == "2021-11-19 15:13:58":
                           vana_median_temp = cor_median
                        
                        if (cor_median < 1.3) and (cor_median > .6) and std < 10:
                           cor_medians.append(cor_median)
                           graph_dates.append(date)
                                                                                                         
                # Calculate medians for uncorrected values (assuming vana_float is similarly structured)
                
                print("Initial Vana_Median is: " + str(vana_median_temp))
                print("Median amount: " + str(len(cor_medians)) + " date amount: " + str(len(graph_dates)))
                 
                 
                if graph_type == "normal" and len(cor_medians) == len(graph_dates):
                    # Create plots and add x and y axis
                    try:
                       plot_name.add_graph("Corrected " + category, graph_dates, cor_medians/vana_median_temp, drawstyle="markers")
                    except:
                       print("issue")
                """
            
            ##########################################################################################
            
            #Graph uncorrected VANA
            
                dates_data1 = set(data1.keys())
                dates_data2 = set(data2.keys())
                
                common_dates = dates_data1.intersection(dates_data2)
                
                for date in common_dates:
                    rocs = data1[date]
                    temp1 = []
                    for roc, values in rocs.items():
                        for vana_val, vbg, luminosity, vbg_initial in values:
                            temp1.append(float(vana_val))
                            
                    if temp1:
                        median = np.median(temp1)
                        if str(date) == "2021-11-19 15:13:58":
                            vana_median_temp1 = median  # Normalization factor for data1
                        if 0.6 < median < 1.3:
                            medians1.append(median)
                            graph_dates_two.append(date)
                            
                for date in common_dates:
                     rocs = data2[date]
                     temp2 = []
                     for roc, values in rocs.items():
                         for vana_val, vbg, luminosity, vbg_initial in values:
                             temp2.append(float(vana_val))
                     if temp2:
                         median = np.median(temp2)
                         if str(date) == "2021-11-19 15:13:58":
                             vana_median_temp2 = median
                         if 0.6 < median < 1.3:
                             medians2.append(median)
                             
                if vana_median_temp1 and vana_median_temp2:
                    normalized_medians1 = [m / vana_median_temp1 for m in medians1]
                    normalized_medians2 = [m / vana_median_temp2 for m in medians2]
                    #print("Normalized medians for data1:", normalized_medians1)
                    #print("Normalized medians for data2:", normalized_medians2)
                    
                    if len(normalized_medians1) == len(normalized_medians2):
                        vana_difference = [
                            n1 - n2 for n1, n2 in zip(normalized_medians1, normalized_medians2)
                        ]
                        
                        try:
                             plot_name.add_graph(
                                "Normalized VANA Difference " + category,
                                graph_dates_two,
                                vana_difference,
                                drawstyle="markers"
                             )
                        except Exception as e:
                             print("Plotting isse:", e)
          ###########################################################################################             
                       
           #Graph VBG
                       
            if variable == "vbg":
                for date, rocs in data1.items():
                    temp = []
                    for roc, values in rocs.items():
                       for vana_val, vbg, luminosity, vbg_initial in values:
                           temp.append(float(vbg)) #convert to float
                    
                    #only caculate mediain if temp is not empty and if the median is below 2
                    if temp:
                        if np.median(temp) < 2:
                           medians.append(np.median(temp))
                           graph_dates.append(date)                     	
                    
                if graph_type == "normal" and len(medians) == len (graph_dates):
                    #create plot and add x and y axis
                    try:
                       plot_name.add_graph("Corrected " + category, graph_dates, medians, drawstyle="markers")
                    except:
                       print("issue")    
                       
                       
            ######################################################################################
            
            if variable == "dose":
                lumis_dates = [thing[0] for thing in recorded_lumis]
                graph_lumis = [thing[1] for thing in recorded_lumis]
                #dose = Decimal(luminosity) * Decimal(lumi_to_dose) / Decimal(10e9) /Decimal(100)
                #Decimal(1 + g(dose)/Decimal(100))
                graph_lumi_cor =  [
                    1 + g(Decimal(luminosity) * Decimal(lumi_to_dose) / Decimal(10e9) /Decimal(100)/100)
                    for luminosity in graph_lumis
                ]
                try:
                   plot_name.add_graph("Lumi Correctin Factor", lumis_dates, graph_lumi_cor, drawstyle="markers")
                except:
                   print("issue")
                   
            #####################################################################################
           
            if variable == "ratio":
                for date, rocs in data1.items():
                   temp = []
                   for roc, values in rocs.items():
                        for vana_val, vbg, luminosity, vbg_initial in values:
                            temp.append(float(vana_val)/float(vbg)) #convert to float
                                                                         
                      #only caculate mediain if temp is not empty and if the median is below 2
                   if temp:
                        if np.median(temp):
                            medians.append(np.median(temp))
                            graph_dates.append(date)
               
                if graph_type == "normal" and len(medians) == len (graph_dates):
                    try:
                         plot_name.add_graph("Layer 1 ratio", graph_dates, medians, drawstyle="markers")
                    except:
                         print("issue")
                    
         #############################################################################################
        return plot_name
