from CRABClient.UserUtilities import config

config = config()

config.General.requestName = __REQUEST_NAME__
config.General.workArea = 'crabproj'
config.General.transferOutputs = True

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'crabconfig.py'
config.JobType.scriptExe = 'analysis_script.sh'
config.JobType.inputFiles = ['Emobject.cpp', 'FrameworkJobReport.xml']
config.JobType.outputFiles = ['outfilephoton_MC_set.root']
config.JobType.scriptArgs = __SCRIPT_ARGS__


#I dont think this matters since I already broke the whole thing
#Also dont know the logic name for the MC set.
#B
#config.Data.inputDataset = '/DoubleEG/Run2016B-ver1_HIPM_UL2016_MiniAODv2_NanoAODv9-v2/NANOAOD'
#G-H
config.Data.inputDataset = '/DoubleEG/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
#C-F
#config.Data.inputDataset = '/DoubleEG/Run2016F-HIPM_UL2016_MiniAODv2_NanoAODv9-v2/NANOAOD'


config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.publication = False
config.Data.outputDatasetTag = '2016_Dataset_V2_MC'

config.Site.storageSite = 'T3_US_FNALLPC'
