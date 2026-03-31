#ifndef ACTION_NODES_H
#define ACTION_NODES_H

#include <behaviortree_cpp/bt_factory.h>
#include <sstream>
#include <string>
#include <thread>

// ============================================================================
// 🤖 SIMULATION-ONLY ACTION NODES
// ============================================================================
// This is the bt_example package - a simplified BehaviorTree.CPP demonstration
// without external hardware dependencies. Perfect for learning and showcasing
// behavior tree concepts.
//
// 🏭 Simulates industrial automation workflow:
//    - 3D Printing
//    - Robotic Pick-and-Place
//    - Transport System
//    - Quality Assurance
// ============================================================================

// 🔧 Helper function for simulating actions
void simulateAction(const std::string& action, int time_ms);

// 🔧 Helper to get blackboard value with default fallback
template<typename T>
T getBlackboardValueOr(const BT::Blackboard::Ptr& bb, const std::string& key, const T& default_value)
{
    auto value = bb->getStamped<T>(key);
    if (value) {
        return value.value().value;
    }
    return default_value;
}

// ============================================================================
// 🚨 CONDITION NODES - Safety & Status Checks
// ============================================================================

// --- ⛔ Emergency Button Status ---
class EmergencyButtonNotPressed : public BT::ConditionNode {
public:
    EmergencyButtonNotPressed(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 🖨️ 3D Printer Ready Status ---
class Printer3DReady : public BT::ConditionNode {
public:
    Printer3DReady(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- ✅ Printing Finished Status ---
class PrintingFinished : public BT::ConditionNode {
public:
    PrintingFinished(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 🤏 Gripper Open Status ---
class IsGripperOpen : public BT::ConditionNode {
public:
    IsGripperOpen(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 🚃 Shuttle Free Status ---
class IsShuttleFree : public BT::ConditionNode {
public:
    IsShuttleFree(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 📍 Product in QA Position Status ---
class ProductInQAPosition : public BT::ConditionNode {
public:
    ProductInQAPosition(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- ✅ Quality Check Result ---
class IsQualityOK : public BT::ConditionNode {
public:
    IsQualityOK(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 🏠 Robot Home Status ---
class IsRobotHome : public BT::ConditionNode {
public:
    IsRobotHome(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 📦 Storage Free Status ---
class IsStorageFree : public BT::ConditionNode {
public:
    IsStorageFree(const std::string& name, const BT::NodeConfig& config)
        : BT::ConditionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// ============================================================================
// ⚙️ CONFIGURATION NODES - AAS Integration (Simulated)
// ============================================================================

// --- 📋 Load Skills Configuration from AAS ---
class LoadSkillsConfig : public BT::SyncActionNode {
public:
    LoadSkillsConfig(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts();
};

// --- 🤖 Load Robot Configuration from AAS ---
class LoadRobotConfig : public BT::SyncActionNode {
public:
    LoadRobotConfig(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts();
};

// ============================================================================
// 🖨️ ACTION NODES - 3D Printing
// ============================================================================

// --- 📂 Get G-Code Path from AAS ---
class GetGCodePath : public BT::SyncActionNode {
public:
    GetGCodePath(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts();
};

// --- 🔥 Initiate Printer (heat up, home axes) ---
class InitiatePrinter : public BT::StatefulActionNode {
private:
    int init_progress_ = 0;
public:
    InitiatePrinter(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts();
};

// --- ▶️ Start Printing ---
class StartPrinting : public BT::StatefulActionNode {
private:
    int print_progress_ = 0;
public:
    StartPrinting(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts();
};

// ============================================================================
// 🦾 ACTION NODES - Robot Manipulation
// ============================================================================

// --- 🤏 Open/Close Gripper ---
class OpenCloseGripper : public BT::SyncActionNode {
public:
    OpenCloseGripper(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts();
};

// --- 🔄 Move Linear Axis ---
class MoveLinear : public BT::StatefulActionNode {
private:
    int linear_loading_ = 0;
public:
    MoveLinear(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts();
};

// --- 🦾 Move Robot ---
class MoveRobot : public BT::StatefulActionNode {
private:
    int Robot_loading_ = 0;
public:
    MoveRobot(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 🏠 Home Robot ---
class HomeRobot : public BT::StatefulActionNode {
private:
    int home_progress_ = 0;
public:
    HomeRobot(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 🤏 Grasp Object ---
class Grasp : public BT::SyncActionNode {
public:
    Grasp(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts();
};

// --- ⚙️ Set Gripper Type ---
class SetGripperType : public BT::SyncActionNode {
public:
    SetGripperType(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    static BT::PortsList providedPorts();
    BT::NodeStatus tick() override;
};

// --- 📤 Release Object ---
class Release : public BT::SyncActionNode {
public:
    Release(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// ============================================================================
// 🚃 ACTION NODES - Transport System
// ============================================================================

// --- 🚚 Transport To Destination ---
class TransportTo : public BT::StatefulActionNode {
private:
    int transport_progress_ = 0;
public:
    TransportTo(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts();
};

// ============================================================================
// 🔍 ACTION NODES - Quality Assurance
// ============================================================================

// --- 🔍 Quality Check ---
class QualityCheck : public BT::StatefulActionNode {
private:
    int QC_loading_ = 0;
public:
    QualityCheck(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- 📷 Take Image for QA ---
class TakeImage : public BT::SyncActionNode {
public:
    TakeImage(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// --- ⚙️ Get Parameters (legacy) ---
class GetParameters : public BT::SyncActionNode {
public:
    GetParameters(const std::string& name, const BT::NodeConfig& config)
        : BT::SyncActionNode(name, config) {}
    BT::NodeStatus tick() override;
    static BT::PortsList providedPorts() { return {}; }
};

// ============================================================================
// 📦 ACTION NODES - Storage System
// ============================================================================

// --- 📦 Store In Storage ---
class StoreInStorage : public BT::StatefulActionNode {
private:
    int store_progress_ = 0;
public:
    StoreInStorage(const std::string& name, const BT::NodeConfig& config)
        : BT::StatefulActionNode(name, config) {}
    BT::NodeStatus onStart() override;
    BT::NodeStatus onRunning() override;
    void onHalted() override;
    static BT::PortsList providedPorts();
};

// 🔧 Registration helper
void RegisterNodes(BT::BehaviorTreeFactory& factory);

#endif // ACTION_NODES_H
