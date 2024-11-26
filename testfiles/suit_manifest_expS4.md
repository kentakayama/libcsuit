<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 4: Is Dependency Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'3CB3EE2C45E6F1F9F8EF857EA1AC4E1E6BD1451CAC5BBBC0845543298FC9BE36'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'F74D044D536C3AAFF2CDC726C3EB7C6522986C89EBB84B85E261D21670E90C02E84DAB8A1E925A7537F1793C1CBDE608379267D6B1C323531115FC83FE070BFB'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 0,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
            'dependent.suit'
          ]
        }
      },
      / components / 2: [
        ['10']
      ]
    } >>,
    / manifest-component-id / 5: [
      'depending.suit'
    ],
    / invoke / 9: << [
      / directive-set-component-index / 12, true,
      / directive-try-each / 15, [
        << [
          / condition-is-dependency / 8, 0,
          / directive-process-depenency / 11, 0
        ] >>,
        << [
          / directive-override-parameters / 20, {
            / parameter-invoke-args / 23: 'cat 00 10'
          },
          / directive-invoke / 23, 15
        ] >>
      ]
    ] >>,
    / dependency-resolution / 15: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'2EEEC4ACEC877EE13D8B52DB16C4390C93E5D84FD9F25AEAE0717B861BE0C4A2'
        ] >>,
        / parameter-image-size / 14: 190,
        / parameter-fetch-args / 16: 'Content-Type: application/cbor',
        / parameter-uri / 21: "#dependent.suit"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 20: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,

      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-content / 18: ' in multiple trust domains'
      },
      / directive-write / 18, 15
    ] >>
  } >>,
  "#dependent.suit": h'D86BA2025873825824822F58200F02CAF6D3E61920D36BF3CEA7F862A13BB8FB1F09C3F4C29B121FEAB78EF3D8584AD28443A10126A0F65840D0703EA193E12381A66FFADEF2F0949711CFE05ED2322818D73D19F2BBD91BE5C52F1604B45C405E96B0642F3D49B2D7C6E3B2C0B40030BDDFBD27AF930B1F8B035842A6010102000347A102818142303005814E646570656E64656E742E73756974094D8414A11746636174203030170F14528414A1124B68656C6C6F20776F726C64120F'
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA3025873825824822F58203CB3EE2C45E6F1F9F8EF857EA1AC4E1E6B
D1451CAC5BBBC0845543298FC9BE36584AD28443A10126A0F65840F74D04
4D536C3AAFF2CDC726C3EB7C6522986C89EBB84B85E261D21670E90C02E8
4DAB8A1E925A7537F1793C1CBDE608379267D6B1C323531115FC83FE070B
FB0358E7A70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E7375697409581C
840CF50F82458408000B00508414A11749636174203030203130170F0F58
65880C0114A4035824822F58202EEEC4ACEC877EE13D8B52DB16C4390C93
E5D84FD9F25AEAE0717B861BE0C4A20E18BE10581E436F6E74656E742D54
7970653A206170706C69636174696F6E2F63626F72156F23646570656E64
656E742E737569741502030F1458288A0C010B000C0014A112581A20696E
206D756C7469706C6520747275737420646F6D61696E73120F6F23646570
656E64656E742E7375697458BED86BA2025873825824822F58200F02CAF6
D3E61920D36BF3CEA7F862A13BB8FB1F09C3F4C29B121FEAB78EF3D8584A
D28443A10126A0F65840D0703EA193E12381A66FFADEF2F0949711CFE05E
D2322818D73D19F2BBD91BE5C52F1604B45C405E96B0642F3D49B2D7C6E3
B2C0B40030BDDFBD27AF930B1F8B035842A6010102000347A10281814230
3005814E646570656E64656E742E73756974094D8414A117466361742030
30170F14528414A1124B68656C6C6F20776F726C64120F
~~~~
