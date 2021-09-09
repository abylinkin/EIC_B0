//____________________________________________________________________________..
//
// This is the interface to the framework. You only need to define the parameters
// you use for your detector in the SetDefaultParameters() method here
// The place to do this is marked by //implement your own here//
// The parameters have no units, they need to be converted in the
// EICG4B0ZDCDetector::ConstructMe() method
// but the convention is as mentioned cm and deg
//____________________________________________________________________________..
//
#include "EICG4B0ZDCSubsystem.h"

#include "EICG4B0ZDCDetector.h"
#include "EICG4B0ZDCSteppingAction.h"

#include <phparameter/PHParameters.h>

#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4SteppingAction.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/PHObject.h>
#include <phool/getClass.h>

using namespace std;

//_______________________________________________________________________
EICG4B0ZDCSubsystem::EICG4B0ZDCSubsystem(const std::string &name, const int lyr)
  : PHG4DetectorSubsystem(name, lyr)
  , m_Detector(nullptr)
  , m_SteppingAction(nullptr)
{
  // call base class method which will set up parameter infrastructure
  // and call our SetDefaultParameters() method
  InitializeParameters();
}
//_______________________________________________________________________
int EICG4B0ZDCSubsystem::InitRunSubsystem(PHCompositeNode *topNode)
{
  PHNodeIterator iter(topNode);
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  PHNodeIterator dstIter(dstNode);
  if (GetParams()->get_int_param("active"))
  {
    PHCompositeNode *DetNode = dynamic_cast<PHCompositeNode *>(dstIter.findFirst("PHCompositeNode", Name()));
    if (!DetNode)
    {
      DetNode = new PHCompositeNode(Name());
      dstNode->addNode(DetNode);
    }
    std::string g4hitnodename = "G4HIT_" + Name();
    PHG4HitContainer *g4_hits = findNode::getClass<PHG4HitContainer>(DetNode, g4hitnodename);
    if (!g4_hits)
    {
      g4_hits = new PHG4HitContainer(g4hitnodename);
      DetNode->addNode(new PHIODataNode<PHObject>(g4_hits, g4hitnodename, "PHObject"));
    }
  }
  // create detector
  m_Detector = new EICG4B0ZDCDetector(this, topNode, GetParams(), Name(), GetLayer());
  m_Detector->SuperDetector(SuperDetector());
  m_Detector->OverlapCheck(CheckOverlap());
  // create stepping action if detector is active
  if (GetParams()->get_int_param("active"))
  {
    m_SteppingAction = new EICG4B0ZDCSteppingAction(m_Detector, GetParams());
  }
  return 0;
}
//_______________________________________________________________________
int EICG4B0ZDCSubsystem::process_event(PHCompositeNode *topNode)
{
  // pass top node to stepping action so that it gets
  // relevant nodes needed internally
  if (m_SteppingAction)
  {
    m_SteppingAction->SetInterfacePointers(topNode);
  }
  return 0;
}
//_______________________________________________________________________
void EICG4B0ZDCSubsystem::Print(const string &what) const
{
  if (m_Detector)
  {
    m_Detector->Print(what);
  }
  return;
}

//_______________________________________________________________________
PHG4Detector *EICG4B0ZDCSubsystem::GetDetector(void) const
{
  return m_Detector;
}

//_______________________________________________________________________
void EICG4B0ZDCSubsystem::SetDefaultParameters()
{
  // sizes are in cm
  // angles are in deg
  // units should be converted to G4 units when used
  //implement your own here//
  set_default_double_param("place_x", 0.); //subdetector position
  set_default_double_param("place_y", 0.); //subdetector position
  set_default_double_param("place_z", 0.); //subdetector position
  set_default_double_param("pipe_ir", 2.8); //beam pipe inner radius (for future implementation)
  set_default_double_param("pipe_or", 3.05); //beam pipe outer raidus (for future implementation)
  set_default_double_param("pipe_hole", 5.0); //beam pipe cut off radius in the detector volume
  set_default_double_param("pipe_x", -3.4); //beam pipe position
  set_default_double_param("pipe_y", 0.); //beam pipe position
  set_default_double_param("pipe_z", 0.); //beam pipe position
  set_default_double_param("rot_y", 0.); //subdetector rotation
  set_default_double_param("outer_radius", 2.); //detector outer radiues
  set_default_double_param("d_radius", 5.); //packman cutoff size
  set_default_double_param("length", 10.); //detector length
  set_default_double_param("startAngle", 0.); //start Angle for packman cutoff
  set_default_double_param("spanningAngle", 360.); //spanning Angle of the detector (for packman cutoff)
  set_default_double_param("detid", 0.); //detector id
  set_default_int_param("ispipe", 0); //pipe or detector (for future implementation)

  set_default_string_param("material", "G4_PbWO4"); //detector material
}
