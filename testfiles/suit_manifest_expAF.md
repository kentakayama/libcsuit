<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 1: Fetch and Decrypt AES-KW + Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'C6A66263CCF4C6FF5992AE4074B30DDD34520AA099F6BAD96B2F60FE79F07EC4'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'DA08C3A6455FF30865A97A7F4FBC3BA15F954E39B57167DEA9FE16EBA12CFE33D58790DB64CB70A08F89513B15CFF9951222868195224E1AB87D46FA37F58864'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        ['plaintext-firmware'],
        ['encrypted-firmware']
      ]
    } >>,
    / install / 17: << [
      / fetch encrypted firmware /
      / directive-set-component-index / 12, 1 / ['encrypted-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-image-size / 14: 46,
        / parameter-uri / 21: "https://example.com/encrypted-firmware"
      },
      / directive-fetch / 21, 15,

      / decrypt encrypted firmware /
      / directive-set-component-index / 12, 0 / ['plaintext-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'11D40BB56C3836AD44B39835B3ABC7FC'
          },
          / payload: / null / detached ciphertext /,
          / recipients: / [
            [
              / protected: / << {
              } >>,
              / unprotected: / {
                / alg / 1: -3 / A128KW /,
                / kid / 4: 'kid-1'
              },
              / payload: / h'E01F4443C88CA89DF93A9C7E6D79D1C9BC330757C7D2D75A' / CEK encrypted with KEK /
            ]
          ]
        ]) >>,
        / parameter-source-component / 22: 1 / ['encrypted-firmware'] /
      },
      / directive-copy / 22, 15 / consumes the SUIT_Encryption_Info above /
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F5820C6A66263CCF4C6FF5992AE4074B30DDD34
520AA099F6BAD96B2F60FE79F07EC4584AD28443A10126A0F65840DA08C3
A6455FF30865A97A7F4FBC3BA15F954E39B57167DEA9FE16EBA12CFE33D5
8790DB64CB70A08F89513B15CFF9951222868195224E1AB87D46FA37F588
640358E5A40101020103582BA102828152706C61696E746578742D666972
6D776172658152656E637279707465642D6669726D776172651158AF900C
0114A20E182E15782668747470733A2F2F6578616D706C652E636F6D2F65
6E637279707465642D6669726D77617265150F0C0014A2135843D8608443
A10101A1055011D40BB56C3836AD44B39835B3ABC7FCF6818341A0A20122
04456B69642D315818E01F4443C88CA89DF93A9C7E6D79D1C9BC330757C7
D2D75A1601160F14A2035824822F582036921488FE6680712F734E11F58D
87EEB66D4B21A8A1AD3441060814DA16D50F0E181E030F
~~~~
