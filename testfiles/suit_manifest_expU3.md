<!--
 Copyright (c) 2020-2024 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 3: Component Metadata Example
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
      ['usr', 'bin', 'example'],
      ['usr', 'local', 'bin'],
      ['usr', 'local', 'bin', 'example3']
    ],
    e'suit-shared-sequence': << [
      e'suit-directive-set-component-index', true,
      e'suit-directive-override-parameters', {
        e'suit-parameter-vendor-identifier': h'fa6b4a53d5ad5fdfbe9de663e4d41ffe',
        e'suit-parameter-class-identifier': h'1492af1425695e48bf429b2d51f2ab45',
        e'suit-parameter-image-digest': << [
          / algorithm-id: / e'cose-alg-sha-256',
          / digest-bytes: / ref'comp-meta.digest.hdiag'
        ] >>,
        e'suit-parameter-image-size': ref'comp-meta.size.gdiag'
      }
    ] >>
  } >>,
  e'suit-payload-fetch': << [
    / create a directory /
    e'suit-directive-set-component-index',
      1 / ['usr', 'local', 'bin'] /,
    e'suit-directive-override-parameters', {
      e'suit-parameter-content': h'',
      e'suit-parameter-component-metadata': << {
        e'suit-meta-file-type': e'suit-filetype-directory'
      } >>
    },
    e'suit-directive-write', 2,

    / fetch the binary /
    e'suit-directive-set-component-index',
      2 / ['usr', 'local', 'bin', 'example3'] /,
    e'suit-directive-override-parameters', {
      e'suit-parameter-uri': "https://cdn.example/example3.bin",
      e'suit-parameter-component-metadata': << {
        e'suit-meta-default-permissions': 0b100 / r-- /,
        e'suit-meta-user-permissions': {
          / default user / 1000: 0b110 / rw- /
        },
        e'suit-meta-group-permissions': {
          / default group / 1000: 0b100 / r-- /
        },
        e'suit-meta-file-type': e'suit-filetype-regular'
      } >>
    },
    e'suit-directive-fetch', 2,
    e'suit-condition-image-match', 15
  ] >>,
  e'suit-install': << [
    / create a symbolic link /
    e'suit-directive-set-component-index',
      0 / ['usr', 'bin', 'example'] /,
    e'suit-directive-override-parameters', {
      e'suit-parameter-content': '/usr/local/bin/example3',
      e'suit-parameter-component-metadata': << {
        e'suit-meta-file-type': e'suit-filetype-symlink'
      } >>
    },
    e'suit-directive-write', 2
  ] >>
} >>
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F5820F3D4B741F5C160CEAE81D884B54327DF0B
7AE29C98902B44962541E084931DE9584AD28443A10126A0F65840F7BED8
C591DAB332DADAED94886229470AE9EF5E12D7ACA3B61A75CBCA84E08669
0049B17BC820198FFC8A04D575085E16E6CC28D35AB40E0B69BFFE67A2F7
2103590113A501010200035891A2028383437573724362696E476578616D
706C658343757372456C6F63616C4362696E8443757372456C6F63616C43
62696E486578616D706C6533045853840CF514A40150FA6B4A53D5AD5FDF
BE9DE663E4D41FFE02501492AF1425695E48BF429B2D51F2AB4503582482
2F582036921488FE6680712F734E11F58D87EEB66D4B21A8A1AD34410608
14DA16D50F0E181E10584E8E0C0114A21240181E43A1050212020C0214A2
15782068747470733A2F2F63646E2E6578616D706C652F6578616D706C65
332E62696E181E51A4010402A11903E80603A11903E80405011502030F14
5826860C0014A212572F7573722F6C6F63616C2F62696E2F6578616D706C
6533181E43A105031202
~~~~
