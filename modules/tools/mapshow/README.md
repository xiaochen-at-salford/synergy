# Map Show

## About

Mapshow is a tool to display hdmap info on a map.

## Setup

Source the "apollo_base.sh" if needed
```bash
# In apollo root dir:
source scripts/apollo_base.sh
```

Add "/apollo/bazelbin" and "/apollo/modules" to paython path

/apollo/bazelbin" is a soft link of "/apollo/bazel-bin". Doing this is to avoid python import error
```python
from bazelbin.modules.localization.proto.localization_pb2 import LocalizationEstimate
```

Run the tool scripts from the Apollo root directory

## Usage of **mapshow**

> usage: python mapshow.py \[-h] -m MAP \[-sl] [-l LANEID [LANEID ...]]
>
> optional arguments:
>
>  -h, --help            show this help message and exit
>
>  -m MAP, --map MAP     Specify the map file in txt or binary format
>
>  -sl, --showlaneids    Show all lane ids in map
>
>  -l, --laneid    Show specific lane id(s) in map
>
>  -l LANEID \[LANEID ...], --laneid LANEID \[LANEID ...]  Show specific lane id(s) in map
>
>  -signal, --showsignals    Show all signal light stop lines with ids in map
>
>  -stopsign, --showstopsigns    Show all stop sign stop lines with ids in map
>
>  -junction, --showjunctions    Show all pnc-junctions with ids in map
>


## Examples
### Using **python**
```bash
# Load a ".bin" map
cd /apollo
source scripts/apollo_base.sh
python modules/tools/mapview/mapview.py -m /apollo/modules/map/data/borregas_ave/base_map.bin
```


### Using **python**
```bash
# Load a ".txt" map 
cd /apollo
source scripts/apollo_base.sh
bazel run //modules/tools/mapshow:mapshow -- -m /apollo/modules/map/data/demo/base_map.txt
```

```bash
# Load a ".bin" map
cd /apollo
source scripts/apollo_base.sh
bazel run //modules/tools/mapshow:mapshow -- -m /apollo/modules/map/data/borregas_ave/base_map.bin
```