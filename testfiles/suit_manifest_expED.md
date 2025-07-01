<!--
 Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.

 SPDX-License-Identifier: BSD-2-Clause
-->

## Example 3: ES-ECDH + AES-KW + Process Dependency
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope_Tagged / 107({
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / SHA256 /,
      / digest-bytes: / h'A00CB6C85515C1EF471B50B542FACDD88B71B3C7EA2A43DE13D32C4A99056FE9'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -9 / ESP256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'3000301B7C54B3383CC4723C4B7BE667C6760C504213A105DD38401BED5EEF8EB915F8313420104F59467D76790A0EA220B6021B4ED87051B3B4A8D05F7E0254'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 1,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
             'dependency-manifest.suit'
          ]
        }
      },
      / components / 2: [
        ['decrypted-firmware']
      ]
    } >>,
    / manifest-component-id / 5: [
      'dependent-manifest.suit'
    ],
    / install / 20: << [
      / NOTE: set SUIT_Encryption_Info /
      / directive-set-component-index / 12, 0 / ['decrypted-firmware'] /,
      / directive-override-parameters / 20, {
        / parameter-content / 18:
          h'758C4B7BBAE2C4C1D462423E0F0DC3164FFA7B85BB94D4BD6D7ED26AB32FEB063385D4D3465927EC82CB5E198A59',
        / parameter-encryption-info / 19: << 96([
          / protected: / << {
            / alg / 1: 1 / A128GCM /
          } >>,
          / unprotected: / {
            / IV / 5: h'F14AAB9D81D51F7AD943FE87'
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
                  / x / -2: h'73024F415AA51529A66CCEFD88F3F62A734492FF45F6AD37FD2888E73EAF19DA',
                  / y / -3: h'4005B48A6FD091AA6ABFE3CFBEEDE88B347E521D43405FDBD7D2CFF0EBC21B26'
                },
                / kid / 4: 'kid-2'
              },
              / payload: / h'A06B8E6550F308712B1DF044B21B7D11D9B22792F1DE0997'
                / CEK encrypted with KEK /
            ]
          ]
        ]) >>
      },

      / NOTE: call dependency-manifest /
      / directive-set-component-index / 12, 1 / ['dependenty-manifest.suit'] /,
      / directive-override-parameters / 20, {
        / parameter-image-digest / 3: << [
          / algorithm-id / -16 / SHA256 /,
          / digest-bytes / h'4B15C90FBD776A820E7E733DF040D90B356B5C75982ECAECE8673818179BDF16'
        ] >>,
        / parameter-image-size / 14: 247,
        / parameter-uri / 21: "#dependency-manifest"
      },
      / directive-fetch / 21, 15,
      / condition-dependency-integrity / 7, 15,
      / directive-process-dependency / 11, 15
    ] >>
  } >>,
  "#dependency-manifest": <<
    / SUIT_Envelope_Tagged / 107({
      / authentication-wrapper / 2: << [
        << [
          / digest-algorithm-id: / -16 / SHA256 /,
          / digest-bytes: / h'4B15C90FBD776A820E7E733DF040D90B356B5C75982ECAECE8673818179BDF16'
        ] >>,
        << / COSE_Sign1_Tagged / 18([
          / protected: / << {
            / algorithm-id / 1: -9 / ESP256 /
          } >>,
          / unprotected: / {},
          / payload: / null,
          / signature: / h'BF95C29295B45470EF819E7F4E3C9084F4534E26469C0A0F2B8B9664881A5359D500F81BD3A6436A025C3E92E51CD7148F83DF47D29FF253F8D41B4D350A2B75'
        ]) >>
      ] >>,
      / manifest / 3: << {
        / manifest-version / 1: 1,
        / manifest-sequence-number / 2: 1,
        / common / 3: << {
          / components / 2: [
            ['decrypted-firmware']
          ],
          / shared-sequence / 4: << [
            / directive-set-componnt-index / 12, 0 / ['decrypted-firmware'] /,
            / directive-override-parameters / 20, {
              / parameter-image-digest / 3: << [
                / algorithm-id / -16 / SHA256 /,
                / digest-bytes /
                  h'36921488FE6680712F734E11F58D87EEB66D4B21A8A1AD3441060814DA16D50F'
              ] >>,
              / parameter-image-size / 14: 30
            }
          ] >>
        } >>,
        / manifest-component-id / 5: [
          'dependency-manifest.suit'
        ],
        / validate / 7: << [
          / condition-image-match / 3, 15
        ] >>,
        / install / 20: << [
          / directive-set-component-index / 12, 0
            / ['decrypted-firmware'] /,
          / directive-write / 18, 15
            / consumes the SUIT_Encryption_Info set by dependent /,
          / condition-image-match / 3, 15
            / check the integrity of the decrypted payload /
        ] >>
      } >>
    })
  >>
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
D86BA3025873825824822F5820A00CB6C85515C1EF471B50B542FACDD88B
71B3C7EA2A43DE13D32C4A99056FE9584AD28443A10128A0F65840300030
1B7C54B3383CC4723C4B7BE667C6760C504213A105DD38401BED5EEF8EB9
15F8313420104F59467D76790A0EA220B6021B4ED87051B3B4A8D05F7E02
540359016CA501010201035837A201A101A101815818646570656E64656E
63792D6D616E69666573742E73756974028181526465637279707465642D
6669726D77617265058157646570656E64656E742D6D616E69666573742E
737569741459010F8E0C0014A212582E758C4B7BBAE2C4C1D462423E0F0D
C3164FFA7B85BB94D4BD6D7ED26AB32FEB063385D4D3465927EC82CB5E19
8A5913588CD8608443A10101A1054CF14AAB9D81D51F7AD943FE87F68183
44A101381CA220A40102200121582073024F415AA51529A66CCEFD88F3F6
2A734492FF45F6AD37FD2888E73EAF19DA2258204005B48A6FD091AA6ABF
E3CFBEEDE88B347E521D43405FDBD7D2CFF0EBC21B2604456B69642D3258
18A06B8E6550F308712B1DF044B21B7D11D9B22792F1DE09970C0114A303
5824822F58204B15C90FBD776A820E7E733DF040D90B356B5C75982ECAEC
E8673818179BDF160E18F7157423646570656E64656E63792D6D616E6966
657374150F070F0B0F7423646570656E64656E63792D6D616E6966657374
58F7D86BA2025873825824822F58204B15C90FBD776A820E7E733DF040D9
0B356B5C75982ECAECE8673818179BDF16584AD28443A10128A0F65840BF
95C29295B45470EF819E7F4E3C9084F4534E26469C0A0F2B8B9664881A53
59D500F81BD3A6436A025C3E92E51CD7148F83DF47D29FF253F8D41B4D35
0A2B7503587BA601010201035849A2028181526465637279707465642D66
69726D7761726504582F840C0014A2035824822F582036921488FE668071
2F734E11F58D87EEB66D4B21A8A1AD3441060814DA16D50F0E181E058158
18646570656E64656E63792D6D616E69666573742E73756974074382030F
1447860C00120F030F
~~~~
