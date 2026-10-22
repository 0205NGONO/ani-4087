#include "NKWindow/NKMain.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKMath/NkMat.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace nkentseu;
using namespace nkentseu::math;
using namespace nkentseu::renderer;

namespace {

NkMat4f Projection(float32 left, float32 right, float32 up, float32 down,
                   float32 nearPlane, float32 farPlane) {
    const float32 tanLeft = NkTan(left);
    const float32 tanRight = NkTan(right);
    const float32 tanUp = NkTan(up);
    const float32 tanDown = NkTan(down);
    const float32 tanWidth = tanRight - tanLeft;
    const float32 tanHeight = tanUp - tanDown;

    NkMat4f result;
    result.mat[0][0] = 2.f / tanWidth;
    result.mat[1][1] = 2.f / tanHeight;
    result.mat[2][0] = (tanRight + tanLeft) / tanWidth;
    result.mat[2][1] = (tanUp + tanDown) / tanHeight;
    result.mat[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
    result.mat[2][3] = -1.f;
    result.mat[3][2] = 2.f * farPlane * nearPlane / (nearPlane - farPlane);
    return result;
}

void Compare(const char *label, const NkMat4f &manual, const NkMat4f &engine) {
    float32 maximum = 0.f;
    for (int row = 0; row < 4; ++row) {
        float32 errors[4];
        for (int column = 0; column < 4; ++column) {
            errors[column] = std::fabs(manual.mat[column][row] - engine.mat[column][row]);
            maximum = std::max(maximum, errors[column]);
        }
        std::printf("%s, ligne %d: %.9g %.9g %.9g %.9g\n",
                    label, row, errors[0], errors[1], errors[2], errors[3]);
    }
    std::printf("%s : plus grand ecart %.9g\n", label, maximum);
}

} // namespace

int nkmain(const NkEntryState &) {
    constexpr float32 nearPlane = 0.05f;
    constexpr float32 farPlane = 100.f;
    constexpr float32 aspect = 1280.f / 720.f;
    constexpr float32 halfVerticalRad = 70.f * 3.14159265358979323846f / 360.f;
    const float32 halfHorizontalRad = std::atan(aspect * std::tan(halfVerticalRad));

    const NkMat4f symmetric = Projection(-halfHorizontalRad, halfHorizontalRad,
                                         halfVerticalRad, -halfVerticalRad,
                                         nearPlane, farPlane);
    const NkMat4f symmetricEngine = NkMat4f::Perspective(
        NkAngle(70.f), aspect, nearPlane, farPlane);
    Compare("symetrique", symmetric, symmetricEngine);

    constexpr float32 degreesToRadians = 3.14159265358979323846f / 180.f;
    constexpr float32 left = -52.f * degreesToRadians;
    constexpr float32 right = 45.f * degreesToRadians;
    constexpr float32 up = 48.f * degreesToRadians;
    constexpr float32 down = -50.f * degreesToRadians;
    const NkMat4f quest = Projection(left, right, up, down, nearPlane, farPlane);
    NkCamera3D camera;
    camera.SetNearFar(nearPlane, farPlane);
    camera.SetFovAsym(left, right, up, down);
    Compare("quest", quest, camera.GetProj());
    return 0;
}
