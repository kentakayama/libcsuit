<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 2: Write and Decrypt ES-ECDH + AES-KW + Encrypted Payload
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'81DEF8B8DD38839D61434BE169636E52F6C9D4A466437F795F166735AB2CF097'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'1199BD308E8B51072DB381CD56C2EC6ED2C163FE31437419CFB14602150BCF61ABB39E4A3C1547466149310704A64F8FBA89C502FDBC1E440B95679CBECEB028'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / components / 2: [
        ['decrypted-firmware']
      ]
    } >>,
    / install / 20: << [
      / directive-set-component-index / 12, 0 / ['plaintext-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'2F59C3A34D9570FB99A5382E66466A3221A8AD85CE508BA306FB431A60EFA5AAAA078355070205A4B196832DF17F',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / A128GCM /
          } >>,
          / unprotected: / {
            / IV / 5: h'F14AAB9D81D51F7AD943FE87AF4F70CD'
          },
          / payload: / null / detached ciphertext /,
          / recipients: / [
            [
              / protected: / << {
                / alg / 1: -29 / ECDH-ES + A128KW /
              } >>,
              / unprotected: / {
                / ephemeral key / -1: {
                  / kty / 1: 2 / EC2 /,
                  / crv / -1: 1 / P-256 /,
                  / x / -2: h'D3CB3FE8470BF5A6589FAF14E66CDC3876DC7AC242D1ADC4D97664C817BF54EB',
                  / y / -3: h'F9C4FA53979580F5FE5A36538FCDA829AD4225661CBBC8F230F6A550FA0DC7A3'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'DC1787D6715EBC23D02F6D2C6B8F470FE11F482AFF04F53A'
                / CEK encrypted with KEK /
            ]
          ]
        ]) >>
      },
      / directive-write / 18, 15 / consumes the SUIT_Encryption_Info above /
    ] >>
  } >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA2025873825824822F582081DEF8B8DD38839D61434BE169636E52F6
C9D4A466437F795F166735AB2CF097584AD28443A10126A0F658401199BD
308E8B51072DB381CD56C2EC6ED2C163FE31437419CFB14602150BCF61AB
B39E4A3C1547466149310704A64F8FBA89C502FDBC1E440B95679CBECEB0
280358ECA4010102010357A1028181526465637279707465642D6669726D
776172651458CB860C0014A212582E2F59C3A34D9570FB99A5382E66466A
3221A8AD85CE508BA306FB431A60EFA5AAAA078355070205A4B196832DF1
7F135890D8608443A10101A10550F14AAB9D81D51F7AD943FE87AF4F70CD
F6818344A101381CA220A401022001215820D3CB3FE8470BF5A6589FAF14
E66CDC3876DC7AC242D1ADC4D97664C817BF54EB225820F9C4FA53979580
F5FE5A36538FCDA829AD4225661CBBC8F230F6A550FA0DC7A304456B6964
2D325818DC1787D6715EBC23D02F6D2C6B8F470FE11F482AFF04F53A120F
~~~~
