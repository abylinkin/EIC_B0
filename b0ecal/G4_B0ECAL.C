#ifndef MACRO_G4B0ECAL_C
#define MACRO_G4B0ECAL_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4eiccalos/PHG4ForwardCalCellReco.h>
//#include <g4eiccalos/PHG4ForwardHcalSubsystem.h>
#include <eicg4b0/EICG4B0Subsystem.h>
#include <calobase/RawTowerDefsB0.h>//

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>

#include <eiccaloreco/RawClusterBuilderkMA.h>
#include <eiccaloreco/RawClusterBuilderHelper.h>

#include <caloreco/RawClusterBuilderFwd.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcaloB0_io.so)//
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4eiccalos.so)
R__LOAD_LIBRARY(libg4eval.so)

namespace Enable
{
  bool B0ECAL = false;
  bool B0ECAL_ABSORBER = false;
  bool B0ECAL_CELL = false;
  bool B0ECAL_TOWER = false;
  bool B0ECAL_CLUSTER = false;
  bool B0ECAL_EVAL = false;
  bool B0ECAL_OVERLAPCHECK = false;
  int B0ECAL_VERBOSITY = 0;
}  // namespace Enable


namespace G4B0ECAL
{

  double minz = 614;
  double maxz = 634;
  double radius =  20;

  // Default set to B0 Ecal position at IP6

  // Digitization (default photon digi):
  RawTowerDigitizer::enu_digi_algorithm TowerDigi = RawTowerDigitizer::kNo_digitization;
  // directly pass the energy of sim tower to digitized tower
  // kNo_digitization
  // simple digitization with photon statistics, single amplitude ADC conversion and pedestal
  // kSimple_photon_digitization
  // digitization with photon statistics on SiPM with an effective pixel N, ADC conversion and pedestal
  // kSiPM_photon_digitization

}  // namespace G4B0ECAL

void B0ECALInit()
{
}

void B0ECALSetup(PHG4Reco *g4Reco)
{
  const bool AbsorberActive = Enable::ABSORBER || Enable::B0ECAL_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::B0ECAL_OVERLAPCHECK;
  Fun4AllServer *se = Fun4AllServer::instance();

  /** Use dedicated FHCAL module */
  EICG4B0Subsystem *b0ecal = new EICG4B0Subsystem("B0ECAL");

  ostringstream mapping_b0ecal;

  mapping_b0ecal << "B0ECAL_mapping_v0.txt";
  
  b0ecal->SetTowerMappingFile(mapping_b0ecal.str());
  b0ecal->OverlapCheck(OverlapCheck);
  b0ecal->SetActive();
  b0ecal->SuperDetector("B0ECAL");
//  b0cal->SuperDetector("b0Truth");
  if (AbsorberActive) b0ecal->SetAbsorberActive();

  g4Reco->registerSubsystem(b0ecal);
}

void B0ECAL_Cells(int verbosity = 0)
{
  return;
}

void B0ECAL_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::B0ECAL_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_b0ecal;
  mapping_b0ecal << "B0ECAL_mapping_v0.txt";

  RawTowerBuilderByHitIndex *tower_B0ECAL = new RawTowerBuilderByHitIndex("TowerBuilder_B0ECAL");
  tower_B0ECAL->Detector("B0ECAL");
  tower_B0ECAL->set_sim_tower_node_prefix("SIM");
  tower_B0ECAL->GeometryTableFile(mapping_b0ecal.str());

  se->registerSubsystem(tower_B0ECAL);

  
  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("B0ECALRawTowerDigitizer");
  TowerDigitizer->Detector("B0ECAL");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer);

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("B0ECALRawTowerCalibration");
  TowerCalibration->Detector("B0ECAL");
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1. / 0.03898);  // calibrated with muons
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration);
}

void B0ECAL_Clusters()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::B0ECAL_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  RawClusterBuilderFwd *ClusterBuilder = new RawClusterBuilderFwd("B0ECALRawClusterBuilderFwd");
  ClusterBuilder->Detector("B0ECAL");
  ClusterBuilder->Verbosity(verbosity);
  ClusterBuilder->set_threshold_energy(0.100);
  se->registerSubsystem(ClusterBuilder);

  return;
}

void B0ECAL_Eval(const std::string &outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::B0ECAL_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("B0ECALEVALUATOR", "B0ECAL", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
#endif
