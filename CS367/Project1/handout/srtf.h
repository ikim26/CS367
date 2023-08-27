/* Do Not Modify This File */
#ifndef SRTF_H
#define SRTF_H

#include "structs.h"

/* Prototypes in the header_tr API */
void srtf_exit(header_t *header);
header_t *srtf_init();
int srtf_add(header_t *header, process_t *process);
int srtf_stop(header_t *header, int pid);
int srtf_continue(header_t *header, int pid);
process_t *srtf_generate(char *command, int pid, int time_remaining, int is_sudo);
int srtf_reap(header_t *header, int pid);
process_t *srtf_schedule(header_t *header);
int srtf_count(queue_t *ll);
void srtf_free(header_t *header);

#endif 
