<!--
 Copyright (c) 2020-2024 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 2: Wait and Condition Example
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}
NOTE: only SUIT_Manifest part excluding severed suit-coswid is displayed here.

~~~~
<< {
  e'suit-manifest-version': 1,
  e'suit-manifest-sequence-number': 0,
  e'suit-common': << {
    e'suit-components': [
      [h'00']
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
  e'suit-set-version': << [1, 0, 0] >>,
  e'suit-validate': << [
    e'suit-directive-override-parameters', {
      e'suit-parameter-use-before': dt'2016-06-14T23:59:59Z'
    },
    e'suit-condition-image-match', 15,
    e'suit-condition-use-before', 15
  ] >>,
  e'suit-coswid': ref'update.coswid.digest.gdiag',
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
D86BA3025873825824822F582067D84AC24C6A0E5891F21319358BDD708E
A9ECB68CC24E56F89B2777486B13E8584AD28443A10126A0F658403E3A68
2C2F4461C67FCFB885A3F77C4C8AEE64D96998F59FBD49DBD52E317EAF62
EAA9CA7BD2D99B078B0D7593B933A3E3685C6922BB19E61D74ECE01961F2
B40358E6A70101020003586AA202818141000458618814A50150FA6B4A53
D5AD5FDFBE9DE663E4D41FFE02501492AF1425695E48BF429B2D51F2AB45
035824822F582036921488FE6680712F734E11F58D87EEB66D4B21A8A1AD
3441060814DA16D50F0E181E181C46820583010000181C0F010F020F0644
83010000074D8614A1041A57609A7F030F040F0E822F58204AA0230F9DFF
A401D4C55CD36CE0DB342D37517FE2E6490ABEF9B1DD441643F01058378A
14A415781B687474703A2F2F6578616D706C652E636F6D2F66696C652E62
696E181A14181B20181D43A10201181D00181A0F1502030F0E583DA40050
A4156106EBA941A1B98D22E42A5A12870C000175737569742D6669726D77
6172652D6578616D706C6502A2181F67535549542057471821820102
~~~~
