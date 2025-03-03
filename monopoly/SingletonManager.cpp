#include "SingletonManager.hpp"

std::vector<std::function<void()>> SingletonManager::destructors;

void SingletonManager::registerDestructor(std::function<void()> destructor) {
    destructors.push_back(destructor);
}

void SingletonManager::destroyAll() {
    for (auto& destructor : destructors) {
        destructor();
    }
    destructors.clear();
}