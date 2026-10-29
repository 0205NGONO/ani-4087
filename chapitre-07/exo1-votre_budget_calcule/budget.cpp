#include <cstdio>
#include <initializer_list>

int main() {
    for (double hz : {72.0, 90.0, 120.0}) {
        std::printf("budget %.0f Hz : %.3f ms\n", hz, 1000.0 / hz);
    }
}
