<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 2: Basic Dependency Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'E9F48BCC78567721524DF20720F3D161F3801C7674D9FC4CDE502F7BB0EFB5D9'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'02A3C26EAE57CF482F5DDC1898EBEDE33DCD85AA102FEBC7FFA8126FFB50499A4137E7D49E23994F72C977E21BAFAFB5CFCA0E1E95B961914CFA5F01D4D28BCC'
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
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'9971271881eddc8e7ac42c0107b07dac84de8f5165edc9ce0d7efd4d0586feda'
        ] >>,
        / parameter-image-size / 14: 190,
        / parameter-uri / 21: "http://example.com/dependent.suit"
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
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F5820E9F48BCC78567721524DF20720F3D161F3
801C7674D9FC4CDE502F7BB0EFB5D9584AD28443A10126A0F6584002A3C2
6EAE57CF482F5DDC1898EBEDE33DCD85AA102FEBC7FFA8126FFB50499A41
37E7D49E23994F72C977E21BAFAFB5CFCA0E1E95B961914CFA5F01D4D28B
CC0358CEA70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E73756974095286
0C0014A11749636174203030203130170F0F5857880C0114A3035824822F
58209971271881EDDC8E7AC42C0107B07DAC84DE8F5165EDC9CE0D7EFD4D
0586FEDA0E18BE157821687474703A2F2F6578616D706C652E636F6D2F64
6570656E64656E742E737569741502030F1158288A0C010B000C0014A112
581A20696E206D756C7469706C6520747275737420646F6D61696E73120F
~~~~
