#ifndef PARTICLE_ACTIONS_H
#define PARTICLE_ACTIONS_H

#include <string>

#include <Cabana_Core.hpp>

#include "Definitions.h"
#include "TimeStepper.h"
#include "Particles.h"

// This might need to be changed for AMD and Intel GPUs. Nvidia warp size is 32. 
#define VECTOR_LENGTH 64

namespace HACCabana 
{
  class ParticleActions
  {
  public:
    using device_exec = Kokkos::DefaultExecutionSpace::execution_space;
    using device_mem = Kokkos::DefaultExecutionSpace::memory_space;
    using device_type = Kokkos::Device<device_exec, device_mem>;
    using neighbor_type = Cabana::LinkedCellList<device_type>;
    //using device_scratch = Kokkos::ScratchMemorySpace<device_exec>;
    using aosoa_device_type = Cabana::AoSoA<typename Particles::data_types, device_type, VECTOR_LENGTH>;

  private:
    Particles *P;
    aosoa_device_type aosoa_device;

  public:
    neighbor_type cell_list;

    ParticleActions();
    ParticleActions(Particles *P_, const float cm_size, const float min_pos, const float max_pos);
    ~ParticleActions();
    void setParticles(Particles *P_);
    void subCycle(TimeStepper &ts, const int nsub, const float gpscal, const float rmax2, const float rsm2);
    void updatePos(Cabana::AoSoA<HACCabana::Particles::data_types, device_type, VECTOR_LENGTH> aosoa_device,\
        float prefactor);
    void updateVel(Cabana::AoSoA<HACCabana::Particles::data_types, device_type, VECTOR_LENGTH> aosoa_device,\
        Cabana::LinkedCellList<device_type> cell_list,\
        const float c, const float rmax2, const float rsm2);
  };
}
#endif
