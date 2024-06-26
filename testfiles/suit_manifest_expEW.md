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
      / digest-bytes: / h'CEF034223D7F2C39D676876995B4ED4E8221AC5BF184B6606EE62C41C149C266'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'65E59AAB8A35BDE9547458316D1C769FFB2CEA304C9FB6151E5C8A88A002A292C5B8C63C81B5AC0AE31948B610834E12CBDBB2753EA221544B6733076A92EE20'
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
    / install / 17: << [
      / directive-set-component-index / 12, 0 / ['plaintext-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-content / 18: h'344FA2D5AD2F43F6F363DA6FF2C337FE69E33E3D63714D23985BF02499EB0E8B231D45C378245DA3611C160CC511',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'DAE613B2E0DC55F4322BE38BDBA9DC68'
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
                  / x / -2: h'FF6E266DABAF51B7207569E31CF72646183E94CEE64FCDC8695AD9A505AEFDEA',
                  / y / -3: h'5FBC4A29844450B3AC22AB30C7F7004BB59D8BD60D7997734A9FA0124B650895'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'B0E21628283F3E409F8158D8FFCA567F340E379AC39E49C9'
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
D86BA2025873825824822F5820CEF034223D7F2C39D676876995B4ED4E82
21AC5BF184B6606EE62C41C149C266584AD28443A10126A0F6584065E59A
AB8A35BDE9547458316D1C769FFB2CEA304C9FB6151E5C8A88A002A292C5
B8C63C81B5AC0AE31948B610834E12CBDBB2753EA221544B6733076A92EE
200358ECA4010102010357A1028181526465637279707465642D6669726D
776172651158CB860C0014A212582E344FA2D5AD2F43F6F363DA6FF2C337
FE69E33E3D63714D23985BF02499EB0E8B231D45C378245DA3611C160CC5
11135890D8608443A10101A10550DAE613B2E0DC55F4322BE38BDBA9DC68
F6818344A101381CA220A401022001215820FF6E266DABAF51B7207569E3
1CF72646183E94CEE64FCDC8695AD9A505AEFDEA2258205FBC4A29844450
B3AC22AB30C7F7004BB59D8BD60D7997734A9FA0124B65089504456B6964
2D325818B0E21628283F3E409F8158D8FFCA567F340E379AC39E49C9120F
~~~~
