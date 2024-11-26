<!--
 Copyright (c) 2020-2024 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 2: Wait and Condition Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}
NOTE: only SUIT_Manifest part is displayed here.

~~~~
<< {
  e'suit-manifest-version': 1,
  e'suit-manifest-sequence-number': 0,
  e'suit-common': << {
    e'suit-components': [
      ['00']
    ],
    e'suit-shared-sequence': << [
      e'suit-directive-override-parameters', {
        e'suit-parameter-vendor-identifier': h'fa6b4a53d5ad5fdfbe9de663e4d41ffe',
        e'suit-parameter-class-identifier': h'1492af1425695e48bf429b2d51f2ab45',
        e'suit-parameter-image-digest': << [
          / algorithm-id: / e'cose-alg-sha-256',
          / digest-bytes: / ref'update.digest.hdiag'
        ] >>,
        e'suit-parameter-image-size': ref'update.size.gdiag',
        e'suit-parameter-version': << [
          / comparison-type: / e'suit-condition-version-comparison-lesser',
          / comparison-value: / [1, 0, 0]
        ] >>
      },
      e'suit-condition-version', 15,
      e'suit-condition-vendor-identifier', 15,
      e'suit-condition-class-identifier', 15
    ] >>
  } >>,
  e'suit-validate': << [
    e'suit-directive-override-parameters', {
      e'suit-parameter-use-before': dt'2016-06-14T23:59:59Z'
    },
    e'suit-condition-image-match', 15
  ] >>,
  e'suit-payload-fetch': << [
    e'suit-directive-override-parameters', {
      e'suit-parameter-uri': "http://example.com/file.bin",
      e'suit-parameter-minimum-battery': 20,
      e'suit-parameter-update-priority': -1 / e.g. security update /,
      e'suit-parameter-wait-info': << {
        e'suit-wait-event-power': 1 / e.g. external power available /
      } >>
    },
    e'suit-directive-wait', 0,
    e'suit-condition-minimum-battery', 15,
    e'suit-directive-fetch', 2,
    e'suit-condition-image-match', 15
  ] >>
} >>
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F58209912DA952ED385D127F12671CFFBC56F05
1F396C5BDF0A9561E7C3D2671D784A584AD28443A10126A0F65840E6925F
435FB3DDCEC8B7C3C9DF2D6F67C5CC225AE6C260F4D4B0111D14E449A8FD
02F6FF628BCF449AC419B102239FF9EC1926CAAECCC32F69FFF1F1A184A7
4B0358BAA50101020003586BA20281814230300458618814A50150FA6B4A
53D5AD5FDFBE9DE663E4D41FFE02501492AF1425695E48BF429B2D51F2AB
45035824822F582036921488FE6680712F734E11F58D87EEB66D4B21A8A1
AD3441060814DA16D50F0E181E181C46820583010000181C0F010F020F07
4B8414A1041A57609A7F030F1058378A14A415781B687474703A2F2F6578
616D706C652E636F6D2F66696C652E62696E181A14181B20181D43A10201
181D00181A0F1502030F
~~~~
