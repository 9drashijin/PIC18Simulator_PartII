#include "unity.h"
#include "Bytecode.h"
#include "TSTFSZ.h"
#include <stdio.h>
#include "CException.h"

void setUp(void){}
void tearDown(void){}

void test_TSTFSZ_should_test_and_skip_if_FileReg_is_zero(){
	Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = -1, // empty
                   .operand3 = 0,
				  };
  FSR[code.operand1] = 0;	//zero
  tstfsz(&code);
  TEST_ASSERT_EQUAL(0,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
}
void test_TSTFSZ_should_test_and_should_not_skip_if_FileReg_is_not_zero(){
	Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = -1, // empty
                   .operand3 = 0,
				  };
  FSR[code.operand1] = 1;	//not zero
  tstfsz(&code);
  TEST_ASSERT_EQUAL(1,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(1,code.absoluteAddress);
}
void test_TSTFSZ_should_test_and_skip_if_FileReg_is_zero_with_BSR_selected(){
	Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0xFA,
                   .operand2 = -1, // empty
                   .operand3 = BANKED,
				  };
  FSR[code.operand1] = 0;	//zero
  tstfsz(&code);
  TEST_ASSERT_EQUAL(0,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  TEST_ASSERT_EQUAL_HEX8(0x0F,FSR[BSR]);
  TEST_ASSERT_EQUAL_HEX8(0xFFA,code.operand1);
}
void test_TSTFSZ_with_operand2_not_empty_and_should_replace_to_operand3_which_is_empty(){
  Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = ACCESS, 
                   .operand3 = -1,		//operand3 must be empty then only replace operand3 = ACCESS
				  };
  FSR[code.operand1] = 0;
  tstfsz(&code);
  TEST_ASSERT_EQUAL(0,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
}
void test_TSTFSZ_with_operand2_not_empty_and_should_throw_ERROR_if_operand3_already_have_value(){
  Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = ACCESS, 
                   .operand3 = BANKED,	//operand3 already BANKED thus operand2 cant have value,should throw ERROR
				  };
  CException errorRange;
  FSR[code.operand1] = 1;
  Try{
	tstfsz(&code);
	TEST_ASSERT_EQUAL(1,FSR[code.operand1]);
	TEST_ASSERT_EQUAL(1,code.absoluteAddress);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_OPERAND,errorRange);
  }
}
void test_TSTFSZ_invalid_range_of_input(){
  Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0xFFFF, //invalid address
                    .operand2 =	-1, 	//must empty
                    .operand3 = 0, 
                  };
  CException errorRange;
  Try{
	tstfsz(&code);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_RANGE,errorRange);
  }
}
void test_TSTFSZ_invalid_BSR_of_input(){
  Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0xFF,
                    .operand2 =	-1,  	//must empty
                    .operand3 = BANKED, 
                  };
  FSR[BSR] = 30;	// over bank 15	  
  CException errorRange;
  Try{
	tstfsz(&code);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_BSR,errorRange);
  }
}
void test_TSTFSZ_given_the_operand1_value_less_than_0x80_and_should_test_FileReg_and_skip(){
  Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x01,
                   .operand2 = -1, // empty
                   .operand3 = ACCESS,
				  };
  FSR[code.operand1] = 0;
  tstfsz(&code);
  TEST_ASSERT_EQUAL(0,FSR[code.operand1]);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  TEST_ASSERT_EQUAL_HEX8(0x001,code.operand1);
}
void test_TSTFSZ_given_the_operand2_and_operand3_with_invalid_input_should_catch_an_error(){
  Instruction inst = {.mnemonic = TSTFSZ,.name = "tstfsz"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0x01,
                    .operand2 =	ACCESS, 	// invalid with ACCESS in operand2
                    .operand3 = 777, 		// invalid input;
                  };
  CException errorRange;
  FSR[code.operand1] = 0;
  Try{
	tstfsz(&code);
	TEST_ASSERT_EQUAL(0,FSR[code.operand1]);
	TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_OPERAND,errorRange);
  }
}