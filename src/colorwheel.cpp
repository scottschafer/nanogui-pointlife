#include <nanogui/colorwheel.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

#include <Eigen/Dense>

NANOGUI_NAMESPACE_BEGIN

ColorWheel::ColorWheel(Widget *parent, const Vector3f& rgb)
    : Widget(parent) {
    float r = rgb[0],
          g = rgb[1],
          b = rgb[2];

    float max = std::max({ r, g, b });
    float min = std::min({ r, g, b });
    float h, s, l = (max + min) / 2;

    if (max == min) {
        mHue   = 0.;
        mBlack = 1. - l;
        mWhite = l;
    } else {
        float d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        if (max == r)
            h = (g - b) / d + (g < b ? 6 : 0);
        else if (max == g)
            h = (b - r) / d + 2;
        else if (max == b)
            h = (r - g) / d + 4;
        h /= 6;

        mHue   = h;

        Eigen::Matrix<float, 4, 3> M;
        M.topLeftCorner<3,1>() = hue2rgb(h);
        M(3,0)   = 1.;
        M.col(1) = Vector4f { 0., 0., 0., 1. };
        M.col(2) = Vector4f { 1., 1., 1., 1. };

        Vector4f rgb4 { rgb[0], rgb[1], rgb[2], 1. };
        Vector3f bary = M.colPivHouseholderQr().solve(rgb4);

        mBlack = bary[1];
        mWhite = bary[2];
    }
}

Vector2i ColorWheel::preferredSize(NVGcontext *ctx) const {
    return { 250., 250. };
}

void ColorWheel::draw(NVGcontext *ctx) {
    if (!mVisible)
        return;

    float x = mPos.x(),
          y = mPos.y(),
          w = mSize.x(),
          h = mSize.y();

    NVGcontext* vg = ctx;

    int i;
    float r0, r1, ax,ay, bx,by, cx,cy, aeps, r;
    float hue = mHue;
    NVGpaint paint;

    nvgSave(vg);

/*  nvgBeginPath(vg);
    nvgRect(vg, x,y,w,h);
    nvgFillColor(vg, nvgRGBA(255,0,0,128));
    nvgFill(vg);*/

    cx = x + w*0.5f;
    cy = y + h*0.5f;
    r1 = (w < h ? w : h) * 0.5f - 5.0f;
    r0 = r1 - 20.0f;
    aeps = 0.5f / r1;   // half a pixel arc length in radians (2pi cancels out).

    for (i = 0; i < 6; i++) {
        float a0 = (float)i / 6.0f * NVG_PI * 2.0f - aeps;
        float a1 = (float)(i+1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
        nvgBeginPath(vg);
        nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
        nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
        nvgClosePath(vg);
        ax = cx + cosf(a0) * (r0+r1)*0.5f;
        ay = cy + sinf(a0) * (r0+r1)*0.5f;
        bx = cx + cosf(a1) * (r0+r1)*0.5f;
        by = cy + sinf(a1) * (r0+r1)*0.5f;
        paint = nvgLinearGradient(vg, ax,ay, bx,by, nvgHSLA(a0/(NVG_PI*2),1.0f,0.55f,255), nvgHSLA(a1/(NVG_PI*2),1.0f,0.55f,255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
    }

    nvgBeginPath(vg);
    nvgCircle(vg, cx,cy, r0-0.5f);
    nvgCircle(vg, cx,cy, r1+0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,64));
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);

    // Selector
    nvgSave(vg);
    nvgTranslate(vg, cx,cy);
    nvgRotate(vg, hue*NVG_PI*2);

    // Marker on
    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgRect(vg, r0-1,-3,r1-r0+2,6);
    nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
    nvgStroke(vg);

    paint = nvgBoxGradient(vg, r0-3,-5,r1-r0+6,10, 2,4, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, r0-2-10,-4-10,r1-r0+4+20,8+20);
    nvgRect(vg, r0-2,-4,r1-r0+4,8);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    // Center triangle
    r = r0 - 6;
    ax = cosf(120.0f/180.0f*NVG_PI) * r;
    ay = sinf(120.0f/180.0f*NVG_PI) * r;
    bx = cosf(-120.0f/180.0f*NVG_PI) * r;
    by = sinf(-120.0f/180.0f*NVG_PI) * r;
    nvgBeginPath(vg);
    nvgMoveTo(vg, r,0);
    nvgLineTo(vg, ax,ay);
    nvgLineTo(vg, bx,by);
    nvgClosePath(vg);
    paint = nvgLinearGradient(vg, r,0, ax,ay, nvgHSLA(hue,1.0f,0.5f,255), nvgRGBA(255,255,255,255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    paint = nvgLinearGradient(vg, (r+ax)*0.5f,(0+ay)*0.5f, bx,by, nvgRGBA(0,0,0,0), nvgRGBA(0,0,0,255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,64));
    nvgStroke(vg);

    // Select circle on triangle
    float sx = r*(1 - mWhite - mBlack) + ax*mWhite + bx*mBlack;
    float sy =                           ay*mWhite + by*mBlack;

    nvgStrokeWidth(vg, 2.0f);
    nvgBeginPath(vg);
    nvgCircle(vg, sx,sy,5);
    nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
    nvgStroke(vg);

    paint = nvgRadialGradient(vg, sx,sy, 7,9, nvgRGBA(0,0,0,64), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, sx-20,sy-20,40,40);
    nvgCircle(vg, sx,sy,7);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    nvgRestore(vg);

    nvgRestore(vg);
}

bool
ColorWheel::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    if (!mEnabled)
        return false;

    adjustPosition(p);

    return true;
}

bool
ColorWheel::mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers)
{
    adjustPosition(p);
    return true;
}

void
ColorWheel::adjustPosition(const Vector2i &p)
{
    float x = p.x() - mPos.x(),
          y = p.y() - mPos.y(),
          w = mSize.x(),
          h = mSize.y();

    float cx = w*0.5f;
    float cy = h*0.5f;
    float r1 = (w < h ? w : h) * 0.5f - 5.0f;
    float r0 = r1 - 20.0f;

    x -= cx;
    y -= cy;

    float mr = std::sqrt(x*x + y*y);

    if (mr >= r0 && mr <= r1)
    {
        mHue = std::atan(y / x);
        if (x < 0)
            mHue += NVG_PI;
        mHue /= 2*NVG_PI;

        if (mCallback)
            mCallback();

        return;
    }

    float r = r0 - 6;

    float ax = cosf( 120.0f/180.0f*NVG_PI) * r;
    float ay = sinf( 120.0f/180.0f*NVG_PI) * r;
    float bx = cosf(-120.0f/180.0f*NVG_PI) * r;
    float by = sinf(-120.0f/180.0f*NVG_PI) * r;

    typedef     Eigen::Matrix<float,2,2>        Matrix2f;

    Eigen::Matrix<float, 2, 3>  triangle;
    triangle << ax,bx,r,
                ay,by,0;
    triangle = Eigen::Rotation2D<float>(mHue * 2 * NVG_PI).matrix() * triangle;

    Matrix2f T;
    T << triangle(0,0) - triangle(0,2), triangle(0,1) - triangle(0,2),
         triangle(1,0) - triangle(1,2), triangle(1,1) - triangle(1,2);
    Vector2f pos { x - triangle(0,2), y - triangle(1,2) };

    Vector2f bary = T.colPivHouseholderQr().solve(pos);
    float l0 = bary[0],
          l1 = bary[1];
    float l2 = 1 - l0 - l1;

    if (l0 >= 0 && l0 <= 1. &&
        l1 >= 0 && l1 <= 1. &&
        l2 >= 0 && l2 <= 1.)
    {
        mWhite = l0;
        mBlack = l1;
    }

    if (mCallback)
        mCallback();
}

Vector3f
ColorWheel::hue2rgb(float h) const
{
    float s = 1., v = 1.;
    float r,g,b;

    if (h < 0) h += 1;

    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch(i % 6)
    {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return { r, g, b };
}

Vector3f
ColorWheel::color() const
{
    Vector3f rgb    = hue2rgb(mHue);
    Vector3f black  { 0., 0., 0. };
    Vector3f white  { 1., 1., 1. };
    return rgb * (1 - mWhite - mBlack) + black * mBlack + white * mWhite;
}


NANOGUI_NAMESPACE_END

