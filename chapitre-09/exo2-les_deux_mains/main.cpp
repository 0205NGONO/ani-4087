#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Action {
    std::string name;
    std::string hand;
    std::string command;
};

Action describeAction(const std::string& name, const std::string& usage) {
    static const std::vector<std::string> leftUsages{
        "MENU", "MOVE", "SELECT_LEFT", "GRAB_LEFT", "AIM_POSE_LEFT",
        "GRIP_POSE_LEFT", "HAPTIC_LEFT"};

    std::string hand = "DROITE";
    if (std::find(leftUsages.begin(), leftUsages.end(), usage) !=
        leftUsages.end()) {
        hand = "GAUCHE";
    }

    std::string command = "AUCUNE";
    if (usage == "SELECT") {
        command = "SOURIS_G";
    } else if (usage == "GRAB") {
        command = "SOURIS_D";
    } else if (usage == "MENU") {
        command = "TAB";
    } else if (usage == "MOVE") {
        command = "FLECHES";
    } else if (usage == "SELECT_LEFT") {
        command = "SOURIS_M";
    } else if (usage == "GRAB_LEFT") {
        command = "MAJ_G";
    } else if (usage == "MOVE_RIGHT") {
        command = "IJKL";
    }
    return {name, hand, command};
}

int horizontalValue(const std::unordered_set<std::string>& held,
                    const std::string& negative,
                    const std::string& positive) {
    return static_cast<int>(held.count(positive) != 0) -
           static_cast<int>(held.count(negative) != 0);
}

int main() {
    int actionCount = 0;
    int frameCount = 0;
    if (!(std::cin >> actionCount >> frameCount)) {
        return 0;
    }

    std::vector<Action> actions;
    for (int i = 0; i < actionCount; ++i) {
        std::string name;
        std::string usage;
        std::cin >> name >> usage;
        actions.push_back(describeAction(name, usage));
        std::cout << name << ' ' << actions.back().hand << ' '
                  << (actions.back().command == "AUCUNE"
                          ? "AUCUNE"
                          : actions.back().command)
                  << '\n';
    }

    int rightCount = 0;
    int leftCount = 0;
    for (const Action& action : actions) {
        if (action.hand == "DROITE") {
            ++rightCount;
        } else {
            ++leftCount;
        }
    }
    int headFrames = 0;
    for (int frame = 1; frame <= frameCount; ++frame) {
        int heldCount = 0;
        std::cin >> heldCount;
        std::unordered_set<std::string> held;
        for (int i = 0; i < heldCount; ++i) {
            std::string key;
            std::cin >> key;
            held.insert(key);
        }

        const int arrowsX =
            horizontalValue(held, "F_GAUCHE", "F_DROITE");
        const int arrowsY =
            horizontalValue(held, "F_BAS", "F_HAUT");
        const int ijklX = horizontalValue(held, "J", "L");
        const int ijklY = horizontalValue(held, "K", "I");
        const bool headMoved = held.count("Z") || held.count("Q") ||
                               held.count("S") || held.count("D") ||
                               held.count("W") || held.count("A");
        headFrames += static_cast<int>(headMoved);

        std::cout << frame;
        for (const Action& action : actions) {
            std::cout << ' ';
            if (action.command == "AUCUNE") {
                std::cout << '-';
            } else if (action.command == "FLECHES") {
                std::cout << arrowsX << ',' << arrowsY;
            } else if (action.command == "IJKL") {
                std::cout << ijklX << ',' << ijklY;
            } else {
                std::cout << static_cast<int>(
                    held.count(action.command) != 0);
            }
        }
        std::cout << '\n';
    }

    std::cout << "DROITE " << rightCount << '\n'
              << "GAUCHE " << leftCount << '\n'
              << "TETE " << headFrames << '\n';
}
