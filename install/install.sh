setupATLAS

svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/UpgradePhys/SmearingFunctions/UpgradePerformanceFunctions/tags/UpgradePerformanceFunctions-02-07-02 UpgradePerformanceFunctions

git clone ssh://git@gitlab.cern.ch:7999/benitezj/SimpleAnalysis.git

cp tHAnalysis/install/PileupOverlay.cxx UpgradePerformanceFunctions/Root/PileupOverlay.cxx
cp tHAnalysis/install/FlavourTaggingPerformanceFunctions.cxx UpgradePerformanceFunctions/Root/FlavourTaggingPerformanceFunctions.cxx
cp tHAnalysis/install/UpgradePerformanceFunctions.h UpgradePerformanceFunctions/UpgradePerformanceFunctions/UpgradePerformanceFunctions.h  
rcSetup Base,2.4.32
