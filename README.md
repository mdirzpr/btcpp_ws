# btcpp_ws
BehaviorTree.CPP workspace

## Setup

First, clone this repository.

```
git clone -b https://github.com/mdirzpr/btcpp_ws.git
```
Then, build the Docker image.

```
cd btcpp_ws
docker compose build
```

Start a container.

```
docker compose run btcpp_ws
```

## Test
In order to test BehaviorTree.CPP library, run these commands:

```
cd src/BT_example/pick_place
mkdir build
cd build
cmake ..
make
./pickPlace
```

## Developer and maintainer:
- [Mahdi Rezapour](mahdi.rezapour@smartfactory.de)
