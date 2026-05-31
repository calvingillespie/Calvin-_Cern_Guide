So the POM program is already pretty lengthy, so I am just gonna mention what I have added.

So, to run the code, you need a lumi file which is generated from brilcalc. The best way to do that is via running the date_to_lumi_lx.py 
with the dates_lyr1_updated.txt files in lxplus. This will give a massive lumi folder with the assoicated date. The best way to get the 
automated date text folder is by running dates.py. 

After you got the lumi text file and lumi file, you can finally run the plot_update.py. This works with the vana, vbg, and lumi through the 
Oracle database. You MUST create a workspace with the proper endpoints in order to display the code on the localhost site.

There are additionally codes called ROC_list.py and fetch_ROC.py that are meant to get code on all ROCs for all runnumbers that have been pre-generated. 
There is a way to get all the runnumbers, but note that only some ROCs are not functioning properly. The FPIX before the Run 3 Upgrades lack a lot data compared to the 
BPIX due to high radiaiton damage on the forward calorimeter and Silcion tracker.

THe following files are for setting up the work space:

plot_update.py
dates_lx.py
compare_update.py
dates.py
set_up.py
