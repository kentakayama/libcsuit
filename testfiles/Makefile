DIAG2CBOR := diag2cbor.rb
SIGNER := ../bin/suit_manifest_sign
MACER := ../bin/suit_manifest_mac
MANIFESTS := suit_manifest_exp0.suit \
	suit_manifest_exp1.suit \
	suit_manifest_exp2A.suit \
	suit_manifest_exp2B.suit \
	suit_manifest_exp3.suit \
	suit_manifest_exp4.suit \
	suit_manifest_exp5.suit \
	suit_manifest_expU.suit \
	suit_manifest_expI.suit \
	suit_manifest_expD.suit \
	suit_manifest_expAF.suit \
	suit_manifest_expAFS.suit \
	suit_manifest_expAW.suit \
	suit_manifest_expEW.suit \
	suit_manifest_expED.suit \
	suit_manifest_expS0.suit \
	suit_manifest_expS2.suit \
	suit_manifest_expS3.suit \
	suit_manifest_expU0.suit \
	suit_manifest_expU1.suit

.PHONY: all
all: $(MANIFESTS)

suit_manifest_expAF.suit: suit_manifest_expAF.cbor | $(MACER)
suit_manifest_expAFS.suit: suit_manifest_expAFS.cbor | $(MACER)
suit_manifest_expAW.suit: suit_manifest_expAW.cbor | $(MACER)

suit_manifest_expAF.suit suit_manifest_expAFS.suit suit_manifest_expAW.suit:
	$(MACER) $< $@

%.suit: %.cbor | $(SIGNER)
	$(SIGNER) $< $@

%.cbor: %.diag
	$(DIAG2CBOR) $< > $@
