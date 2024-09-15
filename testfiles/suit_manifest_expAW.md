<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 0: Write and Decrypt AES-KW + Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'813E7A9902E6665D85CAF9172CBE8652AD43EF5AE2362DA068DE51DF7E2D6C17'
    ] >>,
    << / COSE_Mac0_Tagged / 17([
      / protected: / << {
        / algorithm-id / 1: 5 / HMAC256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / tag: / h'7C1E5F84712CF3D781A0925D61AB8B10FD9CEC4EEE26739D35D28302E17187F3'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        ['plaintext-firmware']
      ]
    } >>,
    / install / 20: << [
      / fetch encrypted firmware /
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'2F59C3A34D9570FB99A5382E66466A3221A8AD85CE508BA306FB431A60EFA5AAAA078355070205A4B196832DF17F',
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
        ]) >>
      },

      / decrypt encrypted firmware /
      / directive-write / 18, 15
        / consumes the SUIT_Encryption_Info above /
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025853825824822F5820813E7A9902E6665D85CAF9172CBE8652AD
43EF5AE2362DA068DE51DF7E2D6C17582AD18443A10105A0F658207C1E5F
84712CF3D781A0925D61AB8B10FD9CEC4EEE26739D35D28302E17187F303
589CA4010102010357A102818152706C61696E746578742D6669726D7761
726514587B8414A212582E2F59C3A34D9570FB99A5382E66466A3221A8AD
85CE508BA306FB431A60EFA5AAAA078355070205A4B196832DF17F135842
D8608443A10101A10550F14AAB9D81D51F7AD943FE87AF4F70CDF6818340
A2012204456B69642D31581875603FFC9518D794713C8CA8A115A7FB3256
5A6D59534D62120F
~~~~
