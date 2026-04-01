#!/bin/bash
# ============================================================================
# test.sh — Build and run the bt_example simulation with Groot2
#
# Run this script INSIDE the dev container:
#   docker compose run dev bash
#   bash /dev_ws/src/bt_example/../../../test.sh
#
# Or from the mounted source directly:
#   bash test.sh
# ============================================================================
set -e

SRC=/dev_ws/src/bt_example
BUILD=/dev_ws/build/bt_example
BINARY=$BUILD/autonomy_node_sim
TREE=$SRC/bt_structures/simulation_demo.xml
GROOT2=$HOME/Groot2.AppImage

# ----------------------------------------------------------------------------
# Build
# ----------------------------------------------------------------------------
echo "============================================"
echo "  Building bt_example"
echo "============================================"
cmake -S "$SRC" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD" -- -j"$(nproc)"

# ----------------------------------------------------------------------------
# Launch Groot2 in the background (if available)
# ----------------------------------------------------------------------------
echo ""
if [ -x "$GROOT2" ]; then
    echo "============================================"
    echo "  Launching Groot2 (port 1668)"
    echo "============================================"
    "$GROOT2" &
    GROOT2_PID=$!
    echo "  Groot2 PID: $GROOT2_PID"
    # Give Groot2 a moment to start before the tree begins ticking
    sleep 2
else
    echo "  [SKIP] Groot2 not found at $GROOT2"
    GROOT2_PID=""
fi

# ----------------------------------------------------------------------------
# Run the simulation
# ----------------------------------------------------------------------------
echo ""
echo "============================================"
echo "  Running simulation"
echo "  Tree: $TREE"
echo "============================================"
echo ""
"$BINARY" "$TREE"

# ----------------------------------------------------------------------------
# Clean up Groot2 when simulation exits
# ----------------------------------------------------------------------------
if [ -n "$GROOT2_PID" ] && kill -0 "$GROOT2_PID" 2>/dev/null; then
    echo ""
    echo "Stopping Groot2 (PID $GROOT2_PID)..."
    kill "$GROOT2_PID" 2>/dev/null || true
fi
