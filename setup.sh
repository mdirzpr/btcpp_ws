#!/bin/bash
set -e

# Navigate to the workspace
cd /home/$WORKSPACE

# Import the repository sources
vcs import src < src/btcpp.repos

apt-get update
