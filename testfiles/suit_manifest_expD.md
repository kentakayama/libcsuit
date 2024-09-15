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
      / digest-bytes: / h'014F0B68F4D5BCEC33D28C09DB32DBF38273AF57D769E9E3D84895A4BE2C7A24'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'B3972AAD29F143FD4DB46E9844805DB3280D6487A0831E2ECE1A9E4DBE3EDAD12A0A289FD565AE0928387440B4DCE185D6CDC30CD2E93BA7B919166B355C0E03'
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
          / digest-bytes / h'B39B52B0B747EA79588C190F567BFC2C8437BA8A73F7EA983182E79F0148D59B'
        ] >>,
        / parameter-image-size / 14: 389,
        / parameter-uri / 21: "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.suit"
      },
      / directive-fetch / 21, 2
    ] >>,
    / install / 20: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,

      / NOTE: fetch encrypted firmware /
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / NOTE: encrypted payload and encryption-info /
        / parameter-content / 18: h'1B643B18E1081F77FFB9B107D61CD09B60965885A31D7E355A7AC0DAF86FCBE62F3E20CBD59FEFDB7708EE708208C60E20F9056C13A43288224C0FB271',
        / parameter-encryption-info / 19: << 96([
          / protected: / h'',
          / unprotected: / {
            / alg / 1: -65534 / A128CTR /,
            / IV / 5: h'76C8F4729BE242D1E230165C87C39219'
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
                  / x / -2: h'CB86B097AFBB4801961072DEF2A051CE0A1514B8760EDA09D66442CF7E9289D9',
                  / y / -3: h'D50ADBBEB94693103782200CC07C1E1DFD8635150FD9F6A7AD75B33A4F4DF169'
                }
              },
              / payload: / h'9C4E739A485FF8E42C7245FA50E8B55EA7026BD8EC8649E6'
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
A2025873825824822F5820014F0B68F4D5BCEC33D28C09DB32DBF38273AF
57D769E9E3D84895A4BE2C7A24584AD28443A10126A0F65840B3972AAD29
F143FD4DB46E9844805DB3280D6487A0831E2ECE1A9E4DBE3EDAD12A0A28
9FD565AE0928387440B4DCE185D6CDC30CD2E93BA7B919166B355C0E0303
590242A801010203035886A301A101A101844B544545502D446576696365
485365637572654653508D82573A926D4754935332DC29997F7444737569
740281834B544545502D4465766963654853656375726546534B636F6E66
69672E6A736F6E04582D880C0014A20150C0DDD5F15243566087DB4F5B0A
A26C2F0250DB42F7093D8C55BAA8C5265FC5820F4E010F020F05834B5445
45502D4465766963654853656375726546534B636F6E6669672E73756974
075831860C0014A2035824822F58208273468FB64BD84BB04825F8371744
D952B751C73A60F455AF681E167726F1160E183D030F0F5872860C0114A3
035824822F5820B39B52B0B747EA79588C190F567BFC2C8437BA8A73F7EA
983182E79F0148D59B0E19018515783D68747470733A2F2F6578616D706C
652E6F72672F38643832353733612D393236642D343735342D393335332D
3332646332393939376637342E7375697415021458D88A0C010B000C0014
A212583D1B643B18E1081F77FFB9B107D61CD09B60965885A31D7E355A7A
C0DAF86FCBE62F3E20CBD59FEFDB7708EE708208C60E20F9056C13A43288
224C0FB27113588AD8608440A20139FFFD055076C8F4729BE242D1E23016
5C87C39219F6818344A101381CA120A401022001215820CB86B097AFBB48
01961072DEF2A051CE0A1514B8760EDA09D66442CF7E9289D9225820D50A
DBBEB94693103782200CC07C1E1DFD8635150FD9F6A7AD75B33A4F4DF169
58189C4E739A485FF8E42C7245FA50E8B55EA7026BD8EC8649E6120F1818
4A880C010B000C0018210F
~~~~
