<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 0: Copy Params Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'717687D957837552411B8805281F5C3AF5DC0A72252ED7FFEBCD1BA7DAE7917C'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'932A62878B5D8104E1E5EDF8DB89B983899F349D55EC708CBFE12E289027FD7B28854058971DBFBDE55F228F381FBC494AE99BCF4172580446E71173EE07FA5B'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 0,
    / common / 3: << {
      / components / 2: [
        [h'00'],
        [h'01']
      ]
    } >>,
    / install / 17: << [
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-use-before / 4: 1696291200 / Tue 03 Oct 2023 12:00:00AM /,
        / parameter-minimum-battery / 26: 10 / 10% /,
        / parameter-update-priority / 27: -1 / 'critical' /,
        / parameter-version / 28: [
          / comparison-type: / 4 / lesser-equal /,
          / comparison-value: / [ 1, 0 ]
        ]
      },

      / directive-set-component-index / 12, 1,
      / directive-copy-params / 35, {
        / src-index / 0: [ 4, 26, 27 ] / use-before, minimum-battery, update-priority /
      },
      / directive-override-parameters / 20, {
        / parameter-version / 28: [
          / comparison-type: / 5 / lesser /,
          / comparison-value: / [ 1, 0, 2 ]
        ]
      },

      / directive-set-component-index / 12, true,
      / directive-run-sequence / 32, << [
        / condition-use-before / 4, 15,
        / condition-minimum-battery / 26, 15,
        / condition-version / 28, 15,
        / condition-update-authorized / 27, 15
      ] >>
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F5820717687D957837552411B8805281F5C3AF5
DC0A72252ED7FFEBCD1BA7DAE7917C584AD28443A10126A0F65840932A62
878B5D8104E1E5EDF8DB89B983899F349D55EC708CBFE12E289027FD7B28
854058971DBFBDE55F228F381FBC494AE99BCF4172580446E71173EE07FA
5B035852A4010102000349A1028281410081410111583F8E0C0014A4041A
651B5980181A0A181B20181C82048201000C011823A1008304181A181B14
A1181C8205830100020CF518204C88040F181A0F181C0F181B0F
~~~~
