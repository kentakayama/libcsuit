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
      / digest-bytes: / h'6117ACE74E900E65C560D609BBC34C90E4A33E340B12444D525C427E4AD9FF10'
    ] >>,
    << / COSE_Mac0_Tagged / 17([
      / protected: / << {
        / algorithm-id / 1: 5 / HMAC256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'02B4BCA870F62D6A351C13A52B622D2C3779D43823A2FD20C08147A8A337A391'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        ['plaintext-firmware']
      ]
    } >>,
    / install / 17: << [
      / fetch encrypted firmware /
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'9890D8DC740A2E82C2BEA9BAB13E0BFA0FB4EB2BA3C0BCA4B23A0D660C5B3038F8634933921B3C2D1A84EE6C2779',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'93702C81590F845D9EC866CCAC767BD1'
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
              / payload: / h'CA0DF4EE01796D12FE379165602E0AC2476391A4EF122A64' / CEK encrypted with KEK /
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
D86BA2025853825824822F58206117ACE74E900E65C560D609BBC34C90E4
A33E340B12444D525C427E4AD9FF10582AD18443A10105A0F6582002B4BC
A870F62D6A351C13A52B622D2C3779D43823A2FD20C08147A8A337A39103
589DA4010102010357A102818152706C61696E746578742D6669726D7761
726511587C8414A212582E9890D8DC740A2E82C2BEA9BAB13E0BFA0FB4EB
2BA3C0BCA4B23A0D660C5B3038F8634933921B3C2D1A84EE6C2779135843
D8608443A10101A1055093702C81590F845D9EC866CCAC767BD1F6818341
A0A2012204456B69642D315818CA0DF4EE01796D12FE379165602E0AC247
6391A4EF122A64120F
~~~~
