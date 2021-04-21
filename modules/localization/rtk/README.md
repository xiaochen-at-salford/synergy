# Synergy: RTK Localization

## Input
apollo::localization::Gps
```protobuf
message Gps {
  optional apollo.common.Header header = 1;

  // Localization message: from GPS or localization
  optional apollo.localization.Pose localization = 2;
}
```

apollo::localization::CorrectedImu
```protobuf
message CorrectedImu {
  optional apollo.common.Header header = 1;

  // Inertial Measurement Unit(IMU)
  optional apollo.localization.Pose imu = 3;
}
```
drivers::gnss::InsStat
```protobuf
message InsStatus {
  optional apollo.common.Header header = 1;

  enum Type {
    // Invalid solution due to insufficient observations, no initial GNSS, ...
    INVALID = 0;

    // Use with caution. The covariance matrix may be unavailable or incorrect.
    // High-variance result due to aligning, insufficient vehicle dynamics, ...
    CONVERGING = 1;

    // Safe to use. The INS has fully converged.
    GOOD = 2;
  }
  optional Type type = 2 [default = INVALID];
}
```

## Output
LocalizationEstimate
LocalizationStatus

## Associated