
## Example 3: Supplying Personalization Data for Trusted Component Binary {#suit-personalization}

~~~~
/ SUIT_Envelope / {
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / cose-alg-sha256 /,
      / digest-bytes: / h'99D1AD5D2C44320B247BB37B8CDB418CADA53590DF4EF928E30240F5717088FA'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'BC73FA2E58ACEE2A95B3D3B304532696C8A180138208FCC7C17A9595E4BE4D08189FC778347A431833B27ACD663EDFBC3E0165367F48C7073E6B0E5981CA995C'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 3,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
            'TEEP-Device',
            'SecureFS',
            h'8D82573A926D4754935332DC29997F74', / tc-uuid /
            'suit'
          ]
        }
      },
      / components / 2: [
        [
          'TEEP-Device',
          'SecureFS',
          'config.json'
        ]
      ],
      / shared-sequence / 4: << [
        / directive-set-component-index / 12, 0,
        / directive-override-parameters / 20, {
          / vendor-id / 1: h'C0DDD5F15243566087DB4F5B0AA26C2F' / c0ddd5f1-5243-5660-87db-4f5b0aa26c2f /,
          / class-id / 2: h'DB42F7093D8C55BAA8C5265FC5820F4E' / db42f709-3d8c-55ba-a8c5-265fc5820f4e /,
          / image-digest / 3: << [
            / algorithm-id: / -16 / SHA-256 /,
            / digest-bytes: / h'AAABCCCDEEEF00012223444566678889ABBBCDDDEFFF01112333455567778999'
          ] >>,
          / image-size / 14: 64
        },
        / condition-vendor-identifier / 1, 15,
        / condition-class-identifier / 2, 15
      ] >>
    } >>,
    / manifest-component-id / 5: [
      'TEEP-Device',
      'SecureFS',
      'config.suit'
    ],
    / validate / 7: << [
      / directive-set-component-index / 12, 0,
      / condition-image-match / 3, 15
    ] >>,
    / dependency-resolution / 15: << [
      / directive-set-component-index / 12, 1,
      / directive-override-parameters / 20, {
        / uri / 21: "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.suit"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>,
    / install / 17: << [
      / directive-set-component-index / 12, 1,
      / directive-process-dependency / 11, 0,
      / directive-set-component-index / 12, 0,
      / directive-override-parameters / 20, {
        / uri / 21: "https://example.org/config.json"
      },
      / directive-fetch / 21, 2,
      / condition-image-match / 3, 15
    ] >>
  } >>
}
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
A2025873825824822F582099D1AD5D2C44320B247BB37B8CDB418CADA535
90DF4EF928E30240F5717088FA584AD28443A10126A0F65840A2A53D8930
019C96961294C184327A5A44765B06BB033643E0ED079FC5DA14B5B69DE4
20F236080B8C5C764753A28B5575FB13EB5D1AE17CDF8BFAC31975CA4603
590160A7010102030358B0A301A101A101844B544545502D446576696365
485365637572654653508D82573A926D4754935332DC29997F7444737569
740281834B544545502D4465766963654853656375726546534B636F6E66
69672E6A736F6E045857880C0014A40150C0DDD5F15243566087DB4F5B0A
A26C2F0250DB42F7093D8C55BAA8C5265FC5820F4E035824822F5820AAAB
CCCDEEEF00012223444566678889ABBBCDDDEFFF01112333455567778999
0E1840010F020F05834B544545502D446576696365485365637572654653
4B636F6E6669672E737569740745840C00030F0F5849880C0114A115783D
68747470733A2F2F6578616D706C652E6F72672F38643832353733612D39
3236642D343735342D393335332D3332646332393939376637342E737569
741502030F11582F8C0C010B000C0014A115781F68747470733A2F2F6578
616D706C652E6F72672F636F6E6669672E6A736F6E1502030F
~~~~
