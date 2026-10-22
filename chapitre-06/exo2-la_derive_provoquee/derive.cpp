#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkEventDispatcher.h"
#include "NKTime/NkChrono.h"

#include <cstdio>

using namespace nkentseu;

int nkmain(const NkEntryState &) {
    constexpr float32 sensitivity = 0.0025f;

    NkWindowConfig config;
    config.title = "Exercice 2 - Derive provoquee";
    config.width = 960;
    config.height = 540;

    NkWindow window;
    if (!window.Create(config))
        return 1;

    NkChrono clock;
    auto &events = NkEvents();
    bool running = true;
    int32 lastReport = 0;
    float32 yaw = 0.f;

    std::printf("temps_ms,brut_x,lacet_deg\n");
    while (running) {
        NkInput.NewFrame();
        NkEvent *event = nullptr;
        while (events.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>())
                running = false;
        }

        if (!running)
            break;

        const int32 rawX = NkInput.MouseRawDeltaX();
        yaw += static_cast<float32>(rawX) * sensitivity;
        const int32 elapsedMs = static_cast<int32>(clock.Elapsed().ToMilliseconds());
        if (elapsedMs - lastReport >= 1000) {
            lastReport = elapsedMs;
            std::printf("%d,%d,%.3f\n", elapsedMs, rawX,
                        yaw * 180.f / 3.14159265358979323846f);
        }
    }

    return 0;
}
