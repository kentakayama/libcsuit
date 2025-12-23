# Supported features by libcsuit

## Summary
- :green_square: : supported
- :red_square: : **not** supported
### SUIT Manifest
Operation | Metadata | Condition | Directive | Parameter | Text
---|---|---|---|---|---
decode | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square:
encode | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square:
process | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square::green_square:
### SUIT Multiple Trust Domains
Operation | Metadata | Condition | Directive | Parameter | Text
---|---|---|---|---|---
decode | :green_square::green_square::green_square::green_square: | :green_square::green_square: | :green_square::green_square::green_square: |  | 
encode | :green_square::green_square::green_square::green_square: | :green_square::green_square: | :green_square::green_square::green_square: |  | 
process | :green_square::green_square::green_square::green_square: | :green_square::green_square: | :green_square::green_square::green_square: |  | 
### SUIT Update Management
Operation | Metadata | Condition | Directive | Parameter | Text
---|---|---|---|---|---
decode | :green_square::green_square: | :green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square: | 
encode | :green_square::green_square: | :green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square: | 
process | :green_square::green_square: | :green_square::green_square::green_square::green_square::green_square: | :green_square::green_square::green_square: | :green_square::green_square::green_square::green_square::green_square::green_square: | 
### SUIT Encrypted Payload
Operation | Metadata | Condition | Directive | Parameter | Text
---|---|---|---|---|---
decode |  |  |  | :green_square::green_square::green_square: | 
encode |  |  |  | :green_square::green_square::green_square: | 
process |  |  |  | :green_square::green_square::red_square: | 
### SUIT MUD
Operation | Metadata | Condition | Directive | Parameter | Text
---|---|---|---|---|---
decode | :red_square: |  |  |  | 
encode | :red_square: |  |  |  | 
process | :red_square: |  |  |  | 
### SUIT Report
Operation | Report | Capabilities
---|---|---
decode | :green_square::green_square::green_square::green_square::red_square: | :red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square:
encode | :green_square::green_square::green_square::green_square::red_square: | :red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square:
process | :green_square::green_square::green_square::green_square::red_square: | :red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square::red_square:

## NOTE: How to read the Supported Features Tables
**IN**: which document
Name | Reference
---|---
SUIT Manifest | https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest
SUIT Multiple Trust Domains | https://datatracker.ietf.org/doc/html/draft-moran-suit-trust-domains
SUIT Update Manegement | https://datatracker.ietf.org/doc/html/draft-ietf-suit-update-management
SUIT Encrypted Payload | https://datatracker.ietf.org/doc/html/draft-ietf-suit-firmware-encryption
SUIT MUD | https://datatracker.ietf.org/doc/html/draft-ietf-suit-mud
SUIT Report | https://datatracker.ietf.org/doc/html/draft-ietf-suit-report

**IS**:
- It is **REQUIRED** to (or **MUST**) implement
- It is **RECOMMENDED** to (or **SHOULD**) implement
- It is **OPTIONAL** to (or **MAY**) implement
- It is Not Mentioned (**N/M**) to implement
Only SUIT Manifest document defines REQUIRED features for minimal implementation, so that other documents may not mention.

**Supported?**: libcsuit has 3 core features: decode (Read), encode (Write) and process (eXecute).  
SUIT features are listed below for each SUIT documents.
- **R**: supported, and **-**: not supported
- **W**: supported, and **-**: not supported
- **X**: supported, and **-**: not supported


## Supported Features Tables
## SUIT Manifest + Extensions
### SUIT Metadata

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Delegation | 1 | suit-delegation | SUIT Multiple Trust Domains | N/M | RWX
Authentication Wrapper | 2 | suit-authentication-wrapper | SUIT Manifest | N/M | RWX
Manifest | 3 | suit-manifest | SUIT Manifest | N/M | RWX
CoSWID (s) | 14 | suit-coswid | SUIT Update Management | RECOMMENDED | RWX
Payload Fetch (s) | 16 | suit-payload-fetch | SUIT Manifest | OPTIONAL | RWX
Candidate Verification | 18 | suit-candidate-verification | SUIT Multiple Trust Domains | N/M | RWX
Payload Installation (s) | 20 | suit-install | SUIT Manifest | OPTIONAL | RWX
Text Description (s) | 23 | suit-text | SUIT Manifest | OPTIONAL | RWX
SUIT MUD (s) | ? | suit-mud | SUIT MUD | OPTIONAL | ---

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Encoding Version | 1 | suit-manifest-version | SUIT Manifest | REQUIRED | RWX
Sequence Number | 2 | suit-manifest-sequence-number | SUIT Manifest | REQUIRED | RWX
Common Data | 3 | suit-common | SUIT Manifest | REQUIRED | RWX
Reference URI | 4 | suit-reference-uri | SUIT Manifest | N/M | RWX
Manifest Component ID | 5 | suit-manifest-component-id | SUIT Multiple Trust Domains | N/M | RWX
Set Version | 6 | suit-set-version | SUIT Update Management | N/M | RWX
Image Validation | 7 | suit-validate | SUIT Manifest | REQUIRED | RWX
Image Loading | 8 | suit-load | SUIT Manifest | OPTIONAL | RWX
Image Invocation | 9 | suit-invoke | SUIT Manifest | OPTIONAL | RWX
CoSWID | 14 | suit-coswid | SUIT Update Management | RECOMMENDED | RWX
Dependency Resolution | 15 | suit-dependency-resolution | SUIT Multiple Trust Domains | N/M | RWX
Payload Fetch | 16 | suit-payload-fetch | SUIT Manifest | OPTIONAL | RWX
Candidate Verification | 18 | suit-candidate-verification | SUIT Multiple Trust Domains | N/M | RWX
Payload Installation | 20 | suit-install | SUIT Manifest | OPTIONAL | RWX
Text Description | 23 | suit-text | SUIT Manifest | OPTIONAL | RWX
Uninstall | 24 | suit-uninstall | SUIT Multiple Trust Domains | N/M | RWX
SUIT MUD | ? | suit-mud | SUIT MUD | OPTIONAL | ---

### SUIT Common

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Dependencies | 1 | suit-dependencies | SUIT Multiple Trust Domains | N/M | RWX
Components | 2 | suit-components | SUIT Manifest | REQUIRED | RWX
Shared Sequence | 4 | suit-shared-sequence | SUIT Manifest | RECOMMENDED/REQUIRED | RWX

### SUIT Condition

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Vendor Identifier | 1 | suit-condition-vendor-identifier | SUIT Manifest | REQUIRED | RWX
Class Identifier | 2 | suit-condition-class-identifier | SUIT Manifest | N/M | RWX
Image Match | 3 | suit-condition-image-match | SUIT Manifest | REQUIRED | RWX
Use Before | 4 | suit-condition-use-before | SUIT Update Management | OPTIONAL | RWX
Component Slot | 5 | suit-condition-component-slot | SUIT Manifest | N/M | RWX
Check Content | 6 | suit-condition-check-content | SUIT Manifest | N/M | RWX
Dependency Integrity | 7 | suit-condition-dependency-integrity | SUIT Multiple Trust Domains | N/M | RWX
Is Dependency | 8 | suit-condition-is-dependency | SUIT Multiple Trust Domains | N/M | RWX
Abort | 14 | suit-condition-abort | SUIT Manifest | N/M | RWX
Device Identifier | 24 | suit-condition-device-identifier | SUIT Manifest | OPTIONAL | RWX
Image Not Match | 25 | suit-condition-image-not-match | SUIT Update Management | OPTIONAL | RWX
Minimum Battery | 26 | suit-condition-minimum-battery | SUIT Update Management | OPTIONAL | RWX
Update Authorized | 27 | suit-condition-update-authorized | SUIT Update Management | OPTIONAL | RWX
Version | 28 | suit-condition-version | SUIT Update Management | N/M | RWX

### SUIT Directive

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Process Dependency | 11 | suit-directive-process-dependency | SUIT Multiple Trust Domains | N/M | RWX
Set Component Index | 12 | suit-directive-set-component-index | SUIT Manifest | REQUIRED | RWX
Try Each | 15 | suit-directive-try-each | SUIT Manifest | OPTIONAL | RWX
Write | 18 | suit-directive-write | SUIT Manifest | N/M | RWX
Set Parameters | 19 | suit-directive-set-parameters | SUIT Multiple Trust Domains | N/M | RWX
Override Parameters | 20 | suit-directive-override-parameters | SUIT Manifest | N/M | RWX
Fetch | 21 | suit-directive-fetch | SUIT Manifest | N/M | RWX
Copy | 22 | suit-directive-copy | SUIT Manifest | N/M | RWX
Invoke | 23 | suit-directive-invoke | SUIT Manifest | REQUIRED for Bootloader | RWX
Wait | 29 | suit-directive-wait | SUIT Update Management | N/M | RWX
Swap | 31 | suit-directive-swap | SUIT Manifest | N/M | RWX
Run Sequence | 32 | suit-directive-run-sequence | SUIT Manifest | OPTIONAL | RWX
Unlink | 33 | suit-directive-unlink | SUIT Multiple Trust Domains | N/M | RWX
Override Multiple | 34 | suit-directive-override-multiple | SUIT Update Management | N/M | RWX
Copy Params | 35 | suit-directive-copy-params | SUIT Update Management | N/M | RWX

### SUIT Parameters

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Vendor ID | 1 | suit-parameter-vendor-identifier | SUIT Manifest | N/M | RWX
Class ID | 2 | suit-parameter-class-identifier | SUIT Manifest | N/M | RWX
Image Digest | 3 | suit-parameter-image-digest | SUIT Manifest | N/M | RWX
Use Before | 4 | suit-parameter-use-before | SUIT Update Management | N/M | RWX
Component Slot | 5 | suit-parameter-component-slot | SUIT Manifest | N/M | RWX
Strict Order | 12 | suit-parameter-strict-order | SUIT Manifest | N/M | RWX
Soft Failure | 13 | suit-parameter-soft-failure | SUIT Manifest | N/M | RWX
Image Size | 14 | suit-parameter-image-size | SUIT Manifest | N/M | RWX
Content | 18 | suit-parameter-content | SUIT Manifest | N/M | RWX
Encryption Info (AES-KW) | 19 | suit-parameter-encryption-info | SUIT Encrypted Payload | N/M | RWX
Encryption Info (ECDH)   | 19 | suit-parameter-encryption-info | SUIT Encrypted Payload | N/M | RWX
Encryption Info (HPKE)   | 19 | suit-parameter-encryption-info | SUIT Encrypted Payload | N/M | RW-
URI | 21 | suit-parameter-uri | SUIT Manifest | N/M | RWX
Source Component | 22 | suit-parameter-source-component | SUIT Manifest | N/M | RWX
Invoke Args | 23 | suit-parameter-invoke-args | SUIT Manifest | N/M | RWX
Device ID | 24 | suit-parameter-device-identifier | SUIT Manifest | N/M | RWX
Fetch Args | 25 | suit-parameter-fetch-args | SUIT Manifest | N/M | RWX
Minimum Battery | 26 | suit-parameter-minimum-battery | SUIT Update Management | N/M | RWX
Update Priority | 27 | suit-parameter-update-priority | SUIT Update Management | N/M | RWX
Version | 28 | suit-parameter-version | SUIT Update Management | N/M | RWX
Wait Info | 29 | suit-parameter-wait-info | SUIT Update Management | N/M | RWX
Component Metadata | 30 | suit-parameter-component-metadata | SUIT Update Management | N/M | RWX

### SUIT Text

NOTE: libcsuit ignores this while processing a SUIT Manifest

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Manifest Description | 1 | suit-text-manifest-description | SUIT Manifest | OPTIONAL | RWX
Update Description | 2 | suit-text-update-description | SUIT Manifest | OPTIONAL | RWX
Manifest JSON Source | 3 | suit-text-manifest-json-source | SUIT Manifest | OPTIONAL | RWX
Manifest YAML Source | 4 | suit-text-manifest-yaml-source | SUIT Manifest | OPTIONAL | RWX

### SUIT Text Component

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Vendor Name | 1 | suit-text-vendor-name | SUIT Manifest | OPTIONAL | RWX
Model Name | 2 | suit-text-model-name | SUIT Manifest | OPTIONAL | RWX
Vendor Domain | 3 | suit-text-vendor-domain | SUIT Manifest | OPTIONAL | RWX
Model Info | 4 | suit-text-model-info | SUIT Manifest | OPTIONAL | RWX
Component Description | 5 | suit-text-component-description | SUIT Manifest | OPTIONAL | RWX
Component Version | 6 | suit-text-component-version | SUIT Manifest | OPTIONAL | RWX
Version Required | 7 | suit-text-version-required | SUIT Update Management | N/M | RWX

### SUIT Wait Info

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Event Authorization | 1 | suit-wait-event-authorization | SUIT Update Management | N/M | RWX
Event Power | 2 | suit-wait-event-power | SUIT Update Management | N/M | RWX
Event Network | 3 | suit-wait-event-network | SUIT Update Management | N/M | RWX
Event Other Device Version | 4 | suit-wait-event-other-device-version | SUIT Update Management | N/M | RWX
Event Time | 5 | suit-wait-event-time | SUIT Update Management | N/M | RWX
Event Time of Day | 6 | suit-wait-event-time-of-day | SUIT Update Management | N/M | RWX
Event Day of Week | 7 | suit-wait-event-day-of-week | SUIT Update Management | N/M | RWX

### SUIT Component Metadata
Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Default Permissions | 1 | suit-meta-default-permissions | SUIT Update Management | N/M | RWX
User Permissions | 2 | suit-meta-user-permissions | SUIT Update Management | N/M | RWX
Group Permissions | 3 | suit-meta-group-permissions | SUIT Update Management | N/M | RWX
Role Permissions | 4 | suit-meta-role-permissions | SUIT Update Management | N/M | RWX
File Type | 5 | suit-meta-file-type | SUIT Update Management | N/M | RWX
Modification Time | 6 | suit-meta-modification-time | SUIT Update Management | N/M | RWX
Creation Time | 7 | suit-meta-creation-time | SUIT Update Management | N/M | RWX
Creator | 8 | suit-meta-creator | SUIT Update Management | N/M | RWX
## SUIT Report
### SUIT_Report

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Reference | 99 | suit-reference | SUIT Report | N/M | RWX
Nonce | 2 | suit-report-nonce | SUIT Report | N/M | RWX
Record | 3 | suit-report-record | SUIT Report | N/M | RWX
Result | 4 | suit-report-result | SUIT Report | N/M | RWX
Capability Report | 8 | suit-report-capability-report | SUIT Report | N/M | ---

### SUIT Capability Reporting

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Component | 1 | suit-component-capabilities | SUIT Report | N/M | ---
Command | 2 | suit-command-capabilities | SUIT Report | N/M | ---
Parameters | 3 | suit-parameters-capabilities | SUIT Report | N/M | ---
Crypt Algo | 4 | suit-crypt-algo-capabilities | SUIT Report | N/M | ---
Envelope | 5 | suit-envelope-capabilities | SUIT Report | N/M | ---
Manifest | 6 | suit-manivest-capabilities | SUIT Report | N/M | ---
Common | 7 | suit-common-capabilities | SUIT Report | N/M | ---
Text | 8 | suit-text-capabilities | SUIT Report | N/M | ---
Text Component | 9 | suit-text-component-capabilities | SUIT Report | N/M | ---
Dependency | 10 | suit-dependency-capabilities | SUIT Report | N/M | ---
