//
//  process.h
//  testProject
//
//  Created by System Administrator on 15.07.17.
//  Copyright © 2017 Kerschbaumer Stefan. All rights reserved.
//
//conversion from ul to int

#ifndef process_h
#define process_h
#include "include.h"

class attached_Process {
private:
    // setup Informations
    string szpName;
    string szPidGrabExecutionCommand;
    
    struct pIdStruct_t {
    private:
        unsigned long pId;
    public:
        void set(unsigned long id);
        unsigned long get();
        int toInteger();
        bool isValid();
    };
    
    pIdStruct_t pId;
    pIdStruct_t getProcessId();
    
    bool log;
    
    // process infromations
    task_t task;
    bool grabTaskForThisProcess();
    bool lastReadSucceeded_;
    
public:
    attached_Process(string szProcessName);
    bool setup(bool log);
    
    bool lastReadSucceeded();
    
    task_t getTask() {
        return this->task;
    }
    
    template <class type>
    type read(mach_vm_address_t address) {
        type ret;
        vm_offset_t data;
        uint32_t sz;
        (vm_read(this->task, address, sizeof(type), &data, &sz)!= KERN_SUCCESS) ? this->lastReadSucceeded_ = false : this->lastReadSucceeded_ = true;

        if(this->lastReadSucceeded_)
            ret = (type) *(type *)(data);
        else
            ret = NULL;
        
        // vm_deallocate(mach_task_self(), data, sz);
        return ret;
    }
    char *getName() {
        return (char*)this->szpName.c_str();
    }
    
};
extern attached_Process AttachToProcess(string szProcessName);

#endif /* process_h */
