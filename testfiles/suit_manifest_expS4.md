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
      / digest-bytes: / h'8B730999C29AA74D27B1BCB1FDF8AD183A73C919F90B14DBE9345908E1979246'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'ABF42719903DE0516F83C64BB238B1CAB1DDEDDC006EB35C551879C68260393BA4C9C6E1A77C0BD234B57AB7162B857CECA0A74BA6BB969E5DAF5D8C934A0D04'
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
  "#dependent.suit": h'D86BA2025873825824822F58206EA128D7BB19B86F77C4227F2A29F22026A41958ACC45CC0A35BA388B13E2F51584AD28443A10126A0F6584056AF23FBF29A0159B911B5FA86C6330CCA1EC0381068AF620BEBC2AC20BBF3B9C85607518E5353B7FC60C8B9F06482BDBED085B4A1E71E0C21516F8680EBC482035842A6010102000347A102818142303005814E646570656E64656E742E73756974094D8414A11746636174203030170F11528414A1124B68656C6C6F20776F726C64120F'
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA3025873825824822F58208B730999C29AA74D27B1BCB1FDF8AD183A
73C919F90B14DBE9345908E1979246584AD28443A10126A0F65840ABF427
19903DE0516F83C64BB238B1CAB1DDEDDC006EB35C551879C68260393BA4
C9C6E1A77C0BD234B57AB7162B857CECA0A74BA6BB969E5DAF5D8C934A0D
040358C6A70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E7375697409581C
840CF50F82458408000B00508414A11749636174203030203130170F0F58
44880C0114A3035824822F58209971271881EDDC8E7AC42C0107B07DAC84
DE8F5165EDC9CE0D7EFD4D0586FEDA0E18BE156F23646570656E64656E74
2E737569741502030F1158288A0C010B000C0014A112581A20696E206D75
6C7469706C6520747275737420646F6D61696E73120F6F23646570656E64
656E742E7375697458BED86BA2025873825824822F58206EA128D7BB19B8
6F77C4227F2A29F22026A41958ACC45CC0A35BA388B13E2F51584AD28443
A10126A0F6584056AF23FBF29A0159B911B5FA86C6330CCA1EC0381068AF
620BEBC2AC20BBF3B9C85607518E5353B7FC60C8B9F06482BDBED085B4A1
E71E0C21516F8680EBC482035842A6010102000347A10281814230300581
4E646570656E64656E742E73756974094D8414A11746636174203030170F
11528414A1124B68656C6C6F20776F726C64120F
~~~~
