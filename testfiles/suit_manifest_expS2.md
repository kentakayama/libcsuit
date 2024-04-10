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
      / digest-bytes: / h'4874ADC80A9128A2B2057F5FE59C45F8ED10A9BF9C5308FCF951B8BBAF434B95'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'C257E23A34960BE215BB9B927A5A3CEEDD675DFD81AE6E55A66FDD22098868891DF42D71ADB962A64CC008AEF9465DA2153CCF383F00B505F079DB540F64B916'
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
          / digest-bytes: / h'6C86246B90D644F021671F6D42523B2CB5E156F764BE618AA46BFCD0DB23E768'
        ] >>,
        / parameter-image-size / 14: 352,
        / parameter-uri / 21: "http://example.com/dependent.suit"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 17: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'6EA128D7BB19B86F77C4227F2A29F22026A41958ACC45CC0A35BA388B13E2F51'
        ] >>
      },
      / condition-dependency-integrity / 7, 15,
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
D86BA2025873825824822F58204874ADC80A9128A2B2057F5FE59C45F8ED
10A9BF9C5308FCF951B8BBAF434B95584AD28443A10126A0F65840C257E2
3A34960BE215BB9B927A5A3CEEDD675DFD81AE6E55A66FDD22098868891D
F42D71ADB962A64CC008AEF9465DA2153CCF383F00B505F079DB540F64B9
160358FAA70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E73756974095286
0C0014A11749636174203030203130170F0F5858880C0114A3035824822F
58206C86246B90D644F021671F6D42523B2CB5E156F764BE618AA46BFCD0
DB23E7680E190160157821687474703A2F2F6578616D706C652E636F6D2F
646570656E64656E742E737569741502030F1158538E0C0114A103582482
2F58206EA128D7BB19B86F77C4227F2A29F22026A41958ACC45CC0A35BA3
88B13E2F51070F0B000C0014A112581A20696E206D756C7469706C652074
7275737420646F6D61696E73120F
~~~~
