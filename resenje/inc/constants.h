#ifndef _constants_h_
#define _constants_h_

#define COMMENT_SYMBOL "#"
#define LABEL_SYMBOL ":"
#define DIRECTIVE_END ".end"
#define RELOCATION_ABSOLUTE "REL_ABS"
#define RELOCATION_PCRELATIVE "REL_PC"
#define START_SECTION_INDEX 0
#define JUMP_INSTRUCTION_REGEX regex("^(jmp)|(jeq)|(jne)|(jgt)[w]{0,1}$")
#define GLOBAL_DIRECTIVE_SYNTAX_CHECK_REGEX regex("^\\s*\\.global\\s+((?:\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*,)*\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*)\\s*$")
#define EXTERN_DIRECTIVE_SYNTAX_CHECK_REGEX regex("^\\s*\\.extern\\s+((?:\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*,)*\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*)\\s*$")
#define WORD_DIRECTIVE_SYNTAX_CHECK_REGEX regex("^\\s*\\.word\\s+((?:\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|(?:\\+|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})\\s*,)*\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|(?:\\+|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})\\s*)\\s*$")
#define DIRECTIVE_REGEX regex("^.(global|extern|end|skip|word|section)$")
#define DIRECTIVE_GLOBAL_EXTERN_REGEX regex("^.(global|extern)$")
#define LABEL_REGEX regex("^([a-zA-Z_][a-zA-Z0-9_]*):$")
#define DIRECTIVE_END_REGEX regex("^.end$")
#define DIRECTIVES_SKIP_WORD_REGEX regex("^.(skip|word)$")
#define DIRECTIVE_SECTION_REGEX regex("^.section$")
#define INSTRUCTIONS_REGEX regex("^(halt|int|iret|call|ret|jmp|jeq|jne|jgt|(push|pop|xchg|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr|ldr|str)(b|w){0,1})$")
#define LITERAL_DECIMAL_REGEX regex("^(\\-|\\+){0,1}[0-9]+$")
#define LITERAL_HEXADECIMAL_REGEX regex("^0x[0-9a-fA-F]{1,}$")
#define SYMBOL_REGEX regex("^[a-zA-Z_][a-zA-Z0-9_]*$")

#define JUMP_CALL_INSTRUCTION_REGEX regex("^(jmp)|(jeq)|(jne)|(jgt)|(call)[w]{0,1}$")
#define LDR_STR_INSTRUCTION_REGEX regex("^(ldr)|(str)[w]{0,1}$")



#define JUMP_CALL_DIRECT_REGEX regex("\\s*(jmp|jeq|jne|jgt|call)\\s*\\b(0x[0-9a-fA-F]{1,4}|[0-9]{1,4})\\b\\s*$")
#define JUMP_CALL_REGISTER_DIRECT_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s*[*](r[0-8]|psw|sp|pc)\\s*$")
#define JUMP_CALL_REGISTER_INDIRECT_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s*[*]\\[(r[0-8]|psw|sp|pc)\\]\\s*$")
#define JUMP_CALL_PC_RELATIVE_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s*[%]([a-zA-Z_][0-9a-zA-Z_]*)\\s*$")
#define JUMP_CALL_ABSOLUTE_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s*([a-zA-Z_][0-9a-zA-Z_]*)\\s*$")
#define JUMP_CALL_MEMORY_DIRECT_SYMBOL_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s*[*]([a-zA-Z_][0-9a-zA-Z_]*)\\s*$")
#define JUMP_CALL_MEMORY_DIRECT_LITERAL_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s*[*]\\b(0x[0-9a-fA-F]{1,4}|[0-9]{1,4})\\b\\s*$")
#define JUMP_CALL_DISPLAY_LITERAL_REGEX regex("\\s*(jmp|jeq|jne|jgt|call)\\s* \\s*[*]\\[(r[0-8]|psw|sp|pc)\\s*[+]\\s*\\b(0x[0-9a-fA-F]{1,4}|[0-9]{1,4})\\b\\]\\s*$")
#define JUMP_CALL_DISPLAY_SYMBOL_REGEX regex("^\\s*(jmp|jeq|jne|jgt|call)\\s* \\s*[*]\\[(r[0-8]|psw|sp|pc)\\s*[+]\\s*([a-zA-Z_][0-9a-zA-Z_]*)\\]\\s*$")





#define LDR_STR_DIRECT_LITERAL_REGEX regex("^\\s*(ldr|str)\\s(r[0-8]|psw|pc|sp)[,]\\s*[$]\\b(0x[0-9a-fA-F]{1,4}|[0-9]{1,4})\\b\\s*$")
#define LDR_STR_DIRECT_SYMBOL_REGEX regex("^\\s*(ldr|str)\\s(r[0-8]|psw|pc|sp)[,]\\s*[$]([a-zA-Z_][a-zA-Z0-9_]*)\\s*$")
#define LDR_STR_REGISTER_DIRECT_REGEX regex("^\\s*(ldr|str)\\s*(r[0-8]|psw|sp|pc)[,]\\s*(r[0-8]|psw|sp|pc)\\s*$")
#define LDR_STR_REGISTER_INDIRECT_REGEX regex("^\\s*(ldr|str)\\s*(r[0-8]|psw|sp|pc)[,]\\s*\\[(r[0-8]|psw|sp|pc)\\]\\s*$")
#define LDR_STR_MEMORY_DIRECT_ABOSOLUTE_REGEX regex("^\\s*(ldr|str)\\s(r[0-8]|psw|pc|sp)[,]\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*$")
#define LDR_STR_MEMORY_DIRECT_LITERAL_REGEX regex("^\\s*(ldr|str)\\s(r[0-8]|psw|pc|sp)[,]\\s*\\b(0x[0-9a-fA-F]{1,4}|[0-9]{1,4})\\b\\s*$")
#define LDR_STR_PC_RELATIVE_REGEX regex("^\\s*(ldr|str)\\s(r[0-8]|psw|pc|sp)[,]\\s*[%]([a-zA-Z_][a-zA-Z0-9_]*)\\s*$")
#define LDR_STR_DISPLAY_LITERAL_REGEX regex("\\s*(ldr|str)\\s*(r[0-8]|psw|pc|sp)[,]\\s*\\s*\\[(r[0-8]|psw|pc|sp)\\s*[+]\\s*\\b(0x[0-9a-fA-F]{1,4}|[0-9]{1,4})\\b\\][\\t ]*$")
#define LDR_STR_DISPLAY_SYMBOL_REGEX regex("\\s*(ldr|str)\\s*(r[0-8]|psw|pc|sp)[,]\\s*\\s*\\[(r[0-8]|psw|pc|sp)\\s*[+]\\s*([a-zA-Z_][a-zA-Z0-9_)]*)\\][\\t ]*$")


#define NO_OPERANDS_REGEX regex("^\\s*(halt|iret|ret)\\s*$")
#define ONE_OPERAND_REGEX regex("^\\s*(push|pop|int|not)\\s*(r[0-8]|psw|sp|pc)$")
#define TWO_OPERANDS_REGEX regex("^\\s*(add|sub|mul|div|cmp|and|or|xor|shl|shr|test|xchg) \\s*(r[0-8]|psw|sp|pc)\\s*,\\s*(r[0-8]|psw|sp|pc)$")

#define NO_OPERANDS_INSTRUCTIONS_REGEX regex("^(halt)|(iret)|(ret)(w){0,1}$")
#define ONE_OPERAND_INSTRUCTIONS_REGEX regex("^(int)|(push)|(pop)|(not)(w){0,1}$")
#define TWO_OPERANDS_INSTRUCTIONS_REGEX regex("^(xchg)|(add)|(sub)|(mul)|(div)|(cmp)|(and)|(or)|(xor)|(test)|(shl)|(shr)(w){0,1}$")


#define DEFAULT_SECTION_START_ADDRESS -1


#define DEFAULT_STACK_START_ADDRESS 0xFF00
#define MEMORY_SIZE 65536

#endif