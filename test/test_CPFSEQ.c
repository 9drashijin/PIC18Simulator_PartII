#include "unity.h"
#include "Bytecode.h"
#include "CPFSEQ.h"
#include <stdio.h>
#include "CException.h"


void setUp(void){}
void tearDown(void){}

void test_CPFSEQ_should_comapare_and_skip_if_value_in_WREG_and_FileReg_are_same(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = -1, // empty
                   .operand3 = 0,
				  };
  FSR[code.operand1] = 1;
  FSR[WREG] = 1;
  cpfseq(&code);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
}
void test_CPFSEQ_should_comapare_and_not_skip_if_value_in_WREG_and_FileReg_are_different(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = -1, // empty
                   .operand3 = ACCESS,
				  };
  FSR[code.operand1] = 1;
  FSR[WREG] = 5;
  cpfseq(&code);
  TEST_ASSERT_EQUAL(1,code.absoluteAddress);
}
void test_CPFSEQ_should_comapare_and_skip_if_value_in_WREG_and_FileReg_are_same_with_BSR(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x80,
                   .operand2 = -1, // empty
                   .operand3 = BANKED,
				  };
  FSR[code.operand1] = 1;
  FSR[WREG] = 1;
  FSR[BSR] = 13;
  cpfseq(&code);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  TEST_ASSERT_EQUAL_HEX8(0x0F,FSR[BSR]);
  TEST_ASSERT_EQUAL_HEX8(0xF80,code.operand1);
}
void test_CPFSEQ_with_operand2_not_empty_and_should_replace_to_operand3_which_is_empty(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = ACCESS, 
                   .operand3 = -1,		//operand3 must be empty then only replace operand3 = ACCESS
				  };
  FSR[code.operand1] = 1;
  FSR[WREG] = 1;
  cpfseq(&code);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
}
void test_CPFSEQ_with_operand2_not_empty_and_should_throw_ERROR_if_operand3_already_have_value(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};	
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x5A,
                   .operand2 = ACCESS, 
                   .operand3 = BANKED,	//operand3 already BANKED thus operand2 cant have value,should throw ERROR
				  };
  CException errorRange;
  FSR[code.operand1] = 1;
  FSR[WREG] = 1;
  Try{
	cpfseq(&code);
	TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_OPERAND,errorRange);
  }
}
void test_CPFSEQ_invalid_range() {
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0xFFFF, //invalid address
                    .operand2 =	-1, 	//must empty
                    .operand3 = 0, 
                  };
  CException errorRange;
  Try{
	cpfseq(&code);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_RANGE,errorRange);
  }
}
void test_CPFSEQ_invalid_BSR() {
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0xFF,
                    .operand2 =	-1,  	//must empty
                    .operand3 = BANKED, 
                  };
  FSR[BSR] = 30;	// over bank 15	  
  CException errorRange;
  Try{
	cpfseq(&code);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_BSR,errorRange);
  }
}
void test_CPFSEQ_given_the_operand1_value_less_than_0x80_and_should_compare_and_skip(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};
  Bytecode code = {.instruction = &inst,
                   .operand1 = 0x01,
                   .operand2 = -1, // empty
                   .operand3 = ACCESS,
				  };
  FSR[code.operand1] = 1;
  FSR[WREG] = 1;
  cpfseq(&code);
  TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  TEST_ASSERT_EQUAL_HEX8(0x001,code.operand1);
}
void test_CPFSEQ_given_the_operand2_and_operand3_with_invalid_input_should_catch_error(){
  Instruction inst = {.mnemonic = CPFSEQ,.name = "cpfseq"};
  Bytecode code = { .instruction = &inst,
                    .operand1 = 0x01,
                    .operand2 =	BANKED, 	// invalid with BANKED in operand2
                    .operand3 = 1000, 		// invalid 1000 random input;
                  };
  CException errorRange;
  FSR[code.operand1] = 1;
  FSR[WREG] = 1;
  Try{
	cpfseq(&code);
	TEST_ASSERT_EQUAL(2,code.absoluteAddress);
  }
  Catch(errorRange){
	TEST_ASSERT_EQUAL(INVALID_OPERAND,errorRange);
  }
}