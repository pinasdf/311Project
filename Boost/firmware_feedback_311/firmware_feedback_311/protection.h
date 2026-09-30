#ifndef PROTECTION_H
#define PROTECTION_H

#include <stdbool.h>

// ---- Settings you can change ----
#define PROT_VOUT_MAX_V      5.5f    // software overvoltage limit
#define PROT_CURRENT_MAX_A   1.0f    // software overcurrent limit (keep below the hardware limit)
#define PROT_TRIP_COUNT      3       // consecutive loop ticks over the limit before tripping
#define PROT_USE_VO_FLAG     0       // 1 = also trip on the Vo hardware flag (PD2). Keep 0 until PD2 is wired.

void protection_init(void);
void protection_check(float vout, float current);   // call every loop tick
bool protection_fault(void);                        // true once tripped (latched)
void protection_reset(void);                        // clear the fault and release the OVP line

#endif
