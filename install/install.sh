setupATLAS

svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/UpgradePhys/SmearingFunctions/UpgradePerformanceFunctions/tags/UpgradePerformanceFunctions-02-08-02 UpgradePerformanceFunctions

git clone ssh://git@gitlab.cern.ch:7999/benitezj/SimpleAnalysis.git

git clone ssh://git@gitlab.cern.ch:7999/sargyrop/tHBDT.git

git clone ssh://git@gitlab.cern.ch:7999/sargyrop/tHFit.git

cp tHAnalysis/install/PileupOverlay.cxx UpgradePerformanceFunctions/Root/PileupOverlay.cxx
cp tHAnalysis/install/FlavourTaggingPerformanceFunctions.cxx UpgradePerformanceFunctions/Root/FlavourTaggingPerformanceFunctions.cxx
cp tHAnalysis/install/UpgradePerformanceFunctions.h UpgradePerformanceFunctions/UpgradePerformanceFunctions/UpgradePerformanceFunctions.h  
cp tHAnalysis/install/flavor_tags_v1.5.root UpgradePerformanceFunctions/share/.

rcSetup Base,2.4.32
rc find_packages
rc compile 
