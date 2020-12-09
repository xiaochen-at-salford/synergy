# **Leddartech LiDAR Driver**

## 1 Introduction

 **Leddartech**  is the lidar driver kit under Apollo platform.

## 1.1 Requirements

The Leddartech Apollo's driver depends on the LeddarSDK package. At the moment of writing we opted for a custom Apollo docker that bundled the LeddarSDK.

Here is how to use the custom docker:
```bash
$ cd apollo/docker/build
$ ./build_dev.sh -f dev.x86_64.dockerfile -m build
```
Once the custom docker is built the `dev_start.sh` script will need to be modified.

It `dev_start.sh` look for the variable `VERSION_X86_64` and change the string to the string given by `build_dev.sh`. In my case my new image is called: `dev-x86_64-18.04-20201209_1649`

Running the newly created image:
```bash
$ bash docker/scripts/dev_start.sh -l
```

At this point you can follow the documentation here:
https://github.com/ApolloAuto/apollo/blob/master/docs/quickstart/apollo_software_installation_guide.md

## 2 Run

**All the drivers need to be excuted under Apollo docker environment.**

#### 2.1 RS16

```sh
cyber_launch start /apollo/modules/drivers/lidar/robosense/launch/rs16.launch
```

or

```sh
mainboard -d /apollo/modules/drivers/lidar/robosenseag/rs16.dag
```

Default Channel Name：

- Original point cloud -- /apollo/sensor/rs16/PointCloud2

- Scan--/apollo/sensor/rs16/Scan
- Compensation point cloud -- /apollo/sensor/rs16/compensator/PointCloud2

## 3 Parameters Intro

[Intro to parameters](doc/parameter_intro.md)

