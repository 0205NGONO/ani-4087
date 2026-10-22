#include "NKWindow/NKMain.h"
#include "NKMath/NkQuat.h"
#include "NKTime/NkChrono.h"

#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::math;

namespace {

constexpr uint32 kIterations = 1'000'000;

float64 BenchmarkMatrices() {
    NkChrono clock;
    float64 checksum = 0.0;
    for (uint32 i = 0; i < kIterations; ++i) {
        const float32 yawDeg = static_cast<float32>((i * 37u) % 360u);
        const float32 pitchDeg = static_cast<float32>((i * 19u) % 160u) - 80.f;
        const NkMat4f rotation = NkMat4f::RotationY(NkAngle(yawDeg)) *
                                 NkMat4f::RotationX(NkAngle(pitchDeg));
        const NkQuatf q = NkQuatf(rotation).Normalized();
        checksum += q.w;
    }
    const float64 elapsed = clock.Elapsed().ToMilliseconds();
    std::printf("checksum matrices : %.9f\n", checksum);
    return elapsed;
}

float64 BenchmarkQuaternions() {
    NkChrono clock;
    float64 checksum = 0.0;
    for (uint32 i = 0; i < kIterations; ++i) {
        const float32 yawDeg = static_cast<float32>((i * 37u) % 360u);
        const float32 pitchDeg = static_cast<float32>((i * 19u) % 160u) - 80.f;
        const NkQuatf yaw = NkQuatf::RotateY(NkAngle(yawDeg));
        const NkQuatf pitch = NkQuatf::RotateX(NkAngle(pitchDeg));
        const NkQuatf q = (yaw * pitch).Normalized();
        checksum += q.w;
    }
    const float64 elapsed = clock.Elapsed().ToMilliseconds();
    std::printf("checksum quaternions : %.9f\n", checksum);
    return elapsed;
}

void Measure(const char *configuration) {
    for (int run = 0; run < 3; ++run) {
        const float64 matrixMs = BenchmarkMatrices();
        std::printf("%s, matrices : %.3f ms\n", configuration, matrixMs);
        const float64 quaternionMs = BenchmarkQuaternions();
        std::printf("%s, quaternions : %.3f ms\n", configuration, quaternionMs);
    }
}

} // namespace

int nkmain(const NkEntryState &) {
    const NkQuatf byMatrix(
        NkMat4f::RotationY(NkAngle(45.f)) * NkMat4f::RotationX(NkAngle(30.f)));
    const NkQuatf direct =
        (NkQuatf::RotateY(NkAngle(45.f)) * NkQuatf::RotateX(NkAngle(30.f))).Normalized();
    const bool sameQuaternion =
        std::fabs(byMatrix.x - direct.x) < 1e-5f &&
        std::fabs(byMatrix.y - direct.y) < 1e-5f &&
        std::fabs(byMatrix.z - direct.z) < 1e-5f &&
        std::fabs(byMatrix.w - direct.w) < 1e-5f;
    const NkVec3f matrixLook = byMatrix.Normalized() * NkVec3f(0.f, 0.f, -1.f);
    const NkVec3f directLook = direct * NkVec3f(0.f, 0.f, -1.f);
    std::printf("verification quaternion : %s\n", sameQuaternion ? "OK" : "ECHEC");
    std::printf("verification regard : matrix (%.4f, %.4f, %.4f), quaternion (%.4f, %.4f, %.4f)\n",
                matrixLook.x, matrixLook.y, matrixLook.z,
                directLook.x, directLook.y, directLook.z);
    if (!sameQuaternion)
        return 1;
#if defined(NDEBUG) || defined(__OPTIMIZE__)
    Measure("Release");
#else
    Measure("Debug");
#endif
    return 0;
}
