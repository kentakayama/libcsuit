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
      / digest-bytes: / h'01FCD9F6EBC2FB0CC68FF58488D3C9FF304BBB2DF5E5AF820DE1976FB73F155A'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'CC307FB0147A53F7C734DF0F2C110772241BECF14EC17D8FA1402E044DC65AD077A0DD4A14A551D140FF797FE40C57A570827435127706AF64C46AD8593FE3EE'
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
    / install / 20: << [
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-use-before / 4: 1696291200 / Tue 03 Oct 2023 12:00:00AM /,
        / parameter-minimum-battery / 26: 10 / 10% /,
        / parameter-update-priority / 27: -1 / 'critical' /,
        / parameter-version / 28: << [
          / comparison-type: / 4 / lesser-equal /,
          / comparison-value: / [ 1, 0 ]
        ] >>
      },

      / directive-set-component-index / 12, 1,
      / directive-copy-params / 35, {
        / src-index / 0: [ 4, 26, 27 ] / use-before, minimum-battery, update-priority /
      },
      / directive-override-parameters / 20, {
        / parameter-version / 28: << [
          / comparison-type: / 5 / lesser /,
          / comparison-value: / [ 1, 0, 2 ]
        ] >>
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
D86BA2025873825824822F582001FCD9F6EBC2FB0CC68FF58488D3C9FF30
4BBB2DF5E5AF820DE1976FB73F155A584AD28443A10126A0F65840CC307F
B0147A53F7C734DF0F2C110772241BECF14EC17D8FA1402E044DC65AD077
A0DD4A14A551D140FF797FE40C57A570827435127706AF64C46AD8593FE3
EE035854A4010102000349A102828141008141011458418E0C0014A4041A
651B5980181A0A181B20181C4582048201000C011823A1008304181A181B
14A1181C468205830100020CF518204C88040F181A0F181C0F181B0F
~~~~
