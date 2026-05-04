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
