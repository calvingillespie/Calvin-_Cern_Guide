So the following programs and contents are divided into 3 sections:
    *Crab Fetch
    *File Consolidation
    *Histogram

Crab Fetch is for the following:
  CRABconfig_template.py
  Crab_Submission.py
  crabconfig.py
  DatalistB.list - DatalistH.list
  MClist.list
  EmobjectModV2.cpp
  anaylisis_script.sh
  FrameworkJobReport.xml

So I broke the Crab due to the lack of info on how to access a NanoAOD set for the info I needed. The best way I found to make it work for me was to use the DAS aggregation system to find the logic file name, copy them to the Datalist,
and then have Crab_Submission.py send all the fetch requests to CMS servers. This will make mulitple files inside the EOS space. EmobjectModV2.cpp contains the code meant to make a data tree that will get sent back to EOS, 
anaylisis_script.sh will run the code the CMS servers, and the remaining files are used to configure the request. FramworkJOBReport.xml is needed to make the crab fetch work


File Consolidations consist the following:
  Merge.cpp
  copyEOS.sh

Because I did not create these progams the ideal way, you need to reorganize the space in EOS. After using Crab to Fetch to the EOS, you can merge the nessacary files by using copyEOS.sh and then Merge.cpp to bring the files
forward into your area. You can also just use the data in EOS instead, but I provided both ways here.

Histograms are created using the following:
  DiElectron_modified.cpp
  DiPhoton_modified.cpp
  DiElectronWeighted.cpp
  Jet.cpp
  histomake.cpp

These are the individual programs to get the data into histograms. Histomake will make the comparison plots between different histograms. 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Additional Analysis Pipeline Documentation

The remaining programs and files are divided into three major sections:

CRAB Fetch
File Consolidation
Histogram Production

These components form the full NanoAOD → EOS → ROOT histogram workflow.
This contains the event-processing logic used to generate reduced ROOT trees from NanoAOD inputs.
The workflow was not implemented in the most efficient or scalable way, so some manual EOS organization may still be required.

1. CRAB Fetch

This section handles:

      CMS NanoAOD file access
      distributed CRAB job submission
      event processing on CMS servers
      ROOT tree generation
      storage of processed outputs to EOS
      Files
      Configuration / Submission
      CRABconfig_template.py
      Crab_Submission.py
      crabconfig.py
      FrameworkJobReport.xml
      Dataset Lists
      DatalistB.list - DatalistH.list
      MClist.list
      Processing Code
      EmobjectModV2.cpp
      analysis_script.sh
      Workflow

The CRAB workflow was built around manually assembling NanoAOD file lists using the CMS DAS aggregation system.

Because obtaining the required NanoAOD access information through standard methods was difficult, the workflow became:

      Use DAS to locate logical file names (LFNs)
      Copy LFNs into:
      Datalist*.list
      MClist.list

Run:

      Crab_Submission.py
      This will automatically do the following:
         Submit distributed CRAB jobs to CMS servers
         Generate processed ROOT trees
         Store outputs in EOS

The output trees are written back to EOS storage.

            analysis_script.sh

This script runs the analysis executable on CMS worker nodes during CRAB execution.

            FrameworkJobReport.xml

This file is required for proper CRAB job execution and reporting.

2. File Consolidation
Purpose

After CRAB jobs finish, the generated files in EOS must be reorganized and merged into usable local ROOT files.

            Merge.cpp
            copyEOS.sh

After CRAB finishes processing:

Use: copyEOS.sh
This will merge all EOS and makes accessing them a bit easier and less hectic

Use:Merge.cpp
This will merge ROOT outputs into consolidated files

3. Histogram Production
Purpose

These programs process the merged ROOT trees and generate physics histograms and comparison plots.

            DiElectron_modified.cpp
            DiPhoton_modified.cpp
            DiElectronWeighted.cpp
            Jet.cpp
            histomake.cpp
            Individual Histogram Programs


Processes dielectron events and generates corresponding histograms.
Run: DiElectron_modified.cpp

Processes diphoton events and generates histograms for photon studies.
Run: DiPhoton_modified.cpp

Weighted dielectron analysis implementation. You need to run the histomake.cpp and get the comparison folder with the ratio of the dielectron and diphoton graphs before running this code.
Run: DiElectronWeighted.cpp

Jet-related histogram generation and event analysis.
Run: Jet.cpp

Making plots and re-binning:
Run: histomake.cpp
This program generates:

         comparison plots
         combined histogram overlays
         final visualization outputs


Most components are functional but may require:

environment-specific path adjustments
EOS directory restructuring
CRAB configuration updates // I had to make some changes to get the code to work with crab 3, so be viligant.
ROOT dependency setup

If additional clarification is needed regarding:

DAS usage
CRAB submission
EOS organization
ROOT merging
histogram generation
analysis selections

You will most likely need to change the code to fit your needs. Do not hesitate to tear my code into pieces if nessacary. 
please feel free to ask.
