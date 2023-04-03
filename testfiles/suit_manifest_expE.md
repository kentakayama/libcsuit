<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

# SUIT Manifest
    B.1.  Example 0: Firmware Encryption
    https://tools.ietf.org/html/draft-ietf-suit-firmware-encryption#appendix-B.1


## CBOR Diagnostic
~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / cose-alg-sha256 /,
      / digest-bytes: / h'97A9A914D04ACB14C50166EEE64A64E2FF7AE3AC27C9FD541621DE195D370894'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'380F500F7813BD29FCB46F23829299C92D2A4CF0C7EBBD71694DD5700FDBB579AEBB56821915E1C6D84776769316971FF35458A6893635493FC4C34428E561A4'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        [h'00'] / to be decrypted firmware /,
        [h'01'] / encrypted firmware /
      ]
    } >>,
    / install / 17: << [
      / fetch encrypted firmware /
      / directive-set-component-index / 12, 1 / [h'01'] /,
      / directive-override-parameters / 20, {
        / parameter-uri / 21: "https://author.example.com/encrypted-firmware.bin",
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA-256 /,
          / digest-bytes: / h'F63187728B49B0E57FE891B932C9C881735D880EFAE69A9A4D45E0FE72C70DA1'
        ] >>,
        / parameter-image-size / 14: 46
      },
      / directive-fetch / 21, 15,
      / condition-image-match / 3, 15,

      / decrypt encrypted firmware /
      / directive-set-component-index / 12, 0 / [h'00'] /,
      / directive-override-parameters / 20, {
        / parameter-source-component / 22: 1 / [h'01'] /,
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / iv / 5: h'31BD74E751CAD4FDE0100116FE2923A8'
          },
          / payload: / null / detached ciphertext /,
          / recipients: / [ [
            / protected / << {
            } >>,
            / unprotected / {
              / alg / 1: -3 / A128KW /,
              / kid / 4: 'Kid A'
            },
            / payload: / h'5FB2D8A0C21D6D5F6272C7ABF51ADB0D8726B32A4F44E244' / CEK encrypted with KEK /
          ] ]
        ]) >>
      },
      / directive-copy / 22, 15 / consumes the SUIT_Encryption_Info above /,

      / verify decrypted firmware /
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / digest-algorithm-id: / -16 / SHA-256 /,
          / digest-bytes: / h'36921488FE6680712F734E11F58D87EEB66D4B21A8A1AD3441060814DA16D50F'
        ] >>,
        / parameter-image-size / 14: 30
      },
      / condition-image-match / 3, 15
    ] >>
  } >>
})
~~~~

## CBOR binary
~~~~
D86BA2025873825824822F582097A9A914D04ACB14C50166EEE64A64E2FF
7AE3AC27C9FD541621DE195D370894584AD28443A10126A0F65840380F50
0F7813BD29FCB46F23829299C92D2A4CF0C7EBBD71694DD5700FDBB579AE
BB56821915E1C6D84776769316971FF35458A6893635493FC4C34428E561
A40358F6A4010102010349A102828141008141011158E3920C0114A31578
3168747470733A2F2F617574686F722E6578616D706C652E636F6D2F656E
637279707465642D6669726D776172652E62696E035824822F5820F63187
728B49B0E57FE891B932C9C881735D880EFAE69A9A4D45E0FE72C70DA10E
182E150F030F0C0014A21601135843D8608443A10101A1055031BD74E751
CAD4FDE0100116FE2923A8F6818341A0A2012204454B6964204158185FB2
D8A0C21D6D5F6272C7ABF51ADB0D8726B32A4F44E244160F14A203582482
2F582036921488FE6680712F734E11F58D87EEB66D4B21A8A1AD34410608
14DA16D50F0E181E030F
~~~~

## Detached Ciphertext
~~~~
7F9C0B7B3643E8760A343A6F620A3E32E2DB6856D2E0941EAA3BF3B1B2BB
ED679F66AA6BC36BFD85469BE2178155
~~~~
