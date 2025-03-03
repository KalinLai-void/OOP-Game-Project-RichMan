#ifndef SINGLETON_MANAGER_HPP
#define SINGLETON_MANAGER_HPP

#include <functional>
#include <vector>

class SingletonManager {
private:
    static std::vector<std::function<void()>> destructors;

public:
    static void registerDestructor(std::function<void()> destructor);
    static void destroyAll();
};

#endif // SINGLETON_MANAGER_HPP
