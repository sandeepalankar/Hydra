/* -----------------------------------------------------------------------------
 * Copyright 2022 Massachusetts Institute of Technology.
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Research was sponsored by the United States Air Force Research Laboratory and
 * the United States Air Force Artificial Intelligence Accelerator and was
 * accomplished under Cooperative Agreement Number FA8750-19-2-1000. The views
 * and conclusions contained in this document are those of the authors and should
 * not be interpreted as representing the official policies, either expressed or
 * implied, of the United States Air Force or the U.S. Government. The U.S.
 * Government is authorized to reproduce and distribute reprints for Government
 * purposes notwithstanding any copyright notation herein.
 * -------------------------------------------------------------------------- */
#pragma once
#include <config_utilities/virtual_config.h>
#include <kimera_pgmo/MeshFrontendInterface.h>

#include "hydra/frontend/mesh_segmenter_config.h"
#include "hydra/frontend/place_2d_segmenter_config.h"
#include "hydra/places/graph_extractor_interface.h"
#include "hydra/places/gvd_integrator_config.h"

namespace hydra {

struct FrontendConfig {
  size_t min_object_vertices = 20;
  bool prune_mesh_indices = false;
  bool lcd_use_bow_vectors = true;
  kimera_pgmo::MeshFrontendConfig pgmo_config;
  MeshSegmenterConfig object_config;
  Place2dSegmenterConfig place_config;
  bool enable_places = true;
  bool use_2d_places = false;
  bool validate_vertices = false;
  bool filter_places = true;
  size_t min_places_component_size = 3;
  places::GvdIntegratorConfig gvd;
  config::VirtualConfig<places::GraphExtractorInterface> graph_extractor;
};

void declare_config(FrontendConfig& conf);

}  // namespace hydra

namespace kimera_pgmo {

void declare_config(kimera_pgmo::MeshFrontendConfig& conf);

}  // namespace kimera_pgmo
