# Supported features by libcsuit
libcsuit has 3 core features: decode (Read), encode (Write) and process (eXecute).  
SUIT features are listed below for each SUIT documents.  
**R** means that libcsuit supports to decode it, and **r** not.  
**W** means that libcsuit supports to decode it, and **w** not.  
**X** means that libcsuit supports to decode it, and **x** not.  

## SUIT Manifest

### SUIT Metadata



### SUIT Parameters

Name | Label | CDDL Structure | Is | Supported?
---|---|---|---|---
Vendor ID | 1 | suit-parameter-vendor-identifier | N/A | RWX
Class ID | 2 |suit-parameter-class-identifier | N/A | RWX
Image Digest | 3 | suit-parameter-image-digest | N/A | RWX
Component Slot | 5 | suit-parameter-component-slot | N/A | RWX
Strict Order | 12 | suit-parameter-strict-order | N/A | rwx
Soft Failure | 13 | suit-parameter-soft-failure | N/A | rwx
Image Size | 14 | suit-parameter-image-size | N/A | RWX
Content | 18 | suit-parameter-content | N/A | rwx
URI | 21 | suit-parameter-uri | N/A | RWX
Source Component | 22 | suit-parameter-source-component | N/A | rwx
Invoke Args | 23 | suit-parameter-invoke-args | N/A | RWX
Device ID | 24 | suit-parameter-device-identifier | N/A | RWX
Fetch Arguments | ? | suit-parameter-fetch-arguments | N/A | rwx
Custom | nint | suit-parameter-custom | N/A | rwx

### SUIT Condition

Name | Label | CDDL Structure | Is | Supported?
---|---|---|---|---
Vendor Identifier | 1 | suit-condition-vendor-identifier | REQUIRED | RWx
Class Identifier | 2 | suit-condition-class-identifier |  | RWx
Image Match | 3 | suit-condition-image-match | REQUIRED | RWx
Component Slot | 5 | suit-condition-component-slot | 
Check Content | 6 | suit-condition-check-content | 
Abort | 14 | suit-condition-abort | 
Device Identifier | 24 | suit-condition-device-identifier | OPTIONAL | RWx
Custom Condition | ? | suit-condition-custom | N/A | rwx

### SUIT Directive
Name | CDDL Structure | Reference
---|---|---
Set Component Index | 12 | suit-directive-set-component-index | REQUIRED | RWX
Try Each | 15 | suit-directive-try-each | MAY | RWx
Write | 18 | suit-directive-write | N/A | rwx
Override Parameters | 20 | suit-directive-override-parameters | N/A | RWX
Fetch | 21 | suit-directive-fetch | N/A | RWX
Copy | 22 | suit-directive-copy | N/A | rwx
Invoke | 23 | suit-directive-invoke | MAY | RWX
Swap | 31 | suit-directive-swap | N/A | rwx
Run Sequence | 32 | suit-directive-run-sequence | OPTIONAL | rwx

## SUIT Multiple Trust Domains


## SUIT Update Management

## SUIT Encrypted Payloads

## SUIT Report
