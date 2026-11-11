# Hospital Management System — Security Architecture & Threat Model

**Owner:** Pranav Swaroop (Team 14)  
**Implementation:** `src/security/Security.cpp`  
**Public Header:** `include/hms/Security.h`

---

## 1. Security Objectives & Threat Model

The Hospital Management System processes sensitive institutional records, including user credentials and patient medical metadata. The security architecture addresses three primary threats:

1. **Credential Exposure via Storage Leakage:** If raw persistence files (`users.txt`) are accessed or leaked, credentials must remain unrecoverable. Plaintext passwords are never held beyond authentication input.
2. **Rainbow Table & Dictionary Pre-computation:** Storing unsalted or deterministic hashes allows offline lookup attacks.
3. **Side-Channel Timing Attacks:** Early-exit string comparisons in password verification leak information about password prefix accuracy.

---

## 2. Password Hashing Specification

Passwords are transformed using a salted, multi-round mixing algorithm:

$$\text{Stored Format} = \text{HexSalt} \mathbin{\Vert} \text{"\$"} \mathbin{\Vert} \text{HexHash}$$

### A. Salt Generation
* **Entropy Source:** 64-bit random salt produced by hardware-seeded `std::random_device`.
* **Representation:** 16-character hexadecimal string.
* **Uniqueness:** Guarantees that two identical passwords generate completely different hashes.

### B. Multi-Round Bit-Mixing (`mixHash`)
* Salt and password are concatenated into an initial message.
* A 64-round cyclic permutation loop applies:
  - FNV prime multiplication ($1099511628211\text{ULL}$)
  - 13-bit circular rotations
  - Round-dependent bitwise XOR permutations
* The final state is formatted as a 16-character hexadecimal string.

---

## 3. Timing-Attack Defense (`verifyPassword`)

Traditional string equality operators (`==`) exit on the first mismatched character, creating subtle latency variations that allow attackers to deduce passwords character by character.

To prevent this side channel, verification computes an XOR accumulator across the entire digest:

```cpp
unsigned char diff = 0;
for (size_t i = 0; i < expected.size(); ++i) {
    diff |= static_cast<unsigned char>(expected[i] ^ actual[i]);
}
return diff == 0;
```

Execution time remains uniform regardless of how many initial characters match.

---

## 4. Role-Based Access Control (RBAC)

User privileges are enforced at the menu router level:
* **Admin:** User registration, system account audits, administrative menus.
* **Receptionist:** Patient registration, scheduling appointments, billing.
* **Doctor:** Viewing assigned schedules and medical records.
