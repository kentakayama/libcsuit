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
      / digest-bytes: / h'4C56CA660A5D1414BC04C835025D52CCA9AE6101202E127329AD2465B38A1C89'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'ACC8962628B78BF30DD74BDEEA9305D73BFA302D82B280A7E2FCE8331C363F279ECCABE920DA97F9074DF5B3B2AAD1709D844B8DE1D33F80FA99AC806B9778D0'
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
        / parameter-content / 18: h'B94272BD7C7E9A144D12CF46D9CEE6318753574A6F780829B87911BE1CF2B24477BA4E7D1337541F308010088920',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / AES-GCM-128 /
          } >>,
          / unprotected: / {
            / IV / 5: h'3517CE3E78AC2BF3D1CDFDAF955E8600'
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
                  / x / -2: h'AAE9A733DEF11E9160A66BD81CC8215F045ACAC3F8490C7749D58A627323624A',
                  / y / -3: h'08A7B88B7F00762BA0919CA065ABF45C2A303B483E86D674E50B015122F8E515'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'0A44E77C3DBBB0780F2DB42C64FD325D18FBE13A25A9369D'
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
D86BA2025873825824822F58204C56CA660A5D1414BC04C835025D52CCA9
AE6101202E127329AD2465B38A1C89584AD28443A10126A0F65840ACC896
2628B78BF30DD74BDEEA9305D73BFA302D82B280A7E2FCE8331C363F279E
CCABE920DA97F9074DF5B3B2AAD1709D844B8DE1D33F80FA99AC806B9778
D00358ECA4010102010357A1028181526465637279707465642D6669726D
776172651158CB860C0014A212582EB94272BD7C7E9A144D12CF46D9CEE6
318753574A6F780829B87911BE1CF2B24477BA4E7D1337541F3080100889
20135890D8608443A10101A105503517CE3E78AC2BF3D1CDFDAF955E8600
F6818344A101381CA220A401022001215820AAE9A733DEF11E9160A66BD8
1CC8215F045ACAC3F8490C7749D58A627323624A22582008A7B88B7F0076
2BA0919CA065ABF45C2A303B483E86D674E50B015122F8E51504456B6964
2D3258180A44E77C3DBBB0780F2DB42C64FD325D18FBE13A25A9369D120F
~~~~
