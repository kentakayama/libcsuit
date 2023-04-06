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
  / suit-authentication-wrapper /
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 0,
    / common / 3: << {
      / components / 2: [
        [
          '00'
        ]
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
    / install / 17: << [
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
D86BA2025873825824822F58206EA128D7BB19B86F77C4227F2A29F22026
A41958ACC45CC0A35BA388B13E2F51584AD28443A10126A0F6584056AF23
FBF29A0159B911B5FA86C6330CCA1EC0381068AF620BEBC2AC20BBF3B9C8
5607518E5353B7FC60C8B9F06482BDBED085B4A1E71E0C21516F8680EBC4
82035842A6010102000347A102818142303005814E646570656E64656E74
2E73756974094D8414A11746636174203030170F11528414A1124B68656C
6C6F20776F726C64120F
~~~~
