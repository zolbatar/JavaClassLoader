#define false 0
#define true 1

#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1

#define ACC_PUBLIC 0x0001 /* Declared public; may be accessed from outside its package.*/
#define ACC_FINAL 0x0010 /* Declared final; no subclasses allowed. */
#define ACC_SUPER 0x0020 /* Treat superclass methods specially when invoked by the invokespecial instruction. */
#define ACC_INTERFACE 0x0200 /* Is an interface, not a class. */
#define ACC_ABSTRACT 0x0400

#define METHOD_ACC_PUBLIC 0x0001 /* Declared public; may be accessed from outside its package. */
#define METHOD_ACC_PRIVATE 0x0002 /* Declared private; accessible only within the defining class. */
#define METHOD_ACC_PROTECTED 0x0004 /* Declared protected; may be accessed within subclasses. */
#define METHOD_ACC_STATIC 0x0008 /* Declared static. */
#define METHOD_ACC_FINAL 0x0010 /* Declared final; may not be overridden. */
#define METHOD_ACC_SYNCHRONIZED 0x0020 /* Declared synchronized; invocation is wrapped in a monitor lock. */
#define METHOD_ACC_NATIVE 0x0100 /* Declared native; implemented in a language other than Java. */
#define METHOD_ACC_ABSTRACT 0x0400 /* Declared abstract; no implementation is provided. */
#define METHOD_ACC_STRICT 0x0800 /* Declared strictfp; floating-point mode is FP-strict */

#define METHOD_ATTR_OTHER 0
#define METHOD_ATTR_CODE 1
