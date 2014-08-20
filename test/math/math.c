#include <stdio.h>
#include <stdlib.h>
#include <kapusha/math.h>

#define TOLERANCE 1e-6
#define FEQ(a,b) (kpDistancef(a,b) < TOLERANCE)
#define V3EQ(a,b) (FEQ(a.x,b.x) && FEQ(a.y,b.y) && FEQ(a.z,b.z))
#define V4EQ(a,b) (FEQ(a.x,b.x) && FEQ(a.y,b.y) && FEQ(a.z,b.z) && FEQ(a.w,b.w))

#define KP_TEST(t) \
  if (!(t)) { \
    fprintf(stderr, "%s:%d: test (%s) failed!\n", __FILE__, __LINE__, #t); \
  }

#define KP_TEST_FEQ(a,b) { \
  const KPf32 A = (a), B = (b); \
  if (!FEQ(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s)%f != %f(%s) !!!\n", \
      __FILE__, __LINE__, #a, A, B, #b); \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s)%f == %f(%s)\n", \
      __FILE__, __LINE__, #a, A, B, #b); \
  }}

#define KP_TEST_V3EQ(a,b) { \
  const KPvec3f A = (a), B = (b); \
  if (!V3EQ(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s){%f,%f,%f} != {%f,%f,%f}(%s) !!!\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, B.x, B.y, B.z, #b); \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s){%f,%f,%f} == {%f,%f,%f}(%s)\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, B.x, B.y, B.z, #b); \
  }}

#define KP_TEST_V4EQ(a,b) { \
  const KPvec4f A = (a), B = (b); \
  if (!V4EQ(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s){%f,%f,%f,%f} != {%f,%f,%f,%f}(%s) !!!\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, A.w, B.x, B.y, B.z, B.w, #b); \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s){%f,%f,%f,%f} == {%f,%f,%f,%f}(%s)\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, A.w, B.x, B.y, B.z, B.w, #b); \
  }}

int main(int argc, char *argv[]) {

  KP_TEST_FEQ(kpVec3fLength(kpVec3ff(5)),8.66025403784438646763f);

  KP_TEST_V3EQ(kpVec3fCross(kpVec3f(1.f,0.f,0.f),kpVec3f(0.f,1.f,0.f)),
    kpVec3f(0.f,0.f,1.f));

  KP_TEST_V3EQ(kpVec3fCross(kpVec3f(0.f,1.f,0.f),kpVec3f(1.f,0.f,0.f)),
    kpVec3f(0.f,0.f,-1.f));

  KP_TEST_V3EQ(kpVec3fNormalize(kpVec3f(5.f, 0.f, 0.f)),kpVec3f(1.f,0.f,0.f));
  KP_TEST_V3EQ(kpVec3fNormalize(kpVec3ff(1.f)),kpVec3ff(.577350269f));
  KP_TEST_V3EQ(kpVec3fNormalize(kpVec3ff(-1.f)),kpVec3ff(-.577350269f));

  /* test x translation */
  KP_TEST_V4EQ(kpVec4f(2.f,2.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),0,kpVec3f(1,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test y translation */
  KP_TEST_V4EQ(kpVec4f(1.f,3.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),0,kpVec3f(0,1,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test z translation */
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,4.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),0,kpVec3f(0,0,1))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test combined translations */
  KP_TEST_V4EQ(kpVec4f(-2.f,10.f,10.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),0,kpVec3f(-3,8,7))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test rotation around x */
  KP_TEST_V4EQ(kpVec4f(1.f,-3.f,2.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,3.f,-2.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),-KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,-2.f,-3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,-2.f,-3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),-KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(1,0,0),KPF32_2PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test rotation around y */
  KP_TEST_V4EQ(kpVec4f(3.f,2.f,-1.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,1,0),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-3.f,2.f,1.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,-1,0),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,2.f,-3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,1,0),KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,2.f,-3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,1,0),-KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,1,0),KPF32_2PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test rotation around z */
  KP_TEST_V4EQ(kpVec4f(-2.f,1.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(2.f,-1.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,-1),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,-2.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,-2.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),-KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_2PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test combined transform */
  KP_TEST_V4EQ(kpVec4f(-2.f,6.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,5,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-2.f,1.f,8.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,0,5))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(3.f,1.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(5,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(3.f,1.f,3.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfMuldq(
        kpDquatfRotationTranslation(kpVec3f(0,0,0),0,kpVec3f(5,0,0)),
        kpDquatfRotationTranslation(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,0,0))
        )),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(6.f,3.f,4.f,1.f),
    kpMat4fMulv4(
      kpMat4fdq(kpDquatfNormalize(kpDquatfMuldq(
        kpDquatfRotationTranslation(kpVec3f(0,0,0),0,kpVec3f(0,1,5)),
        kpDquatfRotationTranslation(kpVec3f(0,1,0),KPF32_PI2,kpVec3f(3,0,0))
        ))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  return 0;
}
