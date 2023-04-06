<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 0: Write and Decrypt Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'030EE1502D8A4F1A83A69D5EFC093A5F8AAEB5AB7DA35E81BE9CC3B7AD33F756'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'5614696693F1A957387D43B52A16598680414FA4675C36C9D22334AB562F82690DCDC35368182EB9FC1E68BB6C0A4BA94E80294331775979336B731E6E42A028'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        [h'00'] / to be decrypted firmware /,
      ]
    } >>,
    / install / 17: << [
      / fetch encrypted firmware /
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'4298ccb320d12c0ae2a8524e34c926ca17fd33e56277c08fe882820b8acf52ed4c529ad1bbf87bbdf8a2897043e8af',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'798655cf5029eedace10985c165e6a39'
          },
          / payload: / null / detached ciphertext /,
          / recipients: / [
            [
              / protected: / << {
              } >>,
              / unprotected: / {
                / alg / 1: -3 / A128KW /,
                / kid / 4: 'kid-1'
              },
              / payload: / h'72760bb6aa4795657e228deaa96c8675dc1b69b54491bea1' / CEK encrypted with KEK /
            ]
          ]
        ]) >>
      },

      / decrypt encrypted firmware /
      / directive-write / 18, 15 / consumes the SUIT_Encryption_Info above /,

      / verify decrypted firmware /
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA-256 /,
          / digest-bytes: / h'efe16b6a486ff25e9fb5fabf515e2bfc3f38b405de377477b23275b53049b46b'
        ] >>,
        / parameter-image-size / 14: 31
      },
      / condition-image-match / 3, 15
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F5820030EE1502D8A4F1A83A69D5EFC093A5F8A
AEB5AB7DA35E81BE9CC3B7AD33F756584AD28443A10126A0F65840561469
6693F1A957387D43B52A16598680414FA4675C36C9D22334AB562F82690D
CDC35368182EB9FC1E68BB6C0A4BA94E80294331775979336B731E6E42A0
28035843A4010102010346A102818141001158338814A0120F14A2035824
822F5820EFE16B6A486FF25E9FB5FABF515E2BFC3F38B405DE377477B232
75B53049B46B0E181F030F
~~~~
