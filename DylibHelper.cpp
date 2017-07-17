//
//  DylibHelper.cpp
//  testProject
//
//  Created by System Administrator on 15.07.17.
//  Copyright © 2017 Kerschbaumer Stefan. All rights reserved.
//

#include "DylibHelper.h"

uint8_t *DylibHelper::images_t::getBinary() {
    uint8_t * m_data;
    vm_offset_t addr;
    mach_msg_type_number_t sizeMax = 512;
    m_data = new uint8_t[this->st.st_size];
    sizeMax = this->st.st_size;
    vm_read(this->forTask,(vm_address_t)this->baseAddress,sizeMax,&addr,&sizeMax);
    uint64_t address = (uint64_t)addr;
    return this->buffer = (uint8_t *)address;
}


bool DylibHelper::pushImagesToClass(){
    mach_msg_type_number_t sizeMax=512;
    this->allDylibCount = 0;
    vm_offset_t tmpAddress;
    this->images = new images_t[this->allImageInfos->infoArrayCount];
    for(int i = 0; i < this->allImageInfos->infoArrayCount; i++) {
        
        if(vm_read(task,(mach_vm_address_t)this->imageInfoList[i].imageFilePath,sizeMax,&tmpAddress,&sizeMax) != KERN_SUCCESS)
            continue;
        
        char *path = (char *) tmpAddress;
        if(!strstr(path, ".dylib") && !strstr(path, this->szProcessName))
            continue;
        
        
        stat(path, &this->images[allDylibCount].st);
        this->saveCopy(path, this->images[allDylibCount].path);
        
        string name = this->images[allDylibCount].path;
        name = name.substr(name.find_last_of("/")+1, name.length());
        this->saveCopy((char*)name.c_str(), this->images[allDylibCount].name);
  
        this->images[allDylibCount].baseAddress = (mach_vm_address_t)this->imageInfoList[i].imageLoadAddress;
        allDylibCount++;
        
    }
    return true;
}
bool DylibHelper::getAllBaseAddresses() {
    
    if(!this->getTaskInfo() || !this->getAllImageInfoEntry() || !this->getImageInfoList())
        return false;
    this->pushImagesToClass();
    
    return true;
}