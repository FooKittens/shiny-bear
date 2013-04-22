#ifndef SHINYBEAR_COLOR4F_H
#define SHINYBEAR_COLOR4F_H


namespace shinybear {

typedef unsigned long DWORD;

struct Color4f {
  float r, g, b, a;

  Color4f(float r, float g, float b, float a)
    :r(r), g(g), b(b), a(a){

    ClampElement(&r);
    ClampElement(&g);
    ClampElement(&b);
    ClampElement(&a);
  }

  Color4f(int r, int g, int b, int a) 
    :r(ToFloat(r)), g(ToFloat(g)),
     b(ToFloat(b)), a(ToFloat(a)) { }

  void operator*=(const Color4f &rhs);
  void operator/=(const Color4f &rhs);
  void operator+=(const Color4f &rhs);
  void operator-=(const Color4f &rhs);

  operator DWORD() const{
    DWORD c = 0;
    c |= ((int)(a * 255)) << 24;
    c |= ((int)(r * 255)) << 16;
    c |= ((int)(g * 255)) << 8;
    c |= ((int)(b * 255)) << 0;
    return c;
  }
 
private:
  inline static void ClampElement(float *e) {
    if(*e < 0) *e = 0;
    if(*e > 1.0f) *e = 1.0f;
  }

  inline static float ToFloat(int c) {
    if(c < 0) c = 0;
    if(c > 255) c = 255;

    return (float)c / 255.0f;
  }
};


inline void Color4f::operator*=(const Color4f &rhs) {
  r *= rhs.r;
  g *= rhs.g;
  b *= rhs.b;
  a *= rhs.a;
}

inline void Color4f::operator/=(const Color4f &rhs) {
  r /= rhs.r;
  g /= rhs.g;
  b /= rhs.b;
  a /= rhs.a;
}

inline const Color4f operator*(const Color4f &lhs, const Color4f &rhs) {
  return Color4f(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
}

inline const Color4f operator*(const Color4f &lhs, float c) {
  return Color4f(lhs.r * c, lhs.g * c, lhs.b * c, lhs.a * c);
}

inline const Color4f operator/(const Color4f &lhs, float c) {
  return lhs * (1.0f / c);
}

inline const Color4f operator/(const Color4f &lhs, const Color4f &rhs) {
  return Color4f(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b, lhs.a / rhs.a);
}

inline const Color4f operator+(const Color4f &lhs, const Color4f &rhs) {
  return Color4f(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
}

inline const Color4f operator-(const Color4f &lhs, const Color4f &rhs) {
  return lhs + (rhs * -1.0f);
}

} // namespace shinybear

#endif // SHINYBEAR_COLOR4F_H