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
#include "hydra_dsg_builder/frontend_config.h"
#include "hydra_dsg_builder/incremental_mesh_segmenter.h"
#include "hydra_dsg_builder/incremental_types.h"
#include "hydra_dsg_builder/input_queue.h"
#include "hydra_dsg_builder/shared_module_state.h"

#include <hydra_msgs/ActiveLayer.h>
#include <hydra_msgs/ActiveMesh.h>
#include <hydra_topology/nearest_neighbor_utilities.h>
#include <kimera_pgmo/MeshFrontendInterface.h>
#include <hydra_utils/dsg_streaming_interface.h>
#include <pose_graph_tools/PoseGraph.h>
#include <spark_dsg/scene_graph_logger.h>

#include <memory>
#include <mutex>
#include <thread>

namespace hydra {
namespace incremental {

using PlacesLayerMsg = hydra_msgs::ActiveLayer;
using topology::NearestNodeFinder;

struct FrontendInput {
  PlacesLayerMsg::ConstPtr places;
  hydra_msgs::ActiveMesh::ConstPtr mesh;
  std::list<pose_graph_tools::PoseGraph::ConstPtr> pose_graphs;
  Eigen::Vector3d current_position;
  uint64_t timestamp_ns;
};

class DsgFrontend {
 public:
  using FrontendInputQueue = InputQueue<FrontendInput>;
  using InputCallback = std::function<void(const FrontendInput&)>;

  DsgFrontend(const DsgFrontendConfig& config,
              const SharedDsgInfo::Ptr& dsg,
              const SharedModuleState::Ptr& state,
              int robot_id);

  virtual ~DsgFrontend();

  void start();

  void stop();

  void save(const std::string& output_path);

  void spin();

 protected:
  void updateMeshAndObjects(const FrontendInput& input);

  void updatePlaces(const FrontendInput& input);

  void updatePoseGraph(const FrontendInput& input);

 protected:
  void archivePlaces(const NodeIdSet active_places);

  void invalidateMeshEdges();

  void addPlaceObjectEdges(uint64_t timestamp_ns,
                           NodeIdSet* extra_objects_to_check = nullptr);

  void addPlaceAgentEdges(uint64_t timestamp_ns);

  void updatePlaceMeshMapping();

 protected:
  std::atomic<bool> should_shutdown_{false};
  FrontendInputQueue::Ptr queue_;
  std::unique_ptr<std::thread> spin_thread_;

  DsgFrontendConfig config_;
  std::unique_ptr<kimera::SemanticLabel2Color> label_map_;
  SharedDsgInfo::Ptr dsg_;
  SharedModuleState::Ptr state_;
  char robot_prefix_;

  kimera_pgmo::MeshFrontendInterface mesh_frontend_;
  std::unique_ptr<MeshSegmenter> segmenter_;
  SceneGraphLogger frontend_graph_logger_;

  std::unique_ptr<NearestNodeFinder> places_nn_finder_;
  NodeIdSet unlabeled_place_nodes_;
  NodeIdSet previous_active_places_;
  std::set<NodeId> deleted_agent_edge_indices_;
  std::map<LayerPrefix, size_t> last_agent_edge_index_;

  std::vector<InputCallback> input_callbacks_;
};

}  // namespace incremental
}  // namespace hydra
