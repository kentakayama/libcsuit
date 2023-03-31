# Supported features by libcsuit

SUMMARY_START

SUMMARY_END


## NOTE: How to read the Supported Features Tables
**IN**: which document
Name | Reference
---|---
SUIT Manifest | https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest
SUIT Multiple Trust Domains | https://datatracker.ietf.org/doc/html/draft-moran-suit-trust-domains
SUIT Update Manegement | https://datatracker.ietf.org/doc/html/draft-ietf-suit-update-management
SUIT Encrypted Payload | https://datatracker.ietf.org/doc/html/draft-ietf-suit-firmware-encryption
SUIT Report | https://datatracker.ietf.org/doc/html/draft-ietf-suit-report

**IS**:
- It is **REQUIRED** to (or **MUST**) implement
- It is **RECOMMENDED** to (or **SHOULD**) implement
- It is **OPTIONAL** to (or **MAY**) implement
- It is Not Mentioned (**N/M**) to implement
Only SUIT Manifest document defines REQUIRED features for minimal implementation, so that other documents may not mention.

**Supported?**: libcsuit has 3 core features: decode (Read), encode (Write) and process (eXecute).  
SUIT features are listed below for each SUIT documents.
- **R**: supported, **r**: partially supported, and **-**: not supported
- **W**: supported, **w**: partially supported, and **-**: not supported
- **X**: supported, **x**: partially supported, and **-**: not supported


## Supported Features Tables
### SUIT Metadata

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---
Delegation | 1 | suit-delegation | SUIT Multiple Trust Domains | N/M | ---
Authentication Wrapper | 2 | suit-authentication-wrapper | SUIT Manifest | N/M | RWX
Manifest | 3 | suit-manifest | SUIT Manifest | N/M | RWX
CoSWID (s) | 14 | suit-coswid | SUIT Update Management | RECOMMENDED | RW-
Payload Fetch (s) | 16 | suit-payload-fetch | SUIT Manifest | OPTIONAL | RWX
Payload Installation (s) | 17 | suit-install | SUIT Manifest | OPTIONAL | RWX
Text Description (s) | 23 | suit-text | SUIT Manifest | OPTIONAL | RWX
Uninstall (s) | 24 | suit-uninstall | SUIT Multiple Trust Domains | N/M | RW-

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---
Encoding Version | 1 | suit-manifest-version | SUIT Manifest | REQUIRED | RWX
Sequence Number | 2 | suit-manifest-sequence-number | SUIT Manifest | REQUIRED | RWX
Common Data | 3 | suit-common | SUIT Manifest | REQUIRED | RWX
Reference URI | 4 | suit-reference-uri | SUIT Manifest | N/M | RW?
Manifest Component ID | 5 | suit-manifest-component-id | SUIT Multiple Trust Domains | N/M | RWX
Image Validation | 7 | suit-validate | SUIT Manifest | REQUIRED | RWX
Image Loading | 8 | suit-load | SUIT Manifest | OPTIONAL | RWX
Image Invocation | 9 | suit-invoke | SUIT Manifest | OPTIONAL | RWX
CoSWID | 14 | suit-coswid | SUIT Update Management | RECOMMENDED | RW-
Dependency Resolution | 15 | suit-dependency-resolution | SUIT Multiple Trust Domains | N/M | RWX
Payload Fetch | 16 | suit-payload-fetch | SUIT Manifest | OPTIONAL | RWX
Payload Installation | 17 | suit-install | SUIT Manifest | OPTIONAL | RWX
Text Description | 23 | suit-text | SUIT Manifest | OPTIONAL | RWX
Uninstall | 24 | suit-uninstall | SUIT Multiple Trust Domains | N/M | RW-

### SUIT Common ext?

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---
Dependencies | 1 | suit-dependencies | SUIT Multiple Trust Domains | N/M | RW-
Components | 2 | suit-components | SUIT Manifest | REQUIRED | RWX
Shared Sequence | 4 | suit-shared-sequence | SUIT Manifest | RECOMMENDED/REQUIRED | RWX

### SUIT Condition

Name | Label | CDDL Structure | IN | Is | Supported?
---|---|---|---|---|---
Vendor Identifier | 1 | suit-condition-vendor-identifier | SUIT Manifest | REQUIRED | RW-
Class Identifier | 2 | suit-condition-class-identifier | SUIT Manifest | N/M | RW-
Image Match | 3 | suit-condition-image-match | SUIT Manifest | REQUIRED | RW-
Use Before | 4 | suit-condition-use-before | SUIT Update Management | OPTIONAL | RW-
Component Slot | 5 | suit-condition-component-slot | SUIT Manifest | N/M | RW-
Check Content | 6 | suit-condition-check-content | SUIT Manifest | N/M | RW-
Dependency Integrity | 7 | suit-condition-dependency-integrity | SUIT Multiple Trust Domains | N/M | ---
Is Dependency | 8 | suit-condition-is-dependency | SUIT Multiple Trust Domains | N/M | ---
Abort | 14 | suit-condition-abort | SUIT Manifest | N/M | RW-
Device Identifier | 24 | suit-condition-device-identifier | SUIT Manifest | OPTIONAL | RW-
Image Not Match | 25 | suit-condition-image-not-match | SUIT Update Management | OPTIONAL | RW-
Minimum Battery | 26 | suit-condition-minimum-battery | SUIT Update Management | OPTIONAL | RW-
Update Authorized | 27 | suit-condition-update-authorized | SUIT Update Management | OPTIONAL | RW-
Version | 28 | suit-condition-version | SUIT Update Management | N/M | RW-
Custom Condition | ? | suit-condition-custom | SUIT Manifest | N/M | ---

### SUIT Directive

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Process Dependency | 11 | suit-directive-process-dependency | SUIT Multiple Trust Domains | N/M | RW-
Set Component Index | 12 | suit-directive-set-component-index | SUIT Manifest | REQUIRED | RW?
Try Each | 15 | suit-directive-try-each | SUIT Manifest | OPTIONAL | ---
Write | 18 | suit-directive-write | SUIT Manifest | N/M | ---
Set Parameters | 19 | suit-directive-set-parameters | SUIT Multiple Trust Domains | N/M | RW-
Override Parameters | 20 | suit-directive-override-parameters | SUIT Manifest | N/M | RW?
Fetch | 21 | suit-directive-fetch | SUIT Manifest | N/M | RW?
Copy | 22 | suit-directive-copy | SUIT Manifest | N/M | ---
Invoke | 23 | suit-directive-invoke | SUIT Manifest | REQUIRED for Bootloader | RW?
Wait | 29 | suit-directive-wait | SUIT Update Management | N/M | ---
Swap | 31 | suit-directive-swap | SUIT Manifest | N/M | ---
Run Sequence | 32 | suit-directive-run-sequence | SUIT Manifest | OPTIONAL | ---
Unlink | 33 | suit-directive-unlink | SUIT Multiple Trust Domains | N/M | RW-
Override Multiple | ? | suit-directive-override-multiple | SUIT Update Management | N/M | ---
Copy Params | ? | suit-directive-copy-params | SUIT Update Management | N/M | ---

### SUIT Parameters

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Vendor ID | 1 | suit-parameter-vendor-identifier | SUIT Manifest | N/M | RW?
Class ID | 2 | suit-parameter-class-identifier | SUIT Manifest | N/M | RW?
Image Digest | 3 | suit-parameter-image-digest | SUIT Manifest | N/M | RW?
Use Before | 4 | suit-parameter-use-before | SUIT Update Management | N/M | RW-
Component Slot | 5 | suit-parameter-component-slot | SUIT Manifest | N/M | RW?
Strict Order | 12 | suit-parameter-strict-order | SUIT Manifest | N/M | RW-
Soft Failure | 13 | suit-parameter-soft-failure | SUIT Manifest | N/M | RW-
Image Size | 14 | suit-parameter-image-size | SUIT Manifest | N/M | RW?
Content | 18 | suit-parameter-content | SUIT Manifest | N/M | ---
Encryption Info | 19 | suit-parameter-encryption-info | SUIT Encrypted Payload | N/M | ---
URI | 21 | suit-parameter-uri | SUIT Manifest | N/M | RWX
Source Component | 22 | suit-parameter-source-component | SUIT Manifest | N/M | RW-
Invoke Args | 23 | suit-parameter-invoke-args | SUIT Manifest | N/M | RW?
Device ID | 24 | suit-parameter-device-identifier | SUIT Manifest | N/M | RW?
Minimum Battery | 26 | suit-parameter-minimum-battery | SUIT Update Management | N/M | RW-
Update Priority | 27 | suit-parameter-update-priority | SUIT Update Management | N/M | RW-
Version | 28 | suit-parameter-version | SUIT Update Management | N/M | ---
Wait Info | 29 | suit-parameter-wait-info | SUIT Update Management | N/M | ---
Fetch Arguments | ? | suit-parameter-fetch-arguments | SUIT Manifest | N/M | ---
Custom | nint | suit-parameter-custom | SUIT Manifest | N/M | ---

### SUIT Text

NOTE: libcsuit ignores this while processing a SUIT Manifest

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---
Vendor Name | 1 | suit-text-vendor-name | SUIT Manifest | OPTIONAL | RW-
Model Name | 2 | suit-text-model-name | SUIT Manifest | OPTIONAL | RW-
Vendor Domain | 3 | suit-text-vendor-domain | SUIT Manifest | OPTIONAL | RW-
Model Info | 4 | suit-text-model-info | SUIT Manifest | OPTIONAL | RW-
Component Description | 5 | suit-text-component-description | SUIT Manifest | OPTIONAL | RW-
Component Version | 6 | suit-text-component-version | SUIT Manifest | OPTIONAL | RW-
Version Required | 7 | suit-text-version-required | SUIT Update Management | N/M | RW-

