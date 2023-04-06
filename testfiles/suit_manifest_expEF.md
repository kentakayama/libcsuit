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
      / digest-bytes: / h'65576167AF903B981C419CDAC9B5F63C3A697927CC7FBF81CD6E34F4A70BCB60'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'A8C807FF03015088492C9C3B0779A771502639676E7D27C3D33E8432037FA9D1FE948D3C4C82778A6E2B1ADA74287809526014A2BACFDC64600F63847956AE6F'
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
        / parameter-uri / 21: "https://author.example.com/encrypted-firmware.bin",
        / parameter-image-size / 14: 47
      },
      / directive-fetch / 21, 15,

      / decrypt encrypted firmware /
      / directive-set-component-index / 12, 0 / [h'00'] /,
      / directive-override-parameters / 20, {
        / parameter-source-component / 22: 1 / [h'01'] /,
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'1de460e8b5b68d7222c0d6f20484d8ab'
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
              / payload: / h'a86200e4754733e4c00fc08c6a72cc1996e129922eab504f' / CEK encrypted with KEK /
            ]
          ]
        ]) >>
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
D86BA2025873825824822F582065576167AF903B981C419CDAC9B5F63C3A
697927CC7FBF81CD6E34F4A70BCB60584AD28443A10126A0F65840A8C807
FF03015088492C9C3B0779A771502639676E7D27C3D33E8432037FA9D1FE
948D3C4C82778A6E2B1ADA74287809526014A2BACFDC64600F63847956AE
6F0358CDA4010102010349A102828141008141011158BA900C0114A21578
3168747470733A2F2F617574686F722E6578616D706C652E636F6D2F656E
637279707465642D6669726D776172652E62696E0E182F150F0C0014A216
01135843D8608443A10101A105501DE460E8B5B68D7222C0D6F20484D8AB
F6818341A0A2012204456B69642D315818A86200E4754733E4C00FC08C6A
72CC1996E129922EAB504F160F14A2035824822F5820EFE16B6A486FF25E
9FB5FABF515E2BFC3F38B405DE377477B23275B53049B46B0E181F030F
~~~~
