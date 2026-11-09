// Project Plumb VR — Milestone 3 foundation test app.
// Minimal native Quest 3 / Quest 3S Android ARM64 OpenXR app.
// No game code: creates an OpenXR session, renders a stereo scene
// (a coloured cube on a dark background, one per eye), follows head
// tracking, exits cleanly. Built with the OpenXR 1.1.65 loader.
#include <android/log.h>
#include "android_native_app_glue.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#define XR_USE_PLATFORM_ANDROID
#define XR_USE_GRAPHICS_API_OPENGL_ES
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <cmath>
#include <cstring>
#include <vector>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "PlumbVR", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "PlumbVR", __VA_ARGS__)
#define XR_CHECK(x, msg) do { XrResult r = (x); if (XR_FAILED(r)) { LOGE("%s failed: %d", msg, (int)r); return false; } } while (0)

// ---------- tiny mat4 helpers (column-major, OpenGL style) ----------
struct Mat4 { float m[16]; };
static Mat4 MatIdentity() { Mat4 o{}; o.m[0]=o.m[5]=o.m[10]=o.m[15]=1.f; return o; }
static Mat4 MatMul(const Mat4& a, const Mat4& b) {
    Mat4 o{};
    for (int c=0;c<4;c++) for (int r=0;r<4;r++) {
        o.m[c*4+r] = a.m[0*4+r]*b.m[c*4+0] + a.m[1*4+r]*b.m[c*4+1]
                   + a.m[2*4+r]*b.m[c*4+2] + a.m[3*4+r]*b.m[c*4+3];
    }
    return o;
}
static Mat4 MatPerspective(const XrFovf& f, float nearZ, float farZ) {
    float l = tanf(f.angleLeft)*nearZ, r = tanf(f.angleRight)*nearZ;
    float b = tanf(f.angleDown)*nearZ, t = tanf(f.angleUp)*nearZ;
    Mat4 o{};
    o.m[0]=2.f*nearZ/(r-l); o.m[5]=2.f*nearZ/(t-b);
    o.m[8]=(r+l)/(r-l); o.m[9]=(t+b)/(t-b);
    o.m[10]=-(farZ+nearZ)/(farZ-nearZ); o.m[11]=-1.f;
    o.m[14]=-2.f*farZ*nearZ/(farZ-nearZ);
    return o;
}
// Inverse of a rigid pose (rotation + translation), as a view matrix.
static Mat4 MatFromPoseInv(const XrPosef& p) {
    float x=p.orientation.x, y=p.orientation.y, z=p.orientation.z, w=p.orientation.w;
    float xx=x*x, yy=y*y, zz=z*z, xy=x*y, xz=x*z, yz=y*z, wx=w*x, wy=w*y, wz=w*z;
    // Rotation matrix R (column-major), then transpose it.
    Mat4 o{};
    o.m[0]=1-2*(yy+zz); o.m[4]=2*(xy-wz);    o.m[8]=2*(xz+wy);  o.m[12]=0;
    o.m[1]=2*(xy+wz);   o.m[5]=1-2*(xx+zz);  o.m[9]=2*(yz-wx);  o.m[13]=0;
    o.m[2]=2*(xz-wy);   o.m[6]=2*(yz+wx);    o.m[10]=1-2*(xx+yy); o.m[14]=0;
    o.m[3]=0; o.m[7]=0; o.m[11]=0; o.m[15]=1;
    // t' = -R^T * t
    float tx=p.position.x, ty=p.position.y, tz=p.position.z;
    o.m[12]=-(o.m[0]*tx+o.m[4]*ty+o.m[8]*tz);
    o.m[13]=-(o.m[1]*tx+o.m[5]*ty+o.m[9]*tz);
    o.m[14]=-(o.m[2]*tx+o.m[6]*ty+o.m[10]*tz);
    return o;
}
static Mat4 MatTranslate(float x, float y, float z) {
    Mat4 o = MatIdentity(); o.m[12]=x; o.m[13]=y; o.m[14]=z; return o;
}
static Mat4 MatRotateY(float a) {
    Mat4 o = MatIdentity(); float c=cosf(a), s=sinf(a);
    o.m[0]=c; o.m[2]=-s; o.m[8]=s; o.m[10]=c; return o;
}

// ---------- cube ----------
static const float kCubeVerts[] = {
    // pos            // rgb
    -0.5f,-0.5f,-0.5f, 1,0,0,   0.5f,-0.5f,-0.5f, 1,0,0,   0.5f, 0.5f,-0.5f, 1,0,0,
    -0.5f,-0.5f,-0.5f, 1,0,0,   0.5f, 0.5f,-0.5f, 1,0,0,  -0.5f, 0.5f,-0.5f, 1,0,0,
    -0.5f,-0.5f, 0.5f, 0,1,0,   0.5f,-0.5f, 0.5f, 0,1,0,   0.5f, 0.5f, 0.5f, 0,1,0,
    -0.5f,-0.5f, 0.5f, 0,1,0,   0.5f, 0.5f, 0.5f, 0,1,0,  -0.5f, 0.5f, 0.5f, 0,1,0,
    -0.5f,-0.5f,-0.5f, 0,0,1,  -0.5f, 0.5f,-0.5f, 0,0,1,  -0.5f, 0.5f, 0.5f, 0,0,1,
    -0.5f,-0.5f,-0.5f, 0,0,1,  -0.5f, 0.5f, 0.5f, 0,0,1,  -0.5f,-0.5f, 0.5f, 0,0,1,
     0.5f,-0.5f,-0.5f, 1,1,0,   0.5f, 0.5f,-0.5f, 1,1,0,   0.5f, 0.5f, 0.5f, 1,1,0,
     0.5f,-0.5f,-0.5f, 1,1,0,   0.5f, 0.5f, 0.5f, 1,1,0,   0.5f,-0.5f, 0.5f, 1,1,0,
    -0.5f, 0.5f,-0.5f, 1,0,1,   0.5f, 0.5f,-0.5f, 1,0,1,   0.5f, 0.5f, 0.5f, 1,0,1,
    -0.5f, 0.5f,-0.5f, 1,0,1,   0.5f, 0.5f, 0.5f, 1,0,1,  -0.5f, 0.5f, 0.5f, 1,0,1,
    -0.5f,-0.5f,-0.5f, 0,1,1,   0.5f,-0.5f,-0.5f, 0,1,1,   0.5f,-0.5f, 0.5f, 0,1,1,
    -0.5f,-0.5f,-0.5f, 0,1,1,   0.5f,-0.5f, 0.5f, 0,1,1,  -0.5f,-0.5f, 0.5f, 0,1,1,
};
static const char* kVertShader =
    "#version 300 es\nlayout(location=0) in vec3 aPos;\n"
    "layout(location=1) in vec3 aCol;\nuniform mat4 uMVP;\n"
    "out vec3 vCol;\nvoid main(){ gl_Position = uMVP * vec4(aPos,1.0); vCol = aCol; }\n";
static const char* kFragShader =
    "#version 300 es\nprecision mediump float;\nin vec3 vCol;\n"
    "out vec4 fragColor;\nvoid main(){ fragColor = vec4(vCol, 1.0); }\n";

static GLuint CompileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { char buf[512]; glGetShaderInfoLog(s, sizeof(buf), nullptr, buf); LOGE("shader compile: %s", buf); }
    return s;
}

struct App {
    android_app* app = nullptr;
    bool quit = false;
    bool windowReady = false;

    EGLDisplay eglDisplay = EGL_NO_DISPLAY;
    EGLContext eglContext = EGL_NO_CONTEXT;
    EGLConfig eglConfig = nullptr;

    XrInstance instance = XR_NULL_HANDLE;
    XrSystemId systemId = XR_NULL_SYSTEM_ID;
    XrSession session = XR_NULL_HANDLE;
    XrSpace space = XR_NULL_HANDLE;
    XrSwapchain swapchain = XR_NULL_HANDLE;
    XrSessionState sessionState = XR_SESSION_STATE_UNKNOWN;
    bool sessionRunning = false;
    bool xrReady = false;

    uint32_t viewWidth = 0, viewHeight = 0;
    std::vector<XrSwapchainImageOpenGLESKHR> swapImages;
    std::vector<GLuint> fbos;
    std::vector<GLuint> depthRbos;
    GLuint program = 0, vao = 0, vbo = 0;
    GLint uMVP = -1;
    XrTime frameStartTime = 0;
};

static bool InitEGL(App& a) {
    a.eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (a.eglDisplay == EGL_NO_DISPLAY) { LOGE("eglGetDisplay failed"); return false; }
    EGLint major, minor;
    if (!eglInitialize(a.eglDisplay, &major, &minor)) { LOGE("eglInitialize failed"); return false; }
    LOGI("EGL %d.%d", major, minor);
    const EGLint attrs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24, EGL_NONE };
    EGLint n = 0;
    if (!eglChooseConfig(a.eglDisplay, attrs, &a.eglConfig, 1, &n) || n < 1) { LOGE("eglChooseConfig failed"); return false; }
    const EGLint ctxAttrs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    a.eglContext = eglCreateContext(a.eglDisplay, a.eglConfig, EGL_NO_CONTEXT, ctxAttrs);
    if (a.eglContext == EGL_NO_CONTEXT) { LOGE("eglCreateContext failed"); return false; }
    const EGLint surfAttrs[] = { EGL_WIDTH, 16, EGL_HEIGHT, 16, EGL_NONE };
    EGLSurface surf = eglCreatePbufferSurface(a.eglDisplay, a.eglConfig, surfAttrs);
    if (surf == EGL_NO_SURFACE) { LOGE("eglCreatePbufferSurface failed"); return false; }
    if (!eglMakeCurrent(a.eglDisplay, surf, surf, a.eglContext)) { LOGE("eglMakeCurrent failed"); return false; }
    LOGI("GLES %s", glGetString(GL_VERSION));
    return true;
}

static bool InitGL(App& a) {
    GLuint vs = CompileShader(GL_VERTEX_SHADER, kVertShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, kFragShader);
    a.program = glCreateProgram();
    glAttachShader(a.program, vs); glAttachShader(a.program, fs);
    glLinkProgram(a.program);
    GLint ok = 0; glGetProgramiv(a.program, GL_LINK_STATUS, &ok);
    if (!ok) { LOGE("program link failed"); return false; }
    a.uMVP = glGetUniformLocation(a.program, "uMVP");
    glGenVertexArrays(1, &a.vao);
    glGenBuffers(1, &a.vbo);
    glBindVertexArray(a.vao);
    glBindBuffer(GL_ARRAY_BUFFER, a.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVerts), kCubeVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnable(GL_DEPTH_TEST);
    return true;
}

static bool InitXR(App& a) {
    XrApplicationInfo appInfo{};
    strncpy(appInfo.applicationName, "Plumb VR Foundation", XR_MAX_APPLICATION_NAME_SIZE - 1);
    appInfo.applicationVersion = 1;
    strncpy(appInfo.engineName, "PlumbVR", XR_MAX_ENGINE_NAME_SIZE - 1);
    appInfo.engineVersion = 1;
    appInfo.apiVersion = XR_CURRENT_API_VERSION;
    const char* exts[] = { XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME };
    XrInstanceCreateInfo ici{ XR_TYPE_INSTANCE_CREATE_INFO };
    ici.applicationInfo = appInfo;
    ici.enabledExtensionCount = 1;
    ici.enabledExtensionNames = exts;
    XR_CHECK(xrCreateInstance(&ici, &a.instance), "xrCreateInstance");
    LOGI("OpenXR instance created");

    PFN_xrGetOpenGLESGraphicsRequirementsKHR pfnReq = nullptr;
    XR_CHECK(xrGetInstanceProcAddr(a.instance, "xrGetOpenGLESGraphicsRequirementsKHR",
             reinterpret_cast<PFN_xrVoidFunction*>(&pfnReq)), "xrGetInstanceProcAddr requirements");

    XrSystemGetInfo sgi{ XR_TYPE_SYSTEM_GET_INFO };
    sgi.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    XR_CHECK(xrGetSystem(a.instance, &sgi, &a.systemId), "xrGetSystem");
    LOGI("OpenXR system %llu", (unsigned long long)a.systemId);

    XrGraphicsRequirementsOpenGLESKHR req{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR };
    XR_CHECK(pfnReq(a.instance, a.systemId, &req), "xrGetOpenGLESGraphicsRequirementsKHR");
    LOGI("OpenGL ES requirements: min %d.%d max %d.%d",
         (int)XR_VERSION_MAJOR(req.minApiVersionSupported), (int)XR_VERSION_MINOR(req.minApiVersionSupported),
         (int)XR_VERSION_MAJOR(req.maxApiVersionSupported), (int)XR_VERSION_MINOR(req.maxApiVersionSupported));

    // View config: recommended render size.
    uint32_t viewCount = 0;
    XR_CHECK(xrEnumerateViewConfigurationViews(a.instance, a.systemId,
             XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 0, &viewCount, nullptr),
             "xrEnumerateViewConfigurationViews count");
    std::vector<XrViewConfigurationView> views(viewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW });
    XR_CHECK(xrEnumerateViewConfigurationViews(a.instance, a.systemId,
             XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, viewCount, &viewCount, views.data()),
             "xrEnumerateViewConfigurationViews");
    a.viewWidth = views[0].recommendedImageRectWidth;
    a.viewHeight = views[0].recommendedImageRectHeight;
    LOGI("Views: %u, recommended %ux%u per eye", viewCount, a.viewWidth, a.viewHeight);

    XrGraphicsBindingOpenGLESAndroidKHR binding{ XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR };
    binding.display = a.eglDisplay;
    binding.config = a.eglConfig;
    binding.context = a.eglContext;
    XrSessionCreateInfo sci{ XR_TYPE_SESSION_CREATE_INFO };
    sci.next = &binding;
    sci.systemId = a.systemId;
    XR_CHECK(xrCreateSession(a.instance, &sci, &a.session), "xrCreateSession");
    LOGI("OpenXR session created");

    XrReferenceSpaceCreateInfo rci{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
    rci.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
    rci.poseInReferenceSpace = { {0,0,0,1}, {0,0,0} };
    XR_CHECK(xrCreateReferenceSpace(a.session, &rci, &a.space), "xrCreateReferenceSpace");

    // Swapchain format.
    uint32_t fmtCount = 0;
    XR_CHECK(xrEnumerateSwapchainFormats(a.session, 0, &fmtCount, nullptr), "xrEnumerateSwapchainFormats count");
    std::vector<int64_t> fmts(fmtCount);
    XR_CHECK(xrEnumerateSwapchainFormats(a.session, fmtCount, &fmtCount, fmts.data()), "xrEnumerateSwapchainFormats");
    int64_t chosen = fmts[0];
    for (int64_t f : fmts) if (f == GL_SRGB8_ALPHA8) { chosen = f; break; }
    LOGI("Swapchain format %lld", (long long)chosen);

    XrSwapchainCreateInfo swci{ XR_TYPE_SWAPCHAIN_CREATE_INFO };
    swci.usageFlags = XR_SWAPCHAIN_USAGE_SAMPLED_BIT | XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT;
    swci.format = chosen;
    swci.sampleCount = 1;
    swci.width = a.viewWidth;
    swci.height = a.viewHeight;
    swci.faceCount = 1;
    swci.arraySize = 2;
    swci.mipCount = 1;
    XR_CHECK(xrCreateSwapchain(a.session, &swci, &a.swapchain), "xrCreateSwapchain");

    uint32_t imgCount = 0;
    XR_CHECK(xrEnumerateSwapchainImages(a.swapchain, 0, &imgCount, nullptr), "xrEnumerateSwapchainImages count");
    a.swapImages.resize(imgCount, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_ES_KHR });
    XR_CHECK(xrEnumerateSwapchainImages(a.swapchain, imgCount, &imgCount,
             reinterpret_cast<XrSwapchainImageBaseHeader*>(a.swapImages.data())),
             "xrEnumerateSwapchainImages");
    LOGI("Swapchain images: %u", imgCount);

    a.fbos.resize(imgCount); a.depthRbos.resize(imgCount);
    glGenFramebuffers((GLsizei)imgCount, a.fbos.data());
    glGenRenderbuffers((GLsizei)imgCount, a.depthRbos.data());
    for (uint32_t i = 0; i < imgCount; i++) {
        glBindRenderbuffer(GL_RENDERBUFFER, a.depthRbos[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, (GLsizei)a.viewWidth, (GLsizei)a.viewHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, a.fbos[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, a.swapImages[i].image, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, a.depthRbos[i]);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { LOGE("FBO %u incomplete", i); return false; }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    a.xrReady = true;
    LOGI("OpenXR ready: stereo swapchain %ux%u x2", a.viewWidth, a.viewHeight);
    return true;
}

static void PollEvents(App& a) {
    XrEventDataBuffer ev{ XR_TYPE_EVENT_DATA_BUFFER };
    while (xrPollEvent(a.instance, &ev) == XR_SUCCESS) {
        if (ev.type == XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED) {
            auto* s = reinterpret_cast<XrEventDataSessionStateChanged*>(&ev);
            a.sessionState = s->state;
            LOGI("Session state %d", (int)s->state);
            if (s->state == XR_SESSION_STATE_READY && !a.sessionRunning) {
                XrSessionBeginInfo bi{ XR_TYPE_SESSION_BEGIN_INFO };
                bi.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
                if (XR_SUCCEEDED(xrBeginSession(a.session, &bi))) { a.sessionRunning = true; LOGI("Session begun"); }
            } else if (s->state == XR_SESSION_STATE_STOPPING && a.sessionRunning) {
                xrEndSession(a.session);
                a.sessionRunning = false;
                LOGI("Session ended");
            } else if (s->state == XR_SESSION_STATE_EXITING || s->state == XR_SESSION_STATE_LOSS_PENDING) {
                a.quit = true;
            }
        }
        ev.type = XR_TYPE_EVENT_DATA_BUFFER;
    }
}

static bool RenderFrame(App& a) {
    XrFrameState fs{ XR_TYPE_FRAME_STATE };
    XR_CHECK(xrWaitFrame(a.session, nullptr, &fs), "xrWaitFrame");
    XR_CHECK(xrBeginFrame(a.session, nullptr), "xrBeginFrame");
    a.frameStartTime = fs.predictedDisplayTime;

    std::vector<XrCompositionLayerProjectionView> projViews;
    XrCompositionLayerProjection layer{ XR_TYPE_COMPOSITION_LAYER_PROJECTION };
    bool shouldRender = fs.shouldRender;
    if (shouldRender) {
        XrViewLocateInfo vli{ XR_TYPE_VIEW_LOCATE_INFO };
        vli.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
        vli.displayTime = fs.predictedDisplayTime;
        vli.space = a.space;
        XrViewState vs{ XR_TYPE_VIEW_STATE };
        uint32_t viewCount = 0;
        XrView views[2] = { { XR_TYPE_VIEW }, { XR_TYPE_VIEW } };
        XrResult r = xrLocateViews(a.session, &vli, &vs, 2, &viewCount, views);
        if (XR_FAILED(r) || viewCount == 0) { shouldRender = false; }
        else {
            uint32_t imgIndex = 0;
            XrSwapchainImageAcquireInfo ai{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO };
            if (XR_FAILED(xrAcquireSwapchainImage(a.swapchain, &ai, &imgIndex))) { shouldRender = false; }
            else {
                XrSwapchainImageWaitInfo wi{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO };
                wi.timeout = XR_INFINITE_DURATION;
                xrWaitSwapchainImage(a.swapchain, &wi);

                float t = (float)(fs.predictedDisplayTime / 1000000000.0);
                Mat4 model = MatMul(MatTranslate(0.f, 0.f, -2.f), MatRotateY(t * 0.5f));
                glUseProgram(a.program);
                glBindVertexArray(a.vao);
                for (uint32_t i = 0; i < viewCount; i++) {
                    Mat4 proj = MatPerspective(views[i].fov, 0.05f, 100.f);
                    Mat4 view = MatFromPoseInv(views[i].pose);
                    Mat4 mvp = MatMul(proj, MatMul(view, model));
                    glBindFramebuffer(GL_FRAMEBUFFER, a.fbos[imgIndex]);
                    glViewport(0, 0, (GLsizei)a.viewWidth, (GLsizei)a.viewHeight);
                    // Left eye dark blue, right eye dark teal: proves per-eye stereo.
                    if (i == 0) glClearColor(0.05f, 0.05f, 0.25f, 1.f);
                    else        glClearColor(0.05f, 0.20f, 0.20f, 1.f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glUniformMatrix4fv(a.uMVP, 1, GL_FALSE, mvp.m);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                XrSwapchainImageReleaseInfo ri{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO };
                xrReleaseSwapchainImage(a.swapchain, &ri);

                projViews.resize(viewCount, { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW });
                for (uint32_t i = 0; i < viewCount; i++) {
                    projViews[i].pose = views[i].pose;
                    projViews[i].fov = views[i].fov;
                    projViews[i].subImage.swapchain = a.swapchain;
                    projViews[i].subImage.imageRect.offset = {0, 0};
                    projViews[i].subImage.imageRect.extent = { (int32_t)a.viewWidth, (int32_t)a.viewHeight };
                    projViews[i].subImage.imageArrayIndex = i;
                }
                layer.viewCount = viewCount;
                layer.views = projViews.data();
            }
        }
    }
    layer.space = a.space;
    XrCompositionLayerBaseHeader* layers[] = { reinterpret_cast<XrCompositionLayerBaseHeader*>(&layer) };
    XrFrameEndInfo fei{ XR_TYPE_FRAME_END_INFO };
    fei.displayTime = fs.predictedDisplayTime;
    fei.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
    fei.layerCount = shouldRender ? 1u : 0u;
    fei.layers = shouldRender ? layers : nullptr;
    XR_CHECK(xrEndFrame(a.session, &fei), "xrEndFrame");
    return true;
}

static void OnAppCmd(android_app* app, int32_t cmd) {
    App* a = reinterpret_cast<App*>(app->userData);
    if (cmd == APP_CMD_INIT_WINDOW) a->windowReady = true;
    if (cmd == APP_CMD_TERM_WINDOW) a->windowReady = false;
    if (cmd == APP_CMD_DESTROY) a->quit = true;
}

void android_main(android_app* app) {
    App a;
    a.app = app;
    app->userData = &a;
    app->onAppCmd = OnAppCmd;
    LOGI("Plumb VR Foundation starting");

    if (!InitEGL(a) || !InitGL(a)) { LOGE("EGL/GL init failed"); return; }
    if (!InitXR(a)) { LOGE("OpenXR init failed"); return; }

    while (!a.quit) {
        int events = 0;
        android_poll_source* source = nullptr;
        int timeout = a.sessionRunning ? 0 : -1;
        if (ALooper_pollOnce(timeout, nullptr, &events, (void**)&source) >= 0 && source) source->process(app, source);
        if (a.app->destroyRequested) a.quit = true;
        if (a.xrReady) PollEvents(a);
        if (a.quit) break;
        if (a.sessionRunning && a.xrReady) {
            if (!RenderFrame(a)) { LOGE("RenderFrame failed"); break; }
        }
    }

    if (a.sessionRunning) xrEndSession(a.session);
    if (a.space) xrDestroySpace(a.space);
    if (a.swapchain) xrDestroySwapchain(a.swapchain);
    if (a.session) xrDestroySession(a.session);
    if (a.instance) xrDestroyInstance(a.instance);
    LOGI("Plumb VR Foundation exiting cleanly");
}
