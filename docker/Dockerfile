# Dockerfile for BehaviorTree.CPP

# Use Ubuntu 24 as the base image
FROM ubuntu:24.04 AS btcpp_ws

# Set the maintainer label
# LABEL maintainer="youremail@example.com"

# Update and install required packages
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y \
    build-essential \
    cmake git vim bash \
    libzmq3-dev libsqlite3-dev libgtest-dev \
    && apt-get clean

# Define the arguments
ARG WORKSPACE=btcpp_ws

# Set the working directory
WORKDIR /home/${WORKSPACE}

# Copy src and ws_files to the container
COPY src /home/${WORKSPACE}/src
COPY *.sh /home/${WORKSPACE}/.

# Clone the behavior tree repository (replace with the actual repository URL)
RUN git clone https://github.com/BehaviorTree/BehaviorTree.CPP.git

# Build the behavior tree project (replace with actual build commands)
RUN cd /home/${WORKSPACE}/BehaviorTree.CPP && mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && make && make install

# Default command
CMD ["/bin/bash"]
