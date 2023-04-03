
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
      / signature: / h'E9F3FDF01FA0896C3CB29D166A6183C660A5DCEB8FFB9AC7E272C9EF7A9BD5D7651C4C426717A95408B13F56BAEF778BD463DA33D9483FFD88CCB548FE29D850'
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
7252DAFC57A0098284C3E73587584AD28443A10126A0F65840B1C64B76EB
3F7293498E230187F14643F43C3F1DC27B22E7891E01495E1465E072B6FA
5F130FC68B6453B321D7EBBFD2C321C59152F52263ECD57663880B6E9D03
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
