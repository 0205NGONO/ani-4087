#include "NKWindow/NKMain.h"
#include "NKMath/NkQuat.h"

#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::math;

namespace {

void PrintLook(const char *label, const NkQuatf &orientation) {
    const NkVec3f look = orientation * NkVec3f(0.f, 0.f, -1.f);
    std::printf("%s : (%.4f, %.4f, %.4f)\n", label, look.x, look.y, look.z);
}

} // namespace

int nkmain(const NkEntryState &) {
    const NkAngle yaw(45.f);
    const NkAngle pitch(30.f);
    const NkQuatf qYaw = NkQuatf::RotateY(yaw);
    const NkQuatf qPitch = NkQuatf::RotateX(pitch);

    PrintLook("lacet*tangage", (qYaw * qPitch).Normalized());
    PrintLook("tangage*lacet", (qPitch * qYaw).Normalized());

    const NkMat4f rotation = NkMat4f::RotationY(yaw) * NkMat4f::RotationX(pitch);
    PrintLook("matrices", NkQuatf(rotation).Normalized());
    return 0;
}
