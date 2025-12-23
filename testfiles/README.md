## SUIT Protocol Message Examples
The following description Markdown and CBOR files are compliant with each document.

- [draft-ietf-suit-manifest-34](https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest-34)

Example | Human Readable | Binary
----|----|----
Example 0: Secure Boot | [suit_manifest_exp0.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp0.md) | [suit_manifest_exp0.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp0.suit)
Example 1: Simultaneous Download and Installation of Payload | [suit_manifest_exp1.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp1.md) | [suit_manifest_exp1.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp1.suit)
Example 2: Simultaneous Download, Installation, Secure Boot, Severed Fields | [suit_manifest_exp2.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp2.md) | [suit_manifest_exp2.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp2.suit)
Example 3: A/B images | [suit_manifest_exp3.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp3.md) | [suit_manifest_exp3.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp3.suit)
Example 4: Load from External Storage | [suit_manifest_exp4.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp4.md) | [suit_manifest_exp4.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp4.suit)
Example 5: Two Images | [suit_manifest_exp5.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp5.md) | [suit_manifest_exp5.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_exp5.suit)

- [draft-ietf-suit-trust-domains-12](https://datatracker.ietf.org/doc/html/draft-ietf-suit-trust-domains-12)

Example | Human Readable | Binary
----|----|----
Example S0 | Write Plaintext Payload | [suit_manifest_expS0.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS0.md) | [suit_manifest_exp_S0.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS0.suit)
Example S2 | Integrated Dependency | [suit_manifest_expS2.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS2.md) | [suit_manifest_exp_S2.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS2.suit)
Example S3 | TryEach and IsDependency | [suit_manifest_expS3.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS3.md) | [suit_manifest_exp_S3.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expS3.suit)

- [draft-ietf-suit-firmware-encryption-26](https://datatracker.ietf.org/doc/html/draft-ietf-suit-firmware-encryption-26)

Example | Human Readable | Binary
----|----|----
9.1. Example 0: Decrypt A128KW + A128GCM Payload on Write | [suit_manifest_expAW.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expAW.md) | [suit_manifest_expAW.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expAW.suit)
9.2. Example 1: Decrypt A128KW + A128GCM Payload on Fetch | [suit_manifest_expAF.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expAF.md) | [suit_manifest_expAF.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expAF.suit)
9.3. Example 2: Decrypt ECDH-ES+A128KW + A128GCM Payload on Write | [suit_manifest_expEW.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expEW.md) | [suit_manifest_expEW.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expEW.suit)
9.4. Example 3: Decrypt ECDH-ES+A128 + A128GCM Payload on Fetch | [suit_manifest_expEF.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expEF.md) | [suit_manifest_expEF.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expEF.suit)

- [draft-ietf-suit-update-management-10](https://datatracker.ietf.org/doc/html/draft-ietf-suit-update-management-10)

Example | Human Readable | Binary
----|----|----
Example U0: Copy Params Example | [suit_manifest_expU0.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU0.md) | [suit_manifest_expU0.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU0.suit)
Example U1: Override Multiple Example | [suit_manifest_expU1.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU1.md) | [suit_manifest_expU1.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU1.suit)
Example U2: Wait and Condition Example | [suit_manifest_expU2.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU2.md) | [suit_manifest_expU2.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU2.suit)
Example U3: Component Metadata Example | [suit_manifest_expU3.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU3.md) | [suit_manifest_expU3.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU3.suit)

- [draft-ietf-teep-protocol-21](https://datatracker.ietf.org/doc/html/draft-ietf-teep-protocol-21)

Example | Human Readable | Binary
----|----|----
Example 1: Fetch from URI | [suit_manifest_expU.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU.md) | [suit_manifest_expU.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expU.suit)
Example 2: Fetch from Integrated Payload | [suit_manifest_expI.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expI.md) | [suit_manifest_expI.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expI.suit)
Example 3: Personalization Data | [suit_manifest_expD.md](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expD.md) | [suit_manifest_expD.suit](https://github.com/kentakayama/libcsuit/blob/master/testfiles/suit_manifest_expD.suit)
