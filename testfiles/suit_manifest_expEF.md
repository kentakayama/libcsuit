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
      / digest-bytes: / h'A5F31A6AB9B46D9846131A4102B70F57EA5840CEF0A5BBEFBA9BC2EC28727191'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'B3A3670B9AE96EB27E1C1A10700F8355FB9BCA5C83EC8B713FDBC84D4D917A87C6D928F965427E6AB46505D0B60BBE4F65EC2A33ADCDB7571FD055579C73D944'
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
D86BA2025873825824822F5820A5F31A6AB9B46D9846131A4102B70F57EA
5840CEF0A5BBEFBA9BC2EC28727191584AD28443A10126A0F65840B3A367
0B9AE96EB27E1C1A10700F8355FB9BCA5C83EC8B713FDBC84D4D917A87C6
D928F965427E6AB46505D0B60BBE4F65EC2A33ADCDB7571FD055579C73D9
440358CDA4010102010349A102828141008141011158BA900C0114A21578
3168747470733A2F2F617574686F722E6578616D706C652E636F6D2F656E
637279707465642D6669726D776172652E62696E0E182F150F0C0014A216
01135843D8608443A10101A10550798655CF5029EEDACE10985C165E6A39
F6818341A0A2012204456B69642D31581872760BB6AA4795657E228DEAA9
6C8675DC1B69B54491BEA1160F14A2035824822F5820EFE16B6A486FF25E
9FB5FABF515E2BFC3F38B405DE377477B23275B53049B46B0E181F030F
~~~~
