<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 1: SUIT Manifest pointing to URI of the Trusted Component Binary {#suit-uri}
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope / {
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'B39B52B0B747EA79588C190F567BFC2C8437BA8A73F7EA983182E79F0148D59B'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'80E54AB485B320A61654666362928B15EAAABFE6957B1BCB65F16A367E4B19888BFFDBD6F7EA2892FA36FA18A2FCB5DBFEC9832E09B91ED9CD348AB77E25FA74'
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
    / install / 20: << [
      / directive-override-parameters / 20, {
        / parameter-uri / 21: "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.ta"
      },
      / directive-fetch / 21, 15,
      / condition-image-match / 3, 15
    ] >>,
    / uninstall / 24: << [
      / directive-unlink / 33, 15
    ] >>
  } >>
}
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
A2025873825824822F5820B39B52B0B747EA79588C190F567BFC2C8437BA
8A73F7EA983182E79F0148D59B584AD28443A10126A0F6584080E54AB485
B320A61654666362928B15EAAABFE6957B1BCB65F16A367E4B19888BFFDB
D6F7EA2892FA36FA18A2FCB5DBFEC9832E09B91ED9CD348AB77E25FA7403
590108A601010203035884A20281844B544545502D446576696365485365
637572654653508D82573A926D4754935332DC29997F7442746104585486
14A40150C0DDD5F15243566087DB4F5B0AA26C2F0250DB42F7093D8C55BA
A8C5265FC5820F4E035824822F58208CF71AC86AF31BE184EC7A05A411A8
C3A14FD9B77A30D046397481469468ECE80E14010F020F05844B54454550
2D446576696365485365637572654653508D82573A926D4754935332DC29
997F7444737569741458458614A115783B68747470733A2F2F6578616D70
6C652E6F72672F38643832353733612D393236642D343735342D39333533
2D3332646332393939376637342E7461150F030F1818448218210F
~~~~
