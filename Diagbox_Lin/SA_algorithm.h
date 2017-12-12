
#ifndef SA_ALGORITHM_H_
#define SA_ALGORITHM_H_
#include <stdint.h>
#include <linux/can.h>
#include <linux/can/raw.h>


void Seed2Key(struct can_frame* pos_resp, struct can_frame* key_frame);

#endif /* SA_ALGORITH_H_ */
