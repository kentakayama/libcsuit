
## Example 1: SUIT Manifest pointing to URI of the Trusted Component Binary {#suit-uri}
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope / {
  / authentication-wrapper / 2: << [
    << [
      / digest-algorithm-id: / -16 / cose-alg-sha256 /,
      / digest-bytes: / h'B892B75B262B728EC2C16C2774A730DC804BC87252DAFC57A0098284C3E73587'
    ] >>,
    << / COSE_Sign1_Tagged / 18([
      / protected: / << {
        / algorithm-id / 1: -7 / ES256 /
      } >>,
      / unprotected: / {},
      / payload: / null,
      / signature: / h'36B477DB298D1A554A52B3414CDCCE8569D8B2BDE6C0D3A237DC088B749F228CE0D03EFDF089474788549346F5DE21AB03066DA2D1C4D1F4B106D67ACE3B0726'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 3,
    / common / 3: << {
      / components / 2: [
        [
          'TEEP-Device',
          'SecureFS',
          h'8D82573A926D4754935332DC29997F74', / tc-uuid /
          'ta'
        ]
      ],
      / shared-sequence / 4: << [
        / directive-override-parameters / 20, {
          / parameter-vendor-identifier / 1: h'C0DDD5F15243566087DB4F5B0AA26C2F',
          / parameter-class-identifier / 2: h'DB42F7093D8C55BAA8C5265FC5820F4E',
          / parameter-image-digest / 3: << [
            / digest-algorithm-id: / -16 / cose-alg-sha256 /,
            / digest-bytes: / h'8CF71AC86AF31BE184EC7A05A411A8C3A14FD9B77A30D046397481469468ECE8'
          ] >>,
          / parameter-image-size / 14: 20
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
      / directive-override-parameters / 20, {
        / parameter-uri / 21: "https://example.org/8d82573a-926d-4754-9353-32dc29997f74.ta"
      },
      / directive-fetch / 21, 15,
      / condition-image-match / 3, 15
    ] >>
  } >>
}
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
A2025873825824822F5820B892B75B262B728EC2C16C2774A730DC804BC8
7252DAFC57A0098284C3E73587584AD28443A10126A0F6584036B477DB29
8D1A554A52B3414CDCCE8569D8B2BDE6C0D3A237DC088B749F228CE0D03E
FDF089474788549346F5DE21AB03066DA2D1C4D1F4B106D67ACE3B072603
590101A501010203035884A20281844B544545502D446576696365485365
637572654653508D82573A926D4754935332DC29997F7442746104585486
14A40150C0DDD5F15243566087DB4F5B0AA26C2F0250DB42F7093D8C55BA
A8C5265FC5820F4E035824822F58208CF71AC86AF31BE184EC7A05A411A8
C3A14FD9B77A30D046397481469468ECE80E14010F020F05844B54454550
2D446576696365485365637572654653508D82573A926D4754935332DC29
997F7444737569741158458614A115783B68747470733A2F2F6578616D70
6C652E6F72672F38643832353733612D393236642D343735342D39333533
2D3332646332393939376637342E7461150F030F
~~~~
