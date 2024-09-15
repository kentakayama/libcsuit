<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 1: SUIT Delegation Example
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

  / NOTE: signed by delegated authority /
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'A10126A0584FA108A101A4010220012158200E908AA8F066DB1F084E0C3652C6'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'7AAD03ABA68B22582077DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D71D314B2749EE42D29A5840FB2D5ACF66B9C8573CE92E13BFB8D113F798715CC1'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 0,
    / common / 3: << {
      / components / 2: [
        ['00']
      ]
    } >>,
    / manifest-component-id / 5: [
      'dependent.suit'
    ],
    / invoke / 9: << [
      / directive-override-parameters / 20, {
        / parameter-invoke-args / 23: 'cat 00'
      },
      / directive-invoke / 23, 15
    ] >>,
    / install / 20: << [
      / directive-override-parameters / 20, {
        / parameter-content / 18: 'hello world'
      },
      / directive-write / 18, 15
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA301589E8181589AD28443A10126A0584FA108A101A4010220012158
200E908AA8F066DB1F084E0C3652C63952BD99F2A5BDB22F9E01367AAD03
ABA68B22582077DA1BD8AC4F0CB490BA210648BF79AB164D49AD3551D71D
314B2749EE42D29A5840FB2D5ACF66B9C8573CE92E13BFB8D113F798715C
C10B5A0010B11925C155E7245A64E131073B87AC50CAC71650A21315B82D
06CA2298CD1A95519AAE4C4B5315025874835824822F58200F02CAF6D3E6
1920D36BF3CEA7F862A13BB8FB1F09C3F4C29B121FEAB78EF3D8584AD284
43A10126A0F6584075111FE09D12726C09F0CA87B32B1E9A6DA127E171D6
7527437E61676458327707CC0185D84C95C4C9EC77E8258B338182F8DA1B
A7014AD0E26D6C5BEC29DFC740035842A6010102000347A1028181423030
05814E646570656E64656E742E73756974094D8414A11746636174203030
170F14528414A1124B68656C6C6F20776F726C64120F
~~~~
