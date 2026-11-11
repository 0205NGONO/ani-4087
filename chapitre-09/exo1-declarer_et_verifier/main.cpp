#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Action {
    std::string name;
    std::string type;
};

int usageNumber(const std::string& usage) {
    static const std::vector<std::string> usages{
        "SELECT", "GRAB", "MENU", "MOVE", "AIM_POSE", "GRIP_POSE",
        "HAPTIC", "SELECT_LEFT", "GRAB_LEFT", "AIM_POSE_LEFT",
        "GRIP_POSE_LEFT", "HAPTIC_LEFT", "MOVE_RIGHT"};
    for (std::size_t i = 0; i < usages.size(); ++i) {
        if (usages[i] == usage) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int main() {
    int orderCount = 0;
    if (!(std::cin >> orderCount)) {
        return 0;
    }

    std::vector<Action> actions;
    std::unordered_set<std::string> names;
    bool attached = false;
    std::size_t attachedCount = 0;
    std::size_t duplicateNames = 0;

    for (int i = 0; i < orderCount; ++i) {
        std::string command;
        std::cin >> command;

        if (command == "cree") {
            std::string name;
            std::string type;
            std::string usage;
            std::cin >> name >> type >> usage;

            if (names.count(name) != 0) {
                ++duplicateNames;
            }
            names.insert(name);
            actions.push_back({name, type});
            std::cout << "ACTION " << actions.size() << ' ' << name << ' '
                      << type << ' ' << usageNumber(usage) << '\n';
        } else if (command == "attache") {
            if (attached) {
                std::cout << "ATTACHE REFUSE\n";
            } else {
                attached = true;
                attachedCount = actions.size();
                std::cout << "ATTACHE " << attachedCount << '\n';
            }
        } else if (command == "lit") {
            std::uint64_t handle = 0;
            std::cin >> handle;
            if (handle == 0 || handle > actions.size()) {
                std::cout << "LIT " << handle << " INVALIDE\n";
            } else if (!attached || handle > attachedCount) {
                std::cout << "LIT " << handle << " INCONNUE\n";
            } else {
                std::cout << "LIT " << handle << ' '
                          << actions[static_cast<std::size_t>(handle - 1)].name
                          << '\n';
            }
        }
    }

    std::cout << "ACTIONS " << actions.size() << '\n'
              << "ATTACHEES " << (attached ? attachedCount : 0) << '\n'
              << "NOMS EN DOUBLE " << duplicateNames << '\n';
}
