
## E.4. Example 4: Unlink a Trusted Component {#suit-unlink}

~~~~
/ SUIT_Envelope / {
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / cose-alg-sha256 /,
      / digest-bytes: / h'F9D837B7118A8972360022D51AC5319023D245A76F7504079B9ED01C0B4934F2'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'9BAA7D6BD0284C37DF7F85A0E98865F3FC03DE474C69F2A786F0569494DE57B911C245A84105E67F08EB6982358860F48FB31FD95BCB75862713CAF6BA28D198'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 0xFFFFFFFFFFFFFFFF,
    / common / 3: << {
      / components / 2: [
        [
          'TEEP-Device',
          'SecureFS',
          h'8D82573A926D4754935332DC29997F74',  / tc-uuid /
          'ta'
        ]
      ],
      / common-sequence / 4: << [
        / directive-override-parameters / 20, {
          / vendor-id / 1: h'C0DDD5F15243566087DB4F5B0AA26C2F' / c0ddd5f1-5243-5660-87db-4f5b0aa26c2f /,
          / class-id / 2: h'DB42F7093D8C55BAA8C5265FC5820F4E' / db42f709-3d8c-55ba-a8c5-265fc5820f4e /
        },
        / condition-vendor-identifier / 1, 15,
        / condition-class-identifier / 2, 15
      ] >>
    } >>,
    / manifest-component-id / 5: [
      'TEEP-Device',
      'SecureFS',
      h'8D82573A926D4754935332DC29997F74',  / tc-uuid /
      'suit'
    ],
    / install / 17: << [
      / directive-set-component-index / 12, 0,
      / directive-unlink / 33, 0
    ] >>
  } >>
}
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
A2025873825824822F5820F9D837B7118A8972360022D51AC5319023D245
A76F7504079B9ED01C0B4934F2584AD28443A10126A0F658409BAA7D6BD0
284C37DF7F85A0E98865F3FC03DE474C69F2A786F0569494DE57B911C245
A84105E67F08EB6982358860F48FB31FD95BCB75862713CAF6BA28D19803
58A0A50101021BFFFFFFFFFFFFFFFF03585BA20281844B544545502D4465
76696365485365637572654653508D82573A926D4754935332DC29997F74
42746104582B8614A20150C0DDD5F15243566087DB4F5B0AA26C2F0250DB
42F7093D8C55BAA8C5265FC5820F4E010F020F05844B544545502D446576
696365485365637572654653508D82573A926D4754935332DC29997F7444
737569741146840C00182100
~~~~
