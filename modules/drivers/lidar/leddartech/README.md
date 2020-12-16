# **Leddartech LiDAR Driver**

## 1 Introduction

 **Leddartech**  is the lidar driver kit under Apollo platform.

## 1.1 Requirements

The Leddartech Apollo's driver depends on the LeddarSDK package which is provided in the third_party folder. 

One can follow the direction here to setup apollo.
https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_software_installation_guide.md

## 1.2 Notes
If changes needs to be done to the protobuf format. There is a tool available from apollo that automatically generates all the required files.
For the script to works one needs to be in apollo dev shell.

```sh
$ proto_build_generator.py modules/drivers/lidar/leddartech/proto/BUILD
```

## 2 Run

**All the drivers need to be excuted under Apollo docker environment.**

#### 2.1 RS16

```sh
cyber_launch start /apollo/modules/drivers/lidar/leddartech/launch/leddartech.launch
```

or

```sh
mainboard -d /apollo/modules/drivers/lidar/leddartech/dag/leddartech.dag
```

Default Channel Name：

- Original point cloud -- /apollo/sensor/rs16/PointCloud2

- Scan--/apollo/sensor/rs16/Scan
- Compensation point cloud -- /apollo/sensor/rs16/compensator/PointCloud2

## 3 Parameters Intro

[Intro to parameters](doc/parameter_intro.md)

