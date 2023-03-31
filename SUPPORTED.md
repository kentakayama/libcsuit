# Supported features by libcsuit


## NOTE: How to read the Supported Features Tables
**IN**: which document
Name | Reference
---|---
SUIT Manifest | https://datatracker.ietf.org/doc/html/draft-ietf-suit-manifest
SUIT Multiple Trust Domains | https://datatracker.ietf.org/doc/html/draft-moran-suit-trust-domains
SUIT Update Manegement | https://datatracker.ietf.org/doc/html/draft-ietf-suit-update-management
SUIT Encrypted Payload | https://datatracker.ietf.org/doc/html/draft-ietf-suit-firmware-encryption
SUIT Report | https://datatracker.ietf.org/doc/html/draft-ietf-suit-report

**IS**: [ REQUIRED to , OPTIONAL to , MAY ] implemented

**Supported?**: libcsuit has 3 core features: decode (Read), encode (Write) and process (eXecute).  
SUIT features are listed below for each SUIT documents.  
**R** means that libcsuit supports to decode it, and **r** not.  
**W** means that libcsuit supports to encode it, and **w** not.  
**X** means that libcsuit supports to process it, and **x** not.  


## Supported Features Tables
### SUIT Metadata



### SUIT Parameters

Name | Label | CDDL Structure | IN | IS | Supported?
---|---|---|---|---|---
Vendor ID | 1 | suit-parameter-vendor-identifier | SUIT Manifest | N/A | RWX
Class ID | 2 |suit-parameter-class-identifier | SUIT Manifest | N/A | RWX
Image Digest | 3 | suit-parameter-image-digest | SUIT Manifest | N/A | RWX
Component Slot | 5 | suit-parameter-component-slot | SUIT Manifest | N/A | RWX
Strict Order | 12 | suit-parameter-strict-order | SUIT Manifest | N/A | rwx
Soft Failure | 13 | suit-parameter-soft-failure | SUIT Manifest | N/A | rwx
Image Size | 14 | suit-parameter-image-size | SUIT Manifest | N/A | RWX
Content | 18 | suit-parameter-content | SUIT Manifest | N/A | rwx
URI | 21 | suit-parameter-uri | SUIT Manifest | N/A | RWX
Source Component | 22 | suit-parameter-source-component | SUIT Manifest | N/A | rwx
Invoke Args | 23 | suit-parameter-invoke-args | SUIT Manifest | N/A | RWX
Device ID | 24 | suit-parameter-device-identifier | SUIT Manifest | N/A | RWX
Fetch Arguments | ? | suit-parameter-fetch-arguments | SUIT Manifest | N/A | rwx
Custom | nint | suit-parameter-custom | SUIT Manifest | N/A | rwx

### SUIT Condition

Name | Label | CDDL Structure | IN | Is | Supported?
---|---|---|---|---|---
Vendor Identifier | 1 | suit-condition-vendor-identifier | SUIT Manifest | REQUIRED | RWx
Class Identifier | 2 | suit-condition-class-identifier | SUIT Manifest |  | RWx
Image Match | 3 | suit-condition-image-match | SUIT Manifest | REQUIRED | RWx
Component Slot | 5 | suit-condition-component-slot | SUIT Manifest | 
Check Content | 6 | suit-condition-check-content | SUIT Manifest | 
Abort | 14 | suit-condition-abort | SUIT Manifest | 
Device Identifier | 24 | suit-condition-device-identifier | SUIT Manifest | OPTIONAL | RWx
Custom Condition | ? | suit-condition-custom | SUIT Manifest | N/A | rwx

### SUIT Directive
Name | Label | CDDL Structure | IN | IS | Supported
---|---|---|---|---|---
Set Component Index | 12 | suit-directive-set-component-index | SUIT Manifest | REQUIRED | RWX
Try Each | 15 | suit-directive-try-each | SUIT Manifest | MAY | RWx
Write | 18 | suit-directive-write | SUIT Manifest | N/A | rwx
Override Parameters | 20 | suit-directive-override-parameters | SUIT Manifest | N/A | RWX
Fetch | 21 | suit-directive-fetch | SUIT Manifest | N/A | RWX
Copy | 22 | suit-directive-copy | SUIT Manifest | N/A | rwx
Invoke | 23 | suit-directive-invoke | SUIT Manifest | MAY | RWX
Swap | 31 | suit-directive-swap | SUIT Manifest | N/A | rwx
Run Sequence | 32 | suit-directive-run-sequence | SUIT Manifest | OPTIONAL | rwx

