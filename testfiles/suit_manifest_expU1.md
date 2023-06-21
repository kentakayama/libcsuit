<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 1: Override Multiple Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'DC697E3C251C4E03AD84139FA8487902EF5D22BB535CD86CE1EF29854A65FD96'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'8F8AD74434699E3173818954CC94C8EF969E1C6440000E4805D3CC309260ED77646A3DCDFC362982524EB887CD2A77354AEBD09C9714EB5005B41A4246AD6318'
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
      / directive-override-multiple / 34, {
        / index / 0: {
          / parameter-wait-info / 29: << {
            / authorization / 1: -1 / 'critical' /,
            / power / 2: 10 / >= 10% /
          } >>
        },
        / index / 1: {
          / parameter-wait-info / 29: << {
            / time-of-day / 6: 82800 / 23:00:00 /
          } >>
        }
      },

      / directive-set-component-index / 12, true,
      / directive-wait / 29, 15
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F5820DC697E3C251C4E03AD84139FA8487902EF
5D22BB535CD86CE1EF29854A65FD96584AD28443A10126A0F658408F8AD7
4434699E3173818954CC94C8EF969E1C6440000E4805D3CC309260ED7764
6A3DCDFC362982524EB887CD2A77354AEBD09C9714EB5005B41A4246AD63
18035832A4010102000349A1028281410081410111581F861822A200A118
1D45A20120020A01A1181D47A1061A000143700CF5181D0F
~~~~
