# Calvin-_Cern_Guide

#So the code I made to work with the 2016 Nano AOD dataset for EM objects involve the following code: DiElectronWeighted.cpp, DiElectron_modififed.cpp, DiPhoton_modififed.cpp, EmobjectModV2.cpp, and histomake.cpp.

The first 3 codes listed will be used to pull the data from the datasets to make the base histograms and graphs. The 4th code listed is used to remove events and data that is not needed for the anaylsis. The 5th code is used in root to make the histogram final model and for comparisons.




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

To access POM

To connect to LXPLUS
: ssh username@lxplus.cern.ch

To connect to CMS servers from LXPLUS
: ssh username@cmsusr.cern.ch

To connect to TOM server from CMS
: ssh username@tom.cms

To connect local host to CMS by using LXPLUS as a proxy
: ssh -L 8058:tom.cms:8058 -J username@lxplus.cern.ch username@cmsusr.cern.ch

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

