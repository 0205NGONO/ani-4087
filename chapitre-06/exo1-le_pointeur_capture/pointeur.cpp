#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkKeyboardEvent.h"
#include "NKEvent/NkWindowEvent.h"

using namespace nkentseu;

int nkmain(const NkEntryState &) {
    NkWindowConfig config;
    config.title = "Exercice 1 - Pointeur capture";
    config.width = 960;
    config.height = 540;

    NkWindow window;
    if (!window.Create(config))
        return 1;

    window.ShowMouse(false);
    window.CaptureMouse(true);
    window.ClipMouseToClient(true);

    bool captured = true;
    bool running = true;
    auto &events = NkEvents();

    while (running) {
        NkEvent *event = nullptr;
        while (events.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>()) {
                running = false;
                continue;
            }

            const auto *key = event->As<NkKeyPressEvent>();
            if (key == nullptr)
                continue;

            if (key->GetKey() == NkKey::NK_ESCAPE && captured) {
                window.ShowMouse(true);
                window.CaptureMouse(false);
                window.ClipMouseToClient(false);
                captured = false;
            } else if (key->GetKey() == NkKey::NK_F1) {
                window.ShowMouse(false);
                window.CaptureMouse(true);
                window.ClipMouseToClient(true);
                captured = true;
            }
        }
    }

    window.CaptureMouse(false);
    window.ClipMouseToClient(false);
    if (captured)
        window.ShowMouse(true);
    return 0;
}
