
## Example 3: Supplying Personalization Data for Trusted Component Binary {#suit-personalization}
{: numbered='no'}

### CBOR Diagnostic Notation of SUIT Manifest
{: numbered='no'}

~~~~
/ SUIT_Envelope = / 107({
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
      / signature: / h'77D2BBC1E055519CD7EA49A9999AFB0B814C7CEDD29FB42B498CE3B50A014A2608535DCE142620D9C447FE26E5653F405AD5EC7B0D697E7F9C99E4B176D5DC60'
    ]) >>
  ] >>,
  / manifest / 3: << {
    / manifest-version / 1: 1,
    / manifest-sequence-number / 2: 3,
    / common / 3: << {
      / dependencies / 1: {
        / component-index / 1: {
          / dependency-prefix / 1: [
            h'544545502D446576696365',           / "TEEP-Device" /
            h'5365637572654653',                 / "SecureFS" /
            h'8D82573A926D4754935332DC29997F74', / tc-uuid /
            h'73756974'                          / "suit" /
          ]
        }
      },
      / components / 2: [
        [
          h'544545502D446576696365',  / "TEEP-Device" /
          h'5365637572654653',        / "SecureFS" /
          h'636F6E6669672E6A736F6E'   / "config.json" /
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
      h'544545502D446576696365',  / "TEEP-Device" /
      h'5365637572654653',        / "SecureFS" /
      h'636F6E6669672E73756974'   / "config.suit" /
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
})
~~~~


### CBOR Binary in Hex
{: numbered='no'}

~~~~
d86ba2025873825824822f582099d1ad5d2c44320b247bb37b8cdb418cad
a53590df4ef928e30240f5717088fa584ad28443a10126a0f6584077d2bb
c1e055519cd7ea49a9999afb0b814c7cedd29fb42b498ce3b50a014a2608
535dce142620d9c447fe26e5653f405ad5ec7b0d697e7f9c99e4b176d5dc
6003590160a7010102030358b0a301a101a101844b544545502d44657669
6365485365637572654653508d82573a926d4754935332dc29997f744473
7569740281834b544545502d4465766963654853656375726546534b636f
6e6669672e6a736f6e045857880c0014a40150c0ddd5f15243566087db4f
5b0aa26c2f0250db42f7093d8c55baa8c5265fc5820f4e035824822f5820
aaabcccdeeef00012223444566678889abbbcdddefff0111233345556777
89990e1840010f020f05834b544545502d44657669636548536563757265
46534b636f6e6669672e737569740745840c00030f0f5849880c0114a115
783d68747470733a2f2f6578616d706c652e6f72672f3864383235373361
2d393236642d343735342d393335332d3332646332393939376637342e73
7569741502030f11582f8c0c010b000c0014a115781f68747470733a2f2f
6578616d706c652e6f72672f636f6e6669672e6a736f6e1502030f
~~~~
