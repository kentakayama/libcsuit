<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 0: Write and Decrypt AES-KW + Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'5DEFDDB7F175FA20778FFE24BE7B9C369BD8ED06AA4654F28794CD134CDBA932'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'4C4A5FB50738699649BA439237D20ADCADD6EC634A800A8E093733FC1C64984BF2BFEC583C124B5546BF0CDAC543AB0995589543B434951A29A40000EC56CBE7'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        ['plaintext-firmware'],
      ]
    } >>,
    / install / 17: << [
      / fetch encrypted firmware /
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'CE9AB65E7591EE38669C4CCA7A58FA324C1A0DBFDBC2C7C057376AFB805D660048310E8DAB045A2BE0A93F014FC9',
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
        ]) >>
      },

      / decrypt encrypted firmware /
      / directive-write / 18, 15
        / consumes the SUIT_Encryption_Info above /
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F58205DEFDDB7F175FA20778FFE24BE7B9C369B
D8ED06AA4654F28794CD134CDBA932584AD28443A10126A0F658404C4A5F
B50738699649BA439237D20ADCADD6EC634A800A8E093733FC1C64984BF2
BFEC583C124B5546BF0CDAC543AB0995589543B434951A29A40000EC56CB
E703589DA4010102010357A102818152706C61696E746578742D6669726D
7761726511587C8414A212582ECE9AB65E7591EE38669C4CCA7A58FA324C
1A0DBFDBC2C7C057376AFB805D660048310E8DAB045A2BE0A93F014FC913
5843D8608443A10101A1055011D40BB56C3836AD44B39835B3ABC7FCF681
8341A0A2012204456B69642D315818E01F4443C88CA89DF93A9C7E6D79D1
C9BC330757C7D2D75A120F
~~~~
