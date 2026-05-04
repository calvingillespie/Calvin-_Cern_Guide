import FWCore.ParameterSet.Config as cms

#No idea how this works. Leave Alone

process = cms.Process('NoSplit')

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch///store/data/Run2016E/DoubleEG/NANOAOD/HIPM_UL2016_MiniAODv2_NanoAODv9-v2/*'))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("drop *", "keep recoTracks_*_*_*"),
    fileName = cms.untracked.string('outfilephoton_MC_set.root'),
)
process.out = cms.EndPath(process.output)
