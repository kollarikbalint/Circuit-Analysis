#include "Node.h"
#include <stdexcept>
#include <memory>
#include <vector>

void Node::addConnection(const std::shared_ptr<Component>& comp) {
    connections.push_back(comp);
}

void Node::removeConnection(const std::shared_ptr<Component>& comp) {
    connections.erase(std::remove_if(connections.begin(), connections.end(),
        [&comp](const std::weak_ptr<Component>& c) { return c.lock() == comp; }), connections.end());
}

std::vector<std::shared_ptr<Component>> Node::getConnections() const {
    std::vector<std::shared_ptr<Component>> connectedComponents;
    for (const auto& weakComp : connections) {
        if (auto comp = weakComp.lock()) {
            connectedComponents.push_back(comp);
        }
    }
    return connectedComponents;
}

