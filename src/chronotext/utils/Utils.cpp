#include "chronotext/utils/Utils.h"

#include "utf8.h"

using namespace std;
using namespace ci;
using namespace utf8;

string wstringToUtf8(const wstring &s)
{
    vector<char> tmp;

#if defined(CINDER_MSW)
    unchecked::utf16to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
    unchecked::utf32to8(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
    
    return string(&tmp[0], tmp.size());
}

wstring utf8ToWstring(const string &s)
{
    vector<wchar_t> tmp;

#if defined(CINDER_MSW)
    unchecked::utf8to16(s.data(), s.data() + s.size(), back_inserter(tmp));
#else
    unchecked::utf8to32(s.data(), s.data() + s.size(), back_inserter(tmp));
#endif
    
    return wstring(&tmp[0], tmp.size());
}

void dumpCamera(const ci::Camera &cam, const string &name)
{
    Vec3f worldUp = cam.getWorldUp();
    Vec3f eyepoint = cam.getEyePoint();
    Vec3f centerOfInterestPoint = cam.getCenterOfInterestPoint();
    Quatf orientation = cam.getOrientation();
    Vec3f axis = orientation.getAxis();
    
    cout << name << ".setWorldUp(Vec3f(" << worldUp.x << ", " << worldUp.y << ", " << worldUp.z << "));" << endl;
    cout << name << ".setEyePoint(Vec3f(" << eyepoint.x << ", " << eyepoint.y << ", " << eyepoint.z << "));" << endl;
    cout << name << ".setCenterOfInterestPoint(Vec3f(" << centerOfInterestPoint.x << ", " << centerOfInterestPoint.y << ", " << centerOfInterestPoint.z << "));" << endl;
    cout << name << ".setOrientation(Quatf(Vec3f(" << axis.x << ", " << axis.y << ", " << axis.z << "), " << orientation.getAngle() << "));" << endl;
    cout << name << ".setPerspective(" << cam.getFov() << ", getWindowAspectRatio(), " << cam.getNearClip() << ", " << cam.getFarClip() << ");" << endl;
}
