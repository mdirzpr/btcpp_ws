#!/bin/bash
# Entrypoint for BehaviorTree.CPP development container

GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

echo ""
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${BLUE}   BehaviorTree.CPP Development Environment${NC}"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""
echo -e "${GREEN}Ready!${NC} Common commands:"
echo -e "  ${BLUE}# Build bt_example${NC}"
echo -e "  cmake -S /dev_ws/src/bt_example -B /dev_ws/build/bt_example && cmake --build /dev_ws/build/bt_example"
echo -e ""
echo -e "  ${BLUE}# Run the simulation${NC}"
echo -e "  /dev_ws/build/bt_example/autonomy_node_sim /dev_ws/src/bt_example/bt_structures/simulation_demo.xml"
echo -e ""
echo -e "  ${BLUE}# Launch Groot2${NC}"
echo -e "  ~/Groot2.AppImage &"
echo -e "${BLUE}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

exec "$@"
