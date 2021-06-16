# **Leddartech LiDAR Driver**

## 1 Introduction

 **Leddartech**  is the lidar driver kit under Apollo platform.

## 1.1 Requirements

The Leddartech Apollo's driver depends on the LeddarSDK package which is provided in the third_party folder. 

One can follow the direction here to setup apollo.
https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_software_installation_guide.md

## 1.2 Configuration
The first step to configure the sensor is to determine which configuration files that will be use.

- Open the dag file that is going to be used.
- All `config_file_path` will need to be edited
- Change the `ip` and `port` listed here according to the sensor configuration.

## 2 Run

**All the drivers need to be excuted under Apollo docker environment.**

#### 2.1 Single Pixell Sensor

```sh
cyber_launch start /apollo/modules/drivers/lidar/leddartech/launch/leddartech.launch
```

or

```sh
mainboard -d /apollo/modules/drivers/lidar/leddartech/dag/leddartech.dag
```

Default Configuration:
- ip: 192.168.0.2
- port: 48630
- Scan channel: /apollo/sensor/lidar/leddartech/scan
- PointCloud channel: /apollo/sensor/lidar/leddartech/PointCloud

### 2.2 Multi Pixell Sensor

```sh
cyber_launch start /apollo/modules/drivers/lidar/leddartech/launch/leddartech-multi.launch
```

or

```sh
mainboard -d /apollo/modules/drivers/lidar/leddartech/dag/leddartech-multi-pixell.dag
```

Default Configuration:
Left Sensor:
- ip: 192.168.1.2
- port: 48630
- Scan channel: /apollo/sensor/lidar/leddartech/left/scan
- PointCloud channel: /apollo/sensor/lidar/leddartech/left/PointCloud

Right Sensor:
- ip: 192.168.0.2
- port: 48630
- Scan channel: /apollo/sensor/lidar/leddartech/right/scan
- PointCloud channel: /apollo/sensor/lidar/leddartech/right/PointCloud

## 3 Parameters Intro

Visualisation can be done through
- cyber_monitor
  - Console display of data
- cyber_visualizer
  - GUI that display the data on a scene

[Intro to parameters](doc/parameter_intro.md)

