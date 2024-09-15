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
      / digest-bytes: / h'09038B29229C8FED40DA96BB8CB6E55AF54CA3987D186CCB768F5B34D3DA2B28'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'956DA9194ABCC637D2853AE69ACB10D522CA587449861342167EE7815AB672A7C454CB6A392B47D2221F445D089EA1F75BCA084A8D78492F6B5D1254EC77F7F7'
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
        / parameter-content / 18: h'344FA2D5AD2F43F6F363DA6FF2C337FE69E33E3D63714D23985BF02499EB0E8B231D45C378245DA3611C160CC511',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'DAE613B2E0DC55F4322BE38BDBA9DC68'
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
                  / x / -2: h'FF6E266DABAF51B7207569E31CF72646183E94CEE64FCDC8695AD9A505AEFDEA',
                  / y / -3: h'5FBC4A29844450B3AC22AB30C7F7004BB59D8BD60D7997734A9FA0124B650895'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'B0E21628283F3E409F8158D8FFCA567F340E379AC39E49C9'
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
D86BA401589E8181589AD28443A10126A0584FA108A101A4010220012158
200E908AA8F066DB1F084E0C3652C63952BD99F2A5BDB22F9E01367AAD03
ABA68B22582077DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D71D
314B2749EE42D29A5840FB2D5ACF66B9C8573CE92E13BFB8D113F798715C
C10B5A0010B11925C155E7245A64E131073B87AC50CAC71650A21315B82D
06CA2298CD1A95519AAE4C4B5315025873825824822F582009038B29229C
8FED40DA96BB8CB6E55AF54CA3987D186CCB768F5B34D3DA2B28584AD284
43A10126A0F65840956DA9194ABCC637D2853AE69ACB10D522CA58744986
1342167EE7815AB672A7C454CB6A392B47D2221F445D089EA1F75BCA084A
8D78492F6B5D1254EC77F7F703590170A501010201035837A201A101A101
815818646570656E64656E63792D6D616E69666573742E73756974028181
526465637279707465642D6669726D77617265058157646570656E64656E
742D6D616E69666573742E73756974145901138E0C0014A212582E344FA2
D5AD2F43F6F363DA6FF2C337FE69E33E3D63714D23985BF02499EB0E8B23
1D45C378245DA3611C160CC511135890D8608443A10101A10550DAE613B2
E0DC55F4322BE38BDBA9DC68F6818344A101381CA220A401022001215820
FF6E266DABAF51B7207569E31CF72646183E94CEE64FCDC8695AD9A505AE
FDEA2258205FBC4A29844450B3AC22AB30C7F7004BB59D8BD60D7997734A
9FA0124B65089504456B69642D325818B0E21628283F3E409F8158D8FFCA
567F340E379AC39E49C90C0114A3035824822F58204B15C90FBD776A820E
7E733DF040D90B356B5C75982ECAECE8673818179BDF160E18F715742364
6570656E64656E63792D6D616E6966657374150F070F0B0F742364657065
6E64656E63792D6D616E696665737458F7D86BA2025873825824822F5820
4B15C90FBD776A820E7E733DF040D90B356B5C75982ECAECE8673818179B
DF16584AD28443A10126A0F658402B1B9C4E44E52863A78F73DA2A935823
B28AEAE6A85CADAC4C4E3AABAAD56CBCE5A47D288F86B54D0186657E972E
748B48CDB1D420FBAC1285DCC978382F62CC03587BA601010201035849A2
028181526465637279707465642D6669726D7761726504582F840C0014A2
035824822F582036921488FE6680712F734E11F58D87EEB66D4B21A8A1AD
3441060814DA16D50F0E181E05815818646570656E64656E63792D6D616E
69666573742E73756974074382030F1447860C00120F030F
~~~~
