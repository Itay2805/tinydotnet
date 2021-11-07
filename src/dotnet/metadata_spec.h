#pragma once

#include "types.h"

#include <stdint.h>

#define METADATA_MODULE 0x00
typedef struct metadata_module {
    uint16_t generation;
    const char* name;
    guid_t* mvid;
    guid_t* enc_id;
    guid_t* enc_base_id;
} PACKED metadata_module_t;

#define METADATA_TYPE_REF 0x01
typedef struct metadata_type_ref {
    token_t resolution_scope;
    const char* type_name;
    const char* type_namespace;
} PACKED metadata_type_ref_t;

#define METADATA_TYPE_DEF 0x02
typedef struct metadata_type_def {
    uint32_t flags;
    const char* type_name;
    const char* type_namespace;
    token_t extends;
    token_t field_list;
    token_t method_list;
} PACKED metadata_type_def_t;

#define METADATA_FIELD 0x04
typedef struct metadata_field {
    uint16_t flags;
    const char* name;
    const uint8_t* signature;
} PACKED metadata_field_t;

#define METADATA_METHOD_DEF 0x06
typedef struct metadata_method_def {
    uint32_t rva;
    uint16_t impl_flags;
    uint16_t flags;
    const char* name;
    const uint8_t* signature;
    token_t param_list;
} PACKED metadata_method_def_t;

#define METADATA_PARAM 0x08
typedef struct metadata_param {
    uint16_t flags;
    uint16_t sequence;
    const char* name;
} PACKED metadata_param_t;

#define METADATA_MEMBER_REF 0x0a
typedef struct metadata_member_ref {
    token_t class;
    const char* name;
    const uint8_t* signature;
} PACKED metadata_member_ref_t;

#define METADATA_CONSTANT 0x0b
typedef struct metadata_constant {
    uint16_t type;
    token_t parent;
    const uint8_t* value;
} PACKED metadata_constant_t;

#define METADATA_CUSTOM_ATTRIBUTE 0x0c
typedef struct metadata_custom_attribute {
    token_t parent;
    token_t type;
    const uint8_t* value;
} PACKED metadata_custom_attribute_t;

#define METADATA_CLASS_LAYOUT 0x0f
typedef struct metadata_class_layout {
    uint16_t packing_size;
    uint32_t class_size;
    token_t parent;
} PACKED metadata_class_layout_t;

#define METADATA_STAND_ALONE_SIG 0x11
typedef struct metadata_stand_alone_sig {
    const uint8_t* signature;
} PACKED metadata_stand_alone_sig_t;

#define METADATA_PROPERTY_MAP 0x15
typedef struct metadata_property_map {
    token_t parent;
    token_t property_list;
} PACKED metadata_property_map_t;

#define METADATA_PROPERTY 0x17
typedef struct metadata_property {
    uint16_t flags;
    const char* name;
    const uint8_t* type;
} PACKED metadata_property_t;

#define METADATA_METHOD_SEMANTICS 0x18
typedef struct metadata_method_semantics {
    uint16_t semantics;
    token_t method;
    token_t association;
} PACKED metadata_method_semantics_t;

#define METADATA_METHOD_IMPL 0x19
typedef struct metadata_method_impl {
    token_t class;
    token_t method_body;
    token_t method_declaration;
} PACKED metadata_method_impl_t;

#define METADATA_ASSEMBLY 0x20
typedef struct metadata_assembly {
    uint32_t hash_alg_id;
    uint16_t major_version;
    uint16_t minor_version;
    uint16_t build_number;
    uint16_t revision_number;
    uint32_t flags;
    const uint8_t* public_key;
    const char* name;
    const char* culture;
} PACKED metadata_assembly_t;

#define METADATA_ASSEMBLY_REF 0x23
typedef struct metadata_assembly_ref {
    uint16_t major_version;
    uint16_t minor_version;
    uint16_t build_number;
    uint16_t revision_number;
    uint32_t flags;
    const uint8_t* public_key_or_token;
    const char* name;
    const char* culture;
    const uint8_t* hash_value;
} PACKED metadata_assembly_ref_t;

#define METADATA_ASSEMBLY_REF_OS 0x25
typedef struct metadata_assembly_ref_os {
    uint32_t os_platform_id;
    uint32_t os_major_version;
    uint32_t os_minor_version;
    token_t assembly_ref;
} PACKED metadata_assembly_ref_os_t;
