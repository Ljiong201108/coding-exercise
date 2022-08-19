#include "instruction.h"

instruction::instruction(int instruction_id, int operand1, int operand2) : instruction_id(instruction_id), operand1(operand1), operand2(operand2), qid(-1) {}

instruction::instruction(int instruction_id, int operand1, int operand2, int id) : instruction_id(instruction_id), operand1(operand1), operand2(operand2), qid(id) {}

int instruction::get_operator() const {
  return instruction_id;
}

pair<int, int> instruction::get_operand() const {
  return make_pair(operand1, operand2);
}

int instruction::get_qid() const {
  return qid;
}