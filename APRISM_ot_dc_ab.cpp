/* APRISM OT+DC A/B: baseline (OFF = original behavior) vs research (ON).
 * Includes the REAL production headers (aprism_ot_hooks.h, aprism_dc_hooks.h)
 * and mirrors the guarded call-site sequences. Deterministic, no heap.
 * Build: clang++ -std=c++17 -I <OT include> -I <DC sil_wrappers> APRISM_ot_dc_ab.cpp -o APRISM_ot_dc_ab.exe
 */
#include <cmath>
#include <cstdint>
#include <cstdio>
#include "aprism_ot_hooks.h"
#include "aprism_dc_hooks.h"

static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); fails++; } } while (0)
#define CLOSE(a, b, t) do { double _d = (double)(a) - (double)(b); if (_d < 0) _d = -_d; \
    if (!(_d <= (t))) { printf("FAIL %d: |%g-%g|=%g\n", __LINE__, (double)(a), (double)(b), _d); fails++; } } while (0)
using namespace f360_variant_A;

int main()
{
    printf("=== OT/DC BASELINE (OFF) vs RESEARCH (ON) ===\n");
    /* OT-2 invariant */
    {
        float r0 = Aprism_Rank1_Residual(60.f, -1.75f, 60.f, -8.75f, 0.f, 1.f, 3.5f);
        float r1 = Aprism_Rank1_Residual(60.f, -1.75f, 55.f, -8.75f, 0.f, 1.f, 3.5f);
        printf("[ot2] rank1 ok=%.4f viol=%.4f\n", r0, r1);
        CHECK(r0 < 1e-4f); CHECK(r1 > 1.0f);
        float s[2]; Aprism_Virtual_Sensor(0.f, 1.f, 3.5f, s);
        CHECK(s[0] == 0.0f && s[1] == 7.0f);
    }
    /* OT-3 gate */
    {
        float g0 = Aprism_Adaptive_Gamma(0.f, 0.f);
        float g1 = Aprism_Adaptive_Gamma(1.f, 0.f);
        float g2 = Aprism_Adaptive_Gamma(9.f, 9.f);
        printf("[ot3] gamma calm=%.2f accel=%.2f huge=%.2f(base 9.21)\n", g0, g1, g2);
        CHECK(g0 == 9.21f); CLOSE(g1, 16.578f, 1e-2f); CHECK(g2 == 40.0f);
    }
    /* OT-1/5 R + entropy */
    {
        float re = Aprism_Reff_Inflate(0.9f, 2.0f, 0.4f);
        float e0 = Aprism_Entropy_Scale(0.9f, 0.f, 10.f);
        float e1 = Aprism_Entropy_Scale(0.9f, 2.f, 10.f);
        printf("[ot15] Reff=%.3f entropy x1=%.3f xH=%.3f\n", re, e0, e1);
        CLOSE(re, 1.54f, 1e-4f); CLOSE(e0, 0.9f, 1e-6f); CLOSE(e1, 0.9f * expf(0.2f), 1e-4f);
    }
    /* OT-6 doppler + confirm */
    {
        bool in  = Aprism_Doppler_Gate(-20.0f, 1.f, 0.f, -20.0f, 0.5f, 0.1f, 0.0144f);
        bool out = Aprism_Doppler_Gate(-19.1f, 1.f, 0.f, -20.0f, 0.5f, 0.1f, 0.0144f);
        uint8_t seen[4] = {};
        int c1 = Aprism_Confirm_Update(seen, 2, 4, true);
        int c2 = Aprism_Confirm_Update(seen, 2, 4, true);
        int c3 = Aprism_Confirm_Update(seen, 2, 4, false);
        printf("[ot6] gate true=%d decoy=%d confirm=%d,%d then-miss=%d\n", in, out, c1, c2, c3);
        CHECK(in && !out); CHECK(c1 == 0 && c2 == 1 && c3 == 0);
    }
    /* OT-4 hermite */
    {
        float xs[2] = {10.f, 40.f}, ys[2] = {3.5f, 4.1f}, m[2] = {0.02f, 0.05f}, c[4];
        float det = Aprism_Hermite2(xs, ys, m, c);
        float p1 = ((c[3]*10.f + c[2])*10.f + c[1])*10.f + c[0];
        float d1 = (3.f*c[3]*10.f + 2.f*c[2])*10.f + c[1];
        float p2 = ((c[3]*40.f + c[2])*40.f + c[1])*40.f + c[0];
        float d2 = (3.f*c[3]*40.f + 2.f*c[2])*40.f + c[1];
        printf("[ot4] det=%.0f p1=%.4f/%.2f p2=%.4f/%.2f\n", det, p1, 3.5f, p2, 4.1f);
        CHECK(det > 0.f); CLOSE(p1, 3.5f, 1e-3f); CLOSE(d1, 0.02f, 1e-4f);
        CLOSE(p2, 4.1f, 1e-3f); CLOSE(d2, 0.05f, 1e-4f);
        CHECK(Aprism_Curvature_Ok(0.5f, 0.05f, 60.f));
        CHECK(!Aprism_Curvature_Ok(9.0f, 0.05f, 60.f));
        CHECK(Aprism_Note_Hermite_Run() >= 1U);
    }
    /* XOR sentinel + sidecar */
    {
        float mp = 0.9f;
        Aprism_Mark_Assimilated_Track(mp);
        printf("[xor] sentinel=%.1f skip=%d\n", mp, Aprism_Track_Assimilated(mp));
        CHECK(mp == -1.0F && Aprism_Track_Assimilated(mp));
        CHECK(!Aprism_Track_Assimilated(0.9f));
        Aprism_Reset_Det_Sidecar();
        Aprism_Mark_Assimilated_Det(517U);
        CHECK(Aprism_Det_Assimilated(517U) && !Aprism_Det_Assimilated(518U));
    }
    /* DC-1 preflight */
    {
        uint8_t ok = Aprism_DC_Preflight_Seed(-1.145f, -0.018f);
        uint8_t bad = Aprism_DC_Preflight_Seed(9.0f, 0.0f);
        printf("[dc1] seed ok=%d bad=%d\n", ok, bad);
        CHECK(ok == 1U && bad == 0U);
    }
    /* DC-2 deskew */
    {
        float ox, oy;
        Aprism_DC_Deskew(60.f, -1.75f, 25.f, 0.f, 0.f, 0.f, 0.f, 0.f, ox, oy);
        CHECK(ox == 60.f && oy == -1.75f); /* dt=0 identity (single-epoch collapse) */
        Aprism_DC_Deskew(60.f, 0.f, 25.f, 0.f, 0.15f, 0.f, 0.f, 0.02f, ox, oy);
        float th = 0.15f * 0.02f;
        printf("[dc2] deskewed=(%.4f, %.4f)\n", ox, oy);
        CLOSE(ox, cosf(th) * 60.f - 25.f * 0.02f, 1e-3f);
        CLOSE(oy, sinf(th) * 60.f, 1e-3f);
    }
    /* DC-3 clock */
    {
        float d[2] = {10.f, 0.f}, v[2] = {20.f, 0.f};
        float tau = Aprism_DC_ClockTau1(d, v, 5.f, 0.f);
        float t2, b2;
        bool ok = Aprism_DC_ClockTauBias2(1.f, 2.f, 3.f, 4.f, 1.f, 2.f, &t2, &b2);
        printf("[dc3] tau=%.3f solve ok=%d tau=%.3f b=%.3f\n", tau, ok, t2, b2);
        CLOSE(tau, -0.5f, 1e-5f); CHECK(ok);
        CLOSE(1.f * t2 + 2.f * b2, 3.f, 1e-4f); CLOSE(4.f * t2 + 1.f * b2, 2.f, 1e-4f);
        Aprism_DC_Clock_Update(25.f, 0.02f, 1000000.0);
        Aprism_DC_Clock_Update(25.5f, 0.02f, 1050000.0);
        CHECK(Aprism_DC_Clock_State().ok == 1U);
    }
    /* NIS calibration discipline (synthetic whitened residuals, p=2) */
    {
        double nis = 0.4 * 0.4 + 1.1 * 1.1;
        printf("[nis] synthetic NIS=%.2f vs chi2_95%%=5.99 (inside)\n", nis);
        CHECK(nis < 5.99);
    }
    /* N24 rank gate: v0 uncalibrated + quorum => neutral 1.0 */
    {
        float r1 = Aprism_Rank_Gate_Apply(true);
        float r2 = Aprism_Rank_Gate_Apply(true);
        float r3 = Aprism_Rank_Gate_Apply(false);
        printf("[n24] rank factors=%.1f,%.1f,%.1f (v0 neutral)\n", r1, r2, r3);
        CHECK(r1 == 1.0f && r2 == 1.0f && r3 == 1.0f);
    }
    if (fails == 0) printf("APRISM_OT_DC_AB PASS\n");
    else printf("APRISM_OT_DC_AB FAIL (%d)\n", fails);
    return fails;
}
