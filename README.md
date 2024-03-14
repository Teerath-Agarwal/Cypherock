# CYPHEROCK

This repository is used to generate additive shares of multiplication of two integers of maximum size 32 bytes.

## Usage

Execute the following commands:

```
$ git clone git@github.com:Teerath-Agarwal/Cypherock.git
$ cd Cypherock/
$ make
# ./run
```

### Input / Output Format

* The first line of input shall contain a single integer `mode` which is either `0` or `1`.
* If the input `mode` is `0`, the program generates two random 256-bit ingeters `a` and `b` and generates the corresponding output.
* If the input `mode` is `1`, the user must input the two ingtegers `a`and `b`, limiting to the max value of unsigned 64-bit integer, ie.e, `18,446,744,073,709,551,615`.
* The output format is self explanatory.

## Theory

We are given two integers `a` and `b`, which represent the multipicative shares of the secret information. We need to find two corresponding integers `c` and `d` such that: `a x b = c + d`

However, this operation must be conducted using Secure Multi-Party Computation so that no secret information is exchanged among the two parties involved, i.e, Alice, the one who hold `a` and returns `c`, and Bob, the one who holds `b` and returns `d`.

To achieve this, we used ECDSA and COT. The Elliptic Curve used here is `secp256k1`. For the communication of 256-bits of data, every time new connection was established with new public and private keys to ensure utmost security. This however, hinders the performance. The user may choose an alternative route to fix the public and private keys for entire period of communication.

## Working and Explanation

This repository uses ECDSA library from [this](https://github.com/trezor/trezor-firmware) repository. The source files have been copied from the `crypto` folder with slight modifications, according to the needs of the project.

There are separate files for the different parties involved, i.e., `Alice` and `Bob`, with scope limited variables used. However, `checker` includes the private variables of `Alice` and `Bob`, i.e., `a`, `b`, `c` and `d`, so that it can validate the result. But note that it is not possible for anyone except `Alice` and the `checker` to access it's private variables, similarly for the `Bob` as well. This ensures the correctness of the algorithm.

### Algorithm

* The main algorithm is executed 256 times for 256-bit input numbers (`a` and `b`).
* Each time a private key for both parties are generated randomly. Call them `k_a` and `k_b.`
* Generate Alice's public key `P_a = k_a * G` where `G` is is the base point of `secp256k1` elliptic curve.
* Generate Bob's public key `P_b = b[i] * P_a + k_b * G` where `b[i]` is i-th LSB bit of `b`.
* Alice generates two keys for encryption of two messages, `k_0 = SHA_256(k_a * P_b)` and `k_1 = SHA_256(k_a * (P_b - P_a))`.
* Alice generates a random integer c[i]. the encrypted messages tranmitted are `M_0(k_0, c[i])` and `M_1(k_1, c[i] + a)`.
* Bob generates his key `k_r = k_b * P_a`, then decrypts the `M_{b[i]}` message from it and stores it as `d[i]`.
* After the loop ends, `c = - sigma (0 to 255) 2^i * c[i]` and `d = sigma (0 to 255) 2^i * d[i]`.
* Checker is called to validate the result.
