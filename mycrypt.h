#include "kuznechik.h"
#include <vector>

void leftShift(uint8_t *array , int ARRAY_LENGTH);
int MSB(uint8_t cc);
void MAC(uint8_t key_t[32], std::vector<uint8_t> input, uint8_t output[8], bool full);
void MAC(uint8_t key_t[32], std::vector<bool> input, uint8_t output[8]);
int crypt(uint8_t key_t[32], std::vector<uint8_t> input, std::vector<uint8_t> &output);
int crypt(uint8_t key_t[32], std::vector<bool> input, std::vector<bool> &output, uint8_t mac[8]);
int decrypt(uint8_t key_t[32], std::vector<bool> input, std::vector<bool> &output);