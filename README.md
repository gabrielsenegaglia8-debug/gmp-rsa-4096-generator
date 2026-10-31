# gmp-rsa-4096-generator — 0.18s

fast RSA 4096-bit keypair generator in C++ / GMP

No OpenSSL. Direct GMP prime generation with Miller-Rabin.

## Bench (MSYS2 UCRT64, Intel)
- `bench.txt`: 5 runs — avg ~0.19s
- Fastest: 0.184s | ouro.txt generated in 0.359s
- Output: p, q, N (617 / 617 / 1233 digits)

## Proof
- `proof/proof.txt` contains verifiable p, q, N
- `src/ouro.cpp` — full source (3.1K)

Author: Gabriel Senegaglia — Joinville, SC
