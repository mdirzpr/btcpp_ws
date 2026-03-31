// ============================================================================
// SIMULATION-ONLY AUTONOMY NODE (standalone, no ROS 2)
// ============================================================================
// Loads a behavior tree XML file, registers all custom action/condition nodes,
// and ticks the tree in a loop. Groot2 visualization is available on port 1668.
//
// Usage:
//   autonomy_node_sim <path/to/tree.xml>
//
// Example:
//   autonomy_node_sim bt_structures/simulation_demo.xml
// ============================================================================

#include <csignal>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/xml_parsing.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

#include "action_nodes.h"

using namespace std::chrono_literals;

static volatile bool g_running = true;

static void signalHandler(int)
{
    g_running = false;
}

static void registerAllNodes(BT::BehaviorTreeFactory& factory)
{
    // Condition nodes
    factory.registerNodeType<EmergencyButtonNotPressed>("EmergencyButtonNotPressed");
    factory.registerNodeType<Printer3DReady>("Printer3DReady");
    factory.registerNodeType<PrintingFinished>("PrintingFinished");
    factory.registerNodeType<IsGripperOpen>("IsGripperOpen");
    factory.registerNodeType<IsShuttleFree>("IsShuttleFree");
    factory.registerNodeType<ProductInQAPosition>("ProductInQAPosition");
    factory.registerNodeType<IsQualityOK>("IsQualityOK");
    factory.registerNodeType<IsRobotHome>("IsRobotHome");
    factory.registerNodeType<IsStorageFree>("IsStorageFree");

    // Configuration nodes
    factory.registerNodeType<LoadSkillsConfig>("LoadSkillsConfig");
    factory.registerNodeType<LoadRobotConfig>("LoadRobotConfig");

    // 3D Printing
    factory.registerNodeType<GetGCodePath>("GetGCodePath");
    factory.registerNodeType<InitiatePrinter>("InitiatePrinter");
    factory.registerNodeType<StartPrinting>("StartPrinting");

    // Robot manipulation
    factory.registerNodeType<OpenCloseGripper>("OpenCloseGripper");
    factory.registerNodeType<MoveLinear>("MoveLinear");
    factory.registerNodeType<MoveRobot>("MoveRobot");
    factory.registerNodeType<HomeRobot>("HomeRobot");
    factory.registerNodeType<Grasp>("Grasp");
    factory.registerNodeType<SetGripperType>("SetGripperType");
    factory.registerNodeType<Release>("Release");

    // Transport
    factory.registerNodeType<TransportTo>("TransportTo");

    // Quality assurance
    factory.registerNodeType<QualityCheck>("QualityCheck");
    factory.registerNodeType<TakeImage>("TakeImage");

    // Storage
    factory.registerNodeType<StoreInStorage>("StoreInStorage");

    // Legacy
    factory.registerNodeType<GetParameters>("GetParameters");
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: autonomy_node_sim <path/to/tree.xml>\n";
        return 1;
    }

    std::string tree_xml_file = argv[1];

    std::signal(SIGINT,  signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::cout << "╔════════════════════════════════════════════╗\n";
    std::cout << "║  SIMULATION MODE - No Real Hardware        ║\n";
    std::cout << "╚════════════════════════════════════════════╝\n";
    std::cout << "Loading tree from: " << tree_xml_file << "\n";

    BT::BehaviorTreeFactory factory;
    registerAllNodes(factory);
    factory.registerBehaviorTreeFromFile(tree_xml_file);

    auto blackboard = BT::Blackboard::create();
    auto tree = factory.createTree("mainTree", blackboard);

    // Enable live monitoring with Groot2 (connect on port 1668)
    BT::Groot2Publisher publisher(tree, 1668);
    std::cout << "Groot2 publisher active on port 1668\n";
    std::cout << "Running... (press Ctrl+C to stop)\n\n";

    BT::NodeStatus status = BT::NodeStatus::RUNNING;

    while (g_running)
    {
        if (status == BT::NodeStatus::RUNNING) {
            status = tree.tickExactlyOnce();
        } else {
            static bool printed_once = false;
            if (!printed_once) {
                std::cout << "\nTree completed with "
                          << (status == BT::NodeStatus::SUCCESS ? "SUCCESS" : "FAILURE")
                          << "\nWaiting... (Groot2 still active on port 1668)\n";
                printed_once = true;
            }
            std::this_thread::sleep_for(100ms);
        }
        std::this_thread::sleep_for(10ms);
    }

    std::cout << "\nShutting down.\n";
    return 0;
}
