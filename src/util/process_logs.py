#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 24 17:57:39 2019

@author: vsomdec
"""

MPI_SEND, MPI_ISEND, MPI_RECV, MPI_IRECV= 0,1,2,3

def read_all_logs(log_file_prefix, num_procesess):

    
    logs = {}
    
    for p in range(num_procesess):
        p_log = {}
        fn = "{}_{}.csv".format(log_file_prefix, p)
        
        with open(fn) as f:  
            for line in f:
                line = line.strip()
                if line.startswith("id"):
                    continue
                message = line.split(', ')
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
                for _m_id in dest_log:
                    if dest_log[_m_id] is None:
                        continue
                    
                    __call, _source, _, _tag, _wait_time = dest_log[_m_id]
                    
                    if _source == p:
                        pass
                        p_log[m_id] = None
                        dest_log[_m_id] = None
                        associations.append(((p, m_id), (dest, _m_id)))
                        
            elif call in [MPI_RECV, MPI_IRECV]:
                source_log = logs[source]
                for _m_id in source_log:
                    if source_log[_m_id] is None:
                        continue
                    
                    __call, _, _dest, _tag, _wait_time = source_log[_m_id]
                    
                    if _dest == p:
                        pass
                        p_log[m_id] = None
                        source_log[_m_id] = None
                        associations.append(((p, m_id), (source, _m_id)))
    
    
    for p in range(num_processes):
        p_log = logs[p]
        
        clean_p_log = {m_id: p_log[m_id] for m_id in p_log if not p_log[m_id] is None}
        
        logs[p] = clean_p_log
    return associations, logs

log_file_prefix = "DL1"
num_procesess = 1
logs = read_all_logs(log_file_prefix, num_procesess)

associations, unmatched_messages = associate_messages(logs, num_procesess)
