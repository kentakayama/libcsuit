<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## B.5. Example 4: Load from External Storage

https://tools.ietf.org/html/draft-ietf-suit-manifest-27#appendix-B.5

{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope = / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'5b5f6586b1e6cdf19ee479a5adabf206581000bd584b0832a9bdaf4f72cdbdd6'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -9 / ESP256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'57efbf4db6d7f40e12cc7f9c3601a92d16c25486d645d1d8651cbefc8cc3baec09b0e17ed13f6ff29ef94b6658136af1e33a90057601131d191b8c41c5d4d309'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 4,
    / common / 3: << {
      / components / 2: [
        [h'00'],
        [h'02'],
        [h'01']
      ],
      / shared-sequence / 4: << [
        / directive-set-component-index / 12, 0,
        / directive-override-parameters / 20, {
          / vendor-id / 1: h'fa6b4a53d5ad5fdfbe9de663e4d41ffe' / fa6b4a53-d5ad-5fdf-be9d-e663e4d41ffe /,
          / class-id / 2: h'1492af1425695e48bf429b2d51f2ab45' / 1492af14-2569-5e48-bf42-9b2d51f2ab45 /,
          / image-digest / 3: << [
            / algorithm-id: / -16 / SHA-256 /,
            / digest-bytes: / h'00112233445566778899aabbccddeeff0123456789abcdeffedcba9876543210'
          ] >>,
          / image-size / 14: 34768
        },
        / condition-vendor-identifier / 1, 15,
        / condition-class-identifier / 2, 15
      ] >>
    } >>,
    / validate / 7: << [
      / directive-set-component-index / 12, 0,
      / condition-image-match / 3, 15
    ] >>,
    / load / 8: << [
      / directive-set-component-index / 12, 2,
      / directive-override-parameters / 20, {
        / image-digest / 3: << [
          / algorithm-id: / -16 / SHA-256 /,
          / digest-bytes: / h'0123456789abcdeffedcba987654321000112233445566778899aabbccddeeff'
        ] >>,
        / image-size / 14: 76834,
        / source-component / 22: 0
      },
      / directive-copy / 22, 2,
      / condition-image-match / 3, 15
    ] >>,
    / invoke / 9: << [
      / directive-set-component-index / 12, 2,
      / directive-invoke / 23, 2
    ] >>,
    / payload-fetch / 16: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / image-digest / 3: << [
          / algorithm-id: / -16 / SHA-256 /,
          / digest-bytes: / h'00112233445566778899aabbccddeeff0123456789abcdeffedcba9876543210'
        ] >>,
        / uri / 21: "http://example.com/file.bin"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 20: << [
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / source-component / 22: 1
      },
      / directive-copy / 22, 2,
      / condition-image-match / 3, 15
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
d86ba2025873825824822f58205b5f6586b1e6cdf19ee479a5adabf20658
1000bd584b0832a9bdaf4f72cdbdd6584ad28443a10128a0f6584057efbf
4db6d7f40e12cc7f9c3601a92d16c25486d645d1d8651cbefc8cc3baec09
b0e17ed13f6ff29ef94b6658136af1e33a90057601131d191b8c41c5d4d3
0903590116a801010204035867a20283814100814102814101045858880c
0014a40150fa6b4a53d5ad5fdfbe9de663e4d41ffe02501492af1425695e
48bf429b2d51f2ab45035824822f582000112233445566778899aabbccdd
eeff0123456789abcdeffedcba98765432100e1987d0010f020f0745840c
00030f085838880c0214a3035824822f58200123456789abcdeffedcba98
7654321000112233445566778899aabbccddeeff0e1a00012c2216001602
030f0945840c02170210584e880c0114a2035824822f5820001122334455
66778899aabbccddeeff0123456789abcdeffedcba987654321015781b68
7474703a2f2f6578616d706c652e636f6d2f66696c652e62696e1502030f
144b880c0014a116011602030f
~~~~
