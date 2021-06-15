# Synergy: Routing

## Introduction
  The Routing module generates high level navigation information based on requests.

  The Routing module depends on a routing topology file, usually named `routing_map.*` in apollo.

  The routing map can be genreated using this command:
  ```
  bash scripts/generate_routing_topo_graph.sh
  ```

## Input
apollo::routing::RoutingRequest
```protobuf
message RoutingRequest {
    optional apollo.common.Header header = 1;
    // at least two points. The first is start point, the end is final point.
    // The routing must go through each point in waypoint.
    repeated LaneWaypoint waypoint = 2;
    repeated LaneSegment blacklisted_lane = 3;
    repeated string blacklisted_road = 4;
    optional bool broadcast = 5 [default = true];
    optional apollo.hdmap.ParkingSpace parking_space = 6 [deprecated = true];
    optional ParkingInfo parking_info = 7;
}
```

## Output
apollo::routing::RoutingReponse
```protobuf
message RoutingResponse {
    optional apollo.common.Header header = 1;
    repeated RoadSegment road = 2;
    optional Measurement measurement = 3;
    optional RoutingRequest routing_request = 4;

    // the map version which is used to build road graph
    optional bytes map_version = 5;
    optional apollo.common.StatusPb status = 6;
}
```

## Associated 
apollo::hdmap::HDMap

