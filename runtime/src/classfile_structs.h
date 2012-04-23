typedef unsigned int u4;
typedef unsigned short u2;
typedef unsigned char u1;

typedef struct {
	u1 tag;
    void *info;
} cp_info;

typedef struct {
	u1 type;
  	u2 attribute_name_index;
   	u4 attribute_length;
	u1 **info; /*attribute_length*/
} attribute_info;

typedef struct {
	u2 access_flags;
	u2 name_index;
   	u2 descriptor_index;
    u2 attributes_count;
    attribute_info **attributes; /*attributes_count*/
} field_info;

typedef struct  {
   	u2 access_flags;
   	u2 name_index;
   	u2 descriptor_index;
   	u2 attributes_count;
   	attribute_info **attributes; /*attributes_count*/
} method_info;

typedef struct {
	u4 magic;
	u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
	cp_info **constant_pool; /*constant_pool_count-1*/
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces; /*interfaces_count*/
    u2 fields_count;
    field_info **fields; /*fields_count*/
    u2 methods_count;
    method_info **methods; /*methods_count*/
    u2 attributes_count;
	attribute_info **attributes; /*attributes_count*/
} class_file;

typedef struct {
   	u1 tag;
   	u2 class_index;
   	u2 name_and_type_index;
} CONSTANT_Methodref_info;

typedef struct {
   	u1 tag;
   	u2 name_index;
   	u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct {
   	u1 tag;
   	u2 class_index;
   	u2 name_and_type_index;
} CONSTANT_Fieldref_info;

typedef struct  {
   	u1 tag;
   	u2 string_index;
} CONSTANT_String_info;

typedef struct {
   	u1 tag;
   	u2 name_index;
} CONSTANT_Class_info;

typedef struct {
   	u1 tag;
   	u2 length;
   	char *bytes;
} CONSTANT_Utf8_info;

typedef struct {
	u2 start_pc;
	u2 end_pc;
	u2 handler_pc;
	u2 catch_type;
} code_exception;

typedef struct {
	u1 type;
	u2 attribute_name_index;
	u4 attribute_length;
	u2 max_stack;
	u2 max_locals;
	u4 code_length;
	u1 *code; /* code_length */
	u2 exception_table_length;
	code_exception **exception_table; /* exception_table_length */
   	u2 attributes_count;
	attribute_info **attributes; /*attributes_count*/
} code_attribute;