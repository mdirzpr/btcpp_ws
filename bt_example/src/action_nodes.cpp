#include "action_nodes.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

// ============================================================================
// 🔧 HELPER FUNCTION
// ============================================================================
void simulateAction(const std::string& action, int time_ms)
{
    std::cout << "⏳ Executing: " << action << "...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
    std::cout << "✅ " << action << " completed!\n";
}

// ============================================================================
// 🚨 CONDITION NODES IMPLEMENTATION
// ============================================================================

// --- ⛔ Emergency Button Not Pressed ---
BT::NodeStatus EmergencyButtonNotPressed::tick()
{
    std::cout << "🟢 [Safety] Emergency button: NOT PRESSED (Safe to proceed)\n";
    return BT::NodeStatus::SUCCESS;
}

// --- 🖨️ 3D Printer Ready ---
BT::NodeStatus Printer3DReady::tick()
{
    std::cout << "🖨️ [3D Printer] Checking printer status...\n";
    std::cout << "   ✅ No active failures detected\n";
    std::cout << "   ✅ Raw material: PLA - 250g available\n";
    std::cout << "   ✅ G-code file loaded and validated\n";
    std::cout << "🟢 [3D Printer] Status: READY\n";
    return BT::NodeStatus::SUCCESS;
}

// --- ✅ Printing Finished ---
BT::NodeStatus PrintingFinished::tick()
{
    std::cout << "🖨️ [3D Printer] Checking if printing is finished...\n";
    auto& bb = config().blackboard;
    bool finished = getBlackboardValueOr<bool>(bb, "printing_complete", false);

    if (finished) {
        std::cout << "✅ [3D Printer] Printing COMPLETE\n";
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "⏳ [3D Printer] Still printing...\n";
        return BT::NodeStatus::FAILURE;
    }
}

// --- 🤏 Gripper Open Status ---
BT::NodeStatus IsGripperOpen::tick()
{
    auto& bb = config().blackboard;
    bool gripper_open = getBlackboardValueOr<bool>(bb, "gripper_open", true);

    if (gripper_open) {
        std::cout << "🟢 [Gripper] Status: OPEN\n";
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "🔴 [Gripper] Status: CLOSED\n";
        return BT::NodeStatus::FAILURE;
    }
}

// --- 🚃 Shuttle Free Status ---
BT::NodeStatus IsShuttleFree::tick()
{
    auto& bb = config().blackboard;
    bool shuttle_free = getBlackboardValueOr<bool>(bb, "shuttle_free", true);

    if (shuttle_free) {
        std::cout << "🟢 [Transport] Shuttle status: FREE (available)\n";
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "🔴 [Transport] Shuttle status: OCCUPIED (waiting...)\n";
        return BT::NodeStatus::FAILURE;
    }
}

// --- 📍 Product in QA Position ---
BT::NodeStatus ProductInQAPosition::tick()
{
    // Simulate checking product position (always succeeds after 2 seconds)
    simulateAction("📍 [QA] Checking product position", 2000);
    std::cout << "✅ [QA] Product is in QA position\n";
    return BT::NodeStatus::SUCCESS;
}

// --- ✅ Quality OK ---
BT::NodeStatus IsQualityOK::tick()
{
    auto& bb = config().blackboard;
    bool quality_passed = getBlackboardValueOr<bool>(bb, "quality_passed", false);

    if (quality_passed) {
        std::cout << "✅ [QA] Quality Check: PASSED\n";
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "❌ [QA] Quality Check: FAILED\n";
        return BT::NodeStatus::FAILURE;
    }
}

// --- 🏠 Is Robot Home ---
BT::NodeStatus IsRobotHome::tick()
{
    auto& bb = config().blackboard;
    bool robot_at_home = getBlackboardValueOr<bool>(bb, "robot_at_home", true);

    if (robot_at_home) {
        std::cout << "🟢 [Robot] Position: HOME ✓\n";
        return BT::NodeStatus::SUCCESS;
    } else {
        std::cout << "🔴 [Robot] Position: NOT at home\n";
        return BT::NodeStatus::FAILURE;
    }
}

// ============================================================================
// ⚙️ CONFIGURATION NODES IMPLEMENTATION - AAS Integration (Simulated)
// ============================================================================

// --- 📋 Load Skills Configuration from AAS ---
BT::PortsList LoadSkillsConfig::providedPorts() {
    return {
        BT::OutputPort<std::string>("printer_ip"),
        BT::OutputPort<std::string>("printer_skill_node_id"),
        BT::OutputPort<std::string>("transport_ip"),
        BT::OutputPort<std::string>("transport_skill_node_id"),
        BT::OutputPort<std::string>("qa_ip"),
        BT::OutputPort<std::string>("qa_skill_node_id")
    };
}

BT::NodeStatus LoadSkillsConfig::tick()
{
    std::cout << "📋 [AAS] Loading skills configuration from Asset Administration Shell...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    // Simulated device configurations
    std::string printer_ip = "192.168.1.100";
    std::string printer_skill_node_id = "ns=6;s=Printer3D.SkillSet.PrintSkill";
    std::string transport_ip = "192.168.1.101";
    std::string transport_skill_node_id = "ns=6;s=TransportSystem.SkillSet.TransportSkill";
    std::string qa_ip = "192.168.1.102";
    std::string qa_skill_node_id = "ns=6;s=QASystem.SkillSet.InspectionSkill";

    std::cout << "   🖨️ [3D Printer] IP: " << printer_ip << "\n";
    std::cout << "   🚃 [Transport]  IP: " << transport_ip << "\n";
    std::cout << "   🔍 [QA System]  IP: " << qa_ip << "\n";

    setOutput("printer_ip", printer_ip);
    setOutput("printer_skill_node_id", printer_skill_node_id);
    setOutput("transport_ip", transport_ip);
    setOutput("transport_skill_node_id", transport_skill_node_id);
    setOutput("qa_ip", qa_ip);
    setOutput("qa_skill_node_id", qa_skill_node_id);

    config().blackboard->set("printer_ip", printer_ip);
    config().blackboard->set("transport_ip", transport_ip);
    config().blackboard->set("qa_ip", qa_ip);

    std::cout << "✅ [AAS] Skills configuration loaded successfully\n";
    return BT::NodeStatus::SUCCESS;
}

// --- 🤖 Load Robot Configuration from AAS ---
BT::PortsList LoadRobotConfig::providedPorts() {
    return {
        BT::OutputPort<std::string>("robot_ip"),
        BT::OutputPort<std::string>("robot_skill_node_id"),
        BT::OutputPort<std::string>("gripper_type"),
        BT::OutputPort<double>("max_gripper_force"),
        BT::OutputPort<std::string>("workspace_limits")
    };
}

BT::NodeStatus LoadRobotConfig::tick()
{
    std::cout << "🤖 [AAS] Loading robot configuration from Asset Administration Shell...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    std::string robot_ip = "192.168.1.50";
    std::string robot_skill_node_id = "ns=6;s=Robot.SkillSet.ManipulationSkill";
    std::string gripper_type = "parallel_jaw";
    double max_gripper_force = 100.0;
    std::string workspace_limits = "X[-500,500], Y[-500,500], Z[0,800]";

    std::cout << "   🦾 Robot IP: " << robot_ip << "\n";
    std::cout << "   🤏 Gripper Type: " << gripper_type << "\n";
    std::cout << "   💪 Max Force: " << max_gripper_force << " N\n";

    setOutput("robot_ip", robot_ip);
    setOutput("robot_skill_node_id", robot_skill_node_id);
    setOutput("gripper_type", gripper_type);
    setOutput("max_gripper_force", max_gripper_force);
    setOutput("workspace_limits", workspace_limits);

    config().blackboard->set("robot_ip", robot_ip);
    config().blackboard->set("gripper_type", gripper_type);

    std::cout << "✅ [AAS] Robot configuration loaded successfully\n";
    return BT::NodeStatus::SUCCESS;
}

// ============================================================================
// 🖨️ ACTION NODES - 3D Printing
// ============================================================================

// --- 📂 Get G-Code Path ---
BT::PortsList GetGCodePath::providedPorts() {
    return {
        BT::InputPort<std::string>("material_type", "PLA", "Material type (PLA, ABS, etc.)"),
        BT::OutputPort<std::string>("gcode_path", "Path to G-Code file")
    };
}

BT::NodeStatus GetGCodePath::tick()
{
    auto material = getInput<std::string>("material_type").value_or("PLA");

    std::cout << "📂 [AAS] Querying G-code for material: " << material << "\n";

    std::string gcode_path;
    if (material == "PLA") {
        gcode_path = "/gcode/standard_part_PLA.gcode";
    } else if (material == "ABS") {
        gcode_path = "/gcode/standard_part_ABS.gcode";
    } else {
        gcode_path = "/gcode/default_part.gcode";
    }

    setOutput("gcode_path", gcode_path);
    config().blackboard->set("gcode_path", gcode_path);

    simulateAction("📂 Downloading G-code from AAS", 2000);
    std::cout << "✅ [AAS] Retrieved G-code: " << gcode_path << "\n";

    return BT::NodeStatus::SUCCESS;
}

// --- 🔥 Initiate Printer ---
BT::PortsList InitiatePrinter::providedPorts() {
    return { BT::InputPort<std::string>("gcode_path", "/simulated/default.gcode", "Path to G-Code file") };
}

BT::NodeStatus InitiatePrinter::onStart()
{
    auto gcode = getInput<std::string>("gcode_path").value_or("/simulated/default.gcode");

    std::cout << "🔥 [3D Printer] Starting initialization...\n";
    std::cout << "   📂 Loading G-code: " << gcode << "\n";
    init_progress_ = 0;
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus InitiatePrinter::onRunning()
{
    init_progress_ += 25;

    switch (init_progress_) {
        case 25:  std::cout << "🔥 [3D Printer] Heating extruder to 210°C... (25%)\n"; break;
        case 50:  std::cout << "🔥 [3D Printer] Heating print bed to 60°C... (50%)\n"; break;
        case 75:  std::cout << "🔥 [3D Printer] Homing all axes... (75%)\n"; break;
        case 100: std::cout << "🔥 [3D Printer] Loading filament... (100%)\n"; break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    if (init_progress_ >= 100) {
        std::cout << "✅ [3D Printer] Initialization COMPLETE - Ready to print\n";
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
}

void InitiatePrinter::onHalted()
{
    std::cout << "⏸️ [3D Printer] Initialization halted. Cooling down...\n";
    init_progress_ = 0;
}

// --- ▶️ Start Printing ---
BT::PortsList StartPrinting::providedPorts() {
    return { BT::InputPort<std::string>("gcode_path", "/simulated/default.gcode", "Path to G-Code file") };
}

BT::NodeStatus StartPrinting::onStart()
{
    auto gcode_path = getInput<std::string>("gcode_path").value_or("/simulated/default.gcode");

    std::cout << "▶️ [3D Printer] Starting print job...\n";
    std::cout << "   📄 G-Code: " << gcode_path << "\n";
    std::cout << "   🧵 Material: PLA (default)\n";
    print_progress_ = 0;
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus StartPrinting::onRunning()
{
    print_progress_ += 20;
    std::cout << "🖨️ [3D Printer] Printing progress: " << print_progress_ << "%\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    if (print_progress_ >= 100) {
        std::cout << "✅ [3D Printer] Print job COMPLETE!\n";
        config().blackboard->set("printing_complete", true);
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
}

void StartPrinting::onHalted()
{
    std::cout << "⛔ [3D Printer] Print job HALTED!\n";
    print_progress_ = 0;
    config().blackboard->set("printing_complete", false);
}

// ============================================================================
// 🦾 ACTION NODES - Robot Manipulation
// ============================================================================

// --- 🤏 Open/Close Gripper ---
BT::PortsList OpenCloseGripper::providedPorts() {
    return { BT::InputPort<std::string>("command", "open", "Command: 'open' or 'close'") };
}

BT::NodeStatus OpenCloseGripper::tick()
{
    std::string cmd = getInput<std::string>("command").value_or("open");
    if (cmd == "open") {
        simulateAction("🤏 Gripper OPEN", 1000);
        config().blackboard->set("gripper_open", true);
    } else if (cmd == "close") {
        simulateAction("🤏 Gripper CLOSE", 1000);
        config().blackboard->set("gripper_open", false);
    } else {
        std::cout << "❌ [Gripper] Unknown command: " << cmd << "\n";
        return BT::NodeStatus::FAILURE;
    }
    return BT::NodeStatus::SUCCESS;
}

// --- 🔄 Move Linear Axis ---
BT::PortsList MoveLinear::providedPorts() {
    return { BT::InputPort<std::string>("target_position", "home", "Target position name") };
}

BT::NodeStatus MoveLinear::onStart()
{
    auto target = getInput<std::string>("target_position").value_or("home");
    std::cout << "🔄 Starting MoveLinear to position: " << target << "\n";
    linear_loading_ = 0;
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveLinear::onRunning()
{
    linear_loading_ += 20;
    std::cout << "🔄 Move Linear loading: " << linear_loading_ << "%\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    if (linear_loading_ < 100) {
        return BT::NodeStatus::RUNNING;
    }
    else {
        simulateAction("Moving Linear Axis", 3500);
        return BT::NodeStatus::SUCCESS;
    }
}

void MoveLinear::onHalted()
{
    linear_loading_ = 0;
    std::cout << "⏸️ MoveLinear halted. Cleaning up...\n";
}

// --- 🦾 Move Robot ---
BT::NodeStatus MoveRobot::onStart()
{
    std::cout << "🦾 Starting MoveRobot...\n";
    Robot_loading_ = 0;
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveRobot::onRunning()
{
    Robot_loading_ += 20;
    std::cout << "🦾 MoveRobot progress: " << Robot_loading_ << "%\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    if (Robot_loading_ < 100) {
        return BT::NodeStatus::RUNNING;
    }
    else {
        simulateAction("Moving Robot", 3500);
        return BT::NodeStatus::SUCCESS;
    }
}

void MoveRobot::onHalted()
{
    Robot_loading_ = 0;
    std::cout << "⏸️ MoveRobot halted. Cleaning up...\n";
}

// --- 🏠 Home Robot ---
BT::NodeStatus HomeRobot::onStart()
{
    std::cout << "🏠 [Robot] Initiating homing sequence...\n";
    home_progress_ = 0;
    config().blackboard->set("robot_at_home", false);
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus HomeRobot::onRunning()
{
    home_progress_ += 25;
    std::cout << "🏠 [Robot] Homing progress: " << home_progress_ << "%\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    if (home_progress_ >= 100) {
        std::cout << "✅ [Robot] HOME position REACHED\n";
        config().blackboard->set("robot_at_home", true);
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
}

void HomeRobot::onHalted()
{
    std::cout << "⏸️ [Robot] Homing sequence HALTED!\n";
    home_progress_ = 0;
}

// --- 🤏 Grasp Object ---
BT::PortsList Grasp::providedPorts() {
    return { BT::InputPort<std::string>("gripper_type") };
}

BT::NodeStatus Grasp::tick()
{
    auto grip_type = getInput<std::string>("gripper_type");
    if (!grip_type) {
        throw BT::RuntimeError("❌ Missing required input [gripper_type]: ", grip_type.error());
    }
    std::string msg = "🤏 [Grasp] Using gripper type: " + grip_type.value();
    simulateAction(msg, 3000);
    config().blackboard->set("gripper_open", false);
    return BT::NodeStatus::SUCCESS;
}

// --- ⚙️ Set Gripper Type ---
BT::PortsList SetGripperType::providedPorts() {
    return { BT::OutputPort<std::string>("gripper_type") };
}

BT::NodeStatus SetGripperType::tick()
{
    setOutput("gripper_type", "parallel");
    simulateAction("⚙️ Set gripper_type to 'parallel'", 2500);
    return BT::NodeStatus::SUCCESS;
}

// --- 📤 Release Object ---
BT::NodeStatus Release::tick()
{
    simulateAction("📤 Releasing Object", 3500);
    config().blackboard->set("gripper_open", true);
    return BT::NodeStatus::SUCCESS;
}

// ============================================================================
// 🚃 ACTION NODES - Transport System
// ============================================================================

// --- 🚚 Transport To ---
BT::PortsList TransportTo::providedPorts() {
    return { BT::InputPort<std::string>("destination") };
}

BT::NodeStatus TransportTo::onStart()
{
    auto dest = getInput<std::string>("destination");
    if (!dest) {
        throw BT::RuntimeError("❌ Missing required input [destination]: ", dest.error());
    }

    std::cout << "🚚 [Transport] Initiating transport to: " << dest.value() << "\n";
    transport_progress_ = 0;
    config().blackboard->set("shuttle_free", false);
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus TransportTo::onRunning()
{
    transport_progress_ += 10;
    std::cout << "🚃 [Transport] Progress: " << transport_progress_ << "%\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(700));

    if (transport_progress_ >= 100) {
        auto dest = getInput<std::string>("destination");
        std::cout << "✅ [Transport] Arrived at: " << dest.value() << "\n";
        config().blackboard->set("product_position", dest.value());
        config().blackboard->set("shuttle_free", true);
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
}

void TransportTo::onHalted()
{
    std::cout << "⏸️ [Transport] Transport HALTED!\n";
    transport_progress_ = 0;
    config().blackboard->set("shuttle_free", true);
}

// ============================================================================
// 🔍 ACTION NODES - Quality Assurance
// ============================================================================

// --- 🔍 Quality Check ---
BT::NodeStatus QualityCheck::onStart() {
    std::cout << "🔍 Starting QualityCheck...\n";
    QC_loading_ = 0;
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus QualityCheck::onRunning() {
    QC_loading_ += 20;
    std::cout << "🔍 QualityCheck loading: " << QC_loading_ << "%\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    if (QC_loading_ < 100) {
        return BT::NodeStatus::RUNNING;
    }
    else {
        simulateAction("Quality Check of Product", 3000);
        return BT::NodeStatus::SUCCESS;
    }
}

void QualityCheck::onHalted() {
    std::cout << "⏸️ QualityCheck halted. Cleaning up...\n";
    QC_loading_ = 0;
}

// --- 📷 Take Image ---
BT::NodeStatus TakeImage::tick()
{
    std::cout << "📷 [QA Camera] Positioning camera over product...\n";
    simulateAction("📷 Capturing high-resolution image", 1500);
    std::cout << "📷 [QA Camera] Running AI-based defect detection...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Simulate quality check result (90% pass rate)
    bool quality_ok = (rand() % 10) < 9;

    if (quality_ok) {
        std::cout << "✅ [QA Camera] Analysis: No defects detected\n";
        config().blackboard->set("quality_passed", true);
    } else {
        std::cout << "❌ [QA Camera] Analysis: Defects detected!\n";
        config().blackboard->set("quality_passed", false);
    }

    return BT::NodeStatus::SUCCESS;
}

// --- ⚙️ Get Parameters (legacy) ---
BT::NodeStatus GetParameters::tick()
{
    simulateAction("⚙️ Getting Parameters", 2500);
    return BT::NodeStatus::SUCCESS;
}

// ============================================================================
// 📦 ACTION NODES - Storage System
// ============================================================================

// --- 📦 Is Storage Free (Condition) ---
BT::NodeStatus IsStorageFree::tick()
{
    // Always returns SUCCESS (storage always has free space in simulation)
    std::cout << "📦 [Storage] Checking storage availability...\n";
    std::cout << "✅ [Storage] Storage has free space available\n";
    return BT::NodeStatus::SUCCESS;
}

// --- 📦 Store In Storage ---
BT::PortsList StoreInStorage::providedPorts() {
    return { BT::InputPort<std::string>("slot", "A1", "Storage slot identifier") };
}

BT::NodeStatus StoreInStorage::onStart()
{
    auto slot = getInput<std::string>("slot").value_or("A1");
    std::cout << "📦 [Storage] Starting storage operation...\n";
    std::cout << "   📍 Target slot: " << slot << "\n";
    store_progress_ = 0;
    return BT::NodeStatus::RUNNING;
}

BT::NodeStatus StoreInStorage::onRunning()
{
    store_progress_ += 25;

    switch (store_progress_) {
        case 25:  std::cout << "📦 [Storage] Aligning product... (25%)\n"; break;
        case 50:  std::cout << "📦 [Storage] Lowering to slot... (50%)\n"; break;
        case 75:  std::cout << "📦 [Storage] Securing product... (75%)\n"; break;
        case 100: std::cout << "📦 [Storage] Updating inventory... (100%)\n"; break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    if (store_progress_ >= 100) {
        auto slot = getInput<std::string>("slot").value_or("A1");
        std::cout << "✅ [Storage] Product stored successfully in slot: " << slot << "\n";
        config().blackboard->set("product_stored", true);
        config().blackboard->set("storage_slot", slot);
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
}

void StoreInStorage::onHalted()
{
    std::cout << "⏸️ [Storage] Storage operation HALTED!\n";
    store_progress_ = 0;
}
