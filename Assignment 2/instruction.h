#pragma once

#include <utility>

#define KILL 0
#define PUT 1
#define GET 2
#define REMOVE 3
#define INSTRUCTION_SIZE 3
#define RETURN_SIZE 2

using namespace std;

/**
 * encapsulate the basic operation and its operand(s) into one class
 */
class instruction{
private:
  const int instruction_id, operand1, operand2, qid;

public:
  instruction(int instruction_id, int operand1, int operand2);
  instruction(int instruction_id, int operand1, int operand2, int qid);

  int get_operator() const;
  pair<int, int> get_operand() const;
  int get_qid() const;
};