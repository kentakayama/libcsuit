<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 3: Supplying Personalization Data for Trusted Component Binary {#suit-personalization}

~~~~
/ SUIT_Envelope / {
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'DDFC55B9FF63C1A27A7DA1CFB64DC924ACD5EFBAD5D5CAD2E7CFEDEB296C74D5'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'7BBA826B1F0F2D3607C239D94D2ECE75629FD56F86E120CC7731531E61C92ABF436B9CCE296F070FAFD94AE8BA3D33EC37256350DB95E3A67C50978563E76389'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 3,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
             'TEEP-Device',
             'SecureFS',
            h'8D82573A926D4754935332DC29997F74', / tc-uuid /
             'suit'
          ]
        }
      },
      / components / 2: [
        [
          'TEEP-Device',
          'SecureFS',
          'config.json'
        ]
      ],
      / shared-sequence / 4: << [
        / directive-set-component-index / 12, 0,
        / directive-override-parameters / 20, {
          / parameter-vendor-identifier / 1: h'C0DDD5F15243566087DB4F5B0AA26C2F',
          / parameter-class-identifier / 2: h'DB42F7093D8C55BAA8C5265FC5820F4E'
        },
        / condition-vendor-identifier / 1, 15,
        / condition-class-identifier / 2, 15
      ] >>
    } >>,
    / manifest-component-id / 5: [
      'TEEP-Device',
      'SecureFS',
      'config.suit'
    ],
    / validate / 7: << [
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / NOTE: image-digest and image-size of plaintext config.json /
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'8273468FB64BD84BB04825F8371744D952B751C73A60F455AF681E167726F116'
        ] >>,
        / image-size / 14: 61
      },
      / condition-image-match / 3, 15
    ] >>,
    / dependency-resolution / 15: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / uri / 21: "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.suit"
      },
      / directive-fetch / 21, 2
    ] >>,
    / install / 17: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,

      / NOTE: fetch encrypted firmware /
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / NOTE: encrypted payload and encryption-info /
        / parameter-content / 18: h'1A6D7C82357219BF85C334F673FB93E37A7443945A5B4E8E2391668E92B5936AB2B285FB4CEA6EADACC75242CE0B1C779DB36D34E2BDAD74666934778EC534628EFBA31805F5C699F66D7E4A03',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'56255F07A12A0FCD0877D199EBE878DF'
          },
          / payload: / null / detached ciphertext /,
          / recipients: / [
            [
              / protected: / << {
                / alg / 1: -29 / ECDH-ES + A128KW /
              } >>,
              / unprotected: / {
                / ephemeral key / -1: {
                  / kty / 1: 2 / EC2 /,
                  / crv / -1: 1 / P-256 /,
                  / x / -2: h'F8D76E4011FAD67E236AFCE61CC2F472706BD08B451FC588EF758E0D4BF5E200',
                  / y / -3: h'2D8A9B35903CD560F15DC011AB6398209724C6F41A7E9EBC9F3EB1B7BB30FBB8'
                }
              },
              / payload: / h'ABACADEC902D882096A50D88A222644C25E6152B66B96B84'
                / CEK encrypted with KEK /
            ]
          ]
        ]) >>
      },

      / decrypt encrypted firmware /
      / directive-write / 18, 15 / consumes the SUIT_Encryption_Info above /
      / NOTE: decrypted payload would be ``{"name":"FOO Bar","secret":"0123456789abfcdef0123456789abcd"}'' /
    ] >>,
    / uninstall / 24: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,
      / directive-set-component-index / 12, 0,
      / directive-unlink / 33, 15
    ] >>
  } >>
}
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
A2025873825824822F5820DDFC55B9FF63C1A27A7DA1CFB64DC924ACD5EF
BAD5D5CAD2E7CFEDEB296C74D5584AD28443A10126A0F658407BBA826B1F
0F2D3607C239D94D2ECE75629FD56F86E120CC7731531E61C92ABF436B9C
CE296F070FAFD94AE8BA3D33EC37256350DB95E3A67C50978563E7638903
590226A801010203035886A301A101A101844B544545502D446576696365
485365637572654653508D82573A926D4754935332DC29997F7444737569
740281834B544545502D4465766963654853656375726546534B636F6E66
69672E6A736F6E04582D880C0014A20150C0DDD5F15243566087DB4F5B0A
A26C2F0250DB42F7093D8C55BAA8C5265FC5820F4E010F020F05834B5445
45502D4465766963654853656375726546534B636F6E6669672E73756974
075831860C0014A2035824822F58208273468FB64BD84BB04825F8371744
D952B751C73A60F455AF681E167726F1160E183D030F0F5847860C0114A1
15783D68747470733A2F2F6578616D706C652E6F72672F38643832353733
612D393236642D343735342D393335332D3332646332393939376637342E
7375697415021158E78A0C010B000C0014A212584D1A6D7C82357219BF85
C334F673FB93E37A7443945A5B4E8E2391668E92B5936AB2B285FB4CEA6E
ADACC75242CE0B1C779DB36D34E2BDAD74666934778EC534628EFBA31805
F5C699F66D7E4A03135889D8608443A10101A1055056255F07A12A0FCD08
77D199EBE878DFF6818344A101381CA120A401022001215820F8D76E4011
FAD67E236AFCE61CC2F472706BD08B451FC588EF758E0D4BF5E200225820
2D8A9B35903CD560F15DC011AB6398209724C6F41A7E9EBC9F3EB1B7BB30
FBB85818ABACADEC902D882096A50D88A222644C25E6152B66B96B84120F
18184A880C010B000C0018210F
~~~~
