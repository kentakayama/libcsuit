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
      / digest-bytes: / h'526A85341DE35AFA4FAF9EDDDA40164525077DC45DFBE25785B9FF40683EE881'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'4B57A8102D0D86B83BA0368E118917D87DBF7815DC31B19DEB7E154F3D191A1434ADFAE27D5AED39C07A2A4B2A0D78031E73B23D679507C4953DD9E00CA7E541'
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
A3025873825824822F5820526A85341DE35AFA4FAF9EDDDA40164525077D
C45DFBE25785B9FF40683EE881584AD28443A10126A0F658404B57A8102D
0D86B83BA0368E118917D87DBF7815DC31B19DEB7E154F3D191A1434ADFA
E27D5AED39C07A2A4B2A0D78031E73B23D679507C4953DD9E00CA7E54103
58CEA601010203035884A20281844B544545502D44657669636548536563
7572654653508D82573A926D4754935332DC29997F744274610458548614
A40150C0DDD5F15243566087DB4F5B0AA26C2F0250DB42F7093D8C55BAA8
C5265FC5820F4E035824822F58208CF71AC86AF31BE184EC7A05A411A8C3
A14FD9B77A30D046397481469468ECE80E14010F020F05844B544545502D
446576696365485365637572654653508D82573A926D4754935332DC2999
7F744473756974114C8614A11563237463150F030F1818448218210F6323
74635448656C6C6F2C2053656375726520576F726C6421
~~~~
