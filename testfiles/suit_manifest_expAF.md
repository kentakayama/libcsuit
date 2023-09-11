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
      / digest-bytes: / h'9300AD376FE65C505593C82B78F14299BD0125A477720C044AD0552ABD27AAF6'
    ] >>,
    << / COSE_Mac0_Tagged / 17([
      / protected: / << {
        / algorithm-id / 1: 5 / HMAC256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'F48509F3027EEAA2C40473212C3A12F25A9C8BE6699E5E7936816836E91A9003'
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
    / install / 17: << [
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
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'93702C81590F845D9EC866CCAC767BD1'
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
              / payload: / h'CA0DF4EE01796D12FE379165602E0AC2476391A4EF122A64' / CEK encrypted with KEK /
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
D86BA2025853825824822F58209300AD376FE65C505593C82B78F14299BD
0125A477720C044AD0552ABD27AAF6582AD18443A10105A0F65820F48509
F3027EEAA2C40473212C3A12F25A9C8BE6699E5E7936816836E91A900303
58B7A40101020103582BA102828152706C61696E746578742D6669726D77
6172658152656E637279707465642D6669726D776172651158818C0C0114
A20E182E15782668747470733A2F2F6578616D706C652E636F6D2F656E63
7279707465642D6669726D77617265150F0C0014A2135843D8608443A101
01A1055093702C81590F845D9EC866CCAC767BD1F6818341A0A201220445
6B69642D315818CA0DF4EE01796D12FE379165602E0AC2476391A4EF122A
641601160F
~~~~
