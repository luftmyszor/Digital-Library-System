#include "DecisionNode.h"
#include <iostream>

DecisionNode::DecisionNode(const std::string &nodeName,
                           const std::string &choice,
                           DecisionNode *parentNode,
                           Action actionPayload)
    : name(nodeName), choiceText(choice), parent(parentNode), payload(actionPayload) {}

DecisionNode *DecisionNode::addChild(const std::string &childName,
                                     const std::string &choice,
                                     Action actionPayload)
{
    children.push_back(std::make_unique<DecisionNode>(childName, choice, this, actionPayload));
    return children.back().get();
}

void DecisionNode::executePayload() const
{
    if (payload)
    {
        payload();
    }
}

std::string DecisionNode::getName() const
{
    return name;
}

DecisionNode *DecisionNode::getParent() const
{
    return parent;
}

const std::vector<std::unique_ptr<DecisionNode>> &DecisionNode::getChildren() const
{
    return children;
}

void DecisionNode::displayMenu() const
{
    std::cout << "\n========================================\n";
    std::cout << "  " << name << "\n";
    std::cout << "========================================\n";

    for (size_t i = 0; i < children.size(); ++i)
    {
        std::cout << "  [" << (i + 1) << "] " << children[i]->choiceText << "\n";
    }

    if (parent != nullptr)
    {
        std::cout << "  [0] Go Back\n";
    }
    std::cout << "  [Q] Quit\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Select an option: ";
}