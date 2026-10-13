/* A-PRISM OT hooks (Run-14/N9/N5/N20g/T5/N11/N24): header-only helpers + sidecars.
 * Self-contained (cmath/cstdint only, no F360 types) so host harnesses can
 * compile this file standalone. Production call sites are guarded by
 * APRISM_OT_ENABLE (default OFF = bit-identical baseline).
 * Conventions: angles rad, 2-D vectors [x,y], zero heap, bounded loops.
 */
#ifndef APRISM_OT_HOOKS_H
#define APRISM_OT_HOOKS_H

#include <cmath>
#include <cstdint>

namespace f360_variant_A
{

/* XOR contract sentinel: a track stamped with this mirror_prob consumed a ghost
 * measurement this scan; validity must skip mirror penalties for it. */
constexpr float APRISM_ASSIMILATED_MIRROR_PROB = -1.0F;
constexpr float APRISM_OT_MAXDET              = 1024; /* >= MAX_NUMBER_OF_DETECTIONS */
constexpr float APRISM_OT_GHOST_SIGMA_XI      = 0.4F; /* shared reflector sigma [m] */

inline void Aprism_Mark_Assimilated_Track(float &mirror_prob) { mirror_prob = APRISM_ASSIMILATED_MIRROR_PROB; }
inline bool Aprism_Track_Assimilated(float mirror_prob) { return mirror_prob == APRISM_ASSIMILATED_MIRROR_PROB; }
/* Scan-scoped sentinel resolve: -1 -> 0 (not-a-mirror). Call once per track at
 * end of Track_Validity so the sentinel never reaches logs/outputs/next scan.
 * ponytail: sentinel is float-exact (-1.0F assigned, == compared). */
inline void Aprism_Resolve_Assimilated_Track(float &mirror_prob)
{
   if (mirror_prob == APRISM_ASSIMILATED_MIRROR_PROB) mirror_prob = 0.0F;
}
/* Defensive clamp for the log pack path (mirror_prob*100 -> uint8_t). */
inline float Aprism_Clamp01(float v) { return v < 0.0F ? 0.0F : (v > 1.0F ? 1.0F : v); }

/* Per-detection assimilated sidecar (index = global det_props index). */
inline uint8_t &Aprism_Det_Sidecar(uint32_t idx)
{
   static uint8_t seen[1024] = {};
   return seen[idx & 1023U];
}
inline void Aprism_Mark_Assimilated_Det(uint32_t idx) { Aprism_Det_Sidecar(idx) = 1U; }
inline bool Aprism_Det_Assimilated(uint32_t idx) { return Aprism_Det_Sidecar(idx) != 0U; }
inline void Aprism_Reset_Det_Sidecar()
{
   for (uint32_t i = 0U; i < 1024U; i++) {
      Aprism_Det_Sidecar(i) = 0U;
   }
}

/* OT-2 rank-1 invariant residual |p - ps - 2*d*n| (0 when consistent). */
inline float Aprism_Rank1_Residual(float px, float py, float sx, float sy, float nx, float ny, float d)
{
   const float rx = px - sx - 2.0F * d * nx;
   const float ry = py - sy - 2.0F * d * ny;
   return sqrtf(rx * rx + ry * ry);
}

/* Virtual sensor image s* = 2*d*n. */
inline void Aprism_Virtual_Sensor(float nx, float ny, float d, float s_out[2])
{
   s_out[0] = 2.0F * d * nx;
   s_out[1] = 2.0F * d * ny;
}

/* OT-3 adaptive gate, hard-clamped to preserve clutter bounds + runtime. */
inline float Aprism_Adaptive_Gamma(float a_norm, float innov_e, float g0 = 9.21F)
{
   float g = g0 * (1.0F + 0.8F * a_norm + 0.5F * innov_e);
   if (g < 9.21F) g = 9.21F;
   if (g > 40.0F) g = 40.0F;
   return g;
}

/* OT-1/OT-5 coupled reflector inflation: R_eff = R + (J*sig)^2. */
inline float Aprism_Reff_Inflate(float r_base, float jac, float sig_xi) { return r_base + (jac * sig_xi) * (jac * sig_xi); }

/* OT-5 spectral-entropy scaling R(t) = R0*exp(beta*H/SNR), PD-safe floor. */
inline float Aprism_Entropy_Scale(float r0, float entropy_h, float snr, float beta = 1.0F)
{
   if (snr < 1e-6F) snr = 1e-6F;
   const float s = expf(beta * entropy_h / snr);
   return (r0 > 0.0F && s > 0.0F) ? r0 * s : r0;
}
/* OT-5 WSE feed state. Defaults H=0/SNR=1 -> scale 1.0 (identity) until the
 * spray estimator calls Aprism_WSE_Feed. Zero heap, sidecar only. */
struct Aprism_WSE_T {
   float h;
   float snr;
   float beta;
};
inline Aprism_WSE_T &Aprism_WSE_State()
{
   static Aprism_WSE_T s = {0.0F, 1.0F, 1.0F};
   return s;
}
inline void Aprism_WSE_Feed(float h, float snr, float beta = 1.0F)
{
   Aprism_WSE_T &s = Aprism_WSE_State();
   s.h             = h;
   s.snr           = snr;
   s.beta          = beta;
}
inline float Aprism_Entropy_Scale_WSE(float r0)
{
   const Aprism_WSE_T &s = Aprism_WSE_State();
   return Aprism_Entropy_Scale(r0, s.h, s.snr, s.beta);
}

/* OT-6 Doppler gate |meas - u.v| <= 3*sqrt(sig_v^2 + Sv). */
inline bool Aprism_Doppler_Gate(float vr_meas, float ux, float uy, float vx, float vy, float sig_v, float s_v)
{
   const float pred = ux * vx + uy * vy;
   float d          = vr_meas - pred;
   if (d < 0.0F) d = -d;
   return d <= 3.0F * sqrtf(sig_v * sig_v + s_v);
}

/* OT-6 two-scan confirmation counter (per pair id). */
inline int32_t Aprism_Confirm_Update(uint8_t *seen_once, int32_t pair_id, int32_t n_ids, bool gated)
{
   if ((pair_id < 0) || (pair_id >= n_ids) || (seen_once == nullptr)) return 0;
   if (gated) {
      if (seen_once[pair_id] != 0U) {
         seen_once[pair_id] = 0U;
         return 1;
      }
      seen_once[pair_id] = 1U;
   } else {
      seen_once[pair_id] = 0U;
   }
   return 0;
}

/* OT-6 confirmation store (det-indexed, bounded, zero heap). */
inline uint8_t *Aprism_Confirm_Store()
{
   static uint8_t seen[1024] = {};
   return seen;
}

/* OT-4 Hermite cubic from two point-slope pairs; c = monomial coeffs in x.
 * Closed form via shift t = x - x1. Returns |det| scale h^4, 0 if singular. */
inline float Aprism_Hermite2(const float *xs, const float *ys, const float *slopes, float *c)
{
   const float x1 = xs[0], x2 = xs[1], h = x2 - x1;
   if (h == 0.0F) return 0.0F;
   const float h2 = h * h, h3 = h2 * h;
   const float y1 = ys[0], y2 = ys[1], m1 = slopes[0], m2 = slopes[1];
   const float a = (2.0F * (y1 - y2) + h * (m1 + m2)) / h3;
   const float b = (3.0F * (y2 - y1) - h * (2.0F * m1 + m2)) / h2;
   c[3]          = a;
   c[2]          = b - 3.0F * a * x1;
   c[1]          = m1 - 2.0F * b * x1 + 3.0F * a * x1 * x1;
   c[0]          = y1 - m1 * x1 + b * x1 * x1 - a * x1 * x1 * x1;
   return h2 * h2;
}

/* OT-4 curvature bound check |g'| <= 2*kmax*rhomax. */
inline bool Aprism_Curvature_Ok(float gprime, float kmax, float rhomax) { return (gprime < 0.0F ? -gprime : gprime) <= 2.0F * kmax * rhomax; }

/* OT-4 execution heartbeat counter (SiL/qual visibility, no behavior). */
inline uint32_t Aprism_Note_Hermite_Run()
{
   static uint32_t runs = 0U;
   runs++;
   return runs;
}

/* Measurement-count selector: OFF expands to base (bit-identical). */
#if defined(APRISM_OT_ENABLE) && (APRISM_OT_ENABLE == 1)
#define APRISM_OT_COUNT(base, extra) ((base) + (extra))
#else
#define APRISM_OT_COUNT(base, extra) (base)
#endif

/* OT-2 rank-1 pair tolerance [m]. */
constexpr float APRISM_OT_RANK1_TOL = 1.0F;

/* N24 rank gate: explicitly-versioned, quorum-counted gate state. Version 0 =
 * uncalibrated: factor 1.0 (pass-through). Calibration store versions it. */
struct Aprism_Rank_Gate_T {
   uint32_t version;
   float calib_factor;
   uint32_t quorum_need;
   uint32_t quorum_seen;
};
inline Aprism_Rank_Gate_T &Aprism_Rank_Gate_State()
{
   static Aprism_Rank_Gate_T s = {0U, 1.0F, 2U, 0U};
   return s;
}
/* Returns applied factor; counts quorum confirmations (resets on miss). */
inline float Aprism_Rank_Gate_Apply(bool confirmed)
{
   Aprism_Rank_Gate_T &s = Aprism_Rank_Gate_State();
   if (confirmed) {
      s.quorum_seen++;
   } else {
      s.quorum_seen = 0U;
   }
   if ((s.version == 0U) || (s.quorum_seen < s.quorum_need)) return 1.0F;
   return s.calib_factor;
}

} // namespace f360_variant_A

#endif /* APRISM_OT_HOOKS_H */
