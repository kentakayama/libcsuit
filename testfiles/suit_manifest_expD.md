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
      / digest-bytes: / h'FE6CF752367398A8BEBF0EE521242560FF495CBA08883AEDAF8CC4DC5E0DA444'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'DEA71FE757D211B86796EDB058A82F85F93DD95B5494AEC284E19330E376BFDDCAFA97D336F776890ED37B7B068904AD92E0F1657FE3935D78B4AB083ACFCA09'
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
        / parameter-image-digest / 3: << [
          / algorithm-id / -16 / SHA256 /,
          / digest-bytes / h'EF53C7F719CB10041233850AE3211D62CEC9528924E656607688E77BC14886A0'
        ] >>,
        / parameter-image-size / 14: 389,
        / parameter-uri / 21: "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.suit"
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
        / parameter-content / 18: h'6D5BE4F569E98AE01F38B071EF025437B742FF28854AB32C868BC6A76CD33B5CA112FF22BA95EA4672B7199C89A7829183794A21A6BE345C4371DCB0DC',
        / parameter-encryption-info / 19: << 96([
          / protected: / h'',
          / unprotected: / {
            / alg / 1: -65534 / A128CTR /,
            / IV / 5: h'67E3BA7CD42D02BBC39C508B5EA0F1C4'
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
                  / x / -2: h'F2452399667F57993B14C5F1107F667884854C190894FC08531C1E2290A7BA19',
                  / y / -3: h'275EDDE29FD75C9393AFFA706F8FAD3C49D03D67D47F8B0C027BE5F0BCA884CB'
                }
              },
              / payload: / h'7D806DA1ACEC6F704D803F0CFE7420525C81E1957699FCCE'
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
A2025873825824822F5820FE6CF752367398A8BEBF0EE521242560FF495C
BA08883AEDAF8CC4DC5E0DA444584AD28443A10126A0F65840DEA71FE757
D211B86796EDB058A82F85F93DD95B5494AEC284E19330E376BFDDCAFA97
D336F776890ED37B7B068904AD92E0F1657FE3935D78B4AB083ACFCA0903
590242A801010203035886A301A101A101844B544545502D446576696365
485365637572654653508D82573A926D4754935332DC29997F7444737569
740281834B544545502D4465766963654853656375726546534B636F6E66
69672E6A736F6E04582D880C0014A20150C0DDD5F15243566087DB4F5B0A
A26C2F0250DB42F7093D8C55BAA8C5265FC5820F4E010F020F05834B5445
45502D4465766963654853656375726546534B636F6E6669672E73756974
075831860C0014A2035824822F58208273468FB64BD84BB04825F8371744
D952B751C73A60F455AF681E167726F1160E183D030F0F5872860C0114A3
035824822F5820EF53C7F719CB10041233850AE3211D62CEC9528924E656
607688E77BC14886A00E19018515783D68747470733A2F2F6578616D706C
652E6F72672F38643832353733612D393236642D343735342D393335332D
3332646332393939376637342E7375697415021158D88A0C010B000C0014
A212583D6D5BE4F569E98AE01F38B071EF025437B742FF28854AB32C868B
C6A76CD33B5CA112FF22BA95EA4672B7199C89A7829183794A21A6BE345C
4371DCB0DC13588AD8608440A20139FFFD055067E3BA7CD42D02BBC39C50
8B5EA0F1C4F6818344A101381CA120A401022001215820F2452399667F57
993B14C5F1107F667884854C190894FC08531C1E2290A7BA19225820275E
DDE29FD75C9393AFFA706F8FAD3C49D03D67D47F8B0C027BE5F0BCA884CB
58187D806DA1ACEC6F704D803F0CFE7420525C81E1957699FCCE120F1818
4A880C010B000C0018210F
~~~~
