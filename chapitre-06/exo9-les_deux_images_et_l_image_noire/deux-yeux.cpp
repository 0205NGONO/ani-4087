#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"
#include "NKEvent/NkEventSystem.h"
#include "NKEvent/NkWindowEvent.h"
#include "NKRHI/Core/NkDeviceFactory.h"
#include "NKRHI/Core/NkGraphicsApi.h"
#include "NKRenderer/NkRenderer.h"
#include "NKRenderer/Core/NkCamera.h"
#include "NKRenderer/Core/NkRenderGraph.h"
#include "NKRenderer/Core/NkTextureLibrary.h"
#include "NKRenderer/Materials/NkMaterialCollection.h"
#include "NKRenderer/Mesh/NkMeshSystem.h"
#include "NKRenderer/Tools/Offscreen/NkOffscreenTarget.h"
#include "NKRenderer/Tools/Render2D/NkRender2D.h"
#include "NKRenderer/Tools/Render3D/NkRender3D.h"
#include "NKImage/Core/NkImage.h"
#include "NKMath/NkQuat.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace nkentseu;
using namespace nkentseu::math;
using namespace nkentseu::renderer;

namespace {

constexpr uint32 kEyeWidth = 640;
constexpr uint32 kEyeHeight = 720;
constexpr float32 kIpd = 0.063f;
const NkVec3f kHeadPosition = {0.30f, 1.70f, 1.20f};
const NkQuatf kHeadOrientation;

void Pave(NkRender3D *r3d, NkMeshSystem *meshes, NkVec3f center, NkVec3f size,
          NkVec3f tint, float32 roughness = 1.f) {
    NkDrawCall3D draw;
    draw.mesh = meshes->GetCube();
    draw.transform = NkMat4f::Translate(center) * NkMat4f::Scale(size);
    draw.aabb = {{center.x - size.x * 0.5f, center.y - size.y * 0.5f, center.z - size.z * 0.5f},
                 {center.x + size.x * 0.5f, center.y + size.y * 0.5f, center.z + size.z * 0.5f}};
    draw.tint = tint;
    draw.metallic = 0.f;
    draw.roughness = roughness;
    r3d->Submit(draw);
}

void SubmitRoom(NkRender3D *r3d, NkMeshSystem *meshes) {
    constexpr float32 half = 2.f;
    constexpr float32 wall = 0.1f;
    constexpr float32 ceiling = 2.5f;
    const NkVec3f wallColor = {0.72f, 0.70f, 0.66f};
    const NkVec3f wood = {0.55f, 0.38f, 0.22f};

    NkDrawCall3D floor;
    floor.mesh = meshes->GetPlane();
    floor.transform = NkMat4f::Scale({4.f, 1.f, 4.f});
    floor.aabb = {{-half, 0.f, -half}, {half, 0.f, half}};
    floor.castShadow = false;
    floor.tint = {0.38f, 0.36f, 0.34f};
    floor.roughness = 1.f;
    r3d->Submit(floor);

    Pave(r3d, meshes, {0.f, ceiling * 0.5f, -half}, {4.f, ceiling, wall}, wallColor);
    Pave(r3d, meshes, {0.f, ceiling * 0.5f, half}, {4.f, ceiling, wall}, wallColor);
    Pave(r3d, meshes, {-half, ceiling * 0.5f, 0.f}, {wall, ceiling, 4.f}, wallColor);
    Pave(r3d, meshes, {half, ceiling * 0.5f, 0.f}, {wall, ceiling, 4.f}, wallColor);
    Pave(r3d, meshes, {0.f, ceiling + wall * 0.5f, 0.f}, {4.f, wall, 4.f}, wallColor);

    Pave(r3d, meshes, {-0.70f, 1.f, -1.98f}, {0.90f, 2.f, 0.04f}, wood, 0.6f);
    Pave(r3d, meshes, {-1.98f, 1.40f, -0.90f}, {0.04f, 1.f, 1.20f}, {0.75f, 0.85f, 0.92f}, 0.15f);
    Pave(r3d, meshes, {-1.98f, 1.40f, 0.90f}, {0.04f, 1.f, 1.20f}, {0.75f, 0.85f, 0.92f}, 0.15f);
    Pave(r3d, meshes, {0.60f, 0.725f, 0.70f}, {1.20f, 0.05f, 0.80f}, wood, 0.5f);
    for (int i = 0; i < 4; ++i) {
        const float32 dx = ((i & 1) ? 1.f : -1.f) * 0.54f;
        const float32 dz = ((i & 2) ? 1.f : -1.f) * 0.34f;
        Pave(r3d, meshes, {0.60f + dx, 0.35f, 0.70f + dz}, {0.06f, 0.70f, 0.06f}, wood, 0.6f);
    }
    Pave(r3d, meshes, {-1.10f, 0.50f, 1.20f}, {1.f, 1.f, 1.f}, {0.85f, 0.80f, 0.30f}, 0.8f);
}

NkCamera3DData CameraForEye(bool rightEye, bool forgottenOffset) {
    NkCamera3DData camera;
    const NkVec3f right = kHeadOrientation * NkVec3f(1.f, 0.f, 0.f);
    const float32 side = forgottenOffset ? 0.f : (rightEye ? 0.5f : -0.5f) * kIpd;
    camera.position = kHeadPosition + right * side;
    camera.target = camera.position + kHeadOrientation * NkVec3f(0.f, 0.f, -1.f);
    camera.up = kHeadOrientation * NkVec3f(0.f, 1.f, 0.f);
    camera.fovY = 70.f;
    camera.aspect = static_cast<float32>(kEyeWidth) / static_cast<float32>(kEyeHeight);
    camera.nearPlane = 0.05f;
    camera.farPlane = 100.f;
    return camera;
}

void RenderEye(NkRenderer *renderer, NkICommandBuffer *cmd,
               const NkCamera3DData &cameraData, float32 time) {
    renderer->FlushGraphRebuilds();
    NkRender3D *r3d = renderer->GetRender3D();
    r3d->ResetFrame();
    if (NkMaterialCollection *materials = renderer->GetMaterialCollection())
        materials->Upload();

    NkSceneContext scene;
    scene.camera = NkCamera3D(cameraData);
    scene.time = time;
    NkLightDesc sun;
    sun.type = NkLightType::NK_DIRECTIONAL;
    sun.direction = {-0.5f, -1.f, -0.35f};
    sun.color = {1.f, 0.96f, 0.88f};
    sun.intensity = 3.f;
    sun.castShadow = false;
    scene.lights.PushBack(sun);

    r3d->SetIBLStrength(0.35f);
    r3d->BeginScene(scene);
    SubmitRoom(r3d, renderer->GetMeshSystem());
    if (NkRenderGraph *graph = renderer->GetRenderGraph())
        graph->Execute(cmd);
}

bool DrawPair(NkRenderer *screen, NkICommandBuffer *cmd,
              NkOffscreenTarget *left, NkOffscreenTarget *right,
              bool forgottenOffset) {
    NkRender2D *r2d = screen->GetRender2D();
    if (r2d == nullptr)
        return false;
    r2d->Begin(cmd, kEyeWidth * 2, kEyeHeight);
    r2d->DrawImage(left->GetColorResult(), {0.f, 0.f, static_cast<float32>(kEyeWidth), static_cast<float32>(kEyeHeight)});
    r2d->DrawImage(right->GetColorResult(), {static_cast<float32>(kEyeWidth), 0.f,
                                             static_cast<float32>(kEyeWidth), static_cast<float32>(kEyeHeight)});
    r2d->End();
    return true;
}

struct DifferenceStats {
    uint32 changedPixels = 0;
    uint8 maxDifference = 0;
    bool hasVisualVariation = false;
};

bool HasVariation(const NkImage &image) {
    const NkColor first = image.GetPixel(0, 0);
    for (int32 y = 0; y < image.Height(); ++y) {
        for (int32 x = 0; x < image.Width(); ++x) {
            const NkColor pixel = image.GetPixel(x, y);
            if (pixel.r != first.r || pixel.g != first.g || pixel.b != first.b)
                return true;
        }
    }
    return false;
}

bool CompareImages(const char *leftPath, const char *rightPath,
                   const char *differencePath, DifferenceStats &stats) {
    NkImage left;
    NkImage right;
    if (!left.Load(leftPath, 4) || !right.Load(rightPath, 4)) {
        std::fprintf(stderr, "Impossible de charger les captures %s et %s\n", leftPath, rightPath);
        return false;
    }
    if (left.Width() != static_cast<int32>(kEyeWidth) ||
        left.Height() != static_cast<int32>(kEyeHeight) ||
        right.Width() != left.Width() || right.Height() != left.Height()) {
        std::fprintf(stderr, "Les captures doivent faire 640 x 720 pixels\n");
        return false;
    }

    NkImage difference = NkImage::Create(kEyeWidth, kEyeHeight, 4);
    if (!difference.IsValid()) {
        std::fprintf(stderr, "Impossible de creer l'image difference\n");
        return false;
    }
    stats.hasVisualVariation = HasVariation(left) && HasVariation(right);
    for (int32 y = 0; y < left.Height(); ++y) {
        for (int32 x = 0; x < left.Width(); ++x) {
            const NkColor a = left.GetPixel(x, y);
            const NkColor b = right.GetPixel(x, y);
            const uint8 dr = static_cast<uint8>(std::abs(static_cast<int>(a.r) - static_cast<int>(b.r)));
            const uint8 dg = static_cast<uint8>(std::abs(static_cast<int>(a.g) - static_cast<int>(b.g)));
            const uint8 db = static_cast<uint8>(std::abs(static_cast<int>(a.b) - static_cast<int>(b.b)));
            const uint8 delta = std::max(dr, std::max(dg, db));
            if (delta > 1)
                ++stats.changedPixels;
            stats.maxDifference = std::max(stats.maxDifference, delta);
            difference.SetPixel(x, y, NkColor(
                static_cast<uint8>(std::min(255, 4 * static_cast<int>(dr))),
                static_cast<uint8>(std::min(255, 4 * static_cast<int>(dg))),
                static_cast<uint8>(std::min(255, 4 * static_cast<int>(db))), 255));
        }
    }
    if (!difference.SavePNG(differencePath)) {
        std::fprintf(stderr, "Impossible d'ecrire %s\n", differencePath);
        return false;
    }
    return true;
}

} // namespace

int nkmain(const NkEntryState &) {
    NkWindowConfig windowConfig;
    windowConfig.title = "Exercice 9 - deux yeux";
    windowConfig.width = kEyeWidth * 2;
    windowConfig.height = kEyeHeight;
    NkWindow window;
    if (!window.Create(windowConfig)) {
        std::fprintf(stderr, "Impossible de creer la fenetre\n");
        return 1;
    }

    NkDeviceInitInfo deviceInfo{};
    deviceInfo.surface = window.GetSurfaceDesc();
    deviceInfo.width = window.GetSize().x;
    deviceInfo.height = window.GetSize().y;
    deviceInfo.api = NkGraphicsApi::NK_GFX_API_OPENGL;
    NkIDevice *device = NkDeviceFactory::CreateForApi(deviceInfo.api, deviceInfo);
    if (device == nullptr) {
        std::fprintf(stderr, "Impossible de creer le device graphique\n");
        return 2;
    }

    NkRendererConfig screenConfig = NkRendererConfig::For2D(deviceInfo.api, deviceInfo.width, deviceInfo.height);
    NkRenderer *screen = NkRenderer::Create(device, screenConfig);
    if (screen == nullptr || !screen->Initialize()) {
        std::fprintf(stderr, "Impossible d'initialiser le renderer ecran\n");
        NkRenderer::Destroy(screen);
        NkDeviceFactory::Destroy(device);
        return 3;
    }

    NkOffscreenDesc targetDesc;
    targetDesc.width = kEyeWidth;
    targetDesc.height = kEyeHeight;
    targetDesc.hasDepth = true;
    targetDesc.hdr = false;
    targetDesc.colorFmt = NkGPUFormat::NK_RGBA8_UNORM;
    targetDesc.readable = true;
    targetDesc.readback = true;
    targetDesc.name = "Ex9_Eye";
    NkOffscreenTarget *leftTarget = screen->CreateOffscreen(targetDesc);
    targetDesc.name = "Ex9_EyeRight";
    NkOffscreenTarget *rightTarget = screen->CreateOffscreen(targetDesc);
    if (leftTarget == nullptr || rightTarget == nullptr) {
        std::fprintf(stderr, "Impossible de creer les deux cibles hors ecran\n");
        NkRenderer::Destroy(screen);
        NkDeviceFactory::Destroy(device);
        return 4;
    }

    NkRendererConfig sceneConfig = NkRendererConfig::ForGame(deviceInfo.api, kEyeWidth, kEyeHeight);
    sceneConfig.Disable(NK_SS_RENDER2D);
    sceneConfig.Disable(NK_SS_TEXT);
    sceneConfig.Disable(NK_SS_OVERLAY);
    sceneConfig.Enable(NK_SS_OFFSCREEN);
    NkRenderer *leftRenderer = NkRenderer::Create(device, sceneConfig);
    NkRenderer *rightRenderer = NkRenderer::Create(device, sceneConfig);
    if (leftRenderer == nullptr || rightRenderer == nullptr ||
        !leftRenderer->Initialize() || !rightRenderer->Initialize()) {
        std::fprintf(stderr, "Impossible d'initialiser les deux renderers de salle\n");
        NkRenderer::Destroy(rightRenderer);
        NkRenderer::Destroy(leftRenderer);
        NkRenderer::Destroy(screen);
        NkDeviceFactory::Destroy(device);
        return 5;
    }

    leftRenderer->SetFinalColorTarget(screen->GetTextures()->GetRHIHandle(leftTarget->GetColorResult()));
    leftRenderer->SetRenderSizeOverride(kEyeWidth, kEyeHeight);
    rightRenderer->SetFinalColorTarget(screen->GetTextures()->GetRHIHandle(rightTarget->GetColorResult()));
    rightRenderer->SetRenderSizeOverride(kEyeWidth, kEyeHeight);

    bool running = true;
    bool capturedEyes = false;
    bool capturedForgotten = false;
    uint32 frame = 0;
    auto &events = NkEvents();
    while (running && !capturedForgotten) {
        NkInput.NewFrame();
        NkEvent *event = nullptr;
        while (events.PollEvent(event)) {
            if (event->Is<NkWindowCloseEvent>())
                running = false;
        }
        if (!running)
            break;

        if (!screen->BeginFrame()) {
            std::fprintf(stderr, "BeginFrame a refuse l'image\n");
            running = false;
            break;
        }
        NkICommandBuffer *cmd = screen->GetCmd();
        RenderEye(leftRenderer, cmd, CameraForEye(false, capturedEyes), static_cast<float32>(frame) / 60.f);
        RenderEye(rightRenderer, cmd, CameraForEye(true, capturedEyes), static_cast<float32>(frame) / 60.f);
        if (!DrawPair(screen, cmd, leftTarget, rightTarget, capturedEyes)) {
            std::fprintf(stderr, "Le renderer 2D n'est pas disponible\n");
            running = false;
        }
        screen->Present();
        screen->EndFrame();
        ++frame;

        if (frame == 30 && !capturedEyes) {
            if (!leftTarget->Capture("oeil-gauche.png") || !rightTarget->Capture("oeil-droit.png")) {
                std::fprintf(stderr, "Echec de capture des deux yeux\n");
                running = false;
            } else {
                capturedEyes = true;
            }
        } else if (capturedEyes) {
            if (!leftTarget->Capture("oeil-gauche-vue-oubliee.png") ||
                !rightTarget->Capture("oeil-droit-vue-oubliee.png")) {
                std::fprintf(stderr, "Echec de capture de la vue sans decalage\n");
                running = false;
            } else {
                capturedForgotten = true;
            }
        }
    }

    int result = 0;
    if (!capturedEyes || !capturedForgotten) {
        std::fprintf(stderr, "La fenetre a ete fermee avant la capture des deux paires d'images\n");
        result = 6;
    } else {
        DifferenceStats stereoStats;
        DifferenceStats forgottenStats;
        if (!CompareImages("oeil-gauche.png", "oeil-droit.png", "difference.png", stereoStats) ||
            !CompareImages("oeil-gauche-vue-oubliee.png", "oeil-droit-vue-oubliee.png",
                           "difference-vue-oubliee.png", forgottenStats)) {
            result = 7;
        } else if (!stereoStats.hasVisualVariation || !forgottenStats.hasVisualVariation) {
            std::fprintf(stderr, "Une capture est uniforme; les differences ne constituent pas une mesure de scene valide\n");
            result = 8;
        } else {
            std::ofstream report("difference.txt", std::ios::trunc);
            if (!report) {
                std::fprintf(stderr, "Impossible d'ecrire difference.txt\n");
                result = 9;
            } else {
                const uint32 total = kEyeWidth * kEyeHeight;
                report << "deux yeux : " << stereoStats.changedPixels << " pixels sur " << total
                       << ", ecart max " << static_cast<unsigned>(stereoStats.maxDifference) << '\n';
                report << "vue oubliee : " << forgottenStats.changedPixels << " pixels sur " << total
                       << ", ecart max " << static_cast<unsigned>(forgottenStats.maxDifference) << '\n';
                if (!report) {
                    std::fprintf(stderr, "Echec d'ecriture de difference.txt\n");
                    result = 9;
                }
            }
        }
    }

    std::remove("oeil-gauche-vue-oubliee.png");
    std::remove("oeil-droit-vue-oubliee.png");
    NkRenderer::Destroy(rightRenderer);
    NkRenderer::Destroy(leftRenderer);
    NkRenderer::Destroy(screen);
    NkDeviceFactory::Destroy(device);
    return result;
}
