# stewart_platform

Project to control the Stewart platform. The platform use to be control via an OLIMEX board doing the interface IP/CAN to communicate with the servo’s drivers. This project aims to bypass this hardware to communicate directly with the CANopen bus of the lxm32a drivers.

<img src="fig/graph.png" width="800"  />
<img src="fig/sumup.png" width="800"  />

## Usage
### Clone
```bash
git clone https://gitlab-dev.isir.upmc.fr/devillard/stewart_platform.git
```

### Setup
```bash
cd stewart_platform/ && ./switchon_can_interface.sh
```

### Build
```bash 
mkdir build && cd build && cmake .. && make
```

### Run
```bash
./stewart_platform
```

## Electrical Design
<img src="fig/electrical_schematics.png" width="800"  />


## Stucture and Parameters used
### Globale

<img src="fig/geogebra-export.png" width="800"  />

### Base

<img src="fig/fig1.png" width="400"  />


