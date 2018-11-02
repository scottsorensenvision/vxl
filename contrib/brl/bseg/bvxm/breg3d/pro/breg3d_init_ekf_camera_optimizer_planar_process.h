// This is brl/bseg/bvxm/breg3d/pro/breg3d_init_ekf_camera_optimizer_planar_process.h
#ifndef breg3d_init_ekf_camera_optimizer_planar_process_h_
#define breg3d_init_ekf_camera_optimizer_planar_process_h_
//:
// \file
// \brief A process that initializes the state of an extended kalman filter
//
// The EKF is used for camera registration based on the first two
// frames and the assumption that the homography between the frames
// is generated by a dominant world plane.
//
// \author Daniel Crispell
// \date Feb 26, 2008
// \verbatim
//  Modifications
//   Aug 09 2010 jlm  moved to brl/bseg/bvxm/breg3d/pro
// \endverbatim

#include <iostream>
#include <string>
#include <vcl_compiler.h>
#ifdef _MSC_VER
#  include <vcl_msvc_warnings.h>
#endif
#include <bprb/bprb_process.h>

class breg3d_init_ekf_camera_optimizer_planar_process : public bprb_process
{
 public:

   breg3d_init_ekf_camera_optimizer_planar_process();

  //: Copy Constructor (no local data)
  breg3d_init_ekf_camera_optimizer_planar_process(const breg3d_init_ekf_camera_optimizer_planar_process& other): bprb_process(*static_cast<const bprb_process*>(&other)){}

  ~breg3d_init_ekf_camera_optimizer_planar_process() override= default;

  //: Clone the process
  breg3d_init_ekf_camera_optimizer_planar_process* clone() const override {return new breg3d_init_ekf_camera_optimizer_planar_process(*this);}

  std::string name() const override {return "breg3dInitEkfCameraOptimizerPlanar";}

  bool init() override { return true; }
  bool execute() override;
  bool finish() override{return true;}
};


#endif // breg3d_init_ekf_camera_optimizer_planar_process_h_
