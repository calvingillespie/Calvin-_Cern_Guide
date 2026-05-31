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

The last part I was working on was assembling my own list from pixel pro. fetch_ROC.py will fetch the following: Roc_name vd va vana vbg iana Runnumber. I was currently working on a code that can read root files, but it would be better for you to assemble the code from scratch.
I suggest making a code that can run the scurve analysis, most likely a bash script that can run the scurve analysis from all the runnumbers you're working with. After generating the data, then make a bash script that will go through all scurves with an attached root.exec, parse through all the scurve files and store the data in a text file.
I tried to automate as much as possible, so if there is any confusion, please let me know.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Overview for easier reading

This project processes CMS Pixel detector calibration and luminosity data using Oracle database queries, brilcalc, and locally generated plotting tools. The code was developed primarily for BPIX studies and radiation-damage-related detector analysis.

The workflow automates:

# extraction of calibration timestamps
# generation of luminosity files using brilcalc
# querying Oracle calibration data
# plotting detector quantities such as:
    # vana
    # vg
    # corrected vana

Main Workflow
Step 1 — Generate Calibration Dates
Run on CMSUSR:   python dates.py
This generates: dates_lyr1_updated.txt
Which contains the calibration timestamps used for luminosity extraction.

Step 2 — Generate Luminosity Files
Move dates_lyr1_updated to your LXPLUS work area
Run on LXPLUS: python date_to_lumi_lx.py
using the generated: lumi_lyr1_updated.txt

This script runs brilcalc and generates luminosity text files associated with each calibration timestamp.
The output will be a large lumi directory containing all generated luminosity files.

Step 3 — Run Plotting and Database Processing

Move lumi_lyr1_updated.txt to cmsusr
Follow the instructions on the POM gitlab page to start the workspace up. You may need to setup your own workspace if 
my graphs are not what you are looking for. To generate plots and display them through a localhost workspace. 
You MUST create the appropriate workspace and configure the correct endpoints before running the plotting interface locally.

This script combines:

luminosity information
Oracle database calibration data
VANA/VBG detector parameters

Important Files for running my code
        #plot_update.py
        #dates_lx.py
        #compare_update.py
        #dates.py
        #set_up.py

Fetches detector calibration quantities from the Oracle database, including:

      # ROC name
      # vd
      # va
      # vana
      # vbg
      # iana
      # run number

This was intended as the basis for assembling a complete Pixel detector calibration dataset.

Detector Notes

Only some ROCs exhibit problematic behavior. They have already been removed from the Oracle database, but the scruve data will still have it
Pre-Run 3 FPIX data contains significantly less usable information compared to BPIX due to heavy radiation damage affecting:

          #the forward pixel detector
          #portions of the silicon tracker

Most development and testing focused on BPIX.

Current Development Status

The final feature under development was automated ROOT/scurve processing.

The intended workflow was:

Run scurve analysis across many run numbers
Automate execution with bash scripts
Parse generated ROOT/scurve outputs
Store extracted quantities into structured text files

A future implementation should likely include:

batch bash execution scripts
ROOT file parsing
automated extraction pipelines
structured output formatting

It is probably easier to rebuild this portion from scratch rather than continue directly from the unfinished implementation.

Final Notes

A significant amount of the workflow was automated, but some scripts still assume existing directory structures and pre-generated files.

If any part of the pipeline is unclear, feel free to reach out for clarification regarding:

workflow order
database queries
luminosity generation
ROC handling
plotting setup
ROOT/scurve plans
