#include "NKWindow/NKMain.h"
#include "NKMath/NKMath.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace nkentseu;
using namespace nkentseu::math;

namespace {

struct Head {
    float32 yaw = 0.f;
    float32 pitch = 0.f;
    NkVec3f position = {0.f, 0.f, 0.f};
};

Head gHead;
bool gInvertYaw = false;

void DebugSetHead(float32 yawRad, float32 pitchRad, const NkVec3f &position) {
    gHead = {yawRad, pitchRad, position};
}

NkVec3f Regard() {
    const float32 yaw = gInvertYaw ? -gHead.yaw : gHead.yaw;
    const float32 cp = std::cos(gHead.pitch);
    return {-std::sin(yaw) * cp, std::sin(gHead.pitch), -std::cos(yaw) * cp};
}

bool Near(float32 a, float32 b) {
    return std::fabs(a - b) <= 1e-4f;
}

bool Check(const char *label, float32 yaw, float32 pitch, const NkVec3f &expected) {
    DebugSetHead(yaw, pitch, {0.f, 1.7f, 0.f});
    const NkVec3f got = Regard();
    const bool ok = Near(got.x, expected.x) && Near(got.y, expected.y) && Near(got.z, expected.z);
    std::printf("%s : attendu (%.4f, %.4f, %.4f), obtenu (%.4f, %.4f, %.4f), %s\n",
                label, expected.x, expected.y, expected.z, got.x, got.y, got.z,
                ok ? "OK" : "ECHEC");
    return ok;
}

int RunTests(bool invertYaw) {
    gInvertYaw = invertYaw;
    const float32 pi = 3.14159265358979323846f;
    bool ok = true;
    ok = Check("lacet 90, tangage 0", pi * 0.5f, 0.f, {-1.f, 0.f, 0.f}) && ok;
    ok = Check("lacet 0, tangage 0", 0.f, 0.f, {0.f, 0.f, -1.f}) && ok;
    ok = Check("lacet 0, tangage 30", 0.f, pi / 6.f, {0.f, 0.5f, -0.8660254f}) && ok;
    return ok ? 0 : 1;
}

} // namespace

int nkmain(const NkEntryState &) {
    const char *test = std::getenv("MASALLE_TEST");
    if (test != nullptr && std::strcmp(test, "1") == 0) {
        const char *invert = std::getenv("MASALLE_INVERT_YAW");
        return RunTests(invert != nullptr && std::strcmp(invert, "1") == 0);
    }
    return 0;
}
