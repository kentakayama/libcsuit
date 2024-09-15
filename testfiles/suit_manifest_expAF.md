<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 1: Fetch and Decrypt AES-KW + Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'5935D959064FC19D8C6E63B5843F80967368905D4A6D052339F1647B049E4E5D'
    ] >>,
    << / COSE_Mac0_Tagged / 17([
      / protected: / << {
        / algorithm-id / 1: 5 / HMAC256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / tag: / h'A18B371E7DAB7C9F361758EAF8C69EFBA7F2B4D7E5747D57AB3B12828909A4A7'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        ['plaintext-firmware'],
        ['encrypted-firmware']
      ]
    } >>,
    / install / 20: << [
      / fetch encrypted firmware /
      / directive-set-component-index / 12, 1 / ['encrypted-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-image-size / 14: 46,
        / parameter-uri / 21: "https://example.com/encrypted-firmware"
      },
      / directive-fetch / 21, 15,

      / decrypt encrypted firmware /
      / directive-set-component-index / 12, 0 / ['plaintext-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / A128GCM /
          } >>,
          / unprotected: / {
            / IV / 5: h'F14AAB9D81D51F7AD943FE87AF4F70CD'
          },
          / payload: / null / detached ciphertext /,
          / recipients: / [
            [
              / protected: / h'',
              / unprotected: / {
                / alg / 1: -3 / A128KW /,
                / kid / 4: 'kid-1'
              },
              / payload: / h'75603FFC9518D794713C8CA8A115A7FB32565A6D59534D62' / CEK encrypted with KEK /
            ]
          ]
        ]) >>,
        / parameter-source-component / 22: 1 / ['encrypted-firmware'] /
      },
      / directive-copy / 22, 15 / consumes the SUIT_Encryption_Info above /
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025853825824822F58205935D959064FC19D8C6E63B5843F809673
68905D4A6D052339F1647B049E4E5D582AD18443A10105A0F65820A18B37
1E7DAB7C9F361758EAF8C69EFBA7F2B4D7E5747D57AB3B12828909A4A703
58B6A40101020103582BA102828152706C61696E746578742D6669726D77
6172658152656E637279707465642D6669726D776172651458808C0C0114
A20E182E15782668747470733A2F2F6578616D706C652E636F6D2F656E63
7279707465642D6669726D77617265150F0C0014A2135842D8608443A101
01A10550F14AAB9D81D51F7AD943FE87AF4F70CDF6818340A2012204456B
69642D31581875603FFC9518D794713C8CA8A115A7FB32565A6D59534D62
1601160F
~~~~
