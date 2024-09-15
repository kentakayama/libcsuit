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
      / digest-bytes: / h'DCF344CEF63C615213B669E6FAB48881D154069EE6BD4CD18120935466AA3978'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'F86311478A7DCD474700B052CAF14A757CE00709404113B390218E93AE319D564127C4DC3D58CC143FFFB8657E294AA7936B93F51D4A78209479FA66E3186789'
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
          / digest-bytes: / h'CC1758F629D2CDD18698D5A279B532C7003A0331237729C604EE6FD3F3DB535F'
        ] >>,
        / parameter-image-size / 14: 352,
        / parameter-uri / 21: "http://example.com/dependent.suit"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 20: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'0F02CAF6D3E61920D36BF3CEA7F862A13BB8FB1F09C3F4C29B121FEAB78EF3D8'
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
D86BA2025873825824822F5820DCF344CEF63C615213B669E6FAB48881D1
54069EE6BD4CD18120935466AA3978584AD28443A10126A0F65840F86311
478A7DCD474700B052CAF14A757CE00709404113B390218E93AE319D5641
27C4DC3D58CC143FFFB8657E294AA7936B93F51D4A78209479FA66E31867
890358FAA70101020003581CA201A101A101814E646570656E64656E742E
7375697402818142313005814E646570656E64696E672E73756974095286
0C0014A11749636174203030203130170F0F5858880C0114A3035824822F
5820CC1758F629D2CDD18698D5A279B532C7003A0331237729C604EE6FD3
F3DB535F0E190160157821687474703A2F2F6578616D706C652E636F6D2F
646570656E64656E742E737569741502030F1458538E0C0114A103582482
2F58200F02CAF6D3E61920D36BF3CEA7F862A13BB8FB1F09C3F4C29B121F
EAB78EF3D8070F0B000C0014A112581A20696E206D756C7469706C652074
7275737420646F6D61696E73120F
~~~~
