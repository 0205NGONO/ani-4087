#include "NKWindow/NKMain.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKMath/NkQuat.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace nkentseu;
using namespace nkentseu::math;
using namespace nkentseu::renderer;

namespace {

NkQuatf Orientation(float32 yawDeg, float32 pitchDeg, bool wrongOrder) {
    const NkQuatf yaw = NkQuatf::RotateY(NkAngle(yawDeg));
    const NkQuatf pitch = NkQuatf::RotateX(NkAngle(pitchDeg));
    return (wrongOrder ? pitch * yaw : yaw * pitch).Normalized();
}

float32 HeightDegrees(const NkVec3f &look) {
    const float32 horizontal = std::sqrt(look.x * look.x + look.z * look.z);
    return std::atan2(look.y, horizontal) * 180.f / 3.14159265358979323846f;
}

void SetCameraPose(NkCamera3D &camera, const NkQuatf &orientation) {
    const NkVec3f position = {0.30f, 1.70f, 1.20f};
    camera.SetPosition(position);
    camera.SetTarget(position + orientation * NkVec3f(0.f, 0.f, -1.f));
    camera.SetUp(orientation * NkVec3f(0.f, 1.f, 0.f));
}

} // namespace

int nkmain(const NkEntryState &) {
    constexpr float32 yawAngles[] = {0.f, 45.f, 90.f, 135.f, 180.f};
    constexpr float32 pitchDeg = 30.f;

    for (bool wrong : {false, true}) {
        const char *order = wrong ? "tangage*lacet" : "lacet*tangage";
        for (float32 yaw : yawAngles) {
            const NkQuatf q = Orientation(yaw, pitchDeg, wrong);
            const NkVec3f look = q * NkVec3f(0.f, 0.f, -1.f);
            std::printf("%s, lacet %.0f : hauteur %.3f deg\n",
                        order, yaw, HeightDegrees(look));
        }
    }

    const char *yawEnv = std::getenv("MASALLE_LACET");
    const float32 yawDeg = yawEnv == nullptr ? 90.f : std::strtof(yawEnv, nullptr);
    const char *wrongEnv = std::getenv("MASALLE_ORDRE_FAUTIF");
    const bool wrongOrder = wrongEnv != nullptr && wrongEnv[0] == '1';
    NkCamera3D camera;
    SetCameraPose(camera, Orientation(yawDeg, pitchDeg, wrongOrder));
    return 0;
}
