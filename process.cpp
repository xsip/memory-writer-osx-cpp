//
//  process.cpp
//  testProject
//
//  Created by System Administrator on 15.07.17.
//  Copyright © 2017 Kerschbaumer Stefan. All rights reserved.
//

#include <stdio.h>
#include "process.h"

attached_Process AttachToProcess(string szProcessName){
    return *new attached_Process(szProcessName);
}

attached_Process::attached_Process(string szProcessName) {
    this->szpName = szProcessName;
    this->szPidGrabExecutionCommand = "ps -ax | grep -i " + this->szpName + "| grep -v 'grep'";
    this->pId.set(0);
    this->getProcessId();
}

bool attached_Process::setup(bool log) {
    this->log = log;
    return this->getProcessId().isValid() ? this->grabTaskForThisProcess() : false;
}
bool attached_Process::grabTaskForThisProcess() {
    return task_for_pid(mach_task_self(), this->pId.toInteger(), &this->task) != KERN_SUCCESS ? false : true;
}
bool attached_Process::lastReadSucceeded(){
    bool tmp = this->lastReadSucceeded_;
    this->lastReadSucceeded_ = false;
    return tmp;
}

attached_Process::pIdStruct_t attached_Process::getProcessId() {
    printf("getting process id..\n");
    if(!this->pId.isValid()) {
        printf("process id was not valid..");
        FILE* pipe = popen(this->szPidGrabExecutionCommand.c_str() , "r");
        if (!pipe){
            if(this->log)
                printf("\n Error at pipe connection!\n");
            this->pId.set(0);
            return this->pId;
        }
        char buffer[128];
        string result = "";
        while(!feof(pipe))
            if(fgets(buffer, 128, pipe) != NULL)
                    result += buffer;
        pclose(pipe);
        this->pId.set(strtoul(result.substr(0, result.find_first_of("??")).c_str(), NULL, 0));
        if(this->log) {
            printf("\nProcess Id Fetch-Log => \n");
            printf("ProcessId (integer | unsigned Long) => %i\n", this->pId.toInteger());
            printf("ProcessId is valid => %s\n\n", (this->pId.isValid() ? "yes" : "no"));
        }
    }
    return this->pId;
}




//pid struct
void attached_Process::pIdStruct_t::set(unsigned long id) {
    this->pId = id;
}
unsigned long attached_Process::pIdStruct_t::get() {
    return this->pId;
}
int attached_Process::pIdStruct_t::toInteger() {
    return this->pId & INT_MAX;
}
bool attached_Process::pIdStruct_t::isValid() {
    if(this->pId != 0)
        return true;
    return false;
}
