################################################################################
# BehaviorTree.CPP Workspace - Multi-Stage Dockerfile
#
# Author: Mahdi Rezapour
#
# Description:
#   Standalone BehaviorTree.CPP development environment without ROS 2.
#   Includes BehaviorTree.CPP built from source and Groot2 for visualization.
#
# Stages:
#   1. base - Ubuntu 24.04 with BehaviorTree.CPP installed system-wide
#   2. dev  - Development environment with non-root user
#
# Build Args:
#   GROOT2_VERSION - Groot2 AppImage version (default: v1.6.1)
#   USERNAME       - Development user name (default: devuser)
#   UID/GID        - User/Group ID (default: 1000)
################################################################################

ARG GROOT2_VERSION=v1.6.1

################################################################################
# STAGE 1: BASE IMAGE
################################################################################

FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
SHELL ["/bin/bash", "-c"]

################################################################################
# SYSTEM DEPENDENCIES
################################################################################

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates curl git build-essential cmake \
    # GUI libraries (for Groot2)
    libcanberra-gtk-module libcanberra-gtk3-module \
    libqt5svg5-dev \
    libopengl0 \
    # ZMQ (required by BehaviorTree.CPP Groot2 publisher)
    libzmq5 libzmq3-dev \
    # BehaviorTree.CPP dependencies
    libboost-all-dev \
    libsqlite3-dev \
    # Utilities
    iputils-ping \
    && rm -rf /var/lib/apt/lists/*

# Allow AppImages to run without FUSE in Docker
ENV APPIMAGE_EXTRACT_AND_RUN=1

################################################################################
# BUILD BEHAVIORTREE.CPP FROM SOURCE
#
# Installed system-wide to /usr/local so any project can find it via CMake.
################################################################################

RUN git clone --depth 1 https://github.com/BehaviorTree/BehaviorTree.CPP.git /tmp/btcpp \
    && cmake -S /tmp/btcpp -B /tmp/btcpp/build \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DBTCPP_GROOT2_PLUGIN=ON \
        -DBUILD_TESTING=OFF \
    && cmake --build /tmp/btcpp/build -j$(nproc) \
    && cmake --install /tmp/btcpp/build \
    && ldconfig \
    && rm -rf /tmp/btcpp

################################################################################
# GROOT2 BEHAVIOR TREE VISUALIZER
#
# AppImage for real-time behavior tree visualization.
# Connects to running processes via ZMQ (default port: 1668).
################################################################################

ARG GROOT2_VERSION
WORKDIR /root/
RUN curl -L -o Groot2.AppImage \
    https://s3.us-west-1.amazonaws.com/download.behaviortree.dev/groot2_linux_installer/Groot2-${GROOT2_VERSION}-x86_64.AppImage \
    && chmod a+x Groot2.AppImage

################################################################################
# X11 DISPLAY CONFIGURATION
#
# Required for running Groot2 and other GUI applications from the container.
# On the HOST, run: xhost +local:docker
################################################################################

RUN mkdir -p /tmp/runtime-root && chmod 0700 /tmp/runtime-root
ENV XDG_RUNTIME_DIR="/tmp/runtime-root"
ENV NO_AT_BRIDGE=1

COPY ./docker/entrypoint.sh /
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

################################################################################
# STAGE 2: DEVELOPMENT IMAGE
################################################################################

FROM base AS dev

ARG USERNAME=devuser
ARG UID=1000
ARG GID=${UID}

################################################################################
# DEVELOPMENT TOOLS
################################################################################

RUN apt-get update && apt-get install -y --no-install-recommends \
    sudo gdb gdbserver nano \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

################################################################################
# USER CREATION
################################################################################

# Remove pre-existing ubuntu user (common in Ubuntu 24.04 images)
RUN id ubuntu 2>/dev/null \
    && (touch /var/mail/ubuntu \
    && chown ubuntu /var/mail/ubuntu \
    && userdel -r ubuntu) || true

RUN groupadd --gid $GID $USERNAME \
    && useradd --uid ${UID} --gid ${GID} --create-home ${USERNAME} \
    && echo ${USERNAME} ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/${USERNAME} \
    && chmod 0440 /etc/sudoers.d/${USERNAME} \
    && chown -R ${UID}:${GID} /home/${USERNAME}

RUN mv /root/Groot2.AppImage /home/${USERNAME}/Groot2.AppImage \
    && chown ${UID}:${GID} /home/${USERNAME}/Groot2.AppImage

################################################################################
# OVERLAY WORKSPACE: /dev_ws
#
# bt_example is built here during the image build for verification.
# At runtime, the source is mounted from the host for live editing.
# Rebuild inside the container with: cmake --build /dev_ws/build/bt_example
################################################################################

RUN mkdir -p /dev_ws/src
WORKDIR /dev_ws

COPY ./bt_example/ ./src/bt_example/
RUN cmake -S src/bt_example -B build/bt_example \
        -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build/bt_example -j$(nproc)

RUN chown -R ${UID}:${GID} /dev_ws/

USER ${USERNAME}
RUN echo "source /entrypoint.sh" >> /home/${USERNAME}/.bashrc
