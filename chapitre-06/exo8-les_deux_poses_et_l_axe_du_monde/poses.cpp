#include "NKWindow/NKMain.h"
#include "NKMath/NkQuat.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace nkentseu;
using namespace nkentseu::math;

namespace {

constexpr float32 kIpd = 0.063f;
const NkVec3f kHeadPosition = {0.30f, 1.70f, 1.20f};
const NkVec3f kWorldRight = {1.f, 0.f, 0.f};

struct EyePair {
    NkVec3f left;
    NkVec3f right;
};

EyePair EyesAlongHead(const NkQuatf &orientation) {
    const NkVec3f headRight = orientation * kWorldRight;
    const float32 halfIpd = kIpd * 0.5f;
    return {kHeadPosition - headRight * halfIpd,
            kHeadPosition + headRight * halfIpd};
}

EyePair EyesAlongWorld() {
    const float32 halfIpd = kIpd * 0.5f;
    return {kHeadPosition - kWorldRight * halfIpd,
            kHeadPosition + kWorldRight * halfIpd};
}

float32 Distance(const NkVec3f &a, const NkVec3f &b) {
    const NkVec3f d = a - b;
    return std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
}

void PrintPair(const char *label, const EyePair &pair) {
    std::printf("%s : G(%.4f,%.4f,%.4f) D(%.4f,%.4f,%.4f)\n",
                label, pair.left.x, pair.left.y, pair.left.z,
                pair.right.x, pair.right.y, pair.right.z);
}

void PrintPose(const char *label, const NkQuatf &orientation) {
    const EyePair head = EyesAlongHead(orientation);
    const EyePair world = EyesAlongWorld();
    PrintPair((std::string(label) + ", tete").c_str(), head);
    PrintPair((std::string(label) + ", monde").c_str(), world);
    std::printf("%s : ecart %.1f mm\n", label,
                Distance(head.left, world.left) * 1000.f);
}

} // namespace

int nkmain(const NkEntryState &) {
    PrintPose("neutre", NkQuatf());
    PrintPose("roulis 30", NkQuatf::RotateZ(NkAngle(30.f)));
    PrintPose("lacet 90", NkQuatf::RotateY(NkAngle(90.f)));
    return 0;
}
