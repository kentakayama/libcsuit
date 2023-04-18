<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 1: Fetch and Decrypt Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'4F3975683AA91BC2811830C553992FE84FE1EBE7187ECDCF2B5FBF72B1754331'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'700E8CC7A8CC9546C44926CAB314CCDB7BD1019AE3676F415E11E384C9A32BF4E439612D1AB28F7841B5847CFBECA36578EF550D0E5DC762018C334798959741'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        [h'00'] / to be decrypted firmware /,
        [h'01'] / encrypted firmware /
      ]
    } >>,
    / install / 17: << [
      / fetch encrypted firmware /
      / directive-set-component-index / 12, 1 / [h'01'] /,
      / directive-override-parameters / 20, {
        / parameter-image-size / 14: 47,
        / parameter-uri / 21: "https://author.example.com/encrypted-firmware.bin"
      },
      / directive-fetch / 21, 15,

      / decrypt encrypted firmware /
      / directive-set-component-index / 12, 0 / [h'00'] /,
      / directive-override-parameters / 20, {
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'1DE460E8B5B68D7222C0D6F20484D8AB'
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
              / payload: / h'A86200E4754733E4C00FC08C6A72CC1996E129922EAB504F' / CEK encrypted with KEK /
            ]
          ]
        ]) >>,
        / parameter-source-component / 22: 1 / [h'01'] /
      },
      / directive-copy / 22, 15 / consumes the SUIT_Encryption_Info above /,

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
D86BA2025873825824822F58204F3975683AA91BC2811830C553992FE84F
E1EBE7187ECDCF2B5FBF72B1754331584AD28443A10126A0F65840700E8C
C7A8CC9546C44926CAB314CCDB7BD1019AE3676F415E11E384C9A32BF4E4
39612D1AB28F7841B5847CFBECA36578EF550D0E5DC762018C3347989597
410358CDA4010102010349A102828141008141011158BA900C0114A20E18
2F15783168747470733A2F2F617574686F722E6578616D706C652E636F6D
2F656E637279707465642D6669726D776172652E62696E150F0C0014A213
5843D8608443A10101A105501DE460E8B5B68D7222C0D6F20484D8ABF681
8341A0A2012204456B69642D315818A86200E4754733E4C00FC08C6A72CC
1996E129922EAB504F1601160F14A2035824822F5820EFE16B6A486FF25E
9FB5FABF515E2BFC3F38B405DE377477B23275B53049B46B0E181F030F
~~~~
