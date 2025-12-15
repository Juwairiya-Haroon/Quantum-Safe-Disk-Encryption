#include "ShamirSSS.h"
#include <random>

static uint8_t gf_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    while (b) {
        if (b & 1) p ^= a;
        a = (a << 1) ^ (a & 0x80 ? 0x1b : 0);
        b >>= 1;
    }
    return p;
}

static uint8_t gf_inv(uint8_t a) {
    uint8_t t = a, r = 1;
    for (int i = 0; i < 254; i++) r = gf_mul(r, t);
    return r;
}

std::vector<Share> ShamirSSS::splitSecret(
    const std::vector<uint8_t>& secret,
    int k, int n
) {
    std::vector<Share> shares(n);
    std::random_device rd;

    for (size_t i = 0; i < secret.size(); i++) {
        std::vector<uint8_t> coeffs(k);
        coeffs[0] = secret[i];
        for (int j = 1; j < k; j++)
            coeffs[j] = rd();

        for (int x = 1; x <= n; x++) {
            uint8_t y = 0;
            for (int j = k - 1; j >= 0; j--)
                y = gf_mul(y, x) ^ coeffs[j];

            shares[x - 1].index = x;
            shares[x - 1].value.push_back(y);
        }
    }
    return shares;
}

std::vector<uint8_t> ShamirSSS::recoverSecret(
    const std::vector<Share>& shares,
    int k
) {
    std::vector<uint8_t> secret(shares[0].value.size(), 0);

    for (size_t i = 0; i < secret.size(); i++) {
        uint8_t s = 0;
        for (int j = 0; j < k; j++) {
            uint8_t num = 1, den = 1;
            for (int m = 0; m < k; m++) {
                if (m != j) {
                    num = gf_mul(num, shares[m].index);
                    den = gf_mul(den,
                        shares[m].index ^ shares[j].index);
                }
            }
            s ^= gf_mul(shares[j].value[i], gf_mul(num, gf_inv(den)));
        }
        secret[i] = s;
    }
    return secret;
}
