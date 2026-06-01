#pragma once

#include "DecisionNode.h"
#include <memory>

class TreeManager
{
private:
    std::unique_ptr<DecisionNode> root;
    DecisionNode *currentNode;
    bool isRunning;

    void handleInput();

public:
    TreeManager(std::unique_ptr<DecisionNode> rootNode);

    void start();
    void stop();
};