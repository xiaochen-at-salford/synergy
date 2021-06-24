TOP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd -P)"
source ${TOP_DIR}/scripts/apollo.bashrc

sudo ip link set can0 type can bitrate 500000
sudo ip link set up can0

# Set can1 for subscribe status
sudo ip link set can1 type can bitrate 500000
sudo ip link set up can1