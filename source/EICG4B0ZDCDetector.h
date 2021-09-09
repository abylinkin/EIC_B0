// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef EICG4B0ZDCDETECTOR_H
#define EICG4B0ZDCDETECTOR_H

#include <g4main/PHG4Detector.h>

#include <set>
#include <string>  // for string

class G4LogicalVolume;
class G4VPhysicalVolume;
class PHCompositeNode;
class PHG4Subsystem;
class PHParameters;

class EICG4B0ZDCDetector : public PHG4Detector
{
 public:
  //! constructor
  EICG4B0ZDCDetector(PHG4Subsystem *subsys, PHCompositeNode *Node, PHParameters *parameters, const std::string &dnam, const int layer = 0);

  //! destructor
  virtual ~EICG4B0ZDCDetector() override {}

  //! construct
  void ConstructMe(G4LogicalVolume *world) override;

  void Print(const std::string &what = "ALL") const override;

  //!@name volume accessors
  //@{
  int IsInDetector(G4VPhysicalVolume *) const;
  //@}

  int GetDetId(G4VPhysicalVolume*) const;

  void SuperDetector(const std::string &name) { m_SuperDetector = name; }
  const std::string SuperDetector() const { return m_SuperDetector; }
  int get_Layer() const {return m_Layer;}
  PHParameters * getParams();

 private:
  PHParameters *m_Params;

  // active volumes
  std::set<G4VPhysicalVolume *> m_PhysicalVolumesSet;
//  std::set<G4LogicalVolume *>   m_LogicalVolumesSet;
  std::map<G4VPhysicalVolume *, int> m_PhysicalVolumesDet;
//  std::map<G4LogicalVolume *, int>   m_LogicalVolumesDet;
  int m_Layer;
  std::string m_SuperDetector;
};

#endif // EICG4B0ZDCDETECTOR_H
