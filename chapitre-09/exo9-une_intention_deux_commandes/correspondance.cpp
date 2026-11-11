#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Action {
    std::string name;
    std::string usage;
};

struct TableResult {
    std::vector<std::string> imageLines;
};

TableResult runTable(int tableNumber, const std::vector<Action>& actions) {
    int bindingCount = 0;
    std::cin >> bindingCount;
    std::unordered_map<std::string, std::string> bindings;
    for (int i = 0; i < bindingCount; ++i) {
        std::string usage;
        std::string command;
        std::cin >> usage >> command;
        bindings[usage] = command;
    }

    std::cout << "TABLE " << tableNumber << '\n';
    for (const Action& action : actions) {
        if (bindings.count(action.usage) == 0) {
            std::cout << "INACTIVE " << action.name << '\n';
        }
    }

    int frameCount = 0;
    std::cin >> frameCount;
    std::vector<bool> previous(actions.size(), false);
    TableResult result;

    for (int frame = 1; frame <= frameCount; ++frame) {
        int heldCount = 0;
        std::cin >> heldCount;
        std::unordered_set<std::string> held;
        for (int i = 0; i < heldCount; ++i) {
            std::string command;
            std::cin >> command;
            held.insert(command);
        }

        std::vector<bool> current(actions.size(), false);
        std::vector<std::string> pressed;
        for (std::size_t i = 0; i < actions.size(); ++i) {
            const auto binding = bindings.find(actions[i].usage);
            if (binding != bindings.end()) {
                current[i] = held.count(binding->second) != 0;
            }
            if (current[i] && !previous[i]) {
                pressed.push_back(actions[i].name);
            }
        }

        std::string line = std::to_string(frame);
        if (pressed.empty()) {
            line += " -";
        } else {
            for (const std::string& name : pressed) {
                line += ' ' + name;
            }
        }
        result.imageLines.push_back(line);
        std::cout << line << '\n';
        previous = std::move(current);
    }
    return result;
}

int main() {
    int actionCount = 0;
    if (!(std::cin >> actionCount)) {
        return 0;
    }

    std::vector<Action> actions;
    for (int i = 0; i < actionCount; ++i) {
        Action action;
        std::cin >> action.name >> action.usage;
        actions.push_back(std::move(action));
    }

    const TableResult first = runTable(1, actions);
    const TableResult second = runTable(2, actions);
    std::cout << "IDENTIQUES "
              << (first.imageLines == second.imageLines ? "OUI" : "NON")
              << '\n';
}
