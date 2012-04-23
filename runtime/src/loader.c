#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "classfile_structs.h"
#include "classfile_consts.h"

/* Get bytes out of header */
unsigned int get_u1(unsigned char *data, unsigned int *pos)
{
	unsigned int value = (unsigned char)data[*pos];
	*pos += 1;
	return value;
}

unsigned int get_u2(unsigned char *data, unsigned int *pos)
{
	unsigned int value = ((unsigned char)data[*pos] << 8) + (unsigned char)data[*pos + 1];
	*pos += 2;
	return value;
}

unsigned int get_u4(unsigned char *data, unsigned int *pos)
{
	unsigned int value = ((unsigned char)data[*pos] << 24) + ((unsigned char)data[*pos + 1] << 16) + ((unsigned char)data[*pos + 2] << 8) + (unsigned char)data[*pos + 3];
	*pos += 4;
	return value;
}

unsigned char *get_utf8(unsigned char *data, unsigned int *pos, size_t length)
{
	char *string = malloc(length);
	memcpy(string, &data[*pos], length);
	*pos += length;
	return string;
}

void *get_data(unsigned char *data, unsigned int *pos, size_t length)
{
	void *data_out = malloc(length);
	memcpy(data_out, &data[*pos], length);
	*pos += length;
	return data_out;
}

/* Actual decoding logic */
int decode_class_header(unsigned char *header, unsigned int length, class_file *class)
{
	unsigned int pos = 0;

	/* Verify magic */
	class->magic = get_u4(header, &pos);
	if (class->magic != 0xCAFEBABE)
	{
		printf("Magic not found.\n");
		return false;
	}
	
	/* Version */
	class->minor_version = get_u2(header, &pos);
	class->major_version = get_u2(header, &pos);
	
	/* Constant Pool */
	class->constant_pool_count = get_u2(header, &pos);
	class->constant_pool = malloc(class->constant_pool_count * sizeof(void *));
	unsigned int constant;
	for (constant = 0; constant < class->constant_pool_count - 1; constant++)
	{
		unsigned char tag = get_u1(header, &pos);;
		switch (tag)
		{
			case CONSTANT_Methodref:
			{
				class->constant_pool[constant] = malloc(sizeof(CONSTANT_Methodref_info));
				CONSTANT_Methodref_info *obj = ((CONSTANT_Methodref_info *)class->constant_pool[constant]);
				obj->tag = tag;
				obj->class_index = get_u2(header, &pos);
				obj->name_and_type_index = get_u2(header, &pos);
				break;
			}
			case CONSTANT_Fieldref:
			{
				class->constant_pool[constant] = malloc(sizeof(CONSTANT_Fieldref_info));
				CONSTANT_Fieldref_info *obj = ((CONSTANT_Fieldref_info *)class->constant_pool[constant]);
				obj->tag = tag;
				obj->class_index = get_u2(header, &pos);
				obj->name_and_type_index = get_u2(header, &pos);
				break;
			}
			case CONSTANT_String:
			{
				class->constant_pool[constant] = malloc(sizeof(CONSTANT_String_info));
				CONSTANT_String_info *obj = ((CONSTANT_String_info *)class->constant_pool[constant]);
				obj->tag = tag;
				obj->string_index = get_u2(header, &pos);
				break;
			}
			case CONSTANT_NameAndType:
			{
				class->constant_pool[constant] = malloc(sizeof(CONSTANT_NameAndType_info));
				CONSTANT_NameAndType_info *obj = ((CONSTANT_NameAndType_info *)class->constant_pool[constant]);
				obj->tag = tag;
				obj->name_index = get_u2(header, &pos);
				obj->descriptor_index = get_u2(header, &pos);
				break;
			}
			case CONSTANT_Class:
			{
				class->constant_pool[constant] = malloc(sizeof(CONSTANT_Class_info));
				CONSTANT_Class_info *obj = ((CONSTANT_Class_info *)class->constant_pool[constant]);
				obj->tag = tag;
				obj->name_index = get_u2(header, &pos);
				break;
			}
			case CONSTANT_Utf8:
			{
				class->constant_pool[constant] = malloc(sizeof(CONSTANT_Utf8_info));
				CONSTANT_Utf8_info *obj = ((CONSTANT_Utf8_info *)class->constant_pool[constant]);
				obj->tag = tag;
				obj->length = get_u2(header, &pos);
				obj->bytes = get_utf8(header, &pos, obj->length);
				break;
			}
			default:
			{
				printf ("Unsupported constant tag %i", tag);
				exit(1);
			}
		}
	}
	
	/* Access flags */
	class->access_flags = get_u2(header, &pos);
	
	/* Class */
	class->this_class = get_u2(header, &pos);
	class->super_class = get_u2(header, &pos);

	/* Interfaces */
	class->interfaces_count = get_u2(header, &pos);
	class->interfaces = malloc(class->interfaces_count * sizeof(u2));
	unsigned int interface;
	for (interface = 0; interface < class->interfaces_count; interface++)
	{
		class->interfaces[interface] = get_u2(header, &pos);
	}

	/* Fields */
	class->fields_count = get_u2(header, &pos);
	unsigned int field;
	for (field = 0; field < class->fields_count; field++)
	{
		printf("Oh dear, fields not implemented yet!");
		exit(1);
	}
	
	/* Methods */
	class->methods_count = get_u2(header, &pos);
	class->methods = malloc(class->methods_count * sizeof(method_info));
	unsigned int method;
	for (method = 0; method < class->methods_count; method++)
	{
		class->methods[method] = malloc(sizeof(method_info));
		method_info *obj = ((method_info *)class->methods[method]);
		obj->access_flags = get_u2(header, &pos);
		obj->name_index = get_u2(header, &pos);
		obj->descriptor_index = get_u2(header, &pos);
		obj->attributes_count = get_u2(header, &pos);
		
		obj->attributes = malloc(obj->attributes_count * sizeof(attribute_info));
		unsigned int method_attributes;
		for (method_attributes = 0; method_attributes < obj->attributes_count; method_attributes++)
		{
			u2 attribute_name_index = get_u2(header, &pos);
			u4 attribute_length = get_u4(header, &pos);

			/* What is it? */
			CONSTANT_Utf8_info *attr_obj_ref = ((CONSTANT_Utf8_info *)class->constant_pool[attribute_name_index - 1]);
			if (strncmp(attr_obj_ref->bytes, "Code", attr_obj_ref->length) == 0)
			{
				obj->attributes[method] = malloc(sizeof(code_attribute));
				code_attribute *attr_obj = ((code_attribute *)obj->attributes[method]);
				attr_obj->type = METHOD_ATTR_CODE;
				attr_obj->attribute_name_index = attribute_name_index;
				attr_obj->attribute_length;
				
				attr_obj->max_stack = get_u2(header, &pos);
				attr_obj->max_locals = get_u2(header, &pos);
				attr_obj->code_length = get_u4(header, &pos);
				attr_obj->code = get_data(header, &pos, attr_obj->code_length);

				/* Exceptions */
				attr_obj->exception_table_length = get_u2(header, &pos);
				unsigned int exception_item;
				attr_obj->exception_table = malloc(attr_obj->exception_table_length * sizeof(code_exception));
				for (exception_item = 0; exception_item < attr_obj->exception_table_length; exception_item++)
				{
					attr_obj->exception_table[exception_item] = malloc(sizeof(code_exception));
					code_exception *exception = ((code_exception *)attr_obj->exception_table[exception_item]);
					exception->start_pc = get_u2(header, &pos);
					exception->end_pc = get_u2(header, &pos);
					exception->handler_pc = get_u2(header, &pos);
					exception->catch_type = get_u2(header, &pos);
				} 

				/* Attributes */
				attr_obj->attributes_count = get_u2(header, &pos);
				attr_obj->attributes = malloc(attr_obj->attributes_count * sizeof(attribute_info));
				unsigned int code_attribute_item;
				for (code_attribute_item = 0; code_attribute_item < attr_obj->attributes_count; code_attribute_item++)
				{
					attr_obj->attributes[code_attribute_item] = malloc(sizeof(attribute_info));
					attribute_info *code_attribute = ((attribute_info *)attr_obj->attributes[code_attribute_item]);
					code_attribute->attribute_name_index = get_u2(header, &pos);
					code_attribute->attribute_length = get_u4(header, &pos);
					code_attribute->info = get_data(header, &pos, code_attribute->attribute_length);
				}
			}
			else
			{
				obj->attributes[method] = malloc(sizeof(attribute_info));
				attribute_info *attr_obj = ((attribute_info *)obj->attributes[method]);
				attr_obj->type = METHOD_ATTR_OTHER;
				attr_obj->attribute_name_index = attribute_name_index;
				attr_obj->attribute_length;
				attr_obj->info = get_data(header, &pos, attr_obj->attribute_length);
			}
		}
	}

	return true;
}

void delete_class(class_file *class)
{
	free(class->constant_pool);
}

void print_code(unsigned int attribute_code_length, u1 *code)
{
	unsigned int code_pc = 0;
	while (code_pc < attribute_code_length)
	{
		unsigned char opcode = code[code_pc++];
		printf("\t\t[%.8i] [%.2hx]\t\t", code_pc, opcode);
		switch (opcode)
		{
			case 0x12:
			{
				printf("ldc %x", get_u1(code, &code_pc));
				break;
			}
			case 0x2a:
			{
				printf("aload_0");
				break;
			}
			case 0xb1:
			{
				printf("return");
				break;
			}
			case 0xb2:
			{
				printf("getstatic %x", get_u2(code, &code_pc));
				break;
			}
			case 0xb6:
			{
				printf("invokevirtual %x", get_u2(code, &code_pc));
				break;
			}
			case 0xb7:
			{
				printf("invokespecial %x", get_u2(code, &code_pc));
				break;
			}
			default:
			{
				printf("[%i] %hx", code_pc, opcode);
				break;
			}
		}
		printf("\n");
	}	
}

void print_class_header(class_file *class)
{
	/* Summary */
	printf("Summary\n-------\n");
	printf("Version: %X.%X\n", class->minor_version, class->major_version);
	printf("Access Flags: ");
	if ((class->access_flags & ACC_PUBLIC) > 0)
		printf("PUBLIC ");
	if ((class->access_flags & ACC_FINAL) > 0)
		printf("FINAL ");
	if ((class->access_flags & ACC_SUPER) > 0)
		printf("SUPER ");
	if ((class->access_flags & ACC_INTERFACE) > 0)
		printf("INTERFACE ");
	if ((class->access_flags & ACC_ABSTRACT) > 0)
		printf("ABSTRACT ");
	printf("\n");
	printf("This Class: %i\nSuper Class: %i\n", class->this_class, class->super_class);

	/* Constant Pool */
	printf("\nConstant Pool\n-------------\n");
	printf("Constant pool count: %i\n", class->constant_pool_count);
	unsigned int constant;
	for (constant = 0; constant < class->constant_pool_count - 1; constant++)
	{
		cp_info *info = ((cp_info *)class->constant_pool[constant]);
		switch (info->tag)
		{
			case CONSTANT_Methodref:
			{
				CONSTANT_Methodref_info *obj = ((CONSTANT_Methodref_info *)class->constant_pool[constant]);
				printf("[%i] Methodref:%i/%i\n", constant + 1, obj->class_index, obj->name_and_type_index);
				break;
			}
			case CONSTANT_Fieldref:
			{
				CONSTANT_Fieldref_info *obj = ((CONSTANT_Fieldref_info *)class->constant_pool[constant]);
				printf("[%i] Fieldref:%i/%i\n", constant + 1, obj->class_index, obj->name_and_type_index);
				break;
			}
			case CONSTANT_String:
			{
				CONSTANT_String_info *obj = ((CONSTANT_String_info *)class->constant_pool[constant]);
				printf("[%i] String:%i\n", constant + 1, obj->string_index);
				break;
			}
			case CONSTANT_NameAndType:
			{
				CONSTANT_NameAndType_info *obj = ((CONSTANT_NameAndType_info *)class->constant_pool[constant]);
				printf("[%i] NameAndType:%i/%i\n", constant + 1, obj->name_index, obj->descriptor_index);
				break;
			}
			case CONSTANT_Class:
			{
				CONSTANT_Class_info *obj = ((CONSTANT_Class_info *)class->constant_pool[constant]);
				printf("[%i] Class:%i\n", constant + 1, obj->name_index);
				break;
			}
			case CONSTANT_Utf8:
			{
				CONSTANT_Utf8_info *obj = ((CONSTANT_Utf8_info *)class->constant_pool[constant]);
				printf("[%i] UTF8:'%s' Length:%i\n", constant + 1, obj->bytes, obj->length);
				break;
			}
		}
	}
	
	/* Interfaces */
	printf("\nInterfaces\n---------\n");
	printf("Interfaces Count: %i\n", class->interfaces_count);
	unsigned int interface;
	for (interface = 0; interface < class->interfaces_count; interface++)
	{
		printf("[%i] = %i", interface, class->interfaces[interface]);
	}
	
	/* Fields */
	printf("\nFields\n------\n");
	printf("Fields Count: %i\n", class->fields_count);
	unsigned int field;
	for (field = 0; field < class->fields_count; field++)
	{
	}
	
	/* Methods */
	printf("\nMethods\n-------\n");
	printf("Methods Count: %i\n", class->methods_count);
	unsigned int method;
	for (method = 0; method < class->methods_count; method++)
	{
		method_info *obj = ((method_info *)class->methods[method]);
		printf("Method [%i] AccessFlags: ", method);
		if ((obj->access_flags & METHOD_ACC_PUBLIC) > 0)
			printf("PUBLIC ");
		if ((obj->access_flags & METHOD_ACC_PRIVATE) > 0)
			printf("PRIVATE ");
		if ((obj->access_flags & METHOD_ACC_PROTECTED) > 0)
			printf("PROTECTED ");
		if ((obj->access_flags & METHOD_ACC_STATIC) > 0)
			printf("STATIC ");
		if ((obj->access_flags & METHOD_ACC_FINAL) > 0)
			printf("FINAL ");
		if ((obj->access_flags & METHOD_ACC_SYNCHRONIZED) > 0)
			printf("SYNCHRONISED ");
		if ((obj->access_flags & METHOD_ACC_NATIVE) > 0)
			printf("NATIVE ");
		if ((obj->access_flags & METHOD_ACC_ABSTRACT) > 0)
			printf("ABSTRACT ");
		if ((obj->access_flags & METHOD_ACC_STRICT) > 0)
			printf("STRICT ");
		printf("NameIndex:%i DescriptorIndex:%i AttributesCount:%i\n", obj->name_index, obj->descriptor_index, obj->attributes_count);

		unsigned int method_attributes;
		for (method_attributes = 0; method_attributes < obj->attributes_count; method_attributes++)
		{
			attribute_info *attr_obj = ((attribute_info *)obj->attributes[method]);
			switch (attr_obj->type)
			{
				case METHOD_ATTR_OTHER:
				{
					printf("  -> AttributeOTHER [%i] NameIndex:%i Length:%i\n", method_attributes, attr_obj->attribute_name_index, attr_obj->attribute_length);
					break;
				}
				case METHOD_ATTR_CODE:
				{
					code_attribute *attr_obj = ((code_attribute *)obj->attributes[method]);
					printf("  -> AttributeCODE [%i]NameIndex:%i Length:%i ", method_attributes, attr_obj->attribute_name_index, attr_obj->attribute_length);
					printf("MaxStack:%i MaxLocals:%i CodeLength:%i ExceptionTableLength:%i AttributesCount:%i\n", 
						attr_obj->max_stack, attr_obj->max_locals, attr_obj->code_length, attr_obj->exception_table_length, attr_obj->attributes_count);

					unsigned int exception_item;
					for (exception_item = 0; exception_item < attr_obj->exception_table_length; exception_item++)
					{
						code_exception *exception = ((code_exception *)attr_obj->exception_table[exception_item]);
						printf("    --> Exception [%i] StartPC:%i EndPC:%i HandlerPC:%i CatchType:%i\n", exception_item, exception->start_pc, exception->end_pc, exception->handler_pc, exception->catch_type);
					} 

					/* Attributes */
					unsigned int code_attribute_item;
					for (code_attribute_item = 0; code_attribute_item < attr_obj->attributes_count; code_attribute_item++)
					{
						attribute_info *code_attribute = ((attribute_info *)attr_obj->attributes[code_attribute_item]);
						printf("    --> Attribute [%i] NameIndex:%i Length:%i Value:'%s'\n", code_attribute_item, code_attribute->attribute_name_index, code_attribute->attribute_length, (char *)code_attribute->info);
					}
					
					/* Dump code */
					print_code(attr_obj->code_length, attr_obj->code);

					break;
				}
			}
		}
	}
}
