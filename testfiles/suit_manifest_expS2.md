<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 2: Basic Integrated Dependency Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'081285C75B02B48A680774BA4C37C57D1B3C6BA866A4DD795E985BA4CD806087'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'46BF1000C7525AA3A5D440E9DD07F368E7A8C10B8E0DD92ACF792CAA8FFB1874A3060A3717374671B34E6FDCAA67575023D3C8894A6ED40A0958025DDFB9B8BF'
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
        [
          '10'
        ]
      ]
    } >>,
    / manifest-component-id / 5: [
      'depending.suit'
    ],
    / invoke / 9: << [
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-invoke-args / 23: 'cat 00 10'
      },
      / directive-invoke / 23, 15
    ] >>,
    / dependency-resolution / 15: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16,
          / digest-bytes: / h'9971271881eddc8e7ac42c0107b07dac84de8f5165edc9ce0d7efd4d0586feda'
        ] >>,
        / parameter-image-size / 14: 190,
        / parameter-uri / 21: "#dependent.suit"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 17: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,

      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / parameter-content / 18: ' in multiple trust domains'
      },
      / directive-write / 18, 15
    ] >>
  } >>,
  "#dependent.suit": h'd86ba2025873825824822f58206ea128d7bb19b86f77c4227f2a29f22026a41958acc45cc0a35ba388b13e2f51584ad28443a10126a0f6584056af23fbf29a0159b911b5fa86c6330cca1ec0381068af620bebc2ac20bbf3b9c85607518e5353b7fc60c8b9f06482bdbed085b4a1e71e0c21516f8680ebc482035842a6010102000347a102818142303005814e646570656e64656e742e73756974094d8414a11746636174203030170f11528414a1124b68656c6c6f20776f726c64120f'
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA3025873825824822F5820081285C75B02B48A680774BA4C37C57D1B
3C6BA866A4DD795E985BA4CD806087584AD28443A10126A0F6584046BF10
00C7525AA3A5D440E9DD07F368E7A8C10B8E0DD92ACF792CAA8FFB1874A3
060A3717374671B34E6FDCAA67575023D3C8894A6ED40A0958025DDFB9B8
BF0358BBA70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E73756974095286
0C0014A11749636174203030203130170F0F5844880C0114A3035824822F
58209971271881EDDC8E7AC42C0107B07DAC84DE8F5165EDC9CE0D7EFD4D
0586FEDA0E18BE156F23646570656E64656E742E737569741502030F1158
288A0C010B000C0014A112581A20696E206D756C7469706C652074727573
7420646F6D61696E73120F6F23646570656E64656E742E7375697458BED8
6BA2025873825824822F58206EA128D7BB19B86F77C4227F2A29F22026A4
1958ACC45CC0A35BA388B13E2F51584AD28443A10126A0F6584056AF23FB
F29A0159B911B5FA86C6330CCA1EC0381068AF620BEBC2AC20BBF3B9C856
07518E5353B7FC60C8B9F06482BDBED085B4A1E71E0C21516F8680EBC482
035842A6010102000347A102818142303005814E646570656E64656E742E
73756974094D8414A11746636174203030170F11528414A1124B68656C6C
6F20776F726C64120F
~~~~
