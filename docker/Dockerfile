# Dockerfile for BehaviorTree.CPP library

# Use Ubuntu 24 as the base image
FROM ubuntu:24.04 AS btcpp_ws

# Update and install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake git vim \
    libzmq3-dev libsqlite3-dev libgtest-dev \
    && apt-get clean

# Copy src and ws_files to the container
COPY src /home/${WORKSPACE}/src
COPY ws_files /home/${WORKSPACE}/ws_files
COPY *.sh /home/${WORKSPACE}

# Define the arguments
ARG WORKSPACE=btcpp_ws

# Set the working directory
WORKDIR /home/${WORKSPACE}

# Clone the behavior tree repository
RUN git clone https://github.com/BehaviorTree/BehaviorTree.CPP.git

# Build the BehaviorTree.CPP
RUN cd /home/${WORKSPACE}/BehaviorTree.CPP && mkdir build && cd build && cmake .. && make

# Default command
CMD ["/bin/bash"]
