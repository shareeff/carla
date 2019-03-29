// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/NonCopyable.h"
#include "carla/geom/Transform.h"
#include "carla/road/MapData.h"
#include "carla/road/RoadTypes.h"
#include "carla/road/element/LaneMarking.h"
#include "carla/road/element/RoadInfoMarkRecord.h"
#include "carla/road/element/Waypoint.h"

#include <boost/optional.hpp>

#include <vector>

namespace carla {
namespace road {

  class Map : private MovableNonCopyable {
  public:

    using Waypoint = element::Waypoint;

    /// ========================================================================
    /// -- Constructor ---------------------------------------------------------
    /// ========================================================================

    Map(MapData m) : _data(std::move(m)) {}

    /// ========================================================================
    /// -- Georeference --------------------------------------------------------
    /// ========================================================================

    const geom::GeoLocation &GetGeoReference() const {
      return _data.GetGeoReference();
    }

    /// ========================================================================
    /// -- Geometry ------------------------------------------------------------
    /// ========================================================================

    boost::optional<element::Waypoint> GetClosestWaypointOnRoad(
        const geom::Location &location,
        uint32_t lane_type = static_cast<uint32_t>(Lane::LaneType::Driving)) const;

    boost::optional<element::Waypoint> GetWaypoint(
        const geom::Location &location,
        uint32_t lane_type = static_cast<uint32_t>(Lane::LaneType::Driving)) const;

    geom::Transform ComputeTransform(Waypoint waypoint) const;

    /// ========================================================================
    /// -- Road information ----------------------------------------------------
    /// ========================================================================

    const Lane &GetLane(Waypoint waypoint) const;

    Lane::LaneType GetLaneType(Waypoint waypoint) const;

    double GetLaneWidth(Waypoint waypoint) const;

    bool IsJunction(RoadId road_id) const;

    std::pair<const element::RoadInfoMarkRecord *, const element::RoadInfoMarkRecord *>
        GetMarkRecord(Waypoint waypoint) const;

    std::vector<element::LaneMarking> CalculateCrossedLanes(
        const geom::Location &origin,
        const geom::Location &destination) const;

    /// ========================================================================
    /// -- Waypoint generation -------------------------------------------------
    /// ========================================================================

    /// Return the list of waypoints placed at the entrance of each drivable
    /// successor lane; i.e., the list of each waypoint in the next road segment
    /// that a vehicle could drive from @a waypoint.
    std::vector<Waypoint> GetSuccessors(Waypoint waypoint) const;

    /// Return the list of waypoints at @a distance such that a vehicle at @a
    /// waypoint could drive to.
    std::vector<Waypoint> GetNext(Waypoint waypoint, double distance) const;

    /// Return a waypoint at the lane of @a waypoint's right lane.
    boost::optional<Waypoint> GetRight(Waypoint waypoint) const;

    /// Return a waypoint at the lane of @a waypoint's left lane.
    boost::optional<Waypoint> GetLeft(Waypoint waypoint) const;

    /// Generate all the waypoints in @a map separated by @a approx_distance.
    std::vector<Waypoint> GenerateWaypoints(double approx_distance) const;

    /// Generate the minimum set of waypoints that define the topology of @a
    /// map. The waypoints are placed at the entrance of each lane.
    std::vector<std::pair<Waypoint, Waypoint>> GenerateTopology() const;

#ifdef LIBCARLA_WITH_GTEST
    MapData &GetMap() {
      return _data;
    }
#endif // LIBCARLA_WITH_GTEST

private:

    MapData _data;
  };

} // namespace road
} // namespace carla
