<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 0: Basic Content+Write Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'0F02CAF6D3E61920D36BF3CEA7F862A13BB8FB1F09C3F4C29B121FEAB78EF3D8'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -9 / ESP256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'A25F337126369D2E0B451C01DBD8CDB84A77E7F6C39E789DB3D227753494000C9D250001FDDCA39B4B4E3755A7278C11998171905F56C394CFBB907105DA804F'
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
D86BA2025873825824822F58200F02CAF6D3E61920D36BF3CEA7F862A13B
B8FB1F09C3F4C29B121FEAB78EF3D8584AD28443A10128A0F65840A25F33
7126369D2E0B451C01DBD8CDB84A77E7F6C39E789DB3D227753494000C9D
250001FDDCA39B4B4E3755A7278C11998171905F56C394CFBB907105DA80
4F035842A6010102000347A102818142303005814E646570656E64656E74
2E73756974094D8414A11746636174203030170F14528414A1124B68656C
6C6F20776F726C64120F
~~~~
