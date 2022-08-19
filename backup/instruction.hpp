#include <utility>

#define KILL 0
#define PUT 1
#define GET 2
#define REMOVE 3
#define INSTRUCTION_SIZE 3
#define RETURN_SIZE 2

using namespace std;

class instruction{
private:
  const int instruction_id, operand1, operand2, qid;

public:
  instruction(int, int, int);
  instruction(int, int, int, int);

  int get_operator() const;
  pair<int, int> get_operand() const;
  int get_qid() const;
};

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