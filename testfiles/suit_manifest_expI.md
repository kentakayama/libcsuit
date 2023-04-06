<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 2: SUIT Manifest including the Trusted Component Binary {#suit-integrated}

~~~~
/ SUIT_Envelope / {
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'C20DE9B1378EC787D9A9F7C3B0F196E4C51FB519E3FFD45A6B985CE3E9222993'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'72F4E0CC6A4A6128953ABBBB7E73475E66F10E1C3A43F130D4961D21124A903AB7BAA002CF23D75923C82FF119365D364CCB8E81A16E9B13D5E662850BCA4D65'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 3,
    / common / 3: << {
      / components / 2: [
        [
           'TEEP-Device',
           'SecureFS',
          h'8D82573A926D4754935332DC29997F74', / tc-uuid /
           'ta'
        ]
      ],
      / shared-sequence / 4: << [
        / directive-override-parameters / 20, {
          / parameter-vendor-identifier / 1: h'C0DDD5F15243566087DB4F5B0AA26C2F',
          / parameter-class-identifier / 2: h'DB42F7093D8C55BAA8C5265FC5820F4E',
          / parameter-image-digest / 3: << [
            / digest-algorithm-id: / -16 / SHA256 /,
            / digest-bytes: / h'8CF71AC86AF31BE184EC7A05A411A8C3A14FD9B77A30D046397481469468ECE8'
          ] >>,
          / parameter-image-size / 14: 20
        },
        / condition-vendor-identifier / 1, 15,
        / condition-class-identifier / 2, 15
      ] >>
    } >>,
    / manifest-component-id / 5: [
       'TEEP-Device',
       'SecureFS',
      h'8D82573A926D4754935332DC29997F74',  / tc-uuid /
       'suit'
    ],
    / install / 17: << [
      / directive-override-parameters / 20, {
        / uri / 21: "#tc"
      },
      / directive-fetch / 21, 15,
      / condition-image-match / 3, 15
    ] >>,
    / uninstall / 24: << [
      / directive-unlink / 33, 15
    ] >>
  } >>,
  "#tc" : 'Hello, Secure World!'
}
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
A3025873825824822F5820C20DE9B1378EC787D9A9F7C3B0F196E4C51FB5
19E3FFD45A6B985CE3E9222993584AD28443A10126A0F6584072F4E0CC6A
4A6128953ABBBB7E73475E66F10E1C3A43F130D4961D21124A903AB7BAA0
02CF23D75923C82FF119365D364CCB8E81A16E9B13D5E662850BCA4D6503
58CEA601010203035884A20281844B544545502D44657669636548536563
7572654653508D82573A926D4754935332DC29997F744274610458548614
A40150C0DDD5F15243566087DB4F5B0AA26C2F0250DB42F7093D8C55BAA8
C5265FC5820F4E035824822F58208CF71AC86AF31BE184EC7A05A411A8C3
A14FD9B77A30D046397481469468ECE80E14010F020F05844B544545502D
446576696365485365637572654653508D82573A926D4754935332DC2999
7F744473756974114C8614A11563237463150F030F1818448218210F6323
74635448656C6C6F2C2053656375726520576F726C6421
~~~~
