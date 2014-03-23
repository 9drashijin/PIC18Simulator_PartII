#include "unity.h"
#include "Bytecode.h"
#include "INFSNZ.h"
#include <stdio.h>
#include "CException.h"

void setUp(void){}
void tearDown(void){}

void test_INFSNZ_given_fileReg_with_value_1_and_should_increment_and_skip_next_instruction_if_FileReg_not_zero(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x3A,
                   .operand2 = -1, //default = fileReg
                   .operand3 = 0,
				  };
  FSR[code.operand1] = 5;
  infsnz(&code);
  TEST_ASSERT_EQUAL(6,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
}
void test_INFSNZ_given_fileReg_with_value_negative_1_and_should_increment_and_not_skip_next_instruction_if_FileReg_is_zero(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x3A,
                   .operand2 = -1, //default = fileReg
                   .operand3 = 0,
				  };
  FSR[code.operand1] = -1;
  //printf("operand1: %d\n",FSR[code.operand1]);
  infsnz(&code);
  //printf("operand1: %d",FSR[code.operand1]);
  TEST_ASSERT_EQUAL(0,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(1,code.absoluteAddress);
}
void test_INFSNZ_should_increment_and_skip_next_instruction_and_store_in_WREG(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x3A,
                   .operand2 = W,	
                   .operand3 = ACCESS,
				  };
  FSR[code.operand1] = 1;
  //printf("operand1: %d\n",FSR[code.operand1]);
  infsnz(&code);
  //printf("operand1: %d",FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,FSR[WREG]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
}
void test_INFSNZ_given_fileReg_with_value_1_and_should_increment_and_skip_next_instruction_if_FileReg_not_zero_with_BSR_selected(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0xFA,
                   .operand2 = W, 	//store in WREG
                   .operand3 = BANKED,
				  };
  FSR[code.operand1+(0x0F00)] = 5;
  infsnz(&code);
  TEST_ASSERT_EQUAL(6,FSR[WREG+(0x0F00)]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  TEST_ASSERT_EQUAL_HEX8(0x0F,FSR[BSR]);
  TEST_ASSERT_EQUAL_HEX8(0xFFA,code.operand1);
}
void test_INFSNZ_invalid_range_of_input(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0xFFFF, //invalid address
                    .operand2 =	W, 	
                    .operand3 = 0, 
                  };
  CException errorRange;
  Try{
	infsnz(&code);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_RANGE,errorRange);
  }
}
void test_INFSNZ_invalid_BSR_of_input(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0xFF,
                    .operand2 =	F,
                    .operand3 = ACCESS, 
                  };
  FSR[BSR] = 40;	// over bank 15	  
  CException errorRange;
  Try{
	infsnz(&code);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_BSR,errorRange);
  }
}
void test_INFSNZ_given_the_operand2_and_operand3_with_invalid_input_should_catch_an_error(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0x01,
                    .operand2 =	BANKED, 	// invalid with BANKED in operand2
                    .operand3 = 222, 		// invalid input;
                  };
  CException errorRange;
  FSR[code.operand1] = 0;
  Try{
	infsnz(&code);
	TEST_ASSERT_EQUAL(1,FSR[code.operand1]);
	TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_OPERAND,errorRange);
  }
}
void test_INFSNZ_given_the_operand1_value_more_than_0x80_and_should_increment_FileReg_and_skip(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x81,
                   .operand2 = -1, //default >> ACCESS bank
                   .operand3 = ACCESS,
				  };
  FSR[code.operand1+(0x0F00)] = 10;
  infsnz(&code);
  TEST_ASSERT_EQUAL(11,FSR[code.operand1+(0x0F00)]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  TEST_ASSERT_EQUAL_HEX8(11,FSR[0xF81]);
}
void test_INFSNZ_given_the_operand1_value_less_than_0x80_and_should_increment_FileReg_and_skip(){
  Instruction inst = {.mnemonic = INFSNZ,.name = "infsnz"};
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x05,
                   .operand2 = W,  // store in WREG
                   .operand3 = 0,
				  };
  FSR[code.operand1] = -10;
  infsnz(&code);
  TEST_ASSERT_EQUAL(-9,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  //printf("FSR: %d",FSR[0x005]);
  TEST_ASSERT_EQUAL_HEX8(-9,FSR[0x005]);
}