#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 24 17:57:39 2019

@author: vsomdec
"""

import numpy as np

MPI_SEND, MPI_ISEND, MPI_RECV, MPI_IRECV, MPI_BCAST, MPI_IBCAST = 0,1,2,3,4,5

home = "/Users/vsomdec/workspace/Software_Engineering/project/code/coms6156/src/tests/"

def read_all_logs(log_file_prefix, num_procesess):

    
    logs = {}
    
    for p in range(num_procesess):
        p_log = {}
        fn = "{}/{}_{}.csv".format(home, log_file_prefix, p)
        
        with open(fn) as f:  
            for line in f:
                line = line.strip()
                if line.startswith("message"):
                    continue
                message = [int(x) for x in line.split(', ')]
                p_log[message[0]] = message[1:]
                
        logs[p] = p_log
    return logs



def associate_messages(logs, num_processes):
    
    associations = []
    
    for p in range(num_processes):
        p_log = logs[p]
        
        for m_id in p_log:
            if p_log[m_id] is None:
                continue
            call, source, dest, tag, wait_time = p_log[m_id]
            
            if call in [MPI_SEND, MPI_ISEND]:
                dest_log = logs[dest]
                for dest_m_id in dest_log:
                    if dest_log[dest_m_id] is None:
                        continue
                    
                    dest_call, dest_source, _, dest_tag, dest_wait_time = dest_log[dest_m_id]
                    
                    if dest_source == p:
                        logs[p][m_id] = None
                        logs[dest][dest_m_id] = None
                        associations.append(((p, m_id), (dest, dest_m_id)))
                        break
                        
            elif call in [MPI_RECV, MPI_IRECV]:
                source_log = logs[source]
                if source == -1:
                    continue
                for source_m_id in source_log:
                    if source_log[source_m_id] is None:
                        continue
                    
                    source_call, _, source_dest, source_tag, source_wait_time = source_log[source_m_id]
                    
                    if source_dest == p:
                        logs[p][m_id] = None
                        logs[source][source_m_id] = None
                        associations.append(((p, m_id), (source, source_m_id)))
                        break
            elif call in [MPI_BCAST]:
                root = source
                all_found = np.zeros(num_processes) 
                all_found[root] = 1
                for p_dest in range(num_processes):
                    if p_dest == root:
                        continue
                    dest_log = logs[p_dest]
                    for dest_m_id in dest_log:
                        if dest_log[dest_m_id] is None:
                            continue
                        
                        dest_call, dest_source, _, dest_tag, dest_wait_time = dest_log[dest_m_id]
                        
                        if dest_call == MPI_BCAST and dest_source == root:
                            logs[p_dest][dest_m_id] = None
                            associations.append(((p, m_id), (p_dest, dest_m_id)))
                            all_found[p_dest] = 1
                            break
                
                if np.sum(all_found) == num_processes:
                    logs[p][m_id] = None
  
                    

    
    
    for p in range(num_processes):
        p_log = logs[p]
        
        clean_p_log = {m_id: p_log[m_id] for m_id in p_log if not p_log[m_id] is None}
        
        logs[p] = clean_p_log
    return associations, logs

log_file_prefix = "DL4"
num_procesess = 2
logs = read_all_logs(log_file_prefix, num_procesess)

associations, unmatched_messages = associate_messages(logs, num_procesess)
