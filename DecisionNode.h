#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

class DecisionNode
{
public:
    using Action = std::function<void()>;

private:
    std::string name;
    std::string choiceText;
    DecisionNode *parent;
    std::vector<std::unique_ptr<DecisionNode>> children;
    Action payload;

public:
    DecisionNode(const std::string &nodeName,
                 const std::string &choice = "Start",
                 DecisionNode *parentNode = nullptr,
                 Action actionPayload = nullptr);

    DecisionNode *addChild(const std::string &childName,
                           const std::string &choice,
                           Action actionPayload = nullptr);

    void executePayload() const;

    // Getters
    std::string getName() const;
    DecisionNode *getParent() const;
    const std::vector<std::unique_ptr<DecisionNode>> &getChildren() const;

    // UI Display
    void displayMenu() const;
};