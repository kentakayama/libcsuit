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
  / delegation / 1: << [
    [
      / NOTE: signed by trust anchor /
      << 18([
        / protected: / << {
          / alg / 1: -7 / ES256 /
        } >>,
        / unprotected / {
        },
        / payload: / << {
          / cnf / 8: {
            / NOTE: public key of delegated authority /
            / COSE_Key / 1: {
              / kty / 1: 2 / EC2 /,
              / crv / -1: 1 / P-256 /,
              / x / -2: h'0E908AA8F066DB1F084E0C3652C63952BD99F2A5BDB22F9E01367AAD03ABA68B',
              / y / -3: h'77DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D71D314B2749EE42D29A'
            }
          }
        } >>,
        / signature: / h'FB2D5ACF66B9C8573CE92E13BFB8D113F798715CC10B5A0010B11925C155E7245A64E131073B87AC50CAC71650A21315B82D06CA2298CD1A95519AAE4C4B5315'
      ]) >>
    ]
  ] >>,
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'0AAED65D40E72735A582E0886FAC8C8B0E56B20047357291EFDF7AE1B718C442'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'B849655E3E794D7B34BB0C8EAC25E5DEA71B4F8D24533971E7432670D08F88B8C302716FDCA25434D79924F3CC87BF68DACF6128CC516E934BF2468DAA756395'
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
    / install / 17: << [
      / NOTE: set SUIT_Encryption_Info /
      / directive-set-component-index / 12, 0 / ['decrypted-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'B94272BD7C7E9A144D12CF46D9CEE6318753574A6F780829B87911BE1CF2B24477BA4E7D1337541F308010088920',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'3517CE3E78AC2BF3D1CDFDAF955E8600'
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
                  / x / -2: h'AAE9A733DEF11E9160A66BD81CC8215F045ACAC3F8490C7749D58A627323624A',
                  / y / -3: h'08A7B88B7F00762BA0919CA065ABF45C2A303B483E86D674E50B015122F8E515'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'0A44E77C3DBBB0780F2DB42C64FD325D18FBE13A25A9369D'
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
          / digest-bytes / h'1051324059C5193317CAC9A099BBC0B6AFB56184C04277F566A3A4131F4A1C25'
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
          / digest-bytes: / h'1051324059C5193317CAC9A099BBC0B6AFB56184C04277F566A3A4131F4A1C25'
        ] >>,
        << / COSE_Sign1_Tagged / 18([
          / protected: / << {
            / algorithm-id / 1: -7 / ES256 /
          } >>,
          / unprotected: / {},
          / payload: / null,
          / signature: / h'55990F3745DC4F200FF946643A6DE30DDCE57B080B7D68DE9896D8190B9A63E2D60E7C3D9693B67221AA6D07BBF0AB45314C236827A242C22B5E688DDC467269'
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
        / install / 17: << [
          / directive-set-component-index / 12, 0 / ['decrypted-firmware'] /,
          / directive-write / 18, 15
            / consumes the SUIT_Encryption_Info set by dependent-manifest /,
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
D86BA401589E8181589AD28443A10126A0584FA108A101A4010220012158
200E908AA8F066DB1F084E0C3652C63952BD99F2A5BDB22F9E01367AAD03
ABA68B22582077DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D71D
314B2749EE42D29A5840FB2D5ACF66B9C8573CE92E13BFB8D113F798715C
C10B5A0010B11925C155E7245A64E131073B87AC50CAC71650A21315B82D
06CA2298CD1A95519AAE4C4B5315025873825824822F58200AAED65D40E7
2735A582E0886FAC8C8B0E56B20047357291EFDF7AE1B718C442584AD284
43A10126A0F65840B849655E3E794D7B34BB0C8EAC25E5DEA71B4F8D2453
3971E7432670D08F88B8C302716FDCA25434D79924F3CC87BF68DACF6128
CC516E934BF2468DAA75639503590170A501010201035837A201A101A101
815818646570656E64656E63792D6D616E69666573742E73756974028181
526465637279707465642D6669726D77617265058157646570656E64656E
742D6D616E69666573742E73756974115901138E0C0014A212582EB94272
BD7C7E9A144D12CF46D9CEE6318753574A6F780829B87911BE1CF2B24477
BA4E7D1337541F308010088920135890D8608443A10101A105503517CE3E
78AC2BF3D1CDFDAF955E8600F6818344A101381CA220A401022001215820
AAE9A733DEF11E9160A66BD81CC8215F045ACAC3F8490C7749D58A627323
624A22582008A7B88B7F00762BA0919CA065ABF45C2A303B483E86D674E5
0B015122F8E51504456B69642D3258180A44E77C3DBBB0780F2DB42C64FD
325D18FBE13A25A9369D0C0114A3035824822F58201051324059C5193317
CAC9A099BBC0B6AFB56184C04277F566A3A4131F4A1C250E18F715742364
6570656E64656E63792D6D616E6966657374150F070F0B0F742364657065
6E64656E63792D6D616E696665737458F7D86BA2025873825824822F5820
1051324059C5193317CAC9A099BBC0B6AFB56184C04277F566A3A4131F4A
1C25584AD28443A10126A0F6584055990F3745DC4F200FF946643A6DE30D
DCE57B080B7D68DE9896D8190B9A63E2D60E7C3D9693B67221AA6D07BBF0
AB45314C236827A242C22B5E688DDC46726903587BA601010201035849A2
028181526465637279707465642D6669726D7761726504582F840C0014A2
035824822F582036921488FE6680712F734E11F58D87EEB66D4B21A8A1AD
3441060814DA16D50F0E181E05815818646570656E64656E63792D6D616E
69666573742E73756974074382030F1147860C00120F030F
~~~~
