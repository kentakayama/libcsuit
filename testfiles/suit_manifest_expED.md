<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 3: ES-ECDH + AES-KW + Process Dependency
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'79707465642D6669726D77617265058157646570656E64656E742D6D616E6966'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'8E0C0014A212582E2F59C3A34D9570FB99A5382E66466A3221A8AD85CE508BA306FB431A60EFA5AAAA078355070205A4B196832DF17F135890D8608443A10101'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
             'dependency-manifest.suit'
          ]
        }
      },
      / components / 2: [
        ['decrypted-firmware']
      ]
    } >>,
    / manifest-component-id / 5: [
      'dependent-manifest.suit'
    ],
    / install / 20: << [
      / NOTE: set SUIT_Encryption_Info /
      / directive-set-component-index / 12, 0 / ['decrypted-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'2F59C3A34D9570FB99A5382E66466A3221A8AD85CE508BA306FB431A60EFA5AAAA078355070205A4B196832DF17F',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / A128GCM /
          } >>,
          / unprotected: / {
            / IV / 5: h'F14AAB9D81D51F7AD943FE87AF4F70CD'
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
                  / x / -2: h'D3CB3FE8470BF5A6589FAF14E66CDC3876DC7AC242D1ADC4D97664C817BF54EB',
                  / y / -3: h'F9C4FA53979580F5FE5A36538FCDA829AD4225661CBBC8F230F6A550FA0DC7A3'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'DC1787D6715EBC23D02F6D2C6B8F470FE11F482AFF04F53A'
                / CEK encrypted with KEK /
            ]
          ]
        ]) >>
      },

      / NOTE: call dependency-manifest /
      / directive-set-component-index / 12, 1 / ['dependenty-manifest.suit'] /,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / algorithm-id / -16 / SHA256 /,
          / digest-bytes / h'4B15C90FBD776A820E7E733DF040D90B356B5C75982ECAECE8673818179BDF16'
        ] >>,
        / parameter-image-size / 14: 247,
        / parameter-uri / 21: "#dependency-manifest"
      },
      / directive-fetch / 21, 15,
      / condition-dependency-integrity / 7, 15,
      / directive-process-dependency / 11, 15
    ] >>
  } >>,
  "#dependency-manifest": <<
    / SUIT_Envelope_Tagged / 107({
      / authentication-wrapper / 2: << [
        << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'4B15C90FBD776A820E7E733DF040D90B356B5C75982ECAECE8673818179BDF16'
        ] >>,
        << / COSE_Sign1_Tagged / 18([
          / protected: / << {
            / algorithm-id / 1: -7 / ES256 /
          } >>,
          / unprotected: / {},
          / payload: / null,
          / signature: / h'2B1B9C4E44E52863A78F73DA2A935823B28AEAE6A85CADAC4C4E3AABAAD56CBCE5A47D288F86B54D0186657E972E748B48CDB1D420FBAC1285DCC978382F62CC'
        ]) >>
      ] >>,
      / manifest / 3: << {
        / manifest-version / 1: 1,
        / manifest-sequence-number / 2: 1,
        / common / 3: << {
          / components / 2: [
            ['decrypted-firmware']
          ],
          / shared-sequence / 4: << [
            / directive-set-componnt-index / 12, 0 / ['decrypted-firmware'] /,
            / directive-override-parameters / 20, {
              / parameter-image-digest / 3: << [
                / algorithm-id / -16 / SHA256 /,
                / digest-bytes / h'36921488FE6680712F734E11F58D87EEB66D4B21A8A1AD3441060814DA16D50F'
              ] >>,
              / parameter-image-size / 14: 30
            }
          ] >>
        } >>,
        / manifest-component-id / 5: [
          'dependency-manifest.suit'
        ],
        / validate / 7: << [
          / condition-image-match / 3, 15
        ] >>,
        / install / 20: << [
          / directive-set-component-index / 12, 0
            / ['decrypted-firmware'] /,
          / directive-write / 18, 15
            / consumes the SUIT_Encryption_Info set by dependent /,
          / condition-image-match / 3, 15
            / check the integrity of the decrypted payload /
        ] >>
      } >>
    })
  >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA3025873825824822F58206631885AD5F4C0273EB042E9D7E2E7D329
799F18BF476A8BF79F3253C16AAF7F584AD28443A10126A0F658401D0F95
59EB23412946007C9DDF28547CB608E296EF5E4C176E853E2B200171C311
82A31A25886A17289667225EA1745D0B4BFE636088B1E7FBE52B5A594A84
3603590170A501010201035837A201A101A101815818646570656E64656E
63792D6D616E69666573742E73756974028181526465637279707465642D
6669726D77617265058157646570656E64656E742D6D616E69666573742E
73756974145901138E0C0014A212582E2F59C3A34D9570FB99A5382E6646
6A3221A8AD85CE508BA306FB431A60EFA5AAAA078355070205A4B196832D
F17F135890D8608443A10101A10550F14AAB9D81D51F7AD943FE87AF4F70
CDF6818344A101381CA220A401022001215820D3CB3FE8470BF5A6589FAF
14E66CDC3876DC7AC242D1ADC4D97664C817BF54EB225820F9C4FA539795
80F5FE5A36538FCDA829AD4225661CBBC8F230F6A550FA0DC7A304456B69
642D325818DC1787D6715EBC23D02F6D2C6B8F470FE11F482AFF04F53A0C
0114A3035824822F58204B15C90FBD776A820E7E733DF040D90B356B5C75
982ECAECE8673818179BDF160E18F7157423646570656E64656E63792D6D
616E6966657374150F070F0B0F7423646570656E64656E63792D6D616E69
6665737458F7D86BA2025873825824822F58204B15C90FBD776A820E7E73
3DF040D90B356B5C75982ECAECE8673818179BDF16584AD28443A10126A0
F658402B1B9C4E44E52863A78F73DA2A935823B28AEAE6A85CADAC4C4E3A
ABAAD56CBCE5A47D288F86B54D0186657E972E748B48CDB1D420FBAC1285
DCC978382F62CC03587BA601010201035849A20281815264656372797074
65642D6669726D7761726504582F840C0014A2035824822F582036921488
FE6680712F734E11F58D87EEB66D4B21A8A1AD3441060814DA16D50F0E18
1E05815818646570656E64656E63792D6D616E69666573742E7375697407
4382030F1447860C00120F030F
~~~~
