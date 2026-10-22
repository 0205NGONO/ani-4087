#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKEvent/NkMouseEvent.h"
#include "NKEvent/NkEventDispatcher.h"
#include "NKTime/NkChrono.h"

#include <cstdio>

using namespace nkentseu;

int nkmain(const NkEntryState &) {
    constexpr float32 sensitivity = 0.0025f;

    // Symptome : après l'arrêt, la tête continue de tourner.
    // Cause : le dernier delta brut reste lisible dans le module d'entrées.
    // Preuve : la version fautive l'ajoute encore à chaque image immobile.
    // Correction : accumuler les événements bruts, lire une fois, puis vider.

    NkWindowConfig config;
    config.title = "Exercice 3 - Accumuler puis vider";
    config.width = 960;
    config.height = 540;

    NkWindow window;
    if (!window.Create(config))
        return 1;

    NkChrono clock;
    auto &events = NkEvents();
    bool running = true;
    int32 pendingX = 0;
    int32 neverDrainedX = 0;
    int32 lastReport = 0;
    float32 faultyYaw = 0.f;
    float32 drainedYaw = 0.f;
    float32 neverDrainedYaw = 0.f;

    std::printf("t = 0.0 s, lu = 0.0, vide = 0.0, sans vidage = 0.0\n");
    while (running) {
        NkInput.NewFrame();
        NkEvent *event = nullptr;
        while (events.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>()) {
                running = false;
            } else if (const auto *raw = event->As<NkMouseRawEvent>()) {
                pendingX += raw->GetDeltaX();
                neverDrainedX += raw->GetDeltaX();
            }
        }

        if (!running)
            break;

        faultyYaw += static_cast<float32>(NkInput.MouseRawDeltaX()) * sensitivity;
        drainedYaw += static_cast<float32>(pendingX) * sensitivity;
        neverDrainedYaw += static_cast<float32>(neverDrainedX) * sensitivity;
        pendingX = 0;

        const int32 elapsedMs = static_cast<int32>(clock.Elapsed().ToMilliseconds());
        if (elapsedMs - lastReport >= 1000) {
            lastReport = elapsedMs;
            const float32 toDegrees = 180.f / 3.14159265358979323846f;
            std::printf("t = %.1f s, lu = %.1f, vide = %.1f, sans vidage = %.1f\n",
                        elapsedMs / 1000.f, faultyYaw * toDegrees,
                        drainedYaw * toDegrees, neverDrainedYaw * toDegrees);
        }
    }

    return 0;
}
