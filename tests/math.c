#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <kapusha/math.h>
#include <kapusha/ooo.h>

#define TOLERANCE 1e-5
#define FEQ(a,b) (kpDistancef(a,b) < TOLERANCE)
#define V3EQ(a,b) (FEQ(a.x,b.x) && FEQ(a.y,b.y) && FEQ(a.z,b.z))
#define V4EQ(a,b) (FEQ(a.x,b.x) && FEQ(a.y,b.y) && FEQ(a.z,b.z) && FEQ(a.w,b.w))

static int count_ok = 0, count_fail = 0;

#define KP_TEST(t) \
  if (!(t)) { \
    fprintf(stderr, "%s:%d: test (%s) failed!\n", __FILE__, __LINE__, #t); \
    ++count_fail; \
  } else { \
    ++count_ok; \
  }

#define KP_TEST_FEQ(a,b) { \
  const KPf32 A = (a), B = (b); \
  if (!FEQ(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s)%f != %f(%s) !!!\n", \
      __FILE__, __LINE__, #a, A, B, #b); \
    ++count_fail; \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s)%f == %f(%s)\n", \
      __FILE__, __LINE__, #a, A, B, #b); \
    ++count_ok; \
  }}

#define KP_TEST_V3EQ(a,b) { \
  const KPvec3f A = (a), B = (b); \
  if (!V3EQ(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s){%f,%f,%f} != {%f,%f,%f}(%s) !!!\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, B.x, B.y, B.z, #b); \
    ++count_fail; \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s){%f,%f,%f} == {%f,%f,%f}(%s)\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, B.x, B.y, B.z, #b); \
    ++count_ok; \
  }}

#define KP_TEST_V4EQ(a,b) { \
  const KPvec4f A = (a), B = (b); \
  if (!V4EQ(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s){%f,%f,%f,%f} != {%f,%f,%f,%f}(%s) !!!\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, A.w, B.x, B.y, B.z, B.w, #b); \
    ++count_fail; \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s){%f,%f,%f,%f} == {%f,%f,%f,%f}(%s)\n", \
      __FILE__, __LINE__, #a, A.x, A.y, A.z, A.w, B.x, B.y, B.z, B.w, #b); \
    ++count_ok; \
  }}

#define KP_TEST_M4EQS(a,b) { \
  const KPmat4f A = (a), B = (b); \
  if (memcmp(&A, &B, sizeof(A)) != 0) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s) != (%s) !!!\n", \
      __FILE__, __LINE__, #a, #b); \
    ++count_fail; \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s) == (%s)\n", \
      __FILE__, __LINE__, #a, #b); \
    ++count_ok; \
  }}

static int m4eq(const KPmat4f *a, const KPmat4f *b) {
  const KPf32 *pa = &a->r[0].x, *pb = &b->r[0].x;
  int i = 0;
  for (; i < 16; ++i)
    if (!FEQ(*pa, *pb))
      return 0;
  return 1;
}

#define KP_TEST_M4EQ(a,b) { \
  const KPmat4f A = (a), B = (b); \
  if (!m4eq(&A, &B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s) != (%s) !!!\n", \
      __FILE__, __LINE__, #a, #b); \
    ++count_fail; \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s) == (%s)\n", \
      __FILE__, __LINE__, #a, #b); \
    ++count_ok; \
  }}

static int qeq(KPquatf a, KPquatf b) {
  if (a.v.x > 0 && b.v.x < 0) {
    b.v.x = -b.v.x;
    b.v.y = -b.v.y;
    b.v.z = -b.v.z;
  }
  const KPf32 *pa = &a.v.x, *pb = &b.v.x;
  int i = 0;
  for (; i < 4; ++i)
    if (!FEQ(*pa, *pb))
      return 0;
  return 1;
}

#define KP_TEST_QEQ(a,b) { \
  const KPquatf A = (a), B = (b); \
  if (!qeq(A,B)) { \
    fprintf(stderr, "FAIL: %s:%d: test (%s){%f,%f,%f,%f} != {%f,%f,%f,%f}(%s) !!!\n", \
      __FILE__, __LINE__, #a, A.v.x, A.v.y, A.v.z, A.v.w, B.v.x, B.v.y, B.v.z, B.v.w, #b); \
    ++count_fail; \
  } else { \
    fprintf(stderr, "OK: %s:%d: test (%s){%f,%f,%f,%f} == {%f,%f,%f,%f}(%s)\n", \
      __FILE__, __LINE__, #a, A.v.x, A.v.y, A.v.z, A.v.w, B.v.x, B.v.y, B.v.z, B.v.w, #b); \
    ++count_ok; \
  }}

int main(int argc, char *argv[]) {
  KP_UNUSED(argc);
  KP_UNUSED(argv);

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
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),0,kpVec3f(1,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test y translation */
  KP_TEST_V4EQ(kpVec4f(1.f,3.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),0,kpVec3f(0,1,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test z translation */
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,4.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),0,kpVec3f(0,0,1))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test combined translations */
  KP_TEST_V4EQ(kpVec4f(-2.f,10.f,10.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),0,kpVec3f(-3,8,7))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test rotation around x */
  KP_TEST_V4EQ(kpVec4f(1.f,-3.f,2.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,3.f,-2.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),-KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,-2.f,-3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,-2.f,-3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),-KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(1,0,0),KPF32_2PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test rotation around y */
  KP_TEST_V4EQ(kpVec4f(3.f,2.f,-1.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,1,0),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-3.f,2.f,1.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,-1,0),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,2.f,-3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,1,0),KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,2.f,-3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,1,0),-KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,1,0),KPF32_2PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test rotation around z */
  KP_TEST_V4EQ(kpVec4f(-2.f,1.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(2.f,-1.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,-1),KPF32_PI2,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,-2.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-1.f,-2.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),-KPF32_PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(1.f,2.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_2PI,kpVec3f(0,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test combined transform */
  KP_TEST_V4EQ(kpVec4f(-2.f,6.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,5,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(-2.f,1.f,8.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,0,5))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(3.f,1.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(5,0,0))),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(3.f,1.f,3.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMul(
        kpDquatfMakeTransform(kpVec3f(0,0,0),0,kpVec3f(5,0,0)),
        kpDquatfMakeTransform(kpVec3f(0,0,1),KPF32_PI2,kpVec3f(0,0,0))
        )),
      kpVec4f(1.f,2.f,3.f,1.f)));
  KP_TEST_V4EQ(kpVec4f(6.f,3.f,4.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfNormalize(kpDquatfMul(
        kpDquatfMakeTransform(kpVec3f(0,0,0),0,kpVec3f(0,1,5)),
        kpDquatfMakeTransform(kpVec3f(0,1,0),KPF32_PI2,kpVec3f(3,0,0))
        ))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  /* test quaternion to matrix to quaternion */
  KP_TEST_V4EQ(kpVec4f(6.f,3.f,4.f,1.f),
    kpMat4fMulv(
      kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfNormalize(kpDquatfMul(
        kpDquatfMakeTransform(kpVec3f(0,0,0),0,kpVec3f(0,1,5)),
        kpDquatfMakeTransform(kpVec3f(0,1,0),KPF32_PI2,kpVec3f(3,0,0))
        ))))),
      kpVec4f(1.f,2.f,3.f,1.f)));

  int i;
  for (i = 0; i < 16; ++i) {
    KPf32 a = (KPf32)i * KPF32_2PI / 16.f;
    KP_TEST_V3EQ(kpVec3f(3,0,0),
      kpDquatfGetTranslation(kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(
        kpDquatfMakeTransform(kpVec3f(0,1,0),a,kpVec3f(3,0,0)))))
      )));
    KP_TEST_V3EQ(kpVec3f(0,7,16),
      kpDquatfGetTranslation(kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(
        kpDquatfMakeTransform(kpVec3f(0,1,0),a,kpVec3f(0,7,16)))))
      )));
    KP_TEST_V3EQ(kpVec3f(0,7,16),
      kpDquatfGetTranslation(kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(
        kpDquatfMakeTransform(kpVec3f(1,0,0),a,kpVec3f(0,7,16)))))
      )));
    KP_TEST_QEQ(kpQuatfMakeRotation(kpVec3f(1,0,0),a),
      kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(
        kpDquatfMakeTransform(kpVec3f(1,0,0),a,kpVec3f(1,11,0)))))
      ).r);
    KP_TEST_QEQ(kpQuatfMakeRotation(kpVec3f(0,1,0),a),
      kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(
        kpDquatfMakeTransform(kpVec3f(0,1,0),a,kpVec3f(1,11,0)))))
      ).r);
    KP_TEST_QEQ(kpQuatfMakeRotation(kpVec3f(0,0,1),a),
      kpDquatfMakeMat4f(kpMat4fMakeDquatf(kpDquatfMakeMat4f(kpMat4fMakeDquatf(
        kpDquatfMakeTransform(kpVec3f(0,0,1),a,kpVec3f(1,11,0)))))
      ).r);
  }

  KP_TEST_M4EQS(
    kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16)),
    kpMat4fMul(
      kpMat4f(kpVec4f(1,0,0,0),kpVec4f(0,1,0,0),kpVec4f(0,0,1,0),kpVec4f(0,0,0,1)),
      kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16))
      ));

  KP_TEST_M4EQS(
    kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16)),
    kpMat4fMul(
      kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16)),
      kpMat4f(kpVec4f(1,0,0,0),kpVec4f(0,1,0,0),kpVec4f(0,0,1,0),kpVec4f(0,0,0,1))
      ));

  KP_TEST_M4EQS(
    kpMat4f(kpVec4f(1,5,9,13),kpVec4f(2,6,10,14),kpVec4f(3,7,11,15),kpVec4f(4,8,12,16)),
    kpMat4fTranspose(kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16))));

  KP_TEST_M4EQS(
    kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16)),
    kpMat4fTranspose(kpMat4fTranspose(kpMat4f(kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),kpVec4f(9,10,11,12),kpVec4f(13,14,15,16)))));

  { /* matrix order: (AB)v = A(Bv) */
    KP_TEST_V4EQ(
      kpMat4fMulv(kpMat4fMul(
          kpMat4f(
            kpVec4f(17,27,37,47),kpVec4f(57,67,77,87),
            kpVec4f(97,107,117,127),kpVec4f(137,147,157,167)),
          kpMat4f(
            kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),
            kpVec4f(9,10,11,12),kpVec4f(13,14,15,16))),
        kpVec4f(100,200,300,400)),
      kpMat4fMulv(
        kpMat4f(
          kpVec4f(17,27,37,47),kpVec4f(57,67,77,87),
          kpVec4f(97,107,117,127),kpVec4f(137,147,157,167)),
        kpMat4fMulv(
          kpMat4f(
            kpVec4f(1,2,3,4),kpVec4f(5,6,7,8),
            kpVec4f(9,10,11,12),kpVec4f(13,14,15,16)),
          kpVec4f(100,200,300,400))));
  }

  { /* dual quaternion multiplication order == matrix */
    KPdquatf qa, qb;
    KPmat4f ma, mb;
    qa = kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(1,2,3)),4,kpVec3f(5,6,7));
    qb = kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(9,8,7)),6,kpVec3f(5,4,3));
    ma = kpMat4fMakeDquatf(qa);
    mb = kpMat4fMakeDquatf(qb);
    KP_TEST_M4EQ(kpMat4fMul(mb, ma), kpMat4fMakeDquatf(kpDquatfMul(qb, qa)));
  }

  { /* dual quaternion commutativity */
    KPdquatf q;
    KPmat4f m;
    q = kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(1,2,3)),4,kpVec3f(5,6,7)); /* q=AtAr */
    q = kpDquatfMul(kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(9,8,7)),6,kpVec3f(5,4,3)), q); /* q=BtBrAtAr */
    m = kpMat4fMakeDquatf(q); /* m = BtBrAtAr */

    q = kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(1,2,3)),4,kpVec3f(5,6,7)); /* q=AtAr */
    q = kpDquatfMul(kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(9,8,7)),6,kpVec3f(0,0,0)), q); /* q=BrAtAr */
    q = kpDquatfMul(kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(9,8,7)),0,kpVec3f(5,4,3)), q); /* q=BtBrAtAr */
    KP_TEST_M4EQ(m, kpMat4fMakeDquatf(q));
  }

  { /* reframe commutativity */
    KPreframe_t fa, fb;
    kpReframeMakeTransform(&fa,
      kpVec3fNormalize(kpVec3f(1,2,3)),4,kpVec3f(5,6,7));
    kpReframeMakeIdentity(&fb);
    kpReframeRotateAroundAxis(&fb,kpVec3fNormalize(kpVec3f(1,2,3)),4);
    kpReframeTranslate(&fb, kpVec3f(5,6,7));
    KP_TEST_M4EQ(*kpReframeGetMatrix(&fa), *kpReframeGetMatrix(&fb));
  }

  { /* trivial static frame orientation */
    KPreframe_t f;
    kpReframeMakeIdentity(&f);
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,1,0),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,1),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,0),kpReframeGetTranslation(&f));
    kpReframeMakeIdentity(&f);
    kpReframeRotateAroundAxis(&f,kpVec3f(1,0,0),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,1),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,-1,0),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,0),kpReframeGetTranslation(&f));

    kpReframeMakeRotation(&f,kpVec3f(1,0,0),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,1),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,-1,0),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,0),kpReframeGetTranslation(&f));
    kpReframeRotateAroundAxis(&f,kpVec3f(1,0,0),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,-1,0),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,-1),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,0),kpReframeGetTranslation(&f));

    kpReframeMakeRotation(&f,kpVec3f(0,1,0),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(0,0,-1),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,1,0),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,0),kpReframeGetTranslation(&f));

    kpReframeMakeRotation(&f,kpVec3f(0,0,1),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(0,1,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(-1,0,0),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,1),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,0),kpReframeGetTranslation(&f));
  }

  { /* reference frame applying order == matrix */
    KPreframe_t f;
    KPmat4f m;

    kpReframeMakeTransform(&f,
      kpVec3fNormalize(kpVec3f(1,2,3)),4,kpVec3f(5,6,7)); /* f = A */
    m = *kpReframeGetMatrix(&f); /* m = A */

    kpReframeMakeTransform(&f,
      kpVec3fNormalize(kpVec3f(9,8,7)),6,kpVec3f(4,3,1)); /* f = B */
    m = kpMat4fMul(m, *kpReframeGetMatrix(&f)); /* m = AB */

    kpReframeRotateAroundAxis(&f,kpVec3fNormalize(kpVec3f(1,2,3)),4); /* f = fA */
    kpReframeTranslate(&f,kpVec3f(5,6,7));
    KP_TEST_M4EQ(m, *kpReframeGetMatrix(&f));
  }

  {
    KPreframe_t f;
    kpReframeMakeTransform(&f,kpVec3f(1,0,0),KPF32_PI2,kpVec3f(10,5,4));
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,1),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,-1,0),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(10,5,4),kpReframeGetTranslation(&f));
    kpReframeRotateAroundAxis(&f,kpVec3f(1,0,0),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(1,0,0),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,-1,0),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,0,-1),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(10,-4,5),kpReframeGetTranslation(&f));
    kpReframeRotateAroundAxis(&f,kpVec3f(0,1,0),KPF32_PI2);
    KP_TEST_V3EQ(kpVec3f(0,0,-1),kpReframeGetXAxis(&f));
    KP_TEST_V3EQ(kpVec3f(0,-1,0),kpReframeGetYAxis(&f));
    KP_TEST_V3EQ(kpVec3f(-1,0,0),kpReframeGetZAxis(&f));
    KP_TEST_V3EQ(kpVec3f(5,-4,-10),kpReframeGetTranslation(&f));
  }

  { /* reverse dual quaternion */
    KPdquatf q;
    q = kpDquatfMakeTransform(
      kpVec3fNormalize(kpVec3f(1,2,3)),4,kpVec3f(5,6,7));
    KP_TEST_M4EQ(kpMat4fMakeDquatf(q), kpMat4fMakeDquatf(kpDquatfMakeInverse(kpDquatfMakeInverse(q))));
  }

  printf("Results: ok: %d, fail: %d\n", count_ok, count_fail);
  return 0;
}
