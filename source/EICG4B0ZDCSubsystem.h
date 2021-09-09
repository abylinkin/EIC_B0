// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef EICG4B0ZDCSUBSYSTEM_H
#define EICG4B0ZDCSUBSYSTEM_H

#include <g4detectors/PHG4DetectorSubsystem.h>

class PHCompositeNode;
class PHG4Detector;
class EICG4B0ZDCDetector;
class PHG4SteppingAction;

/**
   * \brief Detector Subsystem module
   *
   * The detector is constructed and registered via EICG4B0ZDCDetector
   *
   *
   * \see EICG4B0ZDCDetector
   * \see EICG4B0ZDCSubsystem
   *
   */
class EICG4B0ZDCSubsystem : public PHG4DetectorSubsystem
{
 public:
  //! constructor
  EICG4B0ZDCSubsystem(const std::string& name = "EICG4B0ZDC", const int layer = 0);

  //! destructor
  virtual ~EICG4B0ZDCSubsystem() {}

  /*!
  creates relevant hit nodes that will be populated by the stepping action and stored in the output DST
  */
  int InitRunSubsystem(PHCompositeNode*) override;

  //! event processing
  /*!
  get all relevant nodes from top nodes (namely hit list)
  and pass that to the stepping action
  */
  int process_event(PHCompositeNode*) override;

  //! accessors (reimplemented)
  PHG4Detector* GetDetector() const override;

  PHG4SteppingAction* GetSteppingAction() const override { return m_SteppingAction; }
  //! Print info (from SubsysReco)
  void Print(const std::string& what = "ALL") const override;

 protected:
  // \brief Set default parameter values
  void SetDefaultParameters() override;

 private:
  //! detector construction
  /*! derives from PHG4Detector */
  EICG4B0ZDCDetector  *m_Detector;

  //! particle tracking "stepping" action
  /*! derives from PHG4SteppingActions */
  PHG4SteppingAction *m_SteppingAction;
};

#endif // EICG4B0ZDCSUBSYSTEM_H
