//
//  DylibHelper.h
//  testProject
//
//  Created by System Administrator on 15.07.17.
//  Copyright © 2017 Kerschbaumer Stefan. All rights reserved.
//

#ifndef DylibHelper_h
#define DylibHelper_h
#include "include.h"
class DylibHelper {
private:
    task_dyld_info dyld_info;
    mach_msg_type_number_t dyldInfoCount = TASK_DYLD_INFO_COUNT;
    
    task_t task;
    task_t getTaskInfo() {
        return (task_info(task, TASK_DYLD_INFO, (task_info_t)&dyld_info, &this->dyldInfoCount) == KERN_SUCCESS) ? true: false;
    }
    
    
    
    bool getAllImageInfoEntry() {
        vm_offset_t tmpAddress;
        mach_msg_type_number_t size = sizeof(struct dyld_all_image_infos);
        if(vm_read(this->task,this->dyld_info.all_image_info_addr,size,&tmpAddress,&size) != KERN_SUCCESS)
            return false;
        
        this->getAllImageInfos(tmpAddress);
        
        return true;
    }
    
    dyld_image_info* imageInfoList;
    bool getImageInfoList() {
        vm_offset_t tmpAddress;
        mach_msg_type_number_t allImagesSize = sizeof(struct dyld_image_info) * this->allImageInfos->infoArrayCount;
        
        if(vm_read(task,(mach_vm_address_t) this->allImageInfos->infoArray,allImagesSize,&tmpAddress,&allImagesSize) != KERN_SUCCESS)
           return false;
           
        this->imageInfoList = (struct dyld_image_info*) tmpAddress;
        
        return true;
    }
    
    dyld_all_image_infos *allImageInfos;
    dyld_all_image_infos *getAllImageInfos(vm_offset_t tmpAddress){
        return this->allImageInfos = (struct dyld_all_image_infos *) tmpAddress;
    }
    void saveCopy(char* from, char* &to) {
        to = new char[strlen(from)];
        strcpy(to, from);
    }
    bool pushImagesToClass();
    bool log;
    
    char *szProcessName;
public:
    struct images_t {
        char *name;
        char* path;
        struct stat st;
        mach_vm_address_t baseAddress;
        task_t forTask;
        uint8_t *buffer;
        uint8_t *getBinary();
    };
    images_t *images;
    
   
    images_t *returnImages() {
        return this->images;
    }
    int allDylibCount;
    DylibHelper(task_t task_, char* szProcessName) {
        this->task = task_;
        this->saveCopy((char*)szProcessName, this->szProcessName);
    }
     bool getAllBaseAddresses();
};

#endif /* DylibHelper_h */
