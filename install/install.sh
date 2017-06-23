setupATLAS

svn co svn+ssh://svn.cern.ch/reps/atlasoff/PhysicsAnalysis/UpgradePhys/SmearingFunctions/UpgradePerformanceFunctions/tags/UpgradePerformanceFunctions-02-07-00 UpgradePerformanceFunctions

git clone ssh://git@gitlab.cern.ch:7999/benitezj/SimpleAnalysis.git

rcSetup Base,2.4.32
rc find_packages
rc compile
