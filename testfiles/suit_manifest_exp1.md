<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## B.2. Example 1: Simultaneous Download and Installation of Payload
https://tools.ietf.org/html/draft-ietf-suit-manifest-27#appendix-B.2

{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope = / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'1f2e7acca0dc2786f2fe4eb947f50873a6a3cfaa98866c5b02e621f42074daf2'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -9 / ESP256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'99149241cc392690aa15590cc0d97b8bf5084b3efbf2ca82f33179eed6e7370e44517bdd75f5bab97773fca4601e1ada8a5068619de0c648c1208e5a16141d21'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        [h'00']
      ],
      / shared-sequence / 4: << [
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
      / condition-image-match / 3, 15
    ] >>,
    / install / 20: << [
      / directive-override-parameters / 20, {
        / uri / 21: "http://example.com/file.bin"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
d86ba2025873825824822f58201f2e7acca0dc2786f2fe4eb947f50873a6
a3cfaa98866c5b02e621f42074daf2584ad28443a10128a0f65840991492
41cc392690aa15590cc0d97b8bf5084b3efbf2ca82f33179eed6e7370e44
517bdd75f5bab97773fca4601e1ada8a5068619de0c648c1208e5a16141d
21035894a50101020103585fa202818141000458568614a40150fa6b4a53
d5ad5fdfbe9de663e4d41ffe02501492af1425695e48bf429b2d51f2ab45
035824822f582000112233445566778899aabbccddeeff0123456789abcd
effedcba98765432100e1987d0010f020f074382030f1458258614a11578
1b687474703a2f2f6578616d706c652e636f6d2f66696c652e62696e1502
030f
~~~~
