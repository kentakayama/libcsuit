/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef LIBCSUIT_CONFIG_H
#define LIBCSUIT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

//#define LIBCSUIT_USE_T_COSE_1
//#define LIBCSUIT_DISABLE_ENCRYPTION
//#define LIBCSUIT_DISABLE_MAC
#define LIBCSUIT_USE_DETERMINISTIC_ECDSA

//#define LIBCSUIT_DISABLE_ES384
//#define LIBCSUIT_DISABLE_ES521

/* by document */
//#define LIBCSUIT_DISABLE_MULTIPLE_TRUST_DOMAINS
//#define LIBCSUIT_DISABLE_UPDATE_MANAGEMENT
//#define LIBCSUIT_DISABLE_ENCRYPTED_PAYLOAD

/* envelope */
//#define LIBCSUIT_DISABLE_ENVELOPE_DELEGATION
/* severed elements */
//#define LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION
//#define LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH
//#define LIBCSUIT_DISABLE_ENVELOPE_INSTALL
//#define LIBCSUIT_DISABLE_ENVELOPE_TEXT
//#define LIBCSUIT_DISABLE_ENVELOPE_COSWID

/* manifest */
//#define LIBCSUIT_DISABLE_MANIFEST_REFERENCE_URI
//#define LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID
//#define LIBCSUIT_DISABLE_MANIFEST_LOAD
//#define LIBCSUIT_DISABLE_MANIFEST_INVOKE
//#define LIBCSUIT_DISABLE_MANIFEST_COSWID
//#define LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION
//#define LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH
//#define LIBCSUIT_DISABLE_MANIFEST_INSTALL
//#define LIBCSUIT_DISABLE_MANIFEST_TEXT
//#define LIBCSUIT_DISABLE_MANIFEST_UNINSTALL

/* common */
//#define LIBCSUIT_DISABLE_COMMON_DEPENDENCIES
//#define LIBCSUIT_DISABLE_COMMON_SHARED_SEQUENCE

/* condition */
//#define LIBCSUIT_DISABLE_CONDITION_CLASS_IDENTIFIER
//#define LIBCSUIT_DISABLE_CONDITION_USE_BEFORE
//#define LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT
//#define LIBCSUIT_DISABLE_CONDITION_CHECK_CONTENT
//#define LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY
//#define LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY
//#define LIBCSUIT_DISABLE_CONDITION_ABORT
//#define LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER
//#define LIBCSUIT_DISABLE_CONDITION_IMAGE_NOT_MATCH
//#define LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY
//#define LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED
//#define LIBCSUIT_DISABLE_CONDITION_VERSION

/* directive */
//#define LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY
//#define LIBCSUIT_DISABLE_DIRECTIVE_TRY_EACH
//#define LIBCSUIT_DISABLE_DIRECTIVE_WRITE
//#define LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS
//#define LIBCSUIT_DISABLE_DIRECTIVE_FETCH
//#define LIBCSUIT_DISABLE_DIRECTIVE_COPY
//#define LIBCSUIT_DISABLE_DIRECTIVE_INVOKE
//#define LIBCSUIT_DISABLE_DIRECTIVE_WAIT
//#define LIBCSUIT_DISABLE_DIRECTIVE_SWAP
//#define LIBCSUIT_DISABLE_DIRECTIVE_RUN_SEQUENCE
//#define LIBCSUIT_DISABLE_DIRECTIVE_UNLINK
//#define LIBCSUIT_DISABLE_DIRECTIVE_OVERRIDE_MULTIPLE
//#define LIBCSUIT_DISABLE_DIRECTIVE_COPY_PARAMS

/* parameter */
//#define LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER
//#define LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE
//#define LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT
//#define LIBCSUIT_DISABLE_PARAMETER_STRICT_ORDER
//#define LIBCSUIT_DISABLE_PARAMETER_SOFT_FAILURE
//#define LIBCSUIT_DISABLE_PARAMETER_CONTENT
//#define LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO
//#define LIBCSUIT_DISABLE_PARAMETER_URI
//#define LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT
//#define LIBCSUIT_DISABLE_PARAMETER_INVOKE_ARGS
//#define LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER
//#define LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY
//#define LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY
//#define LIBCSUIT_DISABLE_PARAMETER_VERSION
//#define LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO
//#define LIBCSUIT_DISABLE_PARAMETER_FETCH_ARGS

/* misc */
//#define LIBCSUIT_DISABLE_SUIT_REPORT
//#define LIBCSUIT_DISABLE_COMPONENT_INDEX_TYPE_BOOLEAN
//#define LIBCSUIT_DISABLE_COMPONENT_INDEX_ARRAY


/* XXX: DO NOT EDIT BELOW */
/*
 * Disable features if the document is disabled.
 */
#if defined(LIBCSUIT_DISABLE_MULTIPLE_TRUST_DOMAINS)
#define LIBCSUIT_DISABLE_ENVELOPE_DELEGATION
#define LIBCSUIT_DISABLE_MANIFEST_COMPONENT_ID
#define LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION
#define LIBCSUIT_DISABLE_MANIFEST_UNINSTALL
#define LIBCSUIT_DISABLE_COMMON_DEPENDENCIES
#define LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY
#define LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY
#define LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY
#define LIBCSUIT_DISABLE_DIRECTIVE_SET_PARAMETERS
#define LIBCSUIT_DISABLE_DIRECTIVE_UNLINK
#endif /* LIBCSUIT_DISABLE_MULTIPLE_TRUST_DOMAINS */

#if defined(LIBCSUIT_DISABLE_UPDATE_MANAGEMENT)
#define LIBCSUIT_DISABLE_ENVELOPE_COSWID
#define LIBCSUIT_DISABLE_MANIFEST_COSWID
#define LIBCSUIT_DISABLE_CONDITION_USE_BEFORE
#define LIBCSUIT_DISABLE_CONDITION_IMAGE_NOT_MATCH
#define LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY
#define LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED
#define LIBCSUIT_DISABLE_CONDITION_VERSION
#endif /* LIBCSUIT_DISABLE_UPDATE_MANAGEMENT */

#if defined(LIBCSUIT_DISABLE_ENCRYPTED_PAYLOAD)
#define LIBCSUIT_DISABLE_PARAMETER_ENCRYPTION_INFO
#define LIBCSUIT_DISABLE_PARAMETER_CEK_VERIFICATION
#endif /* LIBCSUIT_DISABLE_ENCRYPTED_PAYLOAD */


/*
 * Disable severed elements if it is disabled in the manifest.
 */
#if defined(LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION)
#define LIBCSUIT_DISABLE_ENVELOPE_DEPENDENCY_RESOLUTION
#endif

#if defined(LIBCSUIT_DISABLE_MANIFEST_COSWID)
#define LIBCSUIT_DISABLE_ENVELOPE_COSWID
#endif

#if defined(LIBCSUIT_DISABLE_MANIFEST_PAYLOAD_FETCH)
#define LIBCSUIT_DISABLE_ENVELOPE_PAYLOAD_FETCH
#endif

#if defined(LIBCSUIT_DISABLE_MANIFEST_INSTALL)
#define LIBCSUIT_DISABLE_ENVELOPE_INSTALL
#endif

#if defined(LIBCSUIT_DISABLE_MANIFEST_TEXT)
#define LIBCSUIT_DISABLE_ENVELOPE_TEXT
#endif

#if defined(LIBCSUIT_DISABLE_MANIFEST_UNINSTALL)
#define LIBCSUIT_DISABLE_ENVELOPE_UNINSTALL
#endif

#if defined(LIBCSUIT_DISABLE_COMMON_DEPENDENCIES)
#define LIBCSUIT_DISABLE_MANIFEST_DEPENDENCY_RESOLUTION
#define LIBCSUIT_DISABLE_CONDITION_DEPENDENCY_INTEGRITY
#define LIBCSUIT_DISABLE_CONDITION_IS_DEPENDENCY
#define LIBCSUIT_DISABLE_DIRECTIVE_PROCESS_DEPENDENCY
#define SUIT_MAX_DEPENDENCY_NUM 0
#endif


#if defined(LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER)
#define LIBCSUIT_DISABLE_PARAMETER_CLASS_IDENTIFIER
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_USE_BEFORE)
#define LIBCSUIT_DISABLE_CONDITION_USE_BEFORE
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_COMPONENT_SLOT)
#define LIBCSUIT_DISABLE_CONDITION_COMPONENT_SLOT
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_CONTENT)
#define LIBCSUIT_DISABLE_DIRECTIVE_WRITE
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_DEVICE_IDENTIFIER)
#define LIBCSUIT_DISABLE_CONDITION_DEVICE_IDENTIFIER
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_MINIMUM_BATTERY)
#define LIBCSUIT_DISABLE_CONDITION_MINIMUM_BATTERY
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_UPDATE_PRIORITY)
#define LIBCSUIT_DISABLE_CONDITION_UPDATE_AUTHORIZED
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_VERSION)
#define LIBCSUIT_DISABLE_CONDITION_VERSION
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_WAIT_INFO)
#define LIBCSUIT_DISABLE_DIRECTIVE_WAIT
#endif

#if defined(LIBCSUIT_DISABLE_PARAMETER_SOURCE_COMPONENT)
#define LIBCSUIT_DISABLE_DIRECTIVE_COPY
#define LIBCSUIT_DISABLE_DIRECTIVE_SWAP
#endif
/* XXX: DO NOT EDIT ABOVE */

#ifdef __cplusplus
}
#endif

#endif /* LIBCSUIT_CONFIG_H */
