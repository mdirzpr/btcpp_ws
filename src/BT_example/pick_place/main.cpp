#include <iostream>

#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/behavior_tree.h>

using namespace BT;

// Define FindBall action node
class FindBall : public BT::SyncActionNode 
{
  public:
    FindBall(const std::string& name) : BT::SyncActionNode(name, {})
    {
    }

    // Define the tick() function for the FindBall node
    NodeStatus tick() override
    {
        std::cout << "[⚓ FindBall ] => \t" << this->name() << std::endl; 
        return BT::NodeStatus::SUCCESS;
    }
};

// Define PickBall action node
class PickBall : public BT::SyncActionNode
{
  public:
    PickBall(const std::string& name) : BT::SyncActionNode(name, {})
    {
    }

    // Define the tick() function for the PickBall node
    NodeStatus tick() override
    {
        std::cout << "[⚓ PickBall ] => \t" << this->name() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

// Define PlaceBall action node
class PlaceBall : public BT::SyncActionNode
{
  public:
    PlaceBall(const std::string& name) : BT::SyncActionNode(name, {})
    {
    }

    // Define the tick() function for the PlaceBall node
    NodeStatus tick() override
    {
        std::cout << "[⚓ PlaceBall ] => \t" << this->name() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }
};

// Define GripperInterface class for interacting with the gripper
class GripperInterface
{
  private:
    bool _opened;

  public:
    GripperInterface() : _opened(true)
    {
    }

    NodeStatus open()
    {
        _opened = true;
        std::cout << "GripperInterface::open" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    NodeStatus close()
    {
        std::cout << "GripperInterface::close" << std::endl;
        _opened = false;
        return BT::NodeStatus::SUCCESS;
    }
};

// Define BallClose condition function
BT::NodeStatus BallClose()
{
    std::cout << "[ Close to ball: NO ]" << std::endl;
    return BT::NodeStatus::FAILURE;
}

// Define BallGrasped condition function
BT::NodeStatus BallGrasped()
{
    std::cout << "[ Grasped: NO ]" << std::endl;
    return BT::NodeStatus::FAILURE;
}

// Define the behavior tree structure in XML format
static const char* xml_text = R"(
 <root main_tree_to_execute = "MainTree" >

     <BehaviorTree ID="MainTree">
        <Sequence name="root_sequence">
            <FindBall   name="found_ok"/>
                <Sequence>
                    <Fallback>
                        <BallClose   name="no_ball"/>
                        <PickBall    name="approach_ball"/>
                    </Fallback>
                    <Fallback>
                        <BallGrasped   name="no_grasp"/>
                        <GraspBall  name="grasp_ball"/>
                    </Fallback>
                </Sequence>
            <PlaceBall   name="ball_placed"/>
        </Sequence>
     </BehaviorTree>
 </root>
 )";

int main()
{    
    BehaviorTreeFactory factory;

    // Register custom action nodes with the factory
    factory.registerNodeType<FindBall>("FindBall");
    factory.registerNodeType<PickBall>("PickBall");
    factory.registerNodeType<PlaceBall>("PlaceBall");

    // Register custom condition nodes with the factory
    factory.registerSimpleCondition("BallClose", std::bind(BallClose));
    factory.registerSimpleCondition("BallGrasped", std::bind(BallGrasped));

    // Create an instance of GripperInterface
    GripperInterface gripper;

    // Register a simple action node using the GripperInterface instance
    factory.registerSimpleAction("GraspBall", std::bind(&GripperInterface::close, &gripper));

    // Create the behavior tree using the XML description
    auto tree = factory.createTreeFromText(xml_text);

    // Run the behavior tree until it finishes
    tree.tickWhileRunning();

    return 0;
}